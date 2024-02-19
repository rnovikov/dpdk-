#include "dpdk_tools.h"
#include <base/base_types.h>
#include <rte_mempool.h>
#include <chrono>
#include <iomanip>
#include <rte_ether.h>
#include "string_tools.h"
#include <rte_pci.h>
#include <rte_ethdev.h>
#include <packet/network_tools.h>


#define TMP_LOCAL_TCP_FLD_STR( x ) ss << #x << ":" << (uint64_t)( tcp->x ) << ",";
#define TMP_LOCAL_TCP_FLD_STR_HEX( x ) ss << #x << ":" << std::hex << (uint64_t)( tcp->x ) << ",";
#define TMP_LOCAL_TCP_FLD_STR_SF( x, sf ) ss << #x << ":" << (uint64_t)( sf( tcp->x ) ) << ",";

#define RTE_ETHER_ADDR_LEN 6

using namespace std;








namespace dc_tools
{

string strTcpHdr( rte_tcp_hdr* tcp )
{
    stringstream ss;
    TMP_LOCAL_TCP_FLD_STR_SF( src_port, rte_bswap16 )
    TMP_LOCAL_TCP_FLD_STR_SF( dst_port, rte_bswap16 )
    TMP_LOCAL_TCP_FLD_STR_SF( sent_seq, rte_bswap32 )
    TMP_LOCAL_TCP_FLD_STR_SF( recv_ack, rte_bswap32 )
    TMP_LOCAL_TCP_FLD_STR( data_off )
    TMP_LOCAL_TCP_FLD_STR( tcp_flags )
    TMP_LOCAL_TCP_FLD_STR( rx_win )
    TMP_LOCAL_TCP_FLD_STR_HEX( cksum )
    TMP_LOCAL_TCP_FLD_STR( tcp_urp )
    return ss.str();
}


bool strToMac( const std::string& macIn, rte_ether_addr* macOut )
{
    vector<string> arrM = splitToVector( macIn, ":" );
    if( arrM.size() != RTE_ETHER_ADDR_LEN )
    {
        return false;
    }
    for( uint i = 0; i < RTE_ETHER_ADDR_LEN; ++i )
    {
        stringstream ss( arrM[i] );
        uint32_t val;
        ss >> hex >> val;
        macOut->addr_bytes[i] = (uint8_t)val;
    }
    return true;
}
string pciToStr( rte_pci_addr* pci )
{
    stringstream ss;
    ss << setfill( '0' ) << setw( 4 ) << hex << pci->domain << ":" << setw( 2 ) << ( (uint32_t)pci->bus ) << ":"
       << setw( 2 ) << ( (uint32_t)pci->devid ) << "." << ( (uint32_t)pci->function );
    return ss.str();
}

string strMacAddr( const rte_ether_addr* mac )
{
    return strMacAddr( mac->addr_bytes );
}
std::string strMempool( rte_mempool* pool )
{
    stringstream ss;
    TA_BAD_POINTER( pool );
    uint64_t totalSize = pool->size;
    totalSize *= pool->elt_size;
    ss << "MEMPOOL:" << pool->name << endl;
    ss << "OBJ_SIZE:" << pool->elt_size << endl;
    ss << "OBJ_COUNT:" << pool->size << endl;
    ss << "TTL_SIZE:" << (uint64_t)( totalSize / ( 1024 * 1024 ) ) << "M" << endl;

    return ss.str();
}

uint64_t get_microseconds()
{
    uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch() )
                      .count();
    return ms;
}

rte_ring* createRing( const std::string& name, uint32_t size )
{
    rte_ring* answer = nullptr;
    THROW_ASSERT( _is_power_of_two( size ), "SIZE SHOULD BE POWER OF 2" );
    answer = rte_ring_create( name.c_str(), size, DC_DEFAULT_SOCKET, RING_F_SP_ENQ | RING_F_SC_DEQ );
    THROW_ASSERT( answer, "CAN NOT CREATE RING " );
    return answer;
}

void freeRing( rte_ring* ring )
{
    L_INIT << "FREE RING " << ring->name;
    rte_ring_free( ring );
}

string strMacAddr( const uint8_t mac_addr[] )
{
    stringstream ss;
    for( uint i = 0; i < RTE_ETHER_ADDR_LEN; ++i )
    {
        if( i )
        {
            ss << ":";
        }
        ss << setfill( '0' ) << setw( 2 ) << hex << ( (uint32_t)mac_addr[i] );
    }

    return ss.str();
}

sTcpOptions parceTcpOptions( rte_tcp_hdr* tcp )
{
    sTcpOptions answer;
    TA_LOGIC_ERROR( 20 == sizeof( rte_tcp_hdr ) );
    uint8_t* startPtr = ( (uint8_t*)tcp ) + sizeof( rte_tcp_hdr );
    uint8_t* stopPtr = ( (uint8_t*)tcp ) + DC_TCP_HEADER_LEN( tcp );

    while( startPtr < stopPtr )
    {
        uint8_t kind = *startPtr++;
        if( kind == 0 )
        {
            break;
        }
        if( kind == 1 )
        {
            // No-op option with no length.
            continue;
        }
        uint8_t size = *startPtr++;
        if( kind == 2 )
        {
            answer.mss_ = rte_bswap16( *(uint16_t*)startPtr );
        }
        else if( kind == 3 )
        {
            answer.wndScale_ = *startPtr;
        }

        startPtr += ( size - 2 );
    }
    return answer;
}

