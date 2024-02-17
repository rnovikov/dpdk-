#ifndef CTESTMANAGER_H
#define CTESTMANAGER_H
#include <base/base_types.h>
#include <map>
#include "itest.h"
class cDpdk;
namespace dc_bench {


class cTestManager
{
public:
    cTestManager( const std::string& workDir, cDpdk& dpdk );
    ~cTestManager();
    bool hasTest( const std::string& name );
    bool isDaemon( const std::string& name );
    void setParams( const sTestParams& testParams );
    void doTest( const std::string& name );
    std::vector<std::string> testsList() const;

    static void signalHandler( int sigNo );

    static volatile bool isWork_;



protected:

private:
    inline tests::iTest* getTestByName( const std::string& name );

    void runTest( tests::iTest* test );
    void appendTest( tests::iTest* test );
    sTestParams testParams_;
    std::string workDir_;
    std::map<std::string, tests::iTest*> tests_;
};
}
#endif // CTESTMANAGER_H
