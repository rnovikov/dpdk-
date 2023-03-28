#include <tools/cpcapreader.h>
#include <general/forward_declaration.h>
#include <pcap.h>
#include <filesystem>
#include <logger/logger_api.h>
#include <general/base_defines.h>
#include <preprocessor_tools/error.h>

using namespace std;
namespace tools
{

void cPcapReader::init(const std::string &pcapFile, cMemPool *mempool, uint32_t maxPacketsToRead)
{
    TA_BAD_POINTER( mempool);
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
        std::string errMsg="NO PCAP FILE FILE \"" +  pcapFile  + "\" does not exist!";
        L_ERR << errMsg;
        currentPacket_ = nullptr;
        THROW_ERROR(errMsg);
    }
}

}