string sTcpOptions::toStr() const
{
    stringstream ss;

    return ss.str();
}

#define TMP_LOCAL_CHECK_OFFLOAD( x ) \
    if( x & offls ) \
    ss << "|" << #x
static inline string strDevOffloads( uint64_t offls )
{
    stringstream ss;
    ss << "OFFLOAD(" << hex << offls << dec << "):";
    TMP_LOCAL_CHECK_OFFLOAD( RTE_ETH_RX_OFFLOAD_VLAN_STRIP );
    TMP_LOCAL_CHECK_OFFLOAD( RTE_ETH_RX_OFFLOAD_IPV4_CKSUM );
    TMP_LOCAL_CHECK_OFFLOAD( RTE_ETH_RX_OFFLOAD_UDP_CKSUM );
    TMP_LOCAL_CHECK_OFFLOAD( RTE_ETH_RX_OFFLOAD_TCP_CKSUM );
    TMP_LOCAL_CHECK_OFFLOAD( RTE_ETH_RX_OFFLOAD_TCP_LRO );
    TMP_LOCAL_CHECK_OFFLOAD( RTE_ETH_RX_OFFLOAD_QINQ_STRIP );
    TMP_LOCAL_CHECK_OFFLOAD( RTE_ETH_RX_OFFLOAD_OUTER_IPV4_CKSUM );
    TMP_LOCAL_CHECK_OFFLOAD( RTE_ETH_RX_OFFLOAD_MACSEC_STRIP );
    TMP_LOCAL_CHECK_OFFLOAD( RTE_ETH_RX_OFFLOAD_VLAN_FILTER );
    TMP_LOCAL_CHECK_OFFLOAD( RTE_ETH_RX_OFFLOAD_VLAN_EXTEND );
    TMP_LOCAL_CHECK_OFFLOAD( RTE_ETH_RX_OFFLOAD_SCATTER );

    TMP_LOCAL_CHECK_OFFLOAD( RTE_ETH_RX_OFFLOAD_TIMESTAMP );
    TMP_LOCAL_CHECK_OFFLOAD( RTE_ETH_RX_OFFLOAD_SECURITY );
    TMP_LOCAL_CHECK_OFFLOAD( RTE_ETH_RX_OFFLOAD_KEEP_CRC );
    TMP_LOCAL_CHECK_OFFLOAD( RTE_ETH_RX_OFFLOAD_SCTP_CKSUM );
    TMP_LOCAL_CHECK_OFFLOAD( RTE_ETH_RX_OFFLOAD_OUTER_UDP_CKSUM );
    TMP_LOCAL_CHECK_OFFLOAD( RTE_ETH_RX_OFFLOAD_RSS_HASH );
    TMP_LOCAL_CHECK_OFFLOAD( RTE_ETH_RX_OFFLOAD_BUFFER_SPLIT );

    return ss.str();
}

#define TMP_LOCAL_STR_INT( x ) ss << #x << ":" << (uint32_t)info->x << endl

string strDevInfo( rte_eth_dev_info* info )
{
    stringstream ss;

    ss << info->driver_name << endl;
    TMP_LOCAL_STR_INT( max_mtu );
    TMP_LOCAL_STR_INT( max_rx_queues );

    ss << strDevOffloads( info->default_rxconf.offloads ) << endl;

    return ss.str();
}

string strPciAddr( rte_pci_device* dev )
{
    TA_BAD_POINTER( dev );
    stringstream ss;
    NOT_IMPLEMENTED;
    return ss.str();
}

static inline void makeIpSpcMskFromSession( const sFiveTypleSession& key, rte_flow_item_ipv4& spc, rte_flow_item_ipv4& msk )
{
    if( key.dstIp_ )
    {
        spc.hdr.dst_addr = key.dstIp_;
        msk.hdr.dst_addr = 0xffffffff;
    }
    if( key.srcIp_ )
    {
        spc.hdr.src_addr = key.srcIp_;
        msk.hdr.src_addr = 0xffffffff;
    }
}
template <class T> static inline void makePortSpcMskFromSession( const sFiveTypleSession& key, T& spc, T& msk )
{
    if( key.dstL4Port_ )
    {
        spc.hdr.dst_port = key.dstL4Port_;
        msk.hdr.dst_port = 0xffff;
    }
    if( key.srcL4Port_ )
    {
        spc.hdr.src_port = key.srcL4Port_;
        msk.hdr.src_port = 0xffff;
    }
}


string strRtpBaseHdr( st_rfc3550_rtp_hdr* rtp )
{
    stringstream ss;

    return ss.str();
}

string strIpAddr(uint32_t ip, bool swapEndian)
{
    if( swapEndian )
    {
        ip = rte_bswap32( ip );
    }
    char str[INET_ADDRSTRLEN];
    inet_ntop( AF_INET, &ip, str, INET_ADDRSTRLEN );

    return string( str );

}

} // namespace tools

