#ifndef _LOGGFILE_HPP_
#define _LOGGFILE_HPP_
#include<string>
#include<memory>
#include"Fileappend.hpp"
#include"MutexLock.hpp"
#include"Nocopyable.hpp"

namespace zvws {
    namespace detail {
class LogFile : Nocopyable {
    public:
        LogFile(const std::string& basename,int flushintervalN = 1024);
        ~LogFile();

        void append(const char* logline,int len);
        void flush();
        void rollFile();


    private:
        void append_unlocked(const char* logline,int len);

        const std::string basename_;
        const int flushintervalN_;
        int count_;
        std::unique_ptr<MutexLock> mutex_;
        std::unique_ptr<FileAppend> file_;
};
}

}


#endif







