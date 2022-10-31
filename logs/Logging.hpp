#ifndef LOGGING_HPP_
#define LOGGING_HPP_
#include<string.h>
#include<string>
#include<mutex>
#include<errno.h>
#include<functional>

#include"Timestamp.hpp"
#include"Logstream.hpp"
#include"Timezone.hpp"
#include"CurrentThread.hpp"

namespace zvws{

   namespace detail{

            class Logging{
                public:
                    enum LogLevel{
                        DEBUG,
                        INFO,
                        WARN,
                        ERROR,
                        FATAL,
                        NUM_LOG_LEVELS,
                    };

                class FileSource{
                    public:
                    template<int Num>
                    FileSource(const char (&arr)[Num]):data_(arr),size_(Num-1){
                        const char*  point = strrchr(data_,'/');
                        if(point){
                            data_ = point + 1;
                            size_ -= std::distance(arr,data_);     // distance(first,last); first和last的距离
                        }
                    }

                    FileSource(const std::string& filename):data_(filename.c_str()){
                        const char* point = strrchr(data_,'/');
                        if(point){
                            data_ = point + 1;
                        }
                        size_= static_cast<int>(strlen(data_));
                    }
                    const char* data_;
                    int size_;
                };

                Logging(FileSource file, int line);
                Logging(FileSource file, int line, Logging::LogLevel level);
                Logging(FileSource file, int line, LogLevel level, const char* str);
                Logging(FileSource file, int line, bool toAbort);
                ~Logging();

                Logstream& stream() {return funcSrc_.stream_;}

                static LogLevel logLevel();
                inline static void setLogLevel(LogLevel level);
                using OutputFunc = std::function<void(const char*,int)>;
                using FlushFunc = std::function<void(void)>;
                static void setOutput(OutputFunc);
                static void setFlush(FlushFunc);
                static void setTimeZone(const TimeZone& tz);

                private:
                    class FuncSource{
                    public:
                        using Loglevel = Logging::LogLevel;
                        FuncSource(Loglevel level,int old_errno,const FileSource& file,int line);
                        void formatTime();
                        void finish();

                        Timestamp time_;
                        Logstream stream_;
                        Loglevel level_;
                        int line_;
                        FileSource basename_;
                    };
                    FuncSource funcSrc_;
            };

            // 全局日志
            extern Logging::LogLevel g_logLevel;
            inline Logging::LogLevel logLevel(){
                return g_logLevel;
            }

            const char* strerror_tl(int savedErrno);

            #define LOG_DEBUG if(Logging::logLevel() <= Logging::DEBUG) \
            Logging(__FILE__,__LINE__,Logging::DEBUG,__func__).stream()
            #define LOG_INFO if(Logging::logLevel() <= Logging::INFO) \
            Logging(__FILE__,__LINE__,Logging::INFO).stream()
            #define LOG_WARN if(Logging::logLevel() <= Logging::WARN) \
            Logging(__FILE__,__LINE__,Logging::WARN).stream()
            #define LOG_ERROR if(Logging::logLevel() <= Logging::ERROR) \
            Logging(__FILE__,__LINE__,Logging::ERROR).stream()
            #define LOG_FATAL if(Logging::logLevel() <= Logging::FATAL) \
            Logging(__FILE__,__LINE__,Logging::FATAL).stream()
   }
}




#endif // LOGGING_H_