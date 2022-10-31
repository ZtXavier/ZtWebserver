#ifndef ASYNCLOGGING_HPP_
#define ASYNCLOGGING_HPP_
#include<functional>
#include<string>
#include<vector>
#include"CountDownLatch.hpp"
#include"Logstream.hpp"
#include"MutexLock.hpp"
#include"Thread.hpp"
#include"../base/Nocopyable.hpp"

namespace zvws{
    namespace detail{

    
        class AsyncLogging : Nocopyable {
            AsyncLogging(const std::string basename,int flushInterval = 2);
            ~AsyncLogging(){
                if(running_){
                    stop();
                }
            }

            void stop() {
                running_ = false;
                cond_.notify();
                thread_.join();  
            }

            void Async_append(const char* logline,int len);
            
            void start() {
                running_ = true;
                thread_.start();
                latch_.wait();
            }

            private:
                void threadFunc();
                typedef FixedBuffer<LargerBuffer> Buffer;
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