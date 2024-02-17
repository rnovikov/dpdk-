#include "cknimanager.h"
#include "cknicover.h"
#include <rte_kni.h>
#include <tools/dpdk_tools.h>
#include "cdpdk.h"

#include <tools/string_tools.h>

#define LOCK_OPERATION std::lock_guard<std::mutex> guard( myMutex_ )
cKniManager* cKniManager::instance_ = nullptr;
using namespace std;

cKniManager* cKniManager::getInstance()
{
    THROW_ASSERT( instance_, "NO INSTANCE" );
    return instance_;
}

bool cKniManager::isInited()
{
    return instance_ != nullptr;
}

cKniManager::cKniManager( cDpdk& d ) : objKeeper_( "kniCoverKeeper" ), dpdk_( d )
{
}

cKniManager::~cKniManager()
{
    std::vector<cKniCover*> objects = objKeeper_.getAllAvaliableObjects();
    for( cKniCover* item : objects )
    {
        freeKniByPtr( item );
    }
}

int cKniManager::kni_change_mtu( uint16_t port_id, unsigned int new_mtu )
{
    return getInstance()->getKniByIndex( port_id )->changeMtu( new_mtu );
}

int cKniManager::kni_config_network_if( uint16_t port_id, uint8_t if_up )
{
    return getInstance()->getKniByIndex( port_id )->configMetworkIf( if_up );
}

int cKniManager::kni_config_mac_address( uint16_t port_id, uint8_t mac_addr[] )
{
    return getInstance()->getKniByIndex( port_id )->configMacAddress( mac_addr );
}

int cKniManager::kni_config_promiscusity( uint16_t port_id, uint8_t to_on )
{
    return getInstance()->getKniByIndex( port_id )->configPromiscusity( to_on );
}

int cKniManager::kni_config_allmulticast( uint16_t port_id, uint8_t to_on )
{
    return getInstance()->getKniByIndex( port_id )->configAllmulticast( to_on );
}

void cKniManager::init( uint32_t maxKniCount, cDpdk& dpdk )
{
    TA_ALREADY_INITED( instance_ );
    instance_ = new cKniManager( dpdk );
    instance_->mempool_ = dpdk.getDefaultPool();
    instance_->maxKniCount_ = maxKniCount;
    THROW_ASSERT( instance_->mempool_, "BAD MEMPOOL POINTER" );
    int ret = rte_kni_init( maxKniCount );
    THROW_ASSERT( 0 == ret, "CAN NOT INIT KNI" );
}

void cKniManager::deinit()
{
    TA_BAD_POINTER( instance_ );
    if( instance_ )
    {
        delete instance_;
        instance_ = nullptr;
    }
}

cKniCover* cKniManager::createKni( const std::string& name, const std::string& macAddr )
{
    LOCK_OPERATION;

    THROW_ASSERT( objKeeper_.freeIndexesCount(), "NO INDEXES" );
    rte_ether_addr mac;
    dc_tools::strToMac( macAddr, &mac );
    cKniCover* kni = new cKniCover( name, mac );
    THROW_ASSERT( kni, "CAN NOT CREATE KNI OBJ" );
    uint32_t ind = objKeeper_.addNewObj( kni );
    rte_kni_ops ops;
    zero_struct( ops );
    ops.port_id = ind;
    ops.change_mtu = cKniManager::kni_change_mtu;
    ops.config_allmulticast = cKniManager::kni_config_allmulticast;
    ops.config_mac_address = cKniManager::kni_config_mac_address;
    ops.config_network_if = cKniManager::kni_config_network_if;
    ops.config_promiscusity = cKniManager::kni_config_promiscusity;
    kni->init( mempool_, &ops, ind );
    // objKeeper_.setInited( ind );
    return kni;
}

cKniCover* cKniManager::getKniByName( const std::string& name ) const
{
    LOCK_OPERATION;
    std::vector<cKniCover*> allKni = objKeeper_.getAllAvaliableObjects();
    for( cKniCover* kni : allKni )
    {

        if( kni->name() == name )
        {
            return kni;
        }
    }
    return nullptr;
}

uint32_t cKniManager::getKniIndexByName( const std::string& name ) const
{
    uint32_t answer = BAD_UINT32_VALUE;
    cKniCover* kni = getKniByName( name );
    if( kni )
    {
        answer = kni->index();
    }
    return answer;
}

void cKniManager::freeKniByPtr( cKniCover* kni )
{
    freeKniByIndex( kni->index() );
}

void cKniManager::freeKniByIndex( uint32_t index )
{
    cKniCover* kni = objKeeper_.removeObjectById( index );
    L_NOTICE << kni->toStr();
    delete kni;
}

void cKniManager::handleKni()
{
    int index = 0;
    for( cKniCover* item : objKeeper_.getAllAvaliableObjects() )
    {
        item->handleKni();
        index++;
    }
}

string cKniManager::toStr() const
{
    stringstream ss;
    ss << "MaxKni:" << maxKniCount_ << endl;
    ss << "CAPA:" << objKeeper_.capacity() << endl;
    ss << "FREE COUNT:" << objKeeper_.freeIndexesCount() << endl;

    return ss.str();
}

string cKniManager::dump() const
{
    LOCK_OPERATION;
    stringstream ss;
    ss << toStr() << endl;
    return ss.str();
}

// cInterface *cKniManager::makeInterface(const sIfaceParam &param)
//{
//     iPhysPort * phPort=dpdk_.createFakePort(param.portName_,param.portMac_);

//    cKniCover* kni= createKni(param.name_,param.ifMac_);
//    cInterface * answer=new
//    cInterface(phPort->getTxQueues()[0],phPort->getRxQueues()[0],kni,tools::ipFromStr(param.ifaceIp_));

//    return answer;
//}
