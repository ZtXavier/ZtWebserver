#include <iostream>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include  "Httpsolution.hpp"
#include "Channel.hpp"
#include "EventLoop.hpp"
#include "Util.hpp"
#include "time.h"

// 使用pthread_once_t 来确保这个实例在多线程环境下仅被初始化一次

namespace zvws {
    namespace detail {

        pthread_once_t MimeType::once_control = PTHREAD_ONCE_INIT;
        std::unordered_map<std::string, std::string> MimeType::mime;
        const __uint32_t DEFAULT_EVENT = EPOLLIN | EPOLLET | EPOLLONESHOT;
        #define DEFAULT_EXPIRED_TIME  2000
        #define DEFAULT_KEEP_ALIVE_TIME  5 * 60 * 1000

        // 初始化哈希表
        void MimeType::init() {
            mime[".html"] = "text/html";
            mime[".avi"] = "video/x-msvideo";
            mime[".bmp"] = "image/bmp";
            mime[".c"] = "text/plain";
            mime[".doc"] = "application/msword";
            mime[".gif"] = "image/gif";
            mime[".gz"] = "application/x-gzip";
            mime[".htm"] = "text/html";
            mime[".ico"] = "image/x-icon";
            mime[".jpg"] = "image/jpeg";
            mime[".png"] = "image/png";
            mime[".txt"] = "text/plain";
            mime[".mp3"] = "audio/mp3";
            mime["default"] = "text/html";
        }

        // 获取文件后缀名
        std::string MimeType::getMime(const std::string& suffix) {
            pthread_once(&once_control, MimeType::init);
            // 没有找到后缀名
            if (mime.find(suffix) == mime.end()) {
                return mime["default"];
            } else {
                return mime[suffix];
            }
        }

        Httpsolution::Httpsolution(EventLoop *loop, int connfd)
        : loop_(loop),
        channel_(new Channel(loop, connfd)),
        fd_(connfd),
        error_(false),
        connectionState_(H_CONNECTED),
        method_(METHOD_GET),
        httpversion_(HTTP_11),
        nowReadPos_(0),
        state_(STATE_PARSE_URI),
        Hstate_(H_START),
        keepAlive_(false) {
            channel_->setReadHandler(bind(&Httpsolution::handleRead, this));
            channel_->setWriteHandler(bind(&Httpsolution::handleWrite, this));
            channel_->setConnHandler(bind(&Httpsolution::handleConn, this));
        }     

        void Httpsolution::reset() {
            fileName_.clear();
            path_.clear();
            nowReadPos_ = 0;
            state_ = STATE_PARSE_URI;
            Hstate_ = H_START;
            headers_.clear();
            keepAlive_ = false;
            if(timer_.lock()) {
                std::shared_ptr<TimerNode> my_timer(timer_.lock());
                my_timer->chearReq();
                timer_.reset();
            }
        }   


        void Httpsolution::seperateTimer() {
            if(timer_.lock()) {
                std::shared_ptr<TimerNode> my_timer(timer_.lock());
                my_timer.reset();
                timer_.reset();
            }
        }

