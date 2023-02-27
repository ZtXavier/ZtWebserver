#include"Fileappend.hpp"
#include"Logging.hpp"
#include<assert.h>

namespace zvws{
    namespace detail{
FileAppend::FileAppend(const std::string& filename):fp_(::fopen(filename.c_str(),"ae")),writebytes_(0) {
    assert(fp_);
    ::setbuffer(fp_,buffer_,sizeof(buffer_));
}

FileAppend::~FileAppend() {
    fclose(fp_);
}

void 
FileAppend::Append(const char* logline,size_t loglen){
    size_t n = Write(logline,loglen);
    size_t remain = loglen - n;
    while(remain > 0) { 
        // 下面来写入文件,利用循环来判断是否文件全部写入
        size_t writebytes = Write(logline + n,remain);
        if(writebytes == 0){
            int err = ferror(fp_);
            if(err){
                fprintf(stderr,"FileAppend::append() faild\n");
            }
        }
        n += writebytes;
        remain = loglen - n;
    }
}

    void FileAppend::flush() { fflush(fp_); }
    size_t FileAppend::Write(const char* logline, size_t len){
        return fwrite_unlocked(logline, 1, len, fp_);
    }

}
}
