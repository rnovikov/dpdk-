#ifndef CPCAPWRITER_H
#define CPCAPWRITER_H
#include <base/base_types.h>

class cPcapWriter
{
public:
    cPcapWriter();
    ~cPcapWriter();

    void init( const std::string& fileName );
    bool add( cMbuf* buf, uint64_t timestamp = 0 );

    void flush();

private:
    void* dumper_ = nullptr;
    void* pcapId_ = nullptr;
};

#endif // CPCAPWRITER_H
