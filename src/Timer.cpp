#include<sys/time.h>
#include<unistd.h>
#include<queue>
#include"Timer.hpp"


namespace zvws {

    namespace detail {

    
TimerNode::TimerNode(std::shared_ptr<Httpsolution> reqdata, int timeout) 
: deleted_(false), Spthttpdata_(reqdata) {
    struct timeval now;
    gettimeofday(&now, NULL);
    expired_ = (((now.tv_sec % 10000) * 1000) + (now.tv_usec  / 1000)) + timeout;
}

TimerNode::~TimerNode() {
    if(Spthttpdata_) {
        Spthttpdata_->handleClose();
    }
}

TimerNode::TimerNode(TimerNode &tn) 
:Spthttpdata_(tn.Spthttpdata_), expired_(0) {

}

void TimerNode::update(int timeout) {
    struct timeval now;
    gettimeofday(&now,NULL);
    expired_ = (((now.tv_sec % 10000) * 1000) + (now.tv_usec / 1000)) + timeout;
}

bool TimerNode::isValid() {
    struct timeval now;
    gettimeofday(&now,NULL);
    size_t tmp = (((now.tv_sec % 10000) * 1000) + (now.tv_usec / 1000));
    if (tmp < expired_) {
        return true;
    } else {
        this->setDeleted();
        return false;
    }
}

void TimerNode::chearReq() {
    Spthttpdata_->reset();
    this->setDeleted();
}

TimerManager::TimerManager() {}
TimerManager::~TimerManager() {}

void TimerManager::addTimer(std::shared_ptr<Httpsolution> Spthttp, int timeout) {
    SptTimerNode new_node(new TimerNode(Spthttp,timeout));
    tnqueue.push(new_node);
    Spthttp->linkTimer(new_node);
}

void TimerManager::handleExpiredEvent() {

    while(!tnqueue.empty()) {
        SptTimerNode sptimer_now = tnqueue.top();
        if(sptimer_now->isDeleted()) {
            tnqueue.pop();
        } else if(sptimer_now->isValid() == false) {
            tnqueue.pop();
        } else {
            break;
        }

    }
}

}
}
