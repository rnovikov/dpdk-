#include "sfivetyplesession.h"

sFiveTypleSession::sFiveTypleSession()
{

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

