#ifndef CONDITION_HPP_
#define CONDITION_HPP_

#include<pthread.h>
#include<time.h>
#include<cstdint>
#include<errno.h>
#include"../base/Nocopyable.hpp"
#include"MutexLock.hpp"

namespace zvws{
    namespace detail {
    class Condition : Nocopyable {
    public:
        explicit Condition(MutexLock &_mutex):mutex(_mutex) {
            pthread_cond_init(&cond, NULL);
        }
        ~Condition() {
            pthread_cond_destroy(&cond);
        }
        // 等待线程结束
        void wait() {
            pthread_cond_wait(&cond,mutex.get());
        }
        // 唤醒
        void notify() {
            pthread_cond_signal(&cond);
        }
        // 广播全部
        void notifyAll() {
            pthread_cond_broadcast(&cond);
        }
        // 封装指定的等待时间
        bool waitForSecond(int seconds) {
            struct timespec abstime;
            clock_gettime(CLOCK_REALTIME,&abstime);
            abstime.tv_sec += static_cast<time_t>(seconds);
            return ETIMEDOUT == pthread_cond_timedwait(&cond,mutex.get(),&abstime);
        }


    private:
        MutexLock &mutex;
        pthread_cond_t  cond;

    };
    }
}






#endif