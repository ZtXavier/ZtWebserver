#ifndef EPOLL_HPP_
#define EPOLL_HPP_

#include<sys/epoll.h>
#include<memory>
#include<vector>
#include<unordered_map>
#include"Channel.hpp"
#include"Httpsolution.hpp"
#include"Timer.hpp"

namespace zvws {

namespace detail {
class Epoll {
    public:
        Epoll();
        ~Epoll();
        void epoll_add(Spt_Channel request, int timeout);
        void epoll_mod(Spt_Channel requsert, int timeout);
        void epoll_del(Spt_Channel requset);

        std::vector<std::shared_ptr<Channel>> poll();
        std::vector<std::shared_ptr<Channel>> getEventsRequest(int events_num);

        void add_timer(std::shared_ptr<Channel> req_data, int timeout);
        void handleExpired();
        int getEpollfd() {
            return epollfd_;
        }

        private:
            int epollfd_;
            static const int MAX_FD_NUM = 100000;
            std::vector<epoll_event> events_;
            std::shared_ptr<Channel> fdchan_[MAX_FD_NUM];
            std::shared_ptr<Httpsolution> fdhttp_[MAX_FD_NUM];
            TimerManager timermanager_;
};

}
}



#endif