#include <iostream>
#include <string>
#include "Server.hpp"
#include "EventLoop.hpp"
#include "base/Logging.hpp"


        using namespace zvws;
        using namespace detail;
        using namespace CurrentThread;

        int main(int argc ,char *argv[]) {
            int thread_num = 4;
            int default_port = 8888;
            std::string logPath = "./ZtWebserver.log";
            int option;
            const char *str = "t:l:p:";
            while((option = getopt(argc, argv, str)) != -1) {
                switch(option) {
                    case 't' : {
                        thread_num = atoi(optarg);
                        break;
                    }
                    case 'l' : {
                        logPath = optarg;
                        if(logPath.size() < 2 || optarg[0] != '/') {
                            std::cout << "error input your path!!!\n";
                            abort();
                        }
                        break;
                    }

                    case 'p' : {
                        default_port = atoi(optarg);
                        break;
                    }
                    default : break;
                }
            }
            Logging::setLogFileName(logPath);
            #ifndef _PTHREADS
            LOG << "_PTHREADS is not defined !";
            #endif
            EventLoop mlooper;
            Server WebServer(&mlooper, thread_num, default_port);
            WebServer.Start();
            mlooper.Loop();
            return 0;
        }
