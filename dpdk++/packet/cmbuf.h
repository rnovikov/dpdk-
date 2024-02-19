#ifndef CMBUF_H
#define CMBUF_H
#include <rte_mbuf.h>

#include "network_tools.h"
#include <base/base_types.h>
#include "sfivetyplesession.h"
#include <rte_prefetch.h>


#define CACHE_LINES_FOR_PREFETCH 6

struct sMetaData
{

    uint16_t ipPlSize_;
    uint16_t ipv4Offset_;
    uint16_t l4Offset_;
    uint16_t l5Offset_;
    uint16_t payloadOffset_;
    uint16_t payloadSize_;
    uint8_t l4Proto_;
    uint8_t l5Proto_;
    uint32_t tcpSeq_;
    uint32_t tcpRcv_;
    uint16_t etherType_ = 0;

    bool isMcast_;

    void init()
    {

        etherType_ = 0;
        payloadSize_=BAD_UINT16_VALUE;
        payloadOffset_=0;
        ipv4Offset_ = 0;
        l4Offset_ = 0;
        l4Proto_ = 0;
        l5Offset_ = 0;
        l5Proto_ = 0;
        tcpSeq_ = 0;
        tcpRcv_ = 0;
        ipPlSize_=0;
        isMcast_ = false;

    }
};

enum class eMbufTargetPoint : uint8_t
{
    NOT_INITED,
    PORT,
    KNI
};

class cMbuf : public rte_mbuf
{
//    friend class cPcapReader;
//    friend class cPcapWriter;
//    friend class conn_gen::cPacketGenerator;

public:
    void init();

    bool isSegmented()const;

    template <class T> T* getDataByOffset( uint32_t offset )
    {
        return rte_pktmbuf_mtod_offset( getMbuf(), T*, offset );
    }

    uint8_t* getPayload()
    {
        TA_LOGIC_ERROR( meta_.payloadOffset_ );
        return getDataByOffset<uint8_t>( meta_.payloadOffset_ );
    }




    DC_ALWAYS_INLINE bool  parseL3();
    DC_ALWAYS_INLINE void parseL4();

    DC_ALWAYS_INLINE void parseTcp();
    DC_ALWAYS_INLINE void parseUdp();


    template <uint32_t cachelines> DC_ALWAYS_INLINE void prefetchData();


    rte_mbuf* getMbuf()
    {
        return (rte_mbuf*)this;
    }

    uint8_t* getPayloadPtr()
    {
        return getDataByOffset<uint8_t>( meta_.payloadOffset_ );
    }
    sMetaData& getMeta()
    {
        return meta_;
    }
    inline uint32_t getPktLen()const{return pkt_len;}

    void copyMysel( cMbuf* dst );
    cMbuf* cloneMyself( uint32_t plToCopy );

    rte_ipv4_hdr* getIp()
    {
        TA_LOGIC_ERROR( meta_.ipv4Offset_ != 0 );
        return getDataByOffset<rte_ipv4_hdr>( meta_.ipv4Offset_ );
    }
    rte_udp_hdr* getUdp()
    {
        return getDataByOffset<rte_udp_hdr>( meta_.l4Offset_ );
    }
    rte_tcp_hdr* getTcp()
    {
        TA_LOGIC_ERROR( meta_.l4Offset_ != 0 );
        return getDataByOffset<rte_tcp_hdr>( meta_.l4Offset_ );
    }

    DC_ALWAYS_INLINE void swapDirection();
    DC_ALWAYS_INLINE void swapL2Hdr();
    DC_ALWAYS_INLINE void swapL3();
    DC_ALWAYS_INLINE void makeAckFromPush( uint32_t plLen, uint8_t flags, uint16_t wnd, uint16_t ipId );
    DC_ALWAYS_INLINE void resetTcpPayload( uint32_t nSize );
    bool checkIpCsum();
    bool checkTcpCsum();
    uint16_t calculateIpTcpCsum();
    uint32_t getTcpPayloadSize();

    uint32_t rfCount()
    {
        uint32_t answer = rte_mbuf_refcnt_read( (rte_mbuf*)this );

        return answer;
    }

