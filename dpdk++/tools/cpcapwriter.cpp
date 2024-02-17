#include "cpcapwriter.h"
#include <pcap.h>
#include <packet/cmbuf.h>
#define MAX_PCAP_PACKET_SIZE 9100
#define RTE_ETH_PCAP_SNAPSHOT_LEN 65535
#define USEC_IN_SEC 1000000

static unsigned char temp_data_private[MAX_PCAP_PACKET_SIZE]{ 0 };
namespace dc_tools
{
cPcapWriter::cPcapWriter()
{
}

cPcapWriter::~cPcapWriter()
{
    if( pcapId_ )
    {
        pcap_close( (pcap_t*)pcapId_ );
    }
}

void cPcapWriter::init( const std::string& fileName )
{
    pcapId_ = pcap_open_dead_with_tstamp_precision( DLT_EN10MB, RTE_ETH_PCAP_SNAPSHOT_LEN, PCAP_TSTAMP_PRECISION_NANO );
    THROW_ASSERT( pcapId_, "BAD POINTER" );
    dumper_ = pcap_dump_open( (pcap_t*)pcapId_, fileName.c_str() );
    THROW_ASSERT( dumper_, "BAD POINTER" );
}

bool cPcapWriter::add( cMbuf* mbuf, uint64_t timestamp )
{
    pcap_pkthdr header;
    header.caplen = header.len = mbuf->pkt_len;
    if( timestamp == 0 )
    {
        header.ts.tv_sec = mbuf->timestamp_ / USEC_IN_SEC;
        header.ts.tv_usec = mbuf->timestamp_ % USEC_IN_SEC;
        NOT_IMPLEMENTED;
    }
    else
    {
        header.ts.tv_sec = timestamp / USEC_IN_SEC;
        header.ts.tv_usec = timestamp % USEC_IN_SEC;
    }
    THROW_ASSERT( mbuf->next == nullptr, "NOT SUPPORTED" );
    pcap_dump( (uint8_t*)dumper_, &header, mbuf->getDataByOffset<uint8_t>( 0 ) );

    //    pcap_dump()
    return true;
}

void cPcapWriter::flush()
{
    pcap_dump_flush( (pcap_dumper_t*)dumper_ );
}
}
