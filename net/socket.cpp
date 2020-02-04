
#include "../base/types.h"
#include "socket.h"
#include "socket_ops.h"
#include "inetaddress.h"

using namespace std;
void socket::test()
{
    test_func(); 
    cout<<"test socket" << endl;
}

socket::~socket()
{
    if(::close((int)sockfd_) < 0)
        printf("close err\n");
}

bool socket::getTcpInfo(tcp_info* tcpi) const
{
    socklen_t len = sizeof(tcpi);
    memZero(tcpi,len);
    return ::getsockopt(sockfd_,IPPROTO_TCP,0 , tcpi, &len) == 0;
}

std::string socket::getTcpInfoString() const
{
//    tcp_info tcpi;
    std::string s("不好用");
    return s;
}

void socket::bindAddress(const inetaddress& localaddr)
{
    sockets::mybind(sockfd_,localaddr.getSockAddr());
}

void socket::listen()
{
    sockets::mylisten(sockfd_);
}

int socket::accept(inetaddress* peeraddr)
{
    struct sockaddr_in6 addr;
    memZero(&addr,sizeof addr);
    int connfd = sockets::myaccept(sockfd_,&addr);
    if(connfd >= 0)
    {
        peeraddr->setSockAddrInet6(addr);
    }
    return connfd;
}

void socket::shutdownWrite()
{
    if(::shutdown(sockfd_,SHUT_WR) < 0)
    {
        printf("%s\n err",__PRETTY_FUNCTION__);
    }
}

void socket::setTcpNoDelay(bool on)
{
    int optval = on ? 1: 0;
    ::setsockopt(sockfd_,IPPROTO_TCP,TCP_NODELAY,&optval,static_cast<socklen_t>(sizeof optval));

}

void socket::setReuseAddr(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_,SOL_SOCKET,SO_REUSEADDR,&optval,static_cast<socklen_t>(sizeof optval));
}


void socket::setReusePort(bool on)
{
    int optval = on ? 1: 0;
    ::setsockopt(sockfd_,SOL_SOCKET,SO_REUSEPORT,&optval,static_cast<socklen_t>(sizeof optval));
}

void socket::setKeepAlive(bool on)
{
    int optval = on ? 1: 0;
    ::setsockopt(sockfd_,SOL_SOCKET,SO_KEEPALIVE,&optval,static_cast<socklen_t>(sizeof optval));

}