        void Httpsolution::handleRead() {
            __uint32_t &event_ = channel_->getEvents();
            do {
                // 完整读完的标志
                bool zero = false;
                int read_num = readn(fd_, inBuffer_, zero);
                LOG  << "Request: " << inBuffer_;
                // 来判断连接的状态
                if(connectionState_ == H_DISCONNECTING) {
                    inBuffer_.clear();
                    break;
                }
                if(read_num < 0) {
                    perror("error in read");
                    error_ = true;
                    handleError(fd_, 400, "Bad Request");
                    break;
                } else if(zero) {
                    connectionState_ = H_DISCONNECTING;
                    if(read_num == 0) {
                        break;
                    }
                }
                // 进入下一步解析uri
                if(state_ == STATE_PARSE_URI) {
                    UriState flag = this->parseUrl();
                    if(flag == PARSE_URI_AGAIN) {
                        break;
                    } else if (flag == PARSE_URI_ERROR) {
                        perror("error in parse_uri_error");
                        LOG  << "fd = " << fd_ << "," << inBuffer_ << "\n";
                        inBuffer_.clear();
                        error_ = true;
                        handleError(fd_, 400, "Bad Request");
                        break;
                    } else {
                        state_ = STATE_PARSE_HEADERS;
                    }
                }
                // 解析报文头部
                if (state_ == STATE_PARSE_HEADERS) {
                    HeaderState flag = this->parseHeaders();
                    if (flag == PARSE_HEADER_AGAIN) {
                        break;
                    } else if (flag == PARSE_HEADER_ERROR) {
                        perror("error in parse_header_error");
                        error_ = true;
                        LOG  << "fd = " << fd_ << "," << inBuffer_ << "\n";
                        handleError(fd_, 400, "Bad Request");
                        break;
                    }
                    if (method_ == METHOD_POST) {
                        state_ = STATE_RECV_BODY;
                    } else {
                        state_ = STATE_ANALYSIS;
                    }
                }

                if(state_ == STATE_RECV_BODY) {
                    int content_length = -1;
                    if(headers_.find("Content-Length") != headers_.end()) {
                        content_length = stoi(headers_["Content-length"]);
                    } else {
                        error_ = true;
                        handleError(fd_, 400, "Bad Request: Lack of argument (Content-length)");
                        break;
                    }
                    if(static_cast<int>(inBuffer_.size()) < content_length) {
                        break;
                    }    
                    state_ = STATE_ANALYSIS;
                }
                if(state_ == STATE_ANALYSIS) {
                    AnalysisState flag = this->analysisRequest();
                    if (flag == ANALYSIS_SUCCESS) {
                        state_ = STATE_FINISH;
                        break;
                    } else {
                        error_ = true;
                        break;
                    }
                }
            }while(0);

            if(!error_) {
                if(outBuffer_.size() > 0) {
                    handleWrite();
                }
                if(!error_ && state_ == STATE_FINISH) {
                    this->reset();
                    if(inBuffer_.size() > 0) {
                        if(connectionState_ == H_DISCONNECTING) {
                            handleRead();
                        }
                    }
                } else if (!error_ && connectionState_ != H_DISCONNECTED) {
                    event_ |= EPOLLIN;
                }
            }
        }

        void Httpsolution::handleWrite() {
            if(!error_ && connectionState_ != H_DISCONNECTED) {
                __uint32_t &events_ = channel_->getEvents();
                if(writen(fd_, outBuffer_) < 0) {
                    perror("writen error");
                    events_ = 0;
                    error_ = true;
                }
                if(outBuffer_.size() > 0) {
                    events_ |= EPOLLOUT;
                }
            }
        }

        void Httpsolution::handleConn() {
            seperateTimer();
            __uint32_t &events_ = channel_->getEvents();
            if(!error_ && connectionState_ == H_CONNECTED) {
                if(events_ != 0) {
                    int timeout = DEFAULT_EXPIRED_TIME;
                    if(keepAlive_) {
                        timeout = DEFAULT_KEEP_ALIVE_TIME;
                        if((events_ & EPOLLIN) && (events_ & EPOLLOUT)) {
                            events_ = __uint32_t(0);
                            events_ |= EPOLLOUT;
                        }
                        events_ |= EPOLLET;
                        loop_->updatePoller(channel_, timeout);
                    }
                } else if(keepAlive_) {
                    events_ |= (EPOLLIN | EPOLLET);
                    int timeout = DEFAULT_EXPIRED_TIME;
                    loop_->updatePoller(channel_, timeout);
                } else {
                    events_ |= (EPOLLIN | EPOLLET);
                    int timeout = (DEFAULT_EXPIRED_TIME / 2);
                    loop_->updatePoller(channel_, timeout);
                }
            } else if(!error_ && connectionState_ == H_DISCONNECTING && (events_ & EPOLLOUT)) {
                events_ = (EPOLLOUT | EPOLLET);
            } else {
                loop_->RunInLoop(bind(&Httpsolution::handleClose, shared_from_this()));
            }
        }

