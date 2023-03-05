#include <iostream>
#include <vector>
#include <pthread.h>
#include "ThreadPool.hpp"



namespace zvws {
    namespace detail {
        
        pthread_mutex_t ThreadPool::mutex_ = PTHREAD_MUTEX_INITIALIZER;
        pthread_cond_t  ThreadPool::cond_ = PTHREAD_COND_INITIALIZER;
        std::vector<pthread_t> ThreadPool::threads;
        std::vector<ThreadPoolTask> ThreadPool::queue;

        int ThreadPool::thread_count = 0;
        int ThreadPool::queue_size = 0;
        int ThreadPool::head = 0;
        int ThreadPool::tail = 0;
        int ThreadPool::count = 0;
        int ThreadPool::shutdown = 0;
        int ThreadPool::started = 0;

        // 每一个线程的内部需要做一个死循环来执行事件的处理
        void* ThreadPool::threadpool_thread(void* args) {
            while(true) {
                ThreadPoolTask task;
                pthread_mutex_lock(&mutex_);
                while((count == 0) && (!shutdown)) {
                    pthread_cond_wait(&cond_,&mutex_);
                }
                if((shutdown == immediate_shutdown) || ((shutdown == graceful_shutdown) && (count == 0))) {
                    break;
                }
                task.func = queue[head].func;
                task.args = queue[head].args;
                queue[head].func = NULL;
                queue[head].args.reset();
                head = (head + 1) % queue_size;
                --count;
                pthread_mutex_unlock(&mutex_);
                (task.func)(task.args);
            }
            --started;
            pthread_mutex_unlock(&mutex_);\
            std::cout << "This id = " << gettid() << " and " << std::endl;;
            pthread_exit(NULL);
            return 0;
        }


        int ThreadPool::threadpool_create(int threadcount, int queuesize) {
            bool error = false;
            do {
                if(threadcount || threadcount > MAX_THREADS || queuesize > MAX_QUEUE) {
                    thread_count = 10;
                    queue_size = 1024;
                }

                threadcount = 0;
                queuesize = queue_size;
                head = tail = count = 0;
                shutdown = started = 0;

                threads.resize(thread_count);
                queue.resize(queue_size);

                for(int i = 0; i < thread_count; ++i) {
                    if(pthread_create(&threads[i], NULL, threadpool_thread, (void*)(0)) != 0) {
                        return -1;
                    }
                    ++thread_count;
                    ++started;
                }
            }while(0);

            if(error) {
                return -1;
            }
            return 0;
        }

        int ThreadPool::threadpool_add(std::shared_ptr<void> args, std::function<void(std::shared_ptr<void>)> func) {
            int next, error = 0;
            if(pthread_mutex_lock(&mutex_) != 0) {
                return THREADPOOL_LOCK_FAILURE;
            }
            do {
                next = (tail + 1) % queue_size;
                if(count == queue_size) {
                    error = THREADPOOL_QUEUE_FULL;
                    break;
                }
                if(shutdown) {
                    error = THREADPOOL_SHUTDOWN;
                    break;
                }
                queue[tail].func = func;
                queue[tail].args = args;
                tail = next;
                ++count;

                if(pthread_cond_signal(&cond_) != 0) {
                    error = THREADPOOL_LOCK_FAILURE;
                    break;
                }

            }while(0);
            if(pthread_mutex_unlock(&mutex_) != 0) {
                error = THREADPOOL_LOCK_FAILURE;
            }
            return error;
        }

        int ThreadPool::threadpool_destroy(ShutDownOption stdopt) {
            printf("Thread pool destroy! \n");
            int i, error = 0;
            
            if(pthread_mutex_lock(&mutex_) != 0) {
                return THREADPOOL_LOCK_FAILURE;
            }
            do {
                if(shutdown) {
                    error = THREADPOOL_SHUTDOWN;
                    break;
                }
                shutdown = stdopt;
                if((pthread_cond_broadcast(&cond_) != 0) || (pthread_mutex_unlock(&mutex_) != 0)) {
                    error = THREADPOOL_LOCK_FAILURE;
                    break;
                }
                for(i = 0; i < thread_count; ++i) {
                    if(pthread_join(threads[i], NULL) != 0) {
                        error = THREADPOOL_THREAD_FAILURE;
                    }
                }
            }while(0);
            
            if(!error) {
                threadpool_free();
            }

            return error;
        }

        int ThreadPool::threadpool_free() {
            if(started > 0) {
                return -1;
            }
            pthread_mutex_lock(&mutex_);
            pthread_mutex_destroy(&mutex_);
            pthread_cond_destroy(&cond_);
            return 0;
        }


    }
}