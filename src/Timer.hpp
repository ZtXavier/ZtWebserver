#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <unistd.h>
#include <deque>
#include <memory>
#include <queue>
#include "Httpsolution.hpp"
#include "../base/Nocopyable.hpp"
#include "../logs/MutexLock.hpp"

class TimerNode {
    public:
        TimerNode(std::shared_ptr<Httpsolution> requestData,int timeout);
        ~TimerNode();

        TimerNode(TimerNode &timenode);
        void update(int timeout);
        void chearReq();
        void setDeleted() { deleted_ = true; }
        bool isValid();
        bool isDeleted() const {return deleted_; }
        size_t getExpTime() const { return expired_; }
    private:
        bool deleted_;
        size_t expired_;
        std::shared_ptr<Httpsolution> sharedptrhttpdata_;
};




#endif