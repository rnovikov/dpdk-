#ifndef TOOLS_H
#define TOOLS_H
#include <base/base_types.h>
#include <tools/string_tools.h>
#include <nlohmann/json.hpp>
#include <packet/sfivetyplesession.h>

namespace dc_tools
{

static inline uint64_t makeItPowerTwo( const uint64_t v )
{
    uint64_t answer = 0;
    if( v & ( v - 1 ) )
    {
        uint64_t a = 64 - __builtin_clzll( v );
        L_DEBUG << a;
        answer = 1 << a;
    }
    else
    {
        answer = v;
    }

    TA_LOGIC_ERROR( answer >= v );
    return answer;
}

static inline uint64_t makeItLinedTo( const uint64_t v, const uint64_t linedTo = CACHE_LINE_SIZE )
{
    TA_LOGIC_ERROR( linedTo >= CACHE_LINE_SIZE );
    L_DEBUG << linedTo;
    uint64_t result = linedTo & ( linedTo - 1 );
    L_DEBUG << std::hex << ( linedTo ) << "  -> " << ( linedTo - 1 ) << "  RESULT:" << result;

    const bool isPowerOf2 = result == 0;
    TA_LOGIC_ERROR( isPowerOf2 );
    if( v <= linedTo )
    {
        return linedTo;
    }
    TA_LOGIC_ERROR( v > linedTo );
    uint64_t answer = v;
    if( v & ( linedTo - 1 ) )
    {
        answer &= ( MAX_UINT64_VALUE - ( linedTo - 1 ) );
        answer += linedTo;
    }
    TA_LOGIC_ERROR( answer >= v );

    return answer;
}



} // namespace tools
#endif // TOOLS_H
