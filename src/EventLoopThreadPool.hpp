#ifndef EVENTLOOPTHREADPOOL_HPP_
#define EVENTLOOPTHREADPOOL_HPP_

#include <vector>
#include <memory>
#include "EventLoopThread.hpp"
#include "../logs/Logging.hpp"
#include "../base/Nocopyable.hpp"


namespace zvws {

    namespace detail {
        class EventLoopThreadPool : Nocopyable {
            public:
                EventLoopThreadPool(EventLoop* baseLoop, int numThreads);
                ~EventLoopThreadPool() {
                    LOG_INFO << "exit EventLoopThreadPool";
                }
                void StartPool();
                EventLoop* getNextLoop();
            private:
                EventLoop* baseLoop_;
                bool started_;
                int next_;
                int numThreads_;
                std::vector<std::shared_ptr<EventLoopThread>> threads_;
                std::vector<EventLoop*> loops_;
        };
    }
}






#endif