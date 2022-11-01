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

}
}
