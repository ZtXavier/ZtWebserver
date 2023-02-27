#include<iostream>
#include<vector>
#include<deque>
#include<queue>
#include<assert.h>
#include<errno.h>
#include<string>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include"base/Logging.hpp"
#include"Epoll.hpp"
#include"Util.hpp"


namespace zvws {
    namespace detail {
        const int EventNums = 4096;
        const int EpollWait_Time = 10000;

        Epoll::Epoll() : epollfd_(epoll_create1(EPOLL_CLOEXEC)),events_(EventNums) {
            assert(epollfd_ > 0);
        }

        Epoll::~Epoll() {

        }

        void Epoll::epoll_add(Spt_Channel request, int timeout) {
            int fd = request->getFd();
            if(timeout > 0) {
                add_timer(request, timeout);
                // 拿到指针保存在数组中
                fdhttp_[fd]  = request->getHolder();
            }
            struct epoll_event event;
            event.data.fd = fd;
            event.events = request->getEvents();
            request->EqualAndUpdateLastEvents();
            fdchan_[fd] = request;

            if(epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &event) < 0) {
                perror("epoll_add error");
                fdchan_[fd].reset();
            }
        }

        void Epoll::epoll_mod(Spt_Channel request, int timeout) {
            if(timeout > 0) {
                add_timer(request, timeout);
            }
            int fd = request->getFd();
            if(!request->EqualAndUpdateLastEvents()) {
                struct epoll_event event;
                event.data.fd = fd;
                event.events = request->getEvents();
                if(epoll_ctl(epollfd_,  EPOLL_CTL_MOD, fd, &event) < 0) {
                    perror("epoll_mod error");
                    fdchan_[fd].reset();
                }
            }
        }

        void Epoll::epoll_del(Spt_Channel request) {
            int fd = request->getFd();
            struct epoll_event event;
            event.data.fd = fd;
            event.events = request->getLastEvents();
            if(epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, &event) < 0) {
                perror("epoll_del error");
            }
            fdchan_[fd].reset();
            fdhttp_[fd].reset();
        }

        std::vector<Spt_Channel> Epoll::poll() {
            while(true) {
                int event_count = epoll_wait(epollfd_, &*events_.begin(), events_.size(), EpollWait_Time);
                if (event_count < 0) {
                    perror("epoll_wait error");
                }
                std::vector<Spt_Channel> req_data = getEventsRequest(event_count);
                if(req_data.size() > 0) {
                    return req_data;
                }
            }
        }

        void Epoll::handleExpired() {
            timermanager_.handleExpiredEvent();
        }

        std::vector<Spt_Channel> Epoll::getEventsRequest(int events_nums) {
            std::vector<Spt_Channel> req_data;
            for(int i = 0; i < events_nums; i++) {
                int fd = events_[i].data.fd;
                Spt_Channel cur_req = fdchan_[fd];
                if(cur_req) {
                    cur_req->setRevents(events_[i].events);
                    cur_req ->setEvents(0);
                    // TODO
                    //cur_req->seperateTimer()
                    req_data.push_back(cur_req);
                } else {
                    LOG << "Spt cur_req invalid";
                }
            }
            return req_data;
        }

        void Epoll::add_timer(Spt_Channel req_data, int timeout) {
            std::shared_ptr<Httpsolution> httpsln = req_data->getHolder();
            if(httpsln) {
                timermanager_.addTimer(httpsln, timeout);
            } else {
                LOG  << "timer add failed";
            }
        }
    }
}



