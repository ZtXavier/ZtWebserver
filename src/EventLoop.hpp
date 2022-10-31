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

    private:
        bool looping_;
        bool quit_;
        bool eventloophd_;
        bool callingPendingFunctor_;
        int weakupFd_;
        const pid_t threadId_;
        mutable MutexLock mutex_;
        std::vector<Functor> pendingsFunctors_;
        std::shared_ptr<Channel> pwakeupChannel_;
        std::shared_ptr<Epoll> poller_;
};

}
}

#endif