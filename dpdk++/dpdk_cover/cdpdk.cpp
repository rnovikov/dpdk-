#include "cdpdk.h"

#include <rte_eal.h>
#include <rte_common.h>
#include <rte_ring.h>
#include <rte_mempool.h>
#include <rte_mbuf.h>
#include <tools/dpdk_tools.h>

#include <rte_lcore.h>
#include <rte_vhost.h>


#include "cknimanager.h"

#include <tools/string_tools.h>
#include <rte_ethdev.h>



using namespace std;

cDpdk::cDpdk( bool isEnabled ) : isEnabled_( isEnabled )
{
}
// 0 core for linux, first core - index 0, second - index 1
static inline string getCpuMap( uint32_t totalCores )
{
    stringstream ss;
    totalCores += 1;
    uint currentIndex = 0;
    for( uint i = 1; i < totalCores; ++i )
    {
        ss << currentIndex << "@" << i;
        if( i != totalCores - 1 )
        {
            ss << ",";
        }
        currentIndex++;
    }
    return ss.str();
}
uint32_t cDpdk::init(const nlohmann::json &js, const std::string& zeroArg )
{
    uint32_t wrks= js["workers"];

    vector<string> dpdkArgs;
    dpdkArgs.push_back( zeroArg );
    dpdkArgs.push_back( "--file-prefix" );
    dpdkArgs.push_back( "linz_sp" );

    dpdkArgs.push_back( "-n" );
    dpdkArgs.push_back( to_string( 2) );
    dpdkArgs.push_back( "--lcores" );
    dpdkArgs.push_back(getCpuMap(wrks + 1 ) );


    // dpdkArgs.push_back("--trace=.*");
    const char** argv_tmp = new const char*[dpdkArgs.size()];
    for( uint i = 0; i < dpdkArgs.size(); ++i )
    {
        L_INIT << "DPDK_ARG[" << i << "]:" << dpdkArgs[i];
        argv_tmp[i] = dpdkArgs[i].c_str();
    }
    int ret = rte_eal_init( dpdkArgs.size(), (char**)argv_tmp );
    delete[] argv_tmp;
    TA_LOGIC_ERROR( ret >= 0 );

    uint32_t workCores = rte_lcore_count() - 1;
    TA_LOGIC_ERROR( workCores == wrks );

    L_INIT << "DPDK IFACES:" << (uint32_t)rte_eth_dev_count_total() << " aval=" << (uint32_t)rte_eth_dev_count_avail();



    uint32_t createdPools=0;
    for(const nlohmann::json & item:js["pools"])
    {
        string name=item["name"];
        uint64_t objSize=item["objSize"];
        uint64_t objCount=item["objCount"];
        uint32_t perCoreCache=item["perCoreCache"];
        rte_mempool * answer=makeDpdkPool(name,objCount,objSize,perCoreCache);
        TA_BAD_POINTER(answer);
        createdPools++;
    }
    TA_LOGIC_ERROR(createdPools);








    return workCores;
}

uint32_t cDpdk::getDpdkPortsCount()
{
    return rte_eth_dev_count_avail();
}

cDpdk::~cDpdk()
{
    L_INIT << "DPDK DESCTRUCTOR ";
    //    rte_exit(0,"DONE");
}


uint32_t cDpdk::cores() const
{
    uint32_t answer = rte_lcore_count();
    TA_LOGIC_ERROR( answer == 4 );
    return answer;
}




rte_mempool *cDpdk::makeDpdkPool(const std::string &name, uint32_t objCount, uint64_t objSize,uint32_t perCoreBuffer)
{
    rte_mempool * answer=nullptr;
    TA_LOGIC_ERROR(pools_.find(name)==pools_.end());


    L_INIT << "HERE";
    uint64_t totalSize = objSize + RTE_PKTMBUF_HEADROOM + sizeof( rte_mbuf );
    L_INIT << "CREATING POOL[" << name
           << "] TOTAL SIZE:" << totalSize *objCount  << " elSize:" << totalSize;
    answer = rte_pktmbuf_pool_create(
        name.c_str(), objCount, perCoreBuffer, 0, totalSize, DC_DEFAULT_SOCKET );
    if(answer)
    {
        pools_[name]=answer;
    }
    else
    {
        NOT_IMPLEMENTED;
    }


    return answer;

}
