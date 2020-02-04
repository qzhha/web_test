#include <iostream>
#include "inetaddress.h"
#include "socket_ops.h"

#include <machine/endian.h>
#include<netdb.h>



inetaddress::inetaddress(uint16_t port,bool loopbackonly,bool ipv6)
{
    if(ipv6)
    {
        memZero(&addr6_,sizeof addr6_);
        addr6_.sin6_family = AF_INET6;
        in6_addr ip = loopbackonly ? in6addr_loopback : in6addr_any;
        addr6_.sin6_addr = ip;
        addr6_.sin6_port = sockets::hostToNetWork16(port); 
    }
    else
    {
        memZero(&addr_,sizeof addr_);
        addr_.sin_family = AF_INET;
        in_addr_t ip = loopbackonly ? INADDR_LOOPBACK : INADDR_ANY;
        addr_.sin_addr.s_addr = sockets::hostToNetWork32(ip);
        addr_.sin_port = sockets::hostToNetWork16(port); 
    }
}

inetaddress::inetaddress(StringArg ip,uint16_t port,bool ipv6)
{
    if(ipv6)
    {
        memZero(&addr6_,sizeof addr6_);
        sockets::fromIpPort(ip.c_str(),port,&addr6_);

    }
    else
    {
        memZero(&addr_,sizeof addr_);
        sockets::fromIpPort(ip.c_str(),port,&addr_);
    }
}

string inetaddress::toIpPort() const
{
    char buf[64] = "";
    sockets::toIpPort(buf,sizeof buf, getSockAddr());
    return buf;
}

string inetaddress::toIp() const 
{
    char buf[64] = "";
    sockets::toIp(buf, sizeof buf, getSockAddr());
    return buf;
}

uint32_t inetaddress::ipNetEndian() const 
{
    return addr_.sin_addr.s_addr;
}

uint16_t inetaddress::toPort() const
{
    return sockets::networkToHost16(portNetEndian());
}

//__thread是GCC内置的线程局部存储设施，存取效率可以和全局变量相比。
//__thread变量每一个线程有一份独立实体，各个线程的值互不干扰。可以用来修饰那些带有全局性且值可能变，但是又不值得用全局变量保护的变量。
static __thread char t_reosolveBuffer[64 * 1024];

bool inetaddress::resolve(StringArg hostname,inetaddress* out)
{
    if(out == nullptr)
        return false;
    struct hostent hent;
    struct hostent* he = nullptr;
    int herrno = 0;
    memZero(&hent,sizeof hent);

    he = gethostbyname(hostname.c_str());
    if(he != nullptr)
    {
        out->addr_.sin_addr = *reinterpret_cast<struct in_addr*>(he->h_addr);
        return true;
    }
    else 
    {
        printf("%s\n",__PRETTY_FUNCTION__);
        return false;
    }
}

void inetaddress::setScopeId(uint32_t scope_id)
{
    if(family() == AF_INET6)
    {
        addr6_.sin6_scope_id = scope_id;
    }
}
