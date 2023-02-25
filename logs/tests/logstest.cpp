#include<string>
#include<unistd.h>
#include<vector>
#include<iostream>
#include"../Logging.hpp"
#include"../Thread.hpp"

using namespace std;
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

void type_test()
{
    // 13 lines
    cout << "----------type test-----------" << endl;
    LOG_INFO << 0;
    LOG_INFO << 1234567890123;
    LOG_INFO << 1.0f;
    LOG_INFO << 3.1415926;
    LOG_INFO << (short) 1;
    LOG_INFO << (long long) 1;
    LOG_INFO << (unsigned int) 1;
    LOG_INFO << (unsigned long) 1;
    LOG_INFO << (long double) 1.6555556;
    LOG_INFO << (unsigned long long) 1;
    LOG_INFO << 'c';
    LOG_INFO << "abcdefg";
    LOG_INFO << string("This is a string");
}


int main() {
    testfunc();
    type_test();
    return 0;
}