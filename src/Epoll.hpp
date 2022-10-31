#ifndef EPOLL_HPP_
#define EPOLL_HPP_

#include<sys/epoll.h>
#include<memory>
#include<vector>
#include<unordered_map>
#include"Channel.hpp"
#include"Httpsolution.hpp"
#include"Timer.hpp"


class Epoll {
    public:
        Epoll();
        ~Epoll();
        void epoll_add();
};


#endif