    void freeMbuf()
    {


        L_NOTICE<<"["<<timestamp_<<"]FREE "<<(void *)this;
        rte_pktmbuf_free( (rte_mbuf*)this );
    }

    static constexpr uint32_t getKeySessionOffset()
    {
        return sizeof( rte_mbuf );
    }

    std::string toStr();




    DC_ALWAYS_INLINE void setTargetPoint( eMbufTargetPoint tp )
    {
        targetPoint_ = tp;
    }
    DC_ALWAYS_INLINE eMbufTargetPoint getTargetPoint()
    {
        return targetPoint_;
    }

    rte_mbuf* mbuf()
    {
        return (rte_mbuf*)this;
    }

    sFiveTypleSession key_ __rte_cache_aligned;
    void *session_;
    uint64_t timestamp_;
    uint32_t flags_;
    uint32_t reserverd_;
    sMetaData meta_;
    eMbufTargetPoint targetPoint_;
    bool isTcpHandled_;
    uint32_t payloadLength()const;
    cMbuf() = delete;
};
static_assert( sizeof( cMbuf ) <= ( sizeof( rte_mbuf ) + RTE_PKTMBUF_HEADROOM ), " UPDATE HEADROOM" );

template <uint32_t cachelines>
inline void cMbuf::prefetchData()
{
    uint64_t* ptr = (uint64_t*)this;
    for( uint i = 0; i < cachelines; ++i )
    {
        rte_prefetch2( ptr + i );
    }
}



inline bool cMbuf::parseL3()
{
    if( pkt_len != data_len )
    {
        L_EMERG << pool->elt_size << " PLEN:" << (uint32_t)pkt_len;
        TA_LOGIC_ERROR( next == nullptr );
        NOT_IMPLEMENTED;
    }

    uint32_t currentOffset = 0;
    sMetaData& meta = getMeta();
    rte_ether_hdr* ether_hdr = getDataByOffset<rte_ether_hdr>( currentOffset );
    currentOffset += sizeof( rte_ether_hdr );

    meta.isMcast_ = rte_is_multicast_ether_addr( &ether_hdr->dst_addr );
    meta_.etherType_ = ether_hdr->ether_type;
    if( ether_hdr->ether_type == ET_IPV4 )
    {

        meta.ipv4Offset_ = currentOffset;
        TA_LOGIC_ERROR( meta.ipv4Offset_ == 14 );
        rte_ipv4_hdr* ipv4 = getIp();
        //        TA_LOGIC_ERROR(ipv4->fragment_offset==0);
        uint16_t len = rte_ipv4_hdr_len( ipv4 );
        currentOffset += len;
        bool isMy = ipv4->next_proto_id == IP_NP_UDP || ipv4->next_proto_id == IP_NP_TCP;
        if( !isMy )
        {
            //            NOT_IMPLEMENTED;
            return false;
        }
        key_.dstIp_ = ipv4->dst_addr;
        key_.srcIp_ = ipv4->src_addr;
        key_.proto_ = ipv4->next_proto_id;
        meta.l4Offset_ = currentOffset;
        meta.l4Proto_ = ipv4->next_proto_id;
        meta.ipPlSize_=rte_bswap16(ipv4->total_length)-DC_IP_HEADER_LEN(ipv4);


        return true;
    }
    else
    {
        return false;
    }

    return false;
}

inline void cMbuf::parseL4(  )
{
    if( key_.proto_ == IP_NP_TCP )
    {
        parseTcp(  );
    }
    else if( key_.proto_ == IP_NP_UDP )
    {
        parseUdp();
    }
    else
    {
        L_EMERG << key_.toStr();
        NOT_IMPLEMENTED;
    }
}

