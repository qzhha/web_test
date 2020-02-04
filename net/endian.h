#pragma once

#include<stdint.h>
#include <machine/endian.h>
#include <libkern/OSByteOrder.h>
//BSD/Linux-like <endian.h> for MacOS X



namespace sockets
{
inline uint64_t hostToNetWork64(uint64_t var)
{
    return OSSwapHostToBigInt64(var);
}

inline uint32_t hostToNetWork32(uint32_t var)
{
    return OSSwapHostToBigInt32(var);
}

inline uint16_t hostToNetWork16(uint16_t var)
{
    return OSSwapHostToBigInt16(var);
}

inline uint64_t networkToHost64(uint64_t var)
{
    return OSSwapBigToHostInt64(var);
}

inline uint32_t networkToHost32(uint32_t var)
{
    return OSSwapBigToHostInt32(var);
}

inline uint16_t networkToHost16(uint16_t var)
{
    return OSSwapBigToHostInt16(var);
}
}

