#ifndef CPCAPREADER_H
#define CPCAPREADER_H
#include <base/base_types.h>
#include <connections/connection.h>
class cMbuf;
struct rte_mempool;
namespace dc_tools
{
class cPcapReader : public iReceiver
{
public:
    explicit cPcapReader();
    ~cPcapReader();
    bool init( const std::string& pcapFile, rte_mempool* mempool, uint32_t maxPacketsToRead = 0 );

    bool hasNext() const;
    cMbuf* getNext();

    virtual uint32_t internal_receive_objects( cMbuf** buffer, uint64_t size ) final override;

private:
    std::string pcapFile_;

    rte_mempool* mempool_ = nullptr;
    const uint8_t* currentPacket_ = nullptr;
    void* header_ = nullptr;
    void* pcapContext_ = nullptr;
    uint32_t currentCount_ = 0;
    uint32_t maxPacketsToRead_ = 0;

    cPcapReader( const cPcapReader& ) = delete;
    void operator=( const cPcapReader& ) = delete;
};
}
#endif // CPCAPREADER_H
