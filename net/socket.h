#pragma once
#include "../base/noncopyable.h"
#include <string>
#include <netinet/tcp.h>

#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>


class inetaddress;
class socket : public noncopyable
{
public:
    explicit socket(int sockfd):sockfd_(sockfd) {}
    ~socket();

    int fd() { return sockfd_; }

    bool getTcpInfo(struct tcp_info* tcpinfo) const;
    std::string getTcpInfoString() const;
    void bindAddress(const inetaddress& localaddr);
    void listen();
    int accept(inetaddress* peeraddr);
    void shutdownWrite();

    void setTcpNoDelay(bool on);

    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void setKeepAlive(bool on);
    void test();
private:
    const int sockfd_;
};

