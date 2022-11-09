#include <iostream>
#include <string>
#include "./src/server.hpp"
#include "./src/EventLoop.hpp"
#include "./logs/Logging.hpp"


namespace zvws {
    namespace detail {

        int main(int argc ,char *argv[]) {
            int thread_num = 4;
            int default_port = 8888;
            std::string logPath = "./logs/ZtWebserver.log";
            int option;
            const char *str = "t:l:p";
            while((option = getopt(argc, argv, str)) != -1) {
                switch(option) {
                    case 't' : {
                        thread_num = atoi(optarg);
                        break;
                    }
                    case 'l' : {
                        logPath = optarg;
                        if(logPath.size() < 2|| optarg[0] != '/') {
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
            EventLoop mlooper;
            Server WebServer(&mlooper, thread_num, default_port);
            WebServer.Start();
            mlooper.Loop();
            return 0;
        }
    }
}