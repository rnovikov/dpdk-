#include "cpcapreader.h"
#include <pcap.h>
#include <filesystem>

#include <packet/cmbuf.h>
using namespace std;
namespace dc_tools
{
cPcapReader::cPcapReader()
{
    header_ = new pcap_pkthdr();
}

cPcapReader::~cPcapReader()
{
    if( pcapContext_ )
    {
        pcap_close( (pcap_t*)pcapContext_ );
    }
    if( header_ )
    {
        delete( (pcap_pkthdr*)header_ );
    }
}

bool cPcapReader::init( const std::string& pcapFile, rte_mempool* mempool, uint32_t maxPacketsToRead )
{

    THROW_ASSERT( mempool, "BAD POINTER" );

    if( filesystem::exists( pcapFile ) )
    {
        pcapFile_ = pcapFile;
        mempool_ = mempool;
        if( maxPacketsToRead == 0 )
        {
            maxPacketsToRead = BAD_UINT32_VALUE / 4;
        }
        maxPacketsToRead_ = maxPacketsToRead;
        L_DEBUG << pcapFile_;

        char errbuf[PCAP_ERRBUF_SIZE];
        pcapContext_ = pcap_open_offline( pcapFile_.c_str(), errbuf );
        THROW_ASSERT( pcapContext_, "BAD POINTER" );
        pcap_pkthdr* hdr = (pcap_pkthdr*)header_;
        currentPacket_ = pcap_next( (pcap_t*)pcapContext_, hdr );
    }
    else
    {
        L_ERROR << "FILE \"" << pcapFile << "\" does not exist!";
        currentPacket_ = nullptr;
        return false;

        //        THROW_ERROR("NO FILE:"+pcapFile);
    }
    TA_BAD_POINTER(currentPacket_);
    return true;
}

bool cPcapReader::hasNext() const
{
    if( maxPacketsToRead_ )
    {
        if( currentCount_ >= maxPacketsToRead_ )
        {
            return false;
        }
    }
    return currentPacket_ != nullptr;
}

cMbuf* cPcapReader::getNext()
{
    THROW_ASSERT( currentPacket_, "BAD LOGIC" );
    TA_LOGIC_ERROR( mempool_ );
    cMbuf* mbuf = (cMbuf*)rte_pktmbuf_alloc( mempool_ );
    THROW_ASSERT( mbuf, "NO MBUF" );
    mbuf->init();
    TA_LOGIC_ERROR(mbuf->isSegmented()==false);
    pcap_pkthdr* hdr = (pcap_pkthdr*)header_;
    uint32_t cuttedCaplen = hdr->caplen;

    THROW_ASSERT( cuttedCaplen <= rte_pktmbuf_tailroom( (rte_mbuf*)mbuf ), "PACKETS IN PCAP TOO BIG" );
    char* data = mbuf->getDataByOffset<char>( 0 );
    mbuf->data_len = (uint16_t)cuttedCaplen;
    rte_memcpy( data, currentPacket_, cuttedCaplen );
    mbuf->pkt_len = (uint16_t)cuttedCaplen;
    //	mbuf->timestamp_ = (uint64_t)hdr->ts.tv_sec * 1000000 + hdr->ts.tv_usec;
    mbuf->port = BAD_UINT16_VALUE;
    currentPacket_ = pcap_next( (pcap_t*)pcapContext_, hdr );
    currentCount_++;
    TA_BAD_POINTER( mbuf );
    return mbuf;
}

uint32_t cPcapReader::internal_receive_objects( cMbuf** buffer, uint64_t size )
{
    uint32_t answer = 0;
    for( uint i = 0; i < size; ++i )
    {
        if( hasNext() )
        {
            buffer[i] = getNext();
            answer++;
        }
        else
        {
            break;
        }
    }
    return answer;
}
}
