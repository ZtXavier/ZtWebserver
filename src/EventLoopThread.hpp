#ifndef EVENTLOOPTHREAD_HPP_
#define EVENTLOOPTHREAD_HPP_
#include "../base/Nocopyable.hpp"
#include "../logs/Thread.hpp"
#include "../logs/MutexLock.hpp"
#include "../logs/Condition.hpp"
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