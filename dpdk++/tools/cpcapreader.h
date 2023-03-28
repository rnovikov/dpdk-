#ifndef CPCAPREADER_H
#define CPCAPREADER_H
#include <string>
#include <general/forward_declaration.h>
namespace tools{
class cPcapReader
{
public:
    explicit cPcapReader();
    ~cPcapReader();
    void init( const std::string& pcapFile, cMemPool* mempool, uint32_t maxPacketsToRead = 0 );

    bool hasNext() const;
    cMbuf* getNext();

private:
    std::string pcapFile_;
    cMemPool* mempool_ = nullptr;
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
