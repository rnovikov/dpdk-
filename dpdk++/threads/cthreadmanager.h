#ifndef CTHREADMANAGER_H
#define CTHREADMANAGER_H
#define MAX_WORKERS 128

#include "icorethread.h"
#include <tools/cobjectkeeper.h>
class cThreadManager
{
public:
    static void init(cDpdk *dpdk, uint32_t maxCores, iCoreThread* wrkMain );
    static cThreadManager  * getInstance();
    static int run_thread_dpdk( void* arg );
    static int run_single_thread_dpdk( void* arg );
    static bool add_and_run( iCoreThread* wrk );

    void addCoreThread( iCoreThread* worker );
    void removeCoreThread( iCoreThread* wrk );
    void setMain( iCoreThread* worker );
    bool checkCoreThread( iCoreThread* worker ) const;
    bool runCoreThread( iCoreThread* worker );



    iCoreThread *getWorkerByLcore(uint32_t lcoreId);


    void stop( bool includeMain = false );
    uint32_t getFreeWorkers() const;
    void main_loop();
private:
    cThreadManager(cDpdk * dpdk);
    ~cThreadManager();
    dc_tools::cObjKeeper<iCoreThread, MAX_WORKERS> workers_;


    std::mutex myMytex_;

    cDpdk * dpdk_;
    static cThreadManager* instance_;
    static uint64_t maxCores_;


};

#endif // CTHREADMANAGER_H
