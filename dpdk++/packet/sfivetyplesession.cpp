#include "sfivetyplesession.h"
#include <rte_byteorder.h>
#include <tools/dpdk_tools.h>

using namespace std;
sFiveTypleSession::sFiveTypleSession()
{

}

std::string sFiveTypleSession::toStr() const
{
    stringstream ss;
    ss << "[" << (uint32_t)proto_ << "] ";

    ss << dc_tools::strIpAddr( srcIp_ ) << ":" << ( (uint32_t)rte_bswap16( srcL4Port_ ) );
    ss << " -> ";
    ss << dc_tools::strIpAddr( dstIp_ ) << ":" << ( (uint32_t)rte_bswap16( dstL4Port_ ) );
    return ss.str();

}
#define TMP_CHECK_THIS_FIELDS( x ) \
    if( filter.x ) \
        if( key.x != filter.x ) \
    return false



bool sFiveTypleSession::compare_with_filter(const sFiveTypleSession &key, const sFiveTypleSession &filter)
{
    TMP_CHECK_THIS_FIELDS( dstIp_ );
    TMP_CHECK_THIS_FIELDS( srcIp_ );
    TMP_CHECK_THIS_FIELDS( dstL4Port_ );
    TMP_CHECK_THIS_FIELDS( srcL4Port_ );
    TMP_CHECK_THIS_FIELDS( proto_ );
    return true;
}

