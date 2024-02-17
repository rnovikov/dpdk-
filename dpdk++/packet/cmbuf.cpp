#include "cmbuf.h"
#include <tools/string_tools.h>
#include <tools/dpdk_tools.h>

#define ST20_SRD_OFFSET_CONTINUATION ( 0x1 << 15 )
#define ST20_SECOND_FIELD ( 0x1 << 15 )

using namespace std;

void cMbuf::init()
{
    zero_struct( meta_ );
    meta_.init();

    session_ = nullptr;
    isTcpHandled_ = false;
    key_.clear();

    timestamp_ = 0;
    targetPoint_ = eMbufTargetPoint::NOT_INITED;
}

bool cMbuf::isSegmented() const
{
    TA_LOGIC_ERROR( data_len == pkt_len );
    return next!=nullptr;
}

void cMbuf::copyMysel( cMbuf* dst )
{
    rte_memcpy( ( dst->mbuf() + 1 ), ( mbuf() + 1 ), RTE_PKTMBUF_HEADROOM );
}

cMbuf* cMbuf::cloneMyself( uint32_t plToCopy )
{
    TA_LOGIC_ERROR( pkt_len == data_len );
    if( !plToCopy )
    {
        plToCopy = pkt_len;
    }
    if( plToCopy > pkt_len )
    {
        plToCopy == pkt_len;
    }
    cMbuf* copy = (cMbuf*)rte_pktmbuf_alloc( pool );
    TA_BAD_POINTER( copy );
    void* payload = rte_pktmbuf_append( copy, plToCopy );
    TA_BAD_POINTER( payload );

    rte_memcpy( payload, getDataByOffset<uint8_t>( 0 ), plToCopy );
    TA_LOGIC_ERROR( copy );

    copyMysel( copy );
    L_DEBUG << "ORIG:" << (void*)this << " CLONE:" << (void*)copy;
    TA_LOGIC_ERROR( copy->rfCount() == 1 );

    return copy;
}

bool cMbuf::checkIpCsum()
{
    rte_ipv4_hdr ipHdr = *getIp();
    uint16_t csum = ipHdr.hdr_checksum;
    ipHdr.hdr_checksum = 0;
    uint16_t ipCs = rte_ipv4_cksum( &ipHdr );
    L_DEBUG << timestamp_ << "_IPCSUM(" << hex << (uint32_t)csum << ") valid:" << (uint32_t)ipCs;
    //    L_DEBUG<<tools::strIpHdr()
    return ipCs == csum;
}

bool cMbuf::checkTcpCsum()
{
    rte_ipv4_hdr ipHdr = *getIp();
    TA_LOGIC_ERROR( ipHdr.next_proto_id == IP_NP_TCP );
    rte_tcp_hdr* tcp = getTcp();
    uint16_t tmpL4Cs = tcp->cksum;
    tcp->cksum = 0;

    ipHdr.hdr_checksum = 0;

    uint16_t ipCs = rte_ipv4_cksum( &ipHdr );
    //    ipHdr.hdr_checksum=ipCs;
    uint16_t l4Cs = rte_ipv4_udptcp_cksum( &ipHdr, tcp );
    L_DEBUG << dc_tools::strTcpHdr( tcp );
    L_DEBUG << timestamp_ << "_TCPCSUM(" << hex << (uint32_t)tmpL4Cs << ") valid:" << (uint32_t)l4Cs
            << "   IP:" << ipCs;
    tcp->cksum = tmpL4Cs;

    return tmpL4Cs == l4Cs;
}

uint16_t cMbuf::calculateIpTcpCsum()
{
    TA_LOGIC_ERROR( meta_.l4Proto_ == IP_NP_TCP );
    rte_ipv4_hdr* ipHdr = getIp();
    rte_tcp_hdr* tcpHdr = getTcp();
    ipHdr->hdr_checksum = 0;
    tcpHdr->cksum = 0;
    uint16_t ipCs = rte_ipv4_cksum( ipHdr );
    ipHdr->hdr_checksum = ipCs;
    uint16_t l4Cs = rte_ipv4_udptcp_cksum( ipHdr, tcpHdr );

    tcpHdr->cksum = l4Cs;

    ipHdr->hdr_checksum = ipCs;
    L_DEBUG << "CS IP:" << hex << ipCs << " TCP:" << l4Cs;
    return l4Cs;
}

uint32_t cMbuf::getTcpPayloadSize()
{
    TA_LOGIC_ERROR( pkt_len == data_len );

    return pkt_len - ( meta_.payloadOffset_ );
}

std::string cMbuf::toStr()
{
    stringstream ss;
    NOT_IMPLEMENTED;

    return ss.str();
}





uint32_t cMbuf::payloadLength() const
{
    uint32_t answer=pkt_len;
    answer-=meta_.payloadOffset_;
    return answer;
}

