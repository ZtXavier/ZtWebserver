#ifndef EVENTLOOPTHREAD_HPP_
#define EVENTLOOPTHREAD_HPP_
#include"../base/Nocopyable.hpp"
#include"../logs/Thread.hpp"
#include"../logs/MutexLock.hpp"
#include"../logs/Condition.hpp"


class EventLoopThread : Nocopyable {
    public:
        EventLoopThread();
        ~EventLoopThread();
    private:
        void threadFunc();
        
};


#endif