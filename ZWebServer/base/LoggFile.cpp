#include<stdio.h>
#include<time.h>
#include<assert.h>
#include"LoggFile.hpp"
#include"Fileappend.hpp"

namespace zvws {
    namespace detail {
        LogFile::LogFile(const std::string& basename,int flushintervalN)
        : basename_(basename),
        flushintervalN_(flushintervalN),
        count_(0),
        mutex_(new MutexLock) {
            file_.reset(new FileAppend(basename));
        }

        LogFile::~LogFile() {}

        void LogFile::append(const char* logline,int len) {
            // 思考线程安全的问题
            MutexLockGuard lock(*mutex_);
            file_->flush();
        }

        void LogFile::append_unlocked(const char* logline,int len) {
            file_->Append(logline,len);
            ++count_;
            if(count_ >= flushintervalN_) {
                count_ = 0;
                file_->flush();
            }
        }
    }
}