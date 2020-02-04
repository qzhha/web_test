#pragma once
#include "../base/noncopyable.h"
#include "../base/StringPiece.h"
#include "endian.h"

#include <netinet/in.h>

namespace sockets
{
    const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr);
}

class inetaddress : public noncopyable 
{
public:


    explicit inetaddress(uint16_t port = 0,bool loopbackonly = false, bool ipv6 = false);

    inetaddress(StringArg ip, uint16_t port, bool ipv6 = false);

    explicit inetaddress(const struct sockaddr_in& addr) : addr_(addr)
    {}
    explicit inetaddress(const struct sockaddr_in6& addr) : addr6_(addr)
    {}
    sa_family_t family() const { return addr_.sin_family;  }
    string toIp() const;
    string toIpPort() const;
    uint16_t toPort() const;

    const struct sockaddr* getSockAddr() const {return sockets::sockaddr_cast(&addr6_); }
    void setSockAddrInet6(const struct sockaddr_in6& addr6) { addr6_ = addr6; }

    uint32_t ipNetEndian() const;
    uint16_t portNetEndian() const { return addr_.sin_port; }

    //hostname -> ipaddr 返回true成功
    static bool resolve(StringArg hostname,inetaddress* res);
    void setScopeId(uint32_t scope_id);
private:
    union{
        struct sockaddr_in addr_;
        struct sockaddr_in6 addr6_;

    };
};