inline void cMbuf::parseTcp( )
{
    TA_LOGIC_ERROR( data_len == pkt_len ); // NO FRAGMENT PACKETS
    rte_tcp_hdr* tcp = getDataByOffset<rte_tcp_hdr>( meta_.l4Offset_ );
    uint32_t currentOffset = meta_.l4Offset_ + DC_TCP_HEADER_LEN( tcp );
    meta_.l5Offset_ = currentOffset; // WRITE RTP INTO
    key_.dstL4Port_ = tcp->dst_port;
    key_.srcL4Port_ = tcp->src_port;
    meta_.tcpRcv_ = rte_bswap32( tcp->recv_ack );
    meta_.tcpSeq_ = rte_bswap32( tcp->sent_seq );

    meta_.payloadOffset_ = currentOffset;
    meta_.payloadSize_=data_len - currentOffset;
    uint32_t tcpHdrSize=DC_TCP_HEADER_LEN( tcp );

//    TA_LOGIC_ERROR(meta_.payloadOffset_+meta_.payloadSize_== meta_.ipv4Offset_+meta_.ipPlSize_ );


}

inline void cMbuf::parseUdp()
{
    rte_udp_hdr* udpHdr = getDataByOffset<rte_udp_hdr>( meta_.l4Offset_ );
    uint32_t currentOffset = sizeof( rte_udp_hdr ) + meta_.l4Offset_;
    meta_.l5Offset_ = currentOffset;
    meta_.payloadOffset_ = currentOffset;

    key_.dstL4Port_ = udpHdr->dst_port;
    key_.srcL4Port_ = udpHdr->src_port;
}

inline void cMbuf::swapL2Hdr()
{
    rte_ether_addr tmpAddr;
    rte_ether_hdr* hdr = getDataByOffset<rte_ether_hdr>( 0 );
    tmpAddr = hdr->dst_addr;
    hdr->dst_addr = hdr->src_addr;
    hdr->src_addr = tmpAddr;

    if( targetPoint_ == eMbufTargetPoint::KNI )
    {
        targetPoint_ = eMbufTargetPoint::PORT;
    }
    else if( eMbufTargetPoint::PORT == targetPoint_ )
    {
        targetPoint_ = eMbufTargetPoint::KNI;
    }
    else
    {
        NOT_IMPLEMENTED;
    }
}

void cMbuf::swapL3()
{
    rte_ipv4_hdr* ipHdr = getDataByOffset<rte_ipv4_hdr>( meta_.ipv4Offset_ );
    fast_swap_values( ipHdr->dst_addr, ipHdr->src_addr );
}

void cMbuf::makeAckFromPush( uint32_t plLen, uint8_t flags, uint16_t wnd, uint16_t ipId )
{
    TA_LOGIC_ERROR( meta_.l4Proto_ == IP_NP_TCP );
    TA_LOGIC_ERROR( wnd );
    rte_tcp_hdr* tcp = getTcp();
    fast_swap_values( tcp->dst_port, tcp->src_port );
    uint32_t nSentSeq = rte_bswap32( tcp->sent_seq ) + plLen;
    tcp->sent_seq = tcp->recv_ack;
    tcp->recv_ack = rte_bswap32( nSentSeq );
    tcp->tcp_flags = flags;
    tcp->rx_win = wnd;
    getIp()->packet_id = rte_bswap16( ipId );
}

inline void cMbuf::resetTcpPayload( uint32_t nSize )
{
    rte_ipv4_hdr* ipHdr = getIp();
    TA_LOGIC_ERROR( meta_.payloadOffset_ != 0 );
    data_len = pkt_len = meta_.payloadOffset_ + nSize;
    L_DEBUG << (uint32_t)rte_bswap16( ipHdr->total_length );
    uint16_t ttlLen = meta_.payloadOffset_ - meta_.ipv4Offset_ + nSize;
    ipHdr->total_length = rte_bswap16( ttlLen );
    L_DEBUG << (uint32_t)rte_bswap16( ipHdr->total_length );
    uint16_t tcpHdrLen = DC_TCP_HEADER_LEN( getTcp() );
    TA_LOGIC_ERROR( ttlLen == ( rte_ipv4_hdr_len( ipHdr ) + tcpHdrLen + nSize ) );
    TA_LOGIC_ERROR( getTcpPayloadSize() == nSize );
}

#endif // CMBUF_H
