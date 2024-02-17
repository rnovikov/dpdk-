#include "cthreadmanager.h"
#include <dpdk_cover/cdpdk.h>
#include <rte_lcore.h>
using namespace std;



cThreadManager* cThreadManager::instance_ = nullptr;
uint64_t cThreadManager::maxCores_ = 0;
int cThreadManager::run_thread_dpdk( void* arg )
{
    cThreadManager* mng = (cThreadManager*)arg;
    iCoreThread* worker = mng->getWorkerByLcore( rte_lcore_id() );

    L_INIT << "RUN CORE:" << worker->toStr();

    worker->loop();
    lock_guard<mutex> guard( mng->myMytex_ );

    L_WARN << "REMOVING: " << worker->toStr();
    L_INIT << worker->toStr();
    mng->removeCoreThread( worker );

    return 0;
}

cThreadManager* cThreadManager::getInstance()
{
    TA_LOGIC_ERROR( instance_ );
    return instance_;
}

void cThreadManager::init( cDpdk* dpdk, uint32_t maxCores, iCoreThread* wrkMain )
{
    maxCores_ = maxCores;
    TA_LOGIC_ERROR( maxCores_ == maxCores );
    TA_ALREADY_INITED( instance_ );
    instance_ = new cThreadManager( dpdk );


    if( wrkMain )
    {
        instance_->addCoreThread( wrkMain );
        TA_LOGIC_ERROR( wrkMain->lcoreId() == 0 );
    }
}

bool cThreadManager::add_and_run( iCoreThread* wrk )
{
    TA_BAD_POINTER( instance_ );
    TA_BAD_POINTER( wrk );
    instance_->addCoreThread( wrk );
    return instance_->runCoreThread( wrk );
}

void cThreadManager::addCoreThread(iCoreThread *worker)
{
    TA_LOGIC_ERROR( worker );
    for( iCoreThread* item : workers_.getAllAvaliableObjects() )
    {
        TA_LOGIC_ERROR( item != worker );
    }

    uint32_t id = workers_.addNewObj( worker );
    //    worker->setLcoreId(id,controlMessage_[id].reciever_);
    worker->setManagerParams( id );
    L_ERROR << worker->toStr();
}

void cThreadManager::removeCoreThread(iCoreThread *wrk)
{

    iCoreThread* tmp = workers_.removeObjectById( wrk->lcoreId() );
    TA_LOGIC_ERROR( tmp == wrk );
    delete tmp;
}


void cThreadManager::setMain( iCoreThread* worker )
{
    bool ret = workers_.insertObj( worker, 0 );
    TA_LOGIC_ERROR( ret );
}

bool cThreadManager::runCoreThread(iCoreThread *worker)
{
    iCoreThread* wrk = nullptr;

    for( iCoreThread* item : workers_.getAllAvaliableObjects() )
    {
        if( item == worker )
        {
            wrk = worker;
        }
    }
    TA_LOGIC_ERROR( wrk );
    rte_eal_remote_launch( run_thread_dpdk, this, wrk->lcoreId() );
    L_DEBUG<<"WOREKER RUNNED :"<<getFreeWorkers();
    return true;

}

bool cThreadManager::checkCoreThread( iCoreThread* worker ) const
{
    std::vector<iCoreThread*> arr = workers_.getAllAvaliableObjects();
    for( iCoreThread* item : arr )
    {
        if( item == worker )
        {
            return true;
        }
    }
    return false;
}

iCoreThread* cThreadManager::getWorkerByLcore( uint32_t lcoreId )
{
    TA_LOGIC_ERROR( lcoreId );
    TA_LOGIC_ERROR( lcoreId <= workers_.size() );
    iCoreThread* answer = workers_.getObj( lcoreId );
    TA_LOGIC_ERROR( answer );
    TA_LOGIC_ERROR( answer->lcoreId() == lcoreId );

    return answer;
}

void cThreadManager::stop( bool includeMain )
{
    TA_LOGIC_ERROR( rte_lcore_id() == 0 );
    for( iCoreThread* item : workers_.getAllAvaliableObjects() )
    {
        if( includeMain )
        {
            NOT_IMPLEMENTED;
            item->stop();
        }
        else
        {

            if( item->lcoreId() )
            {
                item->stop();
            }
        }
    }
    bool ready = false;
    while( !ready )
    {

        uint32_t unready = 0;
        for( iCoreThread* item : workers_.getAllAvaliableObjects() )
        {
            if( !item->isDone() )
            {
                unready++;
            }
        }
        ready = ( unready == 0 );
        if( unready )
        {
            L_DEBUG << "uready:" << unready;
        }
    }
    L_DEBUG << "ALL WORKERS DONE";
}

void cThreadManager::main_loop()
{
    iCoreThread* worker = workers_.getObj( 0 );
    TA_LOGIC_ERROR( worker );

    worker->loop();
}
cThreadManager::cThreadManager( cDpdk * dpdk ) : dpdk_( dpdk ), workers_( "wrksObjKeeper" )
{
}

cThreadManager::~cThreadManager()
{
    for( iCoreThread* item : workers_.getAllAvaliableObjects() )
    {
        if( item->lcoreId() )
        {
            delete workers_.removeObjectById( item->lcoreId() );
        }
    }
    delete workers_.removeObjectById( 0 );
    NOT_IMPLEMENTED;
}

uint32_t cThreadManager::getFreeWorkers() const
{
    TA_LOGIC_ERROR( workers_.size() <= maxCores_ );
    L_DEBUG << workers_.getAllAvaliableObjects().size() << "  " << workers_.size();

    return maxCores_ - workers_.size();
}
