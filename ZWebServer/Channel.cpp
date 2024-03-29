#include<iostream>
#include<cstdlib>
#include<queue>
#include<unistd.h>
#include"Channel.hpp"
#include"Epoll.hpp"
#include"EventLoop.hpp"
#include"Util.hpp"



namespace zvws {
    namespace detail {
        Channel::Channel(EventLoop *loop) : loop_(loop),events_(0), lastEvents_(0), fd_(0) {

        }
        Channel::Channel(EventLoop *loop, int fd) : loop_(loop),events_(0), lastEvents_(0), fd_(fd) {

        }

        Channel::~Channel() {

        }

        int Channel::getFd() {
            return fd_;
        }

        void Channel::setFd(int fd) {
            fd_ = fd;
        }

        void Channel::handleRead() {
            if(readHandler_) {
                readHandler_();
            }
        }

        void Channel::handleWrite() {
            if(writeHandler_) {
                writeHandler_();
            }
        }

        void Channel::handleConn() {
            if(connHandler_) {
                connHandler_();
            }
        }

    }
}