        UriState Httpsolution::parseUrl() {
            std::string &str = inBuffer_;
            std::string cpystr = str;
            size_t pos = str.find('\r', nowReadPos_);
            if(pos < 0) {
                return PARSE_URI_AGAIN;
            }
            std::string request_line = str.substr(0 ,pos);
            if(str.size() > pos + 1) {
                str = str.substr(pos + 1);
            } else {
                str.clear();
            }
            int posGet = request_line.find("GET");
            int posPost = request_line.find("POST");
            int posHead = request_line.find("HEAD");

            // 找到请求方法后返回请求方法的下标
            if(posGet >= 0) {
                pos = posGet;
                method_ = METHOD_GET;
            }else if(posPost >= 0) {
                pos = posPost;
                method_ = METHOD_POST;
            }if(posHead >= 0) {
                pos = posHead;
                method_ = METHOD_HEAD;
            }

            pos = request_line.find("/", pos);
            if(pos < 0) {
                fileName_ = "index.html";
                httpversion_ = HTTP_11;
                return PARSE_URI_ERROR;
            } else {
                size_t kpos = request_line.find(' ', pos);
                if(kpos < 0) {
                    return PARSE_URI_ERROR;
                } else {
                    if(kpos - pos > 1) {
                        fileName_ = request_line.substr(pos + 1, kpos - pos - 1);
                        size_t nkpos = fileName_.find('?');
                        if(nkpos >= 0) {
                            fileName_ = fileName_.substr(0, nkpos);
                        }
                    } else {
                        fileName_ = "index.html";
                    }
                }
                pos = kpos;
            }
            pos = request_line.find("/", pos);
            if(pos < 0) {
                return PARSE_URI_ERROR;
            } else {
                if(request_line.size() - pos <= 3) {
                    return PARSE_URI_ERROR;
                } else {
                    std::string version = request_line.substr(pos + 1, 3);
                    if(version == "1.0") {
                        httpversion_ = HTTP_10;
                    } else if(version == "1.1"){
                        httpversion_ = HTTP_11;
                    } else {
                        return PARSE_URI_ERROR;
                    }
                }
            }
            return PARSE_URI_SUCCESS;
        }


        // 解析头部
        HeaderState Httpsolution::parseHeaders() {
            std::string &str = inBuffer_;
            int key_start = -1;
            int key_end = -1;
            int value_start = -1;
            int value_end = -1;
            int read_line_start = 0;
            bool notfinished = true;
            size_t i = 0;
            for(; i < str.size() && notfinished; ++i) {
                switch(Hstate_) {
                    case H_START:
                    if(str[i] == '\n' || str[i] == '\r') {
                        break;
                    }
                    Hstate_ = H_KEY;
                    key_start = i;
                    read_line_start = i;
                    break;

                    case H_KEY:
                    if(str[i] == ':') {
                        key_end = i;
                        if(key_end - key_start <= 0) {
                            return PARSE_HEADER_ERROR;
                        }
                        Hstate_ = H_COLON;
                    }else if(str[i] == '\n' || str[i] == '\r') {
                        return PARSE_HEADER_ERROR;
                    }
                    break;

                    case H_COLON:
                    if(str[i] == ' ') {
                        Hstate_ = H_SPACES_AFTER_COLON;
                    } else {
                        return PARSE_HEADER_ERROR;
                    }
                    break;

                    case H_SPACES_AFTER_COLON:
                    Hstate_ = H_VALUE;
                    value_start = i;
                    break;

                    case H_VALUE:
                    if(str[i] == '\r') {
                        Hstate_ = H_CR;
                        value_end = i;
                        if(value_end - value_start <= 0) {
                            return PARSE_HEADER_ERROR;
                        } else if(i - value_start > 255) {
                            return PARSE_HEADER_ERROR;
                        }
                        break;
                    }

                    case H_CR:
                    if(str[i] == '\n') {
                        Hstate_ = H_LF;
                        std::string key(str.begin() + key_start, str.begin() + key_end);
                        std::string value(str.begin() + value_start, str.begin() + value_end);
                        headers_[key] = value;
                        read_line_start = i;
                    } else {
                        return PARSE_HEADER_ERROR;
                    }
                    break;

                    case H_LF:
                    if(str[i] == '\r') {
                        Hstate_ = H_END_CR;
                    } else {
                        key_start = i;
                        Hstate_ = H_KEY; 
                    }
                    break;

                    case H_END_CR:
                    if(str[i] == '\n') {
                        Hstate_ = H_END_LF;
                    } else {
                        return PARSE_HEADER_ERROR;
                    }
                    break;

                    case H_END_LF:
                    notfinished = false;
                    key_start  = i;
                    read_line_start = i;
                    break; 

                }
            }
            if(Hstate_ == H_END_LF) {
                str = str.substr(i);
                return PARSE_HEADER_SUCCESS;
            }
            str = str.substr(read_line_start);
            return PARSE_HEADER_AGAIN;
        }
        
