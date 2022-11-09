#include<string.h>
#include<stdio.h>
#include<errno.h>
#include<sstream>
#include"Logging.hpp"

#include<assert.h>
#include<stdlib.h>
#include<cstdio>

namespace zvws{
    namespace detail{

        thread_local char t_errnoBuf[512];
        thread_local char t_time[64];
        thread_local time_t t_lastSecond;


        const char* strerror_tl(int savedErrno)
        {
            return strerror_r(savedErrno,t_errnoBuf,sizeof(t_errnoBuf));
        }

        Logging::LogLevel initLogLevel(){

                if (::getenv("LOG_DEBUG"))
                    return Logging::DEBUG;
                else
                    return Logging::INFO;
        }

    void defaultOutput(const char* msg, int len){
        size_t n = fwrite(msg, 1, len, stdout);
        (void)n;
    }

void defaultFlush(){
    fflush(stdout);
}

    Logging::LogLevel g_logLevel = initLogLevel();
    Logging::OutputFunc g_output_(defaultOutput);
    Logging::FlushFunc g_flush_(defaultFlush);
    TimeZone g_logTimeZone;

    constexpr const char* LogLevelName[Logging::NUM_LOG_LEVELS] = {
        "DEBUG ",
        "INFO  ",
        "WARN  ",
        "ERROR ",
        "FATAL ",
    };

    //TODO helper class for known string length at compile time
    struct helper {
    constexpr helper(const char* str, unsigned len)
    :str_(str),
    len_(len) {
        assert(strlen(str) == len_);
    }

    const char* str_;
    const unsigned len_;
    };

    inline Logstream& operator<<(Logstream& s, const helper& v){
    s.append(v.str_, v.len_);
    return s;
    }

    inline Logstream& operator<<(Logstream& s, const Logging::FileSource& v){
    s.append(v.data_ , v.size_);
    return s;
    }

    void Logging::setLogLevel(LogLevel level) {
    g_logLevel = level;
    }

    void Logging::setFlush(FlushFunc fun) {
    g_flush_ = fun;
    }

    void Logging::setOutput(OutputFunc fun) {
    g_output_ = fun;
    }

    void Logging::setTimeZone(const TimeZone& tz){
    g_logTimeZone = tz;
    }

    Logging::Logging(FileSource file, int line)
    :funcSrc_(INFO, 0 ,file, line){}

    Logging::Logging(FileSource file, int line, Logging::LogLevel level)
    : funcSrc_(level, 0, file, line){}

    Logging::Logging(FileSource file, int line, LogLevel level, const char* str)
    : funcSrc_(level, 0, file, line){
    funcSrc_.stream_ << str << " ";
    }

    Logging::Logging(FileSource file, int line, bool toAbort)
    : funcSrc_(toAbort?FATAL:ERROR, errno, file, line){}

    Logging::~Logging(){
    funcSrc_.finish();
    const Logstream::Buffer& buf(stream().buffer());
    g_output_(buf.data(),buf.Length());
    if(funcSrc_.level_ == FATAL){
        g_flush_();
        abort();
    }
    }

    Logging::FuncSource::FuncSource(LogLevel level, int old_errno,
                                const FileSource &file, int line)
    : time_(Timestamp::now()),
    stream_(),
    level_(level), 
    line_(line),
    basename_(file){
        
    formatTime();

    CurrentThread::tid();
    stream_ << helper(CurrentThread::tidString(), CurrentThread::tidStringLength());
    stream_ << helper(LogLevelName[static_cast<size_t>(level)], 6);
    if (old_errno != 0)
    {
        stream_ << strerror_tl(old_errno) << " (errno=" << old_errno << ") ";
    }
    }

    void Logging::FuncSource::formatTime() {
    int64_t microSecondsSinceEpoch = time_.Data_microsecond();
    time_t seconds = static_cast<time_t>(microSecondsSinceEpoch / Timestamp::KmicroSecond);
    int microseconds = static_cast<int>(microSecondsSinceEpoch % Timestamp::KmicroSecond);
    struct tm tm_time;
    if (seconds != t_lastSecond){ //秒存储 只更新微秒 这就是优化的地方
        t_lastSecond = seconds;
        if (g_logTimeZone.valid()){
            tm_time = g_logTimeZone.toLocalTime(seconds);
        }else{
            ::gmtime_r(&seconds, &tm_time); // FIXME TimeZone::fromUtcTime
        }

        int len = snprintf(t_time, sizeof(t_time), "%4d%02d%02d %02d:%02d:%02d",
                            tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                            tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
        assert(len == 17); (void)len;
    }

    if (g_logTimeZone.valid()){
        Fmt us(".%06d ", microseconds);
        assert(us.length() == 8);
        stream_ << helper(t_time, 17) << helper(us.data(), 8);
    }else{
        Fmt us(".%06dZ ", microseconds);
        assert(us.length() == 9);
        stream_ << helper(t_time, 17) << helper(us.data(), 9);
    }
    }

    void Logging::FuncSource::finish(){
        stream_ << " - " << basename_ << ':' << line_ << " - ";
    }

}
}