#include <functional>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "Util.hpp"
#include "server.hpp"
#include "../logs/Logging.hpp"

namespace zvws {
    namespace detail {

        Server::Server(EventLoop* loop, int numthreads, int port) 
        : loop_(loop),
        threadNum_(numthreads),
        eventLoopThreadPool_(new EventLoopThreadPool(loop_, numthreads)),
        started_(false),
        acceptChannel_(new Channel(loop_)),
        Port_(port),
        listenFd_(socket_bind_listen(Port_)) {
            handle_for_sigpipe();
            if(setSocketNoBlocking(listenFd_) < 0) {
                perror("set socket non block failed");
                abort();
            }
        }

        void Server::Start() {
            eventLoopThreadPool_->StartPool();
            acceptChannel_->setEvents(EPOLLIN | EPOLLET);
            acceptChannel_->setReadHandler(std::bind(&Server::handNewConn,this));
            acceptChannel_->setConnHandler(std::bind(&Server::handThisConn,this));
            loop_->addToPoller(acceptChannel_, 0);
            started_ = true;
        }


        void Server::handNewConn() {
            struct sockaddr_in client_addr;
            memset(&client_addr, 0,sizeof(struct sockaddr_in));
            socklen_t client_addr_len = sizeof(client_addr);
            int accept_fd = 0;
            while((accept_fd = accept(listenFd_, (struct sockaddr *)&client_addr,  &client_addr_len)) > 0) {
                EventLoop *loop = eventLoopThreadPool_->getNextLoop();
                LOG_INFO << "new connection from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port);
                // 设置tcp保活机制
                int optval = 0;
                socklen_t len_optval = 4;
                getsockopt(accept_fd, SOL_SOCKET, SO_KEEPALIVE, &optval, &len_optval);

                if(accept_fd >= MAXNUMFD) {
                    close(accept_fd);
                    continue;
                }
                if(setSocketNoBlocking(accept_fd) < 0) {
                    LOG_ERROR << "Set noblock failed";
                    return;
                }

                setSocketNodelay(accept_fd);
                setSocketNoLinger(accept_fd);

                std::shared_ptr<Httpsolution> req_info(new Httpsolution(loop, accept_fd));
                req_info->getChannel()->setHolder(req_info);
                loop->QueueInLoop(std::bind(&Httpsolution::newEvent, req_info));
            }
            acceptChannel_->setEvents(EPOLLIN | EPOLLET);
        }
    }
}