#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <unistd.h>
#include <deque>
#include <memory>
#include <queue>
#include "Httpsolution.hpp"
#include "base/Nocopyable.hpp"
#include "base/MutexLock.hpp"

namespace zvws {

namespace detail {


class Httpsolution;

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
        std::shared_ptr<Httpsolution> Spthttpdata_;
};

struct TimerCmp {
    bool operator() (
        std::shared_ptr<TimerNode> &a, std::shared_ptr<TimerNode> &b) const {
            return a->getExpTime() > b->getExpTime();
        }
};


class TimerManager {
    public:
        TimerManager();
        ~TimerManager();

        void addTimer(std::shared_ptr<Httpsolution> SptHttp,int timeout);
        void handleExpiredEvent();

        private:
        // 使用智能指针进行封装类,放入优先队列中
        typedef std::shared_ptr<TimerNode> SptTimerNode;
        std::priority_queue<SptTimerNode, std::deque<SptTimerNode>, TimerCmp> tnqueue;
};

}
}

#endif