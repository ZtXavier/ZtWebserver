#ifndef CHANNEL_HPP_
#define CHANNEL_HPP_

#include<sys/epoll.h>
#include<string>
#include<functional>
#include<memory>
#include<unordered_map>
#include"Timer.hpp"


namespace zvws {
    namespace detail {
class EventLoop;
class Httpsolution;

class Channel {
    private:
        typedef std::function<void()> Callback;
        EventLoop *loop_;
        int fd_;
        __uint32_t events_;
        __uint32_t revents_;
        __uint32_t lastEvents_;

        std::weak_ptr<Httpsolution> holder_;

        int parse_uri();
        int parse_Header();
        int analysisRequest();

        Callback readHandler_;
        Callback writeHandler_;
        Callback errorHandler_;
        Callback connHandler_;

    public:
        Channel(EventLoop *loop);
        Channel(EventLoop *loop,int fd);
        ~Channel();
        int getFd();
        void setFd(int fd);

        void setHolder(std::shared_ptr<Httpsolution> holder) {
            holder_ = holder;
        }
        std::shared_ptr<Httpsolution> getHodler() {
            std::shared_ptr<Httpsolution> ret(holder_.lock());
            return ret;
        }
        // 取右值引用来减少拷贝
        void setReadHandler(Callback && readHandler) { 
            readHandler_ = readHandler; 
        }

        void setWriteHandler(Callback && writeHandler) { 
            writeHandler_ = writeHandler; 
        }

        void setErrorHandler(Callback && errorHandler) { 
            errorHandler_ = errorHandler; 
        }

        void setconnHandler(Callback && connHandler) { 
            connHandler_ = connHandler; 
        }

        void handleEvents() {
            events_ = 0;
            // tcp对端半链接发来数据或者对方发送来数据(可读数据)
            if((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)) {
                events_ = 0;
                return;
            }
            
            if(revents_ & EPOLLERR) {
                if(errorHandler_) {
                   errorHandler_(); 
                }
                events_ = 0;
                return;
            }

            if(revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
                handleRead();
            }

            if(revents_ & EPOLLOUT) {
                handleWrite();
            }
            handleconn();
        }

        void handleRead();
        void handleWrite();
        void handleError(int fd,int error_num,std::string short_msg);
        void handleconn();

        void setRevents(__uint32_t ev) {
             revents_ = ev;
        }

        void setEvents(__uint32_t ev) {
            events_ = ev;
        }

        __uint32_t &getEvents() {
            return events_;
        }

        bool EqualAndUpdateLastEvents() {
            bool ret = (lastEvents_ == events_);
            lastEvents_ = events_;
            return ret;
        }

        __uint32_t getLastEvents() {
            return lastEvents_;
        }
};

    typedef std::shared_ptr<Channel> Spt_Channel;
}

}

#endif