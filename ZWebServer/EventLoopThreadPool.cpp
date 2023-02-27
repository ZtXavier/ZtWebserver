#include "EventLoopThreadPool.hpp"
#include <iostream>
#include <string>
#include <assert.h>

namespace zvws {
    namespace detail {

        EventLoopThreadPool::EventLoopThreadPool(EventLoop *baseLoop, 
        int numThreads) 
        : baseLoop_(baseLoop),
        started_(false), 
        numThreads_(numThreads),
        next_(0) {
            if(numThreads <= 0) {
                LOG << "numThreads <= 0";
                abort();
            }
        }

        void EventLoopThreadPool::StartPool() {
            baseLoop_->assertInLoopThread();
            started_ = true;
            for(int i = 0;i < numThreads_; ++i) {
                std::shared_ptr<EventLoopThread> thread (new EventLoopThread());
                threads_.push_back(thread);
                loops_.push_back(thread->StartLoop());
            }
        }

        EventLoop *EventLoopThreadPool::getNextLoop() {
            baseLoop_->assertInLoopThread();
            assert(started_);
            EventLoop *loop = baseLoop_;
            if(!loops_.empty()) {
                loop = loops_[next_];
                next_ = (next_ + 1) % numThreads_;
            }
            return loop;
        }
    }
}