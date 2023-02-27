#include <iostream>
#include<assert.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include "EventLoop.hpp"
#include "Util.hpp"
#include "base/Logging.hpp"

namespace zvws {
    namespace detail {

        thread_local EventLoop* t_loopInThisThread = 0;

        int CreateEvtfd() {
            int evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
            if(evtfd < 0) {
                LOG  << "Failed in eventfd";
                abort();
            }
            return evtfd;
        }

        EventLoop::EventLoop() 
        : looping_(false),
        poller_(new Epoll()),
        wakeupFd_(CreateEvtfd()),
        quit_(false),
        eventloophandling_(false),
        callingPendingFunctor_(false),
        threadId_(CurrentThread::tid()),
        pwakeupChannel_(new Channel(this,wakeupFd_)) {
            if(t_loopInThisThread) {
                LOG  << "Another EventLoop" << t_loopInThisThread << "exists in the thread" << threadId_;
            } else {
                t_loopInThisThread = this;
            }
            pwakeupChannel_->setEvents(EPOLLIN | EPOLLET);
            pwakeupChannel_->setReadHandler(std::bind(&EventLoop::handleRead,this));
            pwakeupChannel_->setConnHandler(std::bind(&EventLoop::handleConn,this));
            poller_->epoll_add(pwakeupChannel_, 0);
        }

        EventLoop::~EventLoop() {
            close(wakeupFd_);
            t_loopInThisThread = 0;
        }

        void EventLoop::handleConn() {
            updatePoller(pwakeupChannel_, 0);
        }

        void EventLoop::wakeup() {
            uint64_t one = 1;
            ssize_t n = writen(wakeupFd_, (char*)(&one), sizeof one);
            if (n != sizeof one) {
                LOG << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
            }
        }

        void EventLoop::RunInLoop(Functor && cb) {
            if(isInLoopThread()) {
                cb();
            } else {
                QueueInLoop(std::move(cb));
            }
        }


        // 注册每一个回调的方法放到一个向量数组中
        void EventLoop::QueueInLoop(Functor&& cb) {
            {
                MutexLockGuard lock(mutex_);
                pendingFunctors_.emplace_back(std::move(cb));
            }
            if(!isInLoopThread() || callingPendingFunctor_) {
                wakeup();
            }
        }
        // 进入循环
        void EventLoop::Loop() {
            assert(!looping_);
            assert(isInLoopThread());
            looping_ = true;
            quit_ = false;
            std::vector<Spt_Channel> ret;
            while(!quit_) {
                ret.clear();
                ret = poller_->poll();
                eventloophandling_ = true;
                for(auto &it : ret) {
                    it->handleEvents();
                }
                eventloophandling_ = false;
                doPendingFunctors();
                poller_->handleExpired();
            }
            looping_ = false;
        }

        void EventLoop::doPendingFunctors() {
            std::vector<Functor> functors;
            callingPendingFunctor_ = true;
            {
                MutexLockGuard lock(mutex_);
                functors.swap(pendingFunctors_);
            }

            for(size_t i = 0;i < functors.size();i++) {
                functors[i]();
            }
            callingPendingFunctor_ = false;
        }

        void EventLoop::Quit() {
            quit_ = true;
            if(!isInLoopThread()) {
                wakeup();
            }
        }
    }
}