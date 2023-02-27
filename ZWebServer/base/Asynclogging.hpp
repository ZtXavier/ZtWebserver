#ifndef _ASYNCLOGGING_HPP_
#define _ASYNCLOGGING_HPP_
#include<functional>
#include<string>
#include<vector>
#include"CountDownLatch.hpp"
#include"Logstream.hpp"
#include"MutexLock.hpp"
#include"Thread.hpp"
#include"Nocopyable.hpp"

namespace zvws{
    namespace detail{

    
        class AsyncLogging : Nocopyable {
            public:
            AsyncLogging(const std::string basename,int flushInterval = 2);
            ~AsyncLogging(){
                if(running_){
                    stop();
                }
            }
            void start() {
                running_ = true;
                thread_.start();
                latch_.wait();
            }

            void stop() {
                running_ = false;
                cond_.notify();
                thread_.join();  
            }

            

            void Async_append(const char* logline,int len);
            private:
                void threadFunc();
                typedef FixedBuffer<kLargerBuffer> Buffer;
                typedef std::vector<std::shared_ptr<Buffer>> BufferVector;
                typedef std::shared_ptr<Buffer> BufferPtr;
                const int flushInterval_;
                bool running_;
                std::string basename_;
                Thread thread_;
                MutexLock mutex_;
                Condition  cond_;
                BufferPtr currentBuffer_;
                BufferPtr nextBuffer_;
                BufferVector buffers_;
                CountDownLatch latch_;
        };
    }


}








#endif