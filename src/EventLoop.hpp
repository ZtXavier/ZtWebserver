#ifndef EVENTLOOP_HPP_
#define EVENTLOOP_HPP_

#include<iostream>
#include<functional>
#include<vector>
#include<memory>
#include"Channel.hpp"
#include"../logs/Logging.hpp"
#include"../logs/Thread.hpp"
#include"Epoll.hpp"
#include"Util.hpp"

namespace zvws {
    namespace detail {

class EventLoop {
    public:
        typedef std::function<void()> Functor;
        EventLoop();
        ~EventLoop();
        void Quit();
        void Loop();
        void RunInLoop(Functor&& cb);
        void QueueInLoop(Functor&& cb);
        bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
        void shutdown(std::shared_ptr<Channel> channel) {
            shutDownWrite(channel->getFd());
        }

        void addToPoller(std::shared_ptr<Channel> channel, int timeout = 0) {
            poller_->epoll_add(channel, timeout);
        }

        void updatePoller(std::shared_ptr<Channel> channel, int timeout = 0) {
            poller_->epoll_mod(channel, timeout);
        }

        void removeFromPoller(std::shared_ptr<Channel> channel) {
            poller_->epoll_del(channel);
        }

    private:
        bool looping_;
        bool quit_;
        bool eventloophandling_;
        bool callingPendingFunctor_;
        int wakeupFd_;
        const pid_t threadId_;
        mutable MutexLock mutex_;
        std::vector<Functor> pendingFunctors_;
        std::shared_ptr<Channel> pwakeupChannel_;
        std::shared_ptr<Epoll> poller_;

        void wakeup();
        void handleRead();
        void doPendingFunctors();
        void handleConn();

};

}
}

#endif