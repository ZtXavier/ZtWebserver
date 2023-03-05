#ifndef THREADPOOL_HPP_
#define THREADPOOL_HPP_
#include"Channel.hpp"
#include <pthread.h>
#include <functional>
#include <memory>
#include <vector>

namespace zvws {
    namespace detail {
        
        #define THREADPOOL_INVALID   -1
        #define THREADPOOL_LOCK_FAILURE  -2
        #define THREADPOOL_QUEUE_FULL  -3
        #define THREADPOOL_SHUTDOWN  -4
        #define THREADPOOL_THREAD_FAILURE  -5
        #define THREADPOOL_GRACEFUL  1

        #define MAX_THREADS  1024
        #define MAX_QUEUE  66535

        typedef enum {
            immediate_shutdown = 1,
            graceful_shutdown = 2
        }ShutDownOption;

        struct ThreadPoolTask {
            std::function<void(std::shared_ptr<void>)> func;
            std::shared_ptr<void> args;
        };

        class ThreadPool {
            public:
                static int threadpool_create(int threadnums, int queuesize);
                static int threadpool_add(std::shared_ptr<void> args, std::function<void(std::shared_ptr<void>)> func);
                static int threadpool_destroy(ShutDownOption shutdownoption = graceful_shutdown);
                static int threadpool_free();
                static void* threadpool_thread(void* argc);
                // std::function<void*(void*)> Callback;

            private:
                static pthread_mutex_t mutex_;
                static pthread_cond_t cond_;

                static std::vector<pthread_t> threads;
                static std::vector<ThreadPoolTask> queue;


                static int thread_count;
                static int queue_size;
                static int head;
                static int tail;
                static int count;
                static int shutdown;
                static int started;
        };
    }
}



#endif