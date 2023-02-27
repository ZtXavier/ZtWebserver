#ifndef LOGGING_HPP_
#define LOGGING_HPP_
#include<string.h>
#include<string>
#include<mutex>
#include<errno.h>
#include<functional>


#include"Logstream.hpp"

namespace zvws{

   namespace detail{

        class Logging {
        public:    
            Logging(const char *fileName, int line);
            ~Logging();
            Logstream &stream() { return impl_.stream_; }

        static void setLogFileName(std::string fileName) { logFileName_ = fileName; }
        static std::string getLogFileName() { return logFileName_; }

        private:
        class Impl {
        public:
            Impl(const char *fileName, int line);
            void formatTime();

            Logstream stream_;
            int line_;
            std::string basename_;
        };
        Impl impl_;
        static std::string logFileName_;
};

#define LOG Logging(__FILE__, __LINE__).stream()

   }
}




#endif // LOGGING_H_