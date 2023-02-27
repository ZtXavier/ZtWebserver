#include<iostream>
#include<sys/prctl.h>
#include<assert.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include"CurrentThread.hpp"
#include"Thread.hpp"

namespace zvws{
    namespace detail {

    
    namespace CurrentThread {
        thread_local int t_cachedTid = 0;
        thread_local char t_tidString[32];
        thread_local int t_tidStringLength = 6;
        thread_local const char* t_threadName = "defualt";
    }

    pid_t gettid() {
        return static_cast<pid_t>(::syscall(SYS_gettid));
    }

    void CurrentThread::cacheTid() {
        if(t_cachedTid == 0) {
            t_cachedTid = gettid();
            t_tidStringLength = snprintf(t_tidString, sizeof(t_tidString),"%5d",t_cachedTid);
        }
    }

    struct ThreadData {
        typedef Thread::ThreadFunc ThreadFunc;
        ThreadFunc func_;
        std::string name_;
        pid_t* tid_;
        CountDownLatch *latch_; 

        ThreadData(ThreadFunc& func,const std::string& tname,pid_t* tid,CountDownLatch* latch)
        :func_(func),
        name_(tname),
        tid_(tid),
        latch_(latch)
        {
        }

        void runlnThread() {
            // 开始做一个数据的初始化的操作
            *tid_ = CurrentThread::tid();
            tid_ = NULL;
            latch_->countDown();
            latch_ = NULL;
            // 获取当前线程的状态
            CurrentThread::t_threadName = name_.empty() ? "Thread" : name_.c_str();
            //设置进程的名字,在运行的时候可以查看
            ::prctl(PR_SET_NAME,CurrentThread::t_threadName);

            func_();
            CurrentThread::t_threadName = "finished";
        }
    };

    void* startThread(void* obj) {
        ThreadData* data = static_cast<ThreadData*>(obj);
        data->runlnThread();
        delete data;
        return NULL;
    }

    Thread::Thread(const ThreadFunc& func,const std::string& n)
    : started_(false),
    joined_(false),
    pthreadid_(0),
    tid_(0),
    tfunc_(func),
    name_(n),
    latch_(1) {
        setDefaultName();
    }

    Thread::~Thread() {
        if(started_ && !joined_) {
            pthread_detach(pthreadid_);
        }
    }

    void Thread::setDefaultName() {
        if(name_.empty()){
            char buf[32];
            snprintf(buf,sizeof(buf),"Thread");
            name_ = buf;
        }
    }

    void Thread::start() {
        assert(!started_);
        started_ = true;
        ThreadData* data = new ThreadData(tfunc_,name_,&tid_,&latch_);
        if(pthread_create(&pthreadid_,NULL,&startThread,data)) {
            started_ = false;
            delete data;
        } else {
            latch_.wait();
            assert(tid_ > 0);
        }
    }

    int Thread::join() {
        assert(started_);
        assert(!joined_);
        joined_ = true;
        return pthread_join(pthreadid_,NULL);
    }
}

}