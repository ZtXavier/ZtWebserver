#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<functional>
#include"Asynclogging.hpp"
#include"LoggFile.hpp"
#include"Timestamp.hpp"

using namespace std;
namespace zvws{
    namespace detail {
        

        AsyncLogging::AsyncLogging(std::string logFileName, int flushIntervalN)
        :flushInterval_(flushIntervalN),
        running_(false),
        basename_(logFileName),
        thread_(std::bind(&AsyncLogging::threadFunc,this),"Logging"),
        mutex_(),
        cond_(mutex_),
        currentBuffer_(new Buffer),
        nextBuffer_(new Buffer),
        buffers_(),
        latch_(1) {
            assert(logFileName.size() > 1);
            currentBuffer_->setZero();
            nextBuffer_->setZero();
            buffers_.reserve(16);
        }

        void AsyncLogging::Async_append(const char* logline, int len) {
            MutexLockGuard lock(mutex_);
            if( currentBuffer_->avail() > len) {
                currentBuffer_->append(logline, len);
            } else {
                buffers_.push_back(currentBuffer_);
                currentBuffer_.reset();
                if(nextBuffer_) {
                    currentBuffer_ = std::move(nextBuffer_);
                } else {
                    currentBuffer_.reset(new Buffer);
                    currentBuffer_->append(logline, len);
                    cond_.notify();
                }
            }
        }

        void AsyncLogging::threadFunc() {
            assert(running_ == true);
            latch_.countDown();
            LogFile output(basename_);
            BufferPtr newBuffer1(new Buffer);
            BufferPtr newBuffer2(new Buffer);
            newBuffer1->setZero();
            newBuffer2->setZero();
            BufferVector buffersToWrite;
            buffersToWrite.reserve(16);
            while(running_) {
                assert(newBuffer1 && newBuffer1->Length() == 0);
                assert(newBuffer2 && newBuffer2->Length() == 0);
                assert(buffersToWrite.empty());
                {
                    MutexLockGuard lock(mutex_);
                    if(buffers_.empty()) {
                        cond_.waitForSecond(flushInterval_);
                    }
                    buffers_.push_back(currentBuffer_);
                    currentBuffer_.reset();

                    currentBuffer_ = std::move(newBuffer1);
                    buffersToWrite.swap(buffers_);
                    if(!nextBuffer_) {
                        nextBuffer_ = std::move(newBuffer2);
                    }
                }
                assert(!buffersToWrite.empty());

                if(buffersToWrite.size() > 25) {
                    // char buf[256];
                    // snprintf(buf,sizeof(buf),"write log to tmpbuf at %s, %zd larger buffers\n", 
                    // Timestamp::now().toFormattedString().c_str(),
                    // buffersToWrite.size() - 2);
                    // fputs(buf,stderr);
                    // output.append(buf,static_cast<int>(strlen(buf)));
                    buffersToWrite.erase(buffersToWrite.begin() + 2, buffersToWrite.end());
                }
                for(const auto& buffer : buffersToWrite) {
                    output.append(buffer->data(), buffer->Length());
                }

                if(buffersToWrite.size() > 2) {
                    buffersToWrite.resize(2);
                }

                
                if(!newBuffer1) {
                    assert(!buffersToWrite.empty());
                    newBuffer1 = buffersToWrite.back();
                    buffersToWrite.pop_back();
                    newBuffer1->reset();
                }

                if(!newBuffer2) {
                    assert(!buffersToWrite.empty());
                    newBuffer2 = buffersToWrite.back();
                    buffersToWrite.pop_back();
                    newBuffer2->reset();
                }

                buffersToWrite.clear();
                output.flush();
            }
            output.flush();
        }
    }
}

