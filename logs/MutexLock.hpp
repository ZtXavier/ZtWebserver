#ifndef MUTEXLOCK_HPP_
#define MUTEXLOCK_HPP_

#include<pthread.h>
#include<cstdint>
#include"../base/Nocopyable.hpp"


namespace zvws{
    namespace detail {
    class MutexLock : Nocopyable {
        public:
            MutexLock() {
                pthread_mutex_init(&mutex,NULL);
            }
            ~MutexLock() {
                pthread_mutex_lock(&mutex);
                pthread_mutex_destroy(&mutex);
            }

            void lock() {
                pthread_mutex_lock(&mutex);
            }

            void unlock() {
                pthread_mutex_unlock(&mutex);
            }

            pthread_mutex_t *get() {
                return &mutex;
            }


        private:
            pthread_mutex_t  mutex;
        private:
            friend class Condition;

    };

    class MutexLockGuard : Nocopyable {
        public:
        explicit MutexLockGuard(MutexLock &mutex_):mutex(mutex_) {
            mutex.lock();
        }
        ~MutexLockGuard() {
            mutex.unlock();
        }

        private:
            MutexLock &mutex;
    };
    }
}




#endif