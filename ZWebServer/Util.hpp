#ifndef UTIL_HPP_
#define UTIL_HPP_
#include<string>
#include<cstdlib>

ssize_t readn(int fd, void *buff, size_t n);
ssize_t readn(int fd, std::string &inBuffer, bool &zero);
ssize_t readn(int fd, std::string &inBuffer);
ssize_t writen(int fd, void *buff, size_t n);
ssize_t writen(int fd ,std::string &buff);

void handle_for_sigpipe();
void shutDownWrite(int fd);
void setSocketNodelay(int fd);
void setSocketNoLinger(int fd);
int setSocketNoBlocking(int fd);
int socket_bind_listen(int port);

#endif