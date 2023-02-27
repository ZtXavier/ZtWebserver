#include"Util.hpp"
#include<errno.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<netinet/tcp.h>
#include<signal.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>



const int MAX_BUFFSIZE = 4096;
// 底层的read和write的包装,参考muduo
ssize_t readn(int fd,void *buff,size_t n) {
    size_t nleft = n;
    ssize_t nread = 0;
    ssize_t readSum = 0;
    char *ptr = (char*)buff;
    while(nleft > 0) {
        if((nread =  read(fd,ptr,nleft)) < 0) {
            //写的过程中遇到了中断或者非阻塞中一直读一个空的buffer
            if(errno == EINTR) {
                nread = 0;
            } else if(errno == EAGAIN) {
                return readSum;
            } else {
                return -1;
            }
        } else if (nread == 0) {
            break;
        }
        readSum += nread;
        nleft -= nread;
        ptr += nread;
    }
    return readSum;
}

ssize_t readn(int fd, std::string &inbuffer, bool &zero) {
    size_t nread = 0;
    ssize_t readSum = 0;
    while(true) {
        char buffer[MAX_BUFFSIZE];
        if((nread =  read(fd,buffer,MAX_BUFFSIZE)) < 0) {
            //写的过程中遇到了中断或者非阻塞中一直读一个空的buffer
            if(errno == EINTR) {
                continue;
            } else if(errno == EAGAIN) {
                return readSum;
            } else {
                return -1;
            }
        } else if (nread == 0) {
            zero = true;
            break;
        }
        readSum += nread;
       inbuffer += std::string(buffer,buffer + nread);
       printf("the size of inbuffer is%zu\n",inbuffer.size());
    }
    return readSum;
}

ssize_t readn(int fd, std::string &inbuffer) {
    size_t nread = 0;
    ssize_t readSum = 0;
    while(true) {
        char buffer[MAX_BUFFSIZE];
        if((nread =  read(fd,buffer,MAX_BUFFSIZE)) < 0) {
            //写的过程中遇到了中断或者非阻塞中一直读一个空的buffer
            if(errno == EINTR) {
                continue;
            } else if(errno == EAGAIN) {
                return readSum;
            } else {
                return -1;
            }
        } else if (nread == 0) {
            break;
        }
        readSum += nread;
       inbuffer += std::string(buffer,buffer + nread);
       printf("the size of inbuffer is%zu\n",inbuffer.size());
    }
    return readSum;
}

ssize_t writen(int fd, void *buff, size_t n) {
    size_t nleft = n;
    ssize_t nwritten = 0;
    ssize_t writeSum = 0;
    char *ptr = (char *)buff;
    while (nleft > 0) {
        if((nwritten = write(fd, ptr, nleft)) < 0) {
            if(nwritten < 0) {
                if(errno == EINTR) {
                    nwritten = 0;
                    continue;
                } else if(errno == EAGAIN) {
                    return writeSum;
                } else {
                    return -1;
                }
            }
        }
        writeSum += nwritten;
        nleft -= nwritten;
        ptr += nwritten;
    }
    return writeSum;
}

ssize_t writen(int fd,std::string &buffer) {
    size_t nleft = buffer.size();
    ssize_t nwritten = 0;
    ssize_t writeSum = 0;
    const char *ptr = buffer.c_str(); 
    while (nleft > 0) {
        if((nwritten = write(fd, ptr, nleft)) <= 0) {
            if(nwritten < 0) {
                if(errno == EINTR) {
                    nwritten = 0;
                    continue;
                } else if(errno == EAGAIN) {
                    return writeSum;
                } else {
                    return -1;
                }
            }
        }
        writeSum += nwritten;
        nleft -= nwritten;
        ptr += nwritten;
    }
    if(writeSum == static_cast<int>(buffer.size())) {
        buffer.clear();
    } else {
        // 写入多少截取多少
        buffer = buffer.substr(writeSum);
    }
    return writeSum;
}

void handle_for_sigpipe() {
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    if (sigaction(SIGPIPE, &sa, NULL)) {
        return;
    }
}

// 设置非阻塞套接字,能够立刻返回
int setSocketNoBlocking(int fd) {
    int flag = fcntl(fd, F_GETFL, 0);
    if(flag == -1) return -1;
    
    flag |= O_NONBLOCK;
    if(fcntl(fd, F_SETFL, flag) == -1) {
        return -1;
    }
    return 0;
}

// 不设置缓冲区,目的是将来一个包就发送出去,默认是有一个缓冲区,使用nagle算法来将多个包粘在一起一起发送
void setSocketNodelay(int fd) {
    int enable = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void *)&enable, sizeof(enable));
}


// 设置延迟关闭,为了在断开链接后能将剩余的包发送完
void setSocketNoLinger(int fd) {
    struct linger linger_;
    linger_.l_onoff = 1;
    linger_.l_linger = 10;
    setsockopt(fd, SOL_SOCKET, SO_LINGER, (const char*)&linger_,sizeof(linger_));
}

void shutDownWrite(int fd) {
    shutdown(fd,SHUT_WR);
}

int socket_bind_listen(int port) {
    // 检查port值的范围
    if(port < 0 || port > 66535) {
        return -1;
    }

    int listen_fd = 0;
    if((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        return -1;
    }
    int optval = 1;
    if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR,&optval,sizeof(optval)) == -1) {
        close(listen_fd);
        return -1;
    }

    // 设置服务器ip和port,监听绑定
    struct sockaddr_in server_addr;
    bzero((char*)&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((unsigned short)port);
    if(bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        close(listen_fd);
        return -1;
    }

    //设置最长监听队列
    if(listen(listen_fd, 1024) == -1) {
        close(listen_fd);
        return -1;
    }

    if(listen_fd == -1) {
        close(listen_fd);
        return -1;
    }

    return listen_fd;
}