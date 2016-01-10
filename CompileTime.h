//+------------------------------------------------------------------+
//|                                              Compile time Random |
//+------------------------------------------------------------------+
#pragma once
#include <stdint.h>
#if _MSC_VER >= 1900
#if defined(_MSC_VER)

constexpr uint64_t raise_128_to(const uint8_t power)
{
    return ((1ULL<<7)*power);
}

constexpr uint64_t str_hash(const char* const str,const uint8_t current_len)
{
    return *str ? (raise_128_to(current_len-1) * static_cast<uint8_t>(*str)+str_hash(str+1,current_len-1)) : 0;
}

constexpr uint8_t str_len(const char* const str)
{
    return *str ? (1+str_len(str+1)) : 0;
}

constexpr uint64_t GetSeed(char time_line2[])
{
    return(str_hash(time_line2,9));
}

#pragma warning( push )
#pragma warning( disable : 4307 4309) // integral constant overflow
template <uint64_t N>
struct Random
{
    enum
    {
        value=(Random<N-1>::value * 1103515245 + 12345) | (1<<(GetSeed(__TIME__)%31))
    };
};
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4309 ) // integral constant overflow
template <>
struct Random<1>
{
    enum
    {
        value=GetSeed(__TIME__)
    };
};
#pragma warning( pop )

#define CompileTimeRandom(from, to) from + uint64_t(Random<__COUNTER__+1>::value) % (to - from)

#endif
#endif
//+------------------------------------------------------------------+
