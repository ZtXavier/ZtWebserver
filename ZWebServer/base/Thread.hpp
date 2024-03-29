#ifndef THREAD_HPP_
#define THREAD_HPP_
#include<pthread.h>
#include<sys/syscall.h>
#include<unistd.h>
#include<functional>
#include<string>
#include<memory>
#include"Nocopyable.hpp"
#include"CountDownLatch.hpp"


namespace zvws{
    namespace detail{

    
    class Thread : Nocopyable {
        public:
            typedef std::function<void()> ThreadFunc;
            explicit Thread(const ThreadFunc&,const std::string & tname = std::string());
            ~Thread();
            void start();
            int join();
            const std::string& name() const { return name_;}
            pid_t tid() const {return tid_;}
            bool started() const {return started_;}

        private:
            bool started_;
            bool joined_;
            void setDefaultName();
            pthread_t   pthreadid_;
            pid_t   tid_;
            ThreadFunc tfunc_;
            std::string name_;
            CountDownLatch  latch_;
    };
    }
}






#endif