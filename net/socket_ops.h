#pragma once
#include <arpa/inet.h>
namespace sockets
{
int createNonblocking(sa_family_t family);

int myconnect(int sockfd,const struct sockaddr* addr);
void mybind(int sockfd,const struct sockaddr* addr);
void mylisten(int sockfd);
int myaccept(int sockfd, struct sockaddr_in6 * addr);

//一系列强转 
const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr);
const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr);
struct sockaddr* sockaddr_cast(struct sockaddr_in6* addr);
const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr);
void toIp(char* buf, size_t size,const struct sockaddr* addr);
void toIpPort(char* buf, size_t size,const struct sockaddr* addr);
void fromIpPort(const char* ip, uint16_t port,
                struct sockaddr_in* addr);
void fromIpPort(const char* ip, uint16_t port,
                struct sockaddr_in6* addr);

int getSocketError(int sockfd);
struct sockaddr_in6 getLocalAddr(int sockfd);
struct sockaddr_in6 getPeerAddr(int sockfd);
bool isSelfConnect(int sockfd);
}
