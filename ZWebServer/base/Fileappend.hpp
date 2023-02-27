#ifndef _FILEAPPEND_HPP_
#define _FILEAPPEND_HPP_

#include<string>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>
#include<iostream>
#include<errno.h>
#include"Nocopyable.hpp"

namespace zvws{
    namespace detail{

class FileAppend : public Nocopyable{
    public:
        explicit FileAppend(const std::string &file);

        ~FileAppend();

        void flush();

        void Append(const char* logline,size_t loglen);

        off_t HaveWtbytes();



    private:
        FILE *fp_;
        off_t writebytes_;
        char buffer_[64*1024];
        size_t Write(const char* logline,size_t loglen);
};

    }
}


#endif //FILEAPPEND_H_