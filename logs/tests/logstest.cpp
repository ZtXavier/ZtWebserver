#include<string>
#include<unistd.h>
#include<vector>
#include<iostream>
#include"../Logging.hpp"
#include"../Thread.hpp"

using namespace zvws;
using namespace detail;
using namespace CurrentThread;
void testfunc() {
    for(int i = 0;i < 1000000; i++) {
        LOG_DEBUG << "DEBUG" << i;
        LOG_INFO << "INFO" << i;
        LOG_ERROR << "ERROR" << i;
        LOG_WARN << "WARN" << i;
        LOG_FATAL << "FATAL" << i;
    }
}

int main() {
    testfunc();
    return 0;
}