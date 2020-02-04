#include "../base/types.h"
#include "socket_ops.h"
#include "endian.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/uio.h>
//#include <unistd.h>
#include <iostream>

const struct sockaddr*  sockets::sockaddr_cast(const struct sockaddr_in6* addr)
{
  return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
}

struct sockaddr* sockets::sockaddr_cast(struct sockaddr_in6* addr)
{
  return static_cast<struct sockaddr*>(implicit_cast<void*>(addr));
}

const struct sockaddr*  sockets::sockaddr_cast(const struct sockaddr_in* addr)
{
  return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
}

const struct sockaddr_in*  sockets::sockaddr_in_cast(const struct sockaddr* addr)
{
  return static_cast<const struct sockaddr_in*>(implicit_cast<const void*>(addr));
}

const struct sockaddr_in6*  sosockaddr_in6_cast(const struct sockaddr* addr)
{
  return static_cast<const struct sockaddr_in6*>(implicit_cast<const void*>(addr));
}

void setNonBlockAndCloseOnExec(int sockfd)
{
    int flg = ::fcntl(sockfd,F_GETFL, 0);
    flg |= O_NONBLOCK;
    int ret = ::fcntl(sockfd, F_SETFL, flg);

    //好顶
    flg = ::fcntl(sockfd, F_GETFD, 0);
    flg |= FD_CLOEXEC;
    ret = ::fcntl(sockfd, F_SETFD, flg);

    (void)ret;
}

int sockets::createNonblocking(sa_family_t family)
{
    int sockfd = ::socket(family,SOCK_STREAM,IPPROTO_TCP);
    if(sockfd < 0)
    {
        std::cout<<"socket create err"<<std::endl;
    }
    setNonBlockAndCloseOnExec(sockfd);
    return sockfd;
}

void mybind(int sockfd, const struct sockaddr* addr)
{
    int ret = ::bind(sockfd,addr,static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
    if(ret < 0)
    {
        std::cout<<"bind err"<<std::endl;
    }       
}

void mylisten(int sockfd)
{
    int ret = ::listen(sockfd , SOMAXCONN);
    if(ret < 0)
    {
        printf("listen err ");
    }
}

int myaccept(int sockfd, struct sockaddr_in6 * addr)
{
    socklen_t addrlen = static_cast<socklen_t>(sizeof(* addr));

    int connfd = ::accept(sockfd, sockets::sockaddr_cast(addr),&addrlen);
    if(connfd < 0)
    {
        printf("%s \n",__PRETTY_FUNCTION__);
    }
    return connfd;
}

void sockets::toIpPort(char* buf, size_t size,
              const struct sockaddr* addr)
{
    sockets::toIp(buf,size, addr);
    size_t end = ::strlen(buf);
    const struct sockaddr_in* addr4 = sockets::sockaddr_in_cast(addr);
    uint16_t port = sockets::networkToHost16(addr4->sin_port);
    assert(size > end);
    snprintf(buf+end, size-end, ":%u", port);
}

void sockets::toIp(char* buf, size_t size,
                   const struct sockaddr* addr)
{
  if (addr->sa_family == AF_INET)
  {
    assert(size >= INET_ADDRSTRLEN);
    const struct sockaddr_in* addr4 = sockets::sockaddr_in_cast(addr);
    ::inet_ntop(AF_INET, &addr4->sin_addr, buf, static_cast<socklen_t>(size));
  }
  else if (addr->sa_family == AF_INET6)
  {
    assert(size >= INET6_ADDRSTRLEN);

   // const struct sockaddr_in6* addr6 = sockaddr_in6_cast(addr);
    //::inet_ntop(AF_INET6, &addr6->sin6_addr, buf, static_cast<socklen_t>(size));
  }
}


void fromIpPort(const char* ip, uint16_t port,
                         struct sockaddr_in* addr)
{
  addr->sin_family = AF_INET;
  addr->sin_port = sockets::hostToNetWork16(port);
  if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
  {
    printf("sockets::fromIpPort\n");
  }
}

void fromIpPort(const char* ip, uint16_t port,
                         struct sockaddr_in6* addr)
{
  addr->sin6_family = AF_INET6;
  addr->sin6_port = sockets::hostToNetWork16(port);
  if (::inet_pton(AF_INET6, ip, &addr->sin6_addr) <= 0)
  {
    printf("sockets::fromIpPort\n");
  }
}

int getSocketError(int sockfd)
{
  int optval;
  socklen_t optlen = static_cast<socklen_t>(sizeof optval);

  if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
  {
    return errno;
  }
  else
  {
    return optval;
  }
}

struct sockaddr_in6 getLocalAddr(int sockfd)
{
  struct sockaddr_in6 localaddr;
  memZero(&localaddr, sizeof localaddr);
  socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
  if (::getsockname(sockfd, sockets::sockaddr_cast(&localaddr), &addrlen) < 0)
  {
    printf("sockets::getLocalAddr\n");
  }
  return localaddr;
}

struct sockaddr_in6 getPeerAddr(int sockfd)
{
  struct sockaddr_in6 peeraddr;
  memZero(&peeraddr, sizeof peeraddr);
  socklen_t addrlen = static_cast<socklen_t>(sizeof peeraddr);
  if (::getpeername(sockfd, sockets::sockaddr_cast(&peeraddr), &addrlen) < 0)
  {
    printf("sockets::getPeerAddr err\n");
  }
  return peeraddr;
}

bool isSelfConnect(int sockfd)
{
  struct sockaddr_in6 localaddr = getLocalAddr(sockfd);
  struct sockaddr_in6 peeraddr = getPeerAddr(sockfd);
  if (localaddr.sin6_family == AF_INET)
  {
    const struct sockaddr_in* laddr4 = reinterpret_cast<struct sockaddr_in*>(&localaddr);
    const struct sockaddr_in* raddr4 = reinterpret_cast<struct sockaddr_in*>(&peeraddr);
    return laddr4->sin_port == raddr4->sin_port
        && laddr4->sin_addr.s_addr == raddr4->sin_addr.s_addr;
  }
  else if (localaddr.sin6_family == AF_INET6)
  {
    return localaddr.sin6_port == peeraddr.sin6_port
        && memcmp(&localaddr.sin6_addr, &peeraddr.sin6_addr, sizeof localaddr.sin6_addr) == 0;
  }
  else
  {
    return false;
  }
}