        AnalysisState Httpsolution::analysisRequest() {
            if(method_ == METHOD_POST) {
                //TODO
            }
            else if(method_ == METHOD_GET || method_ == METHOD_HEAD) {
                std::string header;
                header += "HTTP/1.1 200 OK\r\n";
                if((headers_.find("Connection") != headers_.end()) &&
                (headers_["Connection"] == "Keep-Alive" || 
                headers_["Connection"] == "keep-Alive")) {
                    keepAlive_ = true;
                    header += std::string("Connecion: Keep-Alive\r\n") + 
                    "Keep-Alive: timeout=" + std::to_string(DEFAULT_KEEP_ALIVE_TIME) +
                    "\r\n";
                }
                int dot_pos = fileName_.find('.');
                std::string filetype;
                if(dot_pos < 0) {
                    filetype = MimeType::getMime("defualt");
                } else {
                    filetype = MimeType::getMime(fileName_.substr(dot_pos));
                }
                if (fileName_ == "hello") {
                    outBuffer_ = "HTTP/1.1 200 OK\r\nContent-type: text/plain\r\n\r\nhello world";
                    return ANALYSIS_SUCCESS;
                }
                if(fileName_ == "favicon.ico") {
                    header += "Content-Type: image/png\r\n";
                    // header += "Content-Length: " + std::to_string(sizeof(favicon) + "\r\n");
                    header += "Server: ZtWebServer\r\n";
                    header += "\r\n";
                    outBuffer_ += header;
                    // outBuffer_ += std::string(favicon, favicon + sizeof(favicon));
                    return ANALYSIS_SUCCESS;
                }
                struct stat sbuf;
                if(stat(fileName_.c_str(), &sbuf) < 0) {
                    header.clear();
                    handleError(fd_, 404, "Not Found!");
                    return ANALYSIS_ERROR;
                }
                header += "Content-Type: " + filetype + "\r\n";
                header += "Content-Length: " + std::to_string(sbuf.st_size) + "\r\n";
                header += "Server: ZtWebServer\r\n";
                header += "\r\n";
                outBuffer_ += header;
                if(method_ == METHOD_HEAD) {
                    return ANALYSIS_SUCCESS;
                }
                // 对文件的操作选择mmap对其做一个映射
                int src_fd = open(fileName_.c_str(), O_RDONLY, 0);
                if (src_fd < 0) {
                    outBuffer_.clear();
                    handleError(fd_, 404, "Not Found!");
                    return ANALYSIS_ERROR;
                }
                // 返回一个映射的首地址(void*) 第一个参数作为null,让内核去选择,这样做很便捷
                void* mmapret = mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE, src_fd, 0);
                close(src_fd);
                if(mmapret == (void*)(-1)) {
                    munmap(mmapret, sbuf.st_size);
                    outBuffer_.clear();
                    handleError(fd_, 404, "Not Found!");
                    return ANALYSIS_ERROR;
                }
                char *src_addr = static_cast<char*> (mmapret);
                outBuffer_ += std::string(src_addr, src_addr + sbuf.st_size);
                munmap(mmapret, sbuf.st_size);    
                return ANALYSIS_SUCCESS;
            }
            return ANALYSIS_ERROR;
        }

        void Httpsolution::handleError(int fd, int err, std::string short_msg) {
            short_msg = " " + short_msg;
            char send_Buffer[4096];
            std::string body_buff;
            body_buff += "<html><title>oh! find error...</title>";
            body_buff += "<body bgcolor=\"0000ff\">";
            body_buff += std::to_string(err) + short_msg;
            body_buff += "<hr><em> XZtWebserver</em>\n</body></html>";

            std::string header_buff;
            header_buff += "HTTP/1.1 " + std::to_string(err) + short_msg + "\r\n";
            header_buff += "Content-Type: text/html\r\n";
            header_buff += "Connection: Close\r\n";
            header_buff += "Content-Length: " + std::to_string(body_buff.size()) + "\r\n";
            header_buff += "XZtWebserver\r\n";
            header_buff += "\r\n";
            sprintf(send_Buffer, "%s", header_buff.c_str());
            writen(fd, send_Buffer, strlen(send_Buffer));
            sprintf(send_Buffer, "%s", body_buff.c_str());
            writen(fd, send_Buffer, strlen(send_Buffer));
        }   


        void Httpsolution::handleClose() {
            connectionState_ = H_DISCONNECTED;
            std::shared_ptr<Httpsolution> guard(shared_from_this());
            loop_->removeFromPoller(channel_);
        }

        void Httpsolution::newEvent() {
            channel_->setRevents(DEFAULT_EVENT);
        }
    }
}


