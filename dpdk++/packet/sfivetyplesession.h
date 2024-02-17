#ifndef SFIVETYPLESESSION_H
#define SFIVETYPLESESSION_H
#include <stdint.h>
#include <array>
#include <base/debug_tools.h>

struct  sFiveTypleSession
{
public:
    sFiveTypleSession();
    uint32_t dstIp_;
    uint32_t srcIp_;
    uint16_t dstL4Port_;
    uint16_t srcL4Port_;
    uint8_t proto_;
    std::array<uint8_t, 3> reserver_; // for size== 16 bytes

    DC_ALWAYS_INLINE void clear()
    {
        zero_struct( *this );
    }
    DC_ALWAYS_INLINE void swap();
    DC_ALWAYS_INLINE bool isSame( const sFiveTypleSession& key ) const;
    DC_ALWAYS_INLINE bool isSameIgnoreDirection( sFiveTypleSession key ) const;
    DC_ALWAYS_INLINE bool isSameOppositDirection( sFiveTypleSession key ) const
    {
        TA_LOGIC_ERROR( isSameIgnoreDirection( key ) );
        return !isSame( key );
    }
    DC_ALWAYS_INLINE bool check() const;

    DC_ALWAYS_INLINE uint64_t hash() const;

    bool isValidTcp() const;
    sFiveTypleSession makeFilter() const;

    std::string toStr() const;

     static  bool compare_with_filter( const sFiveTypleSession& key, const sFiveTypleSession& filter );
};

DC_ALWAYS_INLINE void sFiveTypleSession::swap()
{
    TA_LOGIC_ERROR( check() );
    fast_swap_values( dstIp_, srcIp_ );
    fast_swap_values( dstL4Port_, srcL4Port_ );
}

DC_ALWAYS_INLINE bool sFiveTypleSession::isSame( const sFiveTypleSession& key ) const
{
    TA_LOGIC_ERROR( check() );
    TA_LOGIC_ERROR( key.check() );
    if( dstIp_ != key.dstIp_ )
    {
        return false;
    }
    else
    {
        TA_LOGIC_ERROR( dstIp_ ); // NOT ZERO
    }
    if( srcIp_ != key.srcIp_ )
    {
        return false;
    }
    else
    {
        TA_LOGIC_ERROR( srcIp_ ); // NOT ZERO
    }
    if( dstL4Port_ != key.dstL4Port_ )
    {
        return false;
    }
    else
    {
        TA_LOGIC_ERROR( dstL4Port_ ); // NOT ZERO
    }
    if( srcL4Port_ != key.srcL4Port_ )
    {
        return false;
    }
    else
    {
        TA_LOGIC_ERROR( srcL4Port_ ); // NOT ZERO
    }
    if( proto_ != key.proto_ )
    {
        return false;
    }
    else
    {
        TA_LOGIC_ERROR( proto_ ); // NOT ZERO
    }
    return true;
}

DC_ALWAYS_INLINE bool sFiveTypleSession::isSameIgnoreDirection( sFiveTypleSession key ) const
{
    if( isSame( key ) == false )
    {
        key.swap();
        return isSame( key );
    }
    return true;
}


inline bool sFiveTypleSession::check() const
{
    if( !proto_ || !srcIp_ || !dstIp_ || !dstL4Port_ || !srcL4Port_ )
    {
        // NOT INITED!

        return false;
    }

    for( uint8_t item : reserver_ )
    {
        if( item != 0 )
        {
            THROW_ERROR( "RESERVED FIELD NON ZERO" );
            return false;
        }
    }
    return true;
}
#include <rte_hash_crc.h>
// rte_hash_crc_8byte
// rte_hash_crc_4byte
// rte_hash_crc_2byte
union uHashItem
{
    uint64_t val_;
    uint32_t arr_[2];
};

static_assert( sizeof( uHashItem ) == sizeof( uint64_t ), "BAD" );
inline uint64_t sFiveTypleSession::hash() const
{
    uHashItem item1;
    item1.arr_[0] = rte_hash_crc_4byte( dstIp_ + srcIp_, proto_ );
    item1.arr_[1] = rte_hash_crc_4byte( dstIp_ ^ srcIp_, proto_ );

    uHashItem item2;
    item2.arr_[0] = rte_hash_crc_2byte( srcL4Port_ + dstL4Port_, proto_ );
    item2.arr_[1] = rte_hash_crc_2byte( srcL4Port_ ^ dstL4Port_, proto_ );
    uint32_t answer = rte_hash_crc_8byte( item1.val_, 0 ) ^ rte_hash_crc_8byte( item2.val_, 0 );
    return answer;
}

inline bool sFiveTypleSession::isValidTcp() const
{
    if( proto_ != IP_NP_TCP )
    {
        return false;
    }
    if( !dstIp_ )
    {
        return false;
    }
    if( !srcIp_ )
    {
        return false;
    }
    if( !dstL4Port_ )
    {
        return false;
    }
    if( !srcL4Port_ )
    {
        return false;
    }
    return true;
}

static_assert( sizeof( sFiveTypleSession ) == 16, "SHOULD BE 16" );

#endif // SFIVETYPLESESSION_H
