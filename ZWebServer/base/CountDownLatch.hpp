#ifndef COUNTDOWNLATCH_HPP_
#define COUNTDOWNLATCH_HPP_

#include"Condition.hpp"
#include"MutexLock.hpp"
#include"Nocopyable.hpp"


namespace zvws{
    namespace detail{
    class CountDownLatch : Nocopyable {
        public:
            explicit CountDownLatch(int count);
            void wait();
            void countDown();

        private:
            mutable MutexLock mutex_;
            Condition condition_;
            int count_;
    };
    }
}



#endif