#include<string.h>
#include<stdio.h>
#include<errno.h>
#include<sstream>
#include<assert.h>
#include<stdlib.h>
#include<cstdio>
#include<time.h>
#include<sys/time.h>
#include"Logging.hpp"
#include"Asynclogging.hpp"
#include"Thread.hpp"
#include"CurrentThread.hpp"



namespace zvws{
    namespace detail{

    static pthread_once_t once_control_ = PTHREAD_ONCE_INIT;
    static AsyncLogging *AsyncLogging_;

    std::string Logging::logFileName_ = "./ZtWebServer.log";

        void once_init() {
            AsyncLogging_ = new AsyncLogging(Logging::getLogFileName());
            AsyncLogging_->start(); 
        }

        void output(const char* msg, int len) {
            pthread_once(&once_control_, once_init);
            AsyncLogging_->Async_append(msg, len);
        }

       Logging::Impl::Impl(const char *fileName, int line)
        : stream_(),
            line_(line),
            basename_(fileName) {
            formatTime();
        }

        void Logging::Impl::formatTime() {
            struct timeval tv;
            time_t time;
            char str_t[26] = {0};
            gettimeofday(&tv, NULL);
            time = tv.tv_sec;
            struct tm* p_time = localtime(&time);   
            strftime(str_t, 26, "%Y-%m-%d %H:%M:%S\n", p_time);
            stream_ << str_t;
        }

       Logging::Logging(const char *fileName, int line)
        : impl_(fileName, line)
        { }

       Logging::~Logging() {
            impl_.stream_ << " -- " << impl_.basename_ << ':' << impl_.line_ << '\n';
            const Logstream::Buffer& buf(stream().buffer());
            output(buf.data(), buf.Length());
        }

}
}