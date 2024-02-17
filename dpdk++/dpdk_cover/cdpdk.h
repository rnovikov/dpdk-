#ifndef CDPDK_H
#define CDPDK_H

#include <map>
#include <nlohmann/json.hpp>
#include <base/base_types.h>

struct rte_mempool;
struct rte_ring;


class cDpdk
{
public:
    cDpdk( bool enable );
    cDpdk( const cDpdk& ) = delete;
    uint32_t init(const nlohmann::json & js , const std::string& zeroArg );
    uint32_t getDpdkPortsCount();
    ~cDpdk();
    void runLcores();
    rte_mempool* getDefaultPool(){NOT_IMPLEMENTED; return nullptr;}

    uint32_t cores() const;


    uint64_t getTestMbufMax( uint32_t cores, double mutlpler );




    rte_ring* createNamedRing( const std::string& name, uint32_t size = 0 );
    uint32_t initInterfaces();



protected:

    bool isEnabled_;

    std::map<std::string,rte_mempool *> pools_;


private:
    rte_mempool * makeDpdkPool(const std::string & name, uint32_t objCount, uint64_t objSize, uint32_t perCoreBuffer);
};

#endif // CDPDK_H
