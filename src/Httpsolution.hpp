#ifndef HTTPSOLUTION_HPP_
#define HTTPSOLUTION_HPP_
#include <sys/epoll.h>
#include <unistd.h>
#include <functional>
#include <map>
#include <string>
#include <unordered_map>
#include <memory>

#include"Timer.hpp"

namespace zvws {
namespace detail {

    class EventLoop;
    class TimerNode;
    class Channel;
    
    enum ProcessState {
        STATE_PARSE_URI = 1,
        STATE_PARSE_HEADERS,
        STATE_RECV_BODY,
        STATE_ANALYSIS,
        STATE_FINISH
    };

    enum UriState {
        PARSE_HEADER_SUCCESS = 1,
        PARSE_HEADER_AGAIN,
        PARSE_HEADER_ERROR
    };

    enum AnalysisState {
        ANALYSIS_SUCCESS = 1,
        ANALYSIS_ERROR
    };

    enum ParseState {
        H_START = 0,
        H_KEY,
        H_COLON,
        H_SPACES_AFTER_COLON,
        H_VALUE,
        H_CR,
        H_LF,
        H_END_CR,
        H_END_LF
    };

    enum ConnectionState {
        H_CONNECTED = 0,
        H_DISCONNECTING,
        H_DISCONNECTED
    };

    enum HttpMethod {
        METHOD_POST = 1,
        METHOD_GET,
        METHOD_HEAD
    };

    enum HttpVersion {
        HTTP_10 = 1,
        HTTP_11
    };

    enum HeaderState {
        PARSE_HEADER_SUCCESS = 1,
        PARSE_HEADER_AGAIN,
        PARSE_HEADER_ERROR
    };


    //自定义一个类型封装通过map来存储域名和buffer的对应关系
    class MineType {
        private:
            static void init();
            static std::unordered_map<std::string,std::string> mine;
            MineType();
            MineType(const MineType &minetype);

        public:
            static std::string getMine(const std::string &buffix);
        private:
            static pthread_once_t once_control;
    };

    class Httpsolution : public std::enable_shared_from_this<Httpsolution> {
        public:
            Httpsolution(EventLoop *loop, int connfd);
            ~Httpsolution() {
                close(fd_);
            }
            void reset();
            void seperateTimer();
            void linkTimer(std::shared_ptr<TimerNode> mtimer) {
                timer_ = mtimer;
            }

            std::shared_ptr<Channel> getChannel() {
                return channel_;
            }

            EventLoop *getLoop() {
                return loop_;
            }
            void handleClose();
            void newEvent();
        
        private:
            EventLoop *loop_;
            std::shared_ptr<Channel> channel_;
            std::string inBuffer_;
            std::string outBuffer_;
            int fd_;
            bool error_;

            HttpMethod method_;
            HttpVersion httpversion_;
            std::string fileName_;
            std::string path_;
            ProcessState state_;
            ParseState Hstate_;
            bool keepAlive;
            int nowReadPos_;
            std::map<std::string, std::string> headers_;
            std::weak_ptr<TimerNode> timer_;

            void handleRead();
            void handleWrite();
            void handleConn();
            void handleError(int fd, int err_num, std::string short_msg);
            UriState parseUrl();
            HeaderState parseHeaders();
            AnalysisState analysisRequest();
    };
}
}




#endif