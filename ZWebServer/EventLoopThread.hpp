#ifndef EVENTLOOPTHREAD_HPP_
#define EVENTLOOPTHREAD_HPP_
#include "base/Nocopyable.hpp"
#include "base/Thread.hpp"
#include "base/MutexLock.hpp"
#include "base/Condition.hpp"
#include "EventLoop.hpp"


namespace zvws {

    namespace detail {

        class EventLoopThread : Nocopyable {
            public:
                EventLoopThread();
                ~EventLoopThread();
                EventLoop* StartLoop();
            private:
                void ThreadFunc();
                bool exiting_;
                EventLoop* loop_;
                Thread thread_;
                MutexLock mutex_;
                Condition cond_;
        };
    }
}

#endif