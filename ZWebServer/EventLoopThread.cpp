#include<iostream>
#include<string>
#include<vector>
#include<functional>
#include<assert.h>
#include"EventLoopThread.hpp"



    namespace zvws {
    namespace detail {

        EventLoopThread::EventLoopThread() 
        :loop_(NULL),
        exiting_(false),
        thread_(std::bind(&EventLoopThread::ThreadFunc,this),"EventLoopThread"),
        mutex_(),
        cond_(mutex_) {}

        EventLoopThread::~EventLoopThread() {
            exiting_ = true;
            if(loop_ != NULL) {
                loop_->Quit();
                thread_.join();
            }
        }

        EventLoop* EventLoopThread::StartLoop() {
            assert(!thread_.started());
            thread_.start();
             {
                MutexLockGuard lock(mutex_);
                while (loop_ == NULL) {
                    cond_.wait();
                }
            }
            return loop_;
        }

        void EventLoopThread::ThreadFunc() {
            EventLoop loop;

            {
                MutexLockGuard lock(mutex_);
                loop_ = &loop;
                cond_.notify();
            }
            loop.Loop();
            loop_ = NULL;
        }
    }
    }