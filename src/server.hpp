#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <memory>
#include <iostream>
#include "Channel.hpp"
#include "EventLoop.hpp"
#include "EventLoopThreadPool.hpp"


namespace zvws {

    namespace detail {

    
        class Server {
            public:
                Server(EventLoop* loop, int threadnum, int port);
                ~Server() {}
                EventLoop *getLoop() const {
                    return loop_;
                }
                void Start();
                void handNewConn();
                void handThisConn() {
                    loop_->updatePoller(acceptChannel_);
                }

            private:
                EventLoop *loop_;
                std::unique_ptr<EventLoopThreadPool> eventLoopThreadPool_;
                std::shared_ptr<Channel> acceptChannel_;

                int threadNum_;
                int Port_;
                int listenFd_;
                bool started_;
                static const int MAXNUMFD = 100000;
        };
    }
}









#endif