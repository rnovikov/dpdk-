#include "cknicover.h"
#include <rte_kni.h>
#include <unistd.h>
#include "cmbuf.h"
#define DEF_MTU 9010
using namespace std;

class cKniPortReceiver : public iReceiver
{
public:
    cKniPortReceiver( const std::string name, cKniCover* kni ) : iReceiver( name ), kni_( kni )
    {
    }
    virtual uint32_t internal_receive_objects( cMbuf** buffer, uint64_t size )
    {
        return kni_->recvPackets( (cMbuf**)buffer, size );
    }

private:
    cKniCover* kni_ = nullptr;
};

class cKniPortSender : public iSender
{
public:
    cKniPortSender( const std::string name, cKniCover* kni ) : iSender( name ), kni_( kni )
    {
    }
    virtual uint32_t internal_send_objects( cMbuf** buffer, uint64_t size )
    {
        return kni_->sendPackets( (cMbuf**)buffer, size );
    }

private:
    cKniCover* kni_ = nullptr;
};

cKniCover::cKniCover( const std::string& name, const rte_ether_addr& macAddr ) : name_( name ), mac_( macAddr )
{
    rcvs_ = new cKniPortReceiver( name + "rc", this );
    snd_ = new cKniPortSender( name + "sn", this );
}

void cKniCover::init( rte_mempool* mempool, rte_kni_ops* ops, uint32_t index )
{
    static bool firstCreate = true;
    THROW_ASSERT( ops, "BAD POINTER" );
    THROW_ASSERT( kni_ == nullptr, "ALREADY INITED" );
    TA_BAD_POINTER( mempool );
    mempool_ = mempool;
    index_ = index;
    rte_kni_conf kConf;
    zero_struct( kConf );

    THROW_ASSERT( name_.size() < ( RTE_KNI_NAMESIZE - 1 ), "MANE TOO LONG!!" );
    strcpy( kConf.name, name_.c_str() );
    kConf.force_bind = 0;
    kConf.core_id = 0;
    kConf.min_mtu = 30;
    kConf.max_mtu = 20000;
    kConf.mtu = DEF_MTU;
    kConf.mbuf_size = mempool->elt_size - 256;

    for( uint i = 0; i < 6; ++i )
    {
        kConf.mac_addr[i] = mac_.addr_bytes[i];
    }
    if( firstCreate )
    {
        firstCreate = false;
        kni_ = rte_kni_alloc( mempool_, &kConf, ops );
    }
    else
    {
        kni_ = rte_kni_alloc( mempool_, &kConf, nullptr );
    }

    THROW_ASSERT( nullptr != kni_, "CAN NOT INIT KNI" );

    isInited_ = true;
}

int cKniCover::changeMtu( unsigned int new_mtu )
{
    L_DEBUG << "changeMtu:" << new_mtu;
    NOT_IMPLEMENTED;
    return 0;
}

int cKniCover::configMetworkIf( uint8_t if_up )
{
    L_DEBUG << "configMetworkIf:" << ( (uint32_t)if_up );
    updateLink( true );
    return 0;
}

int cKniCover::configMacAddress( uint8_t mac_addr[] )
{

    NOT_IMPLEMENTED;
    return 0;
}

int cKniCover::configPromiscusity( uint8_t to_on )
{
    L_DEBUG << "configPromiscusity:" << to_on ? "ON" : "OFF";
    return 0;
}

int cKniCover::configAllmulticast( uint8_t to_on )
{
    L_DEBUG << "changeMtu:" << to_on ? "ON" : "OFF";
    NOT_IMPLEMENTED;
    return 0;
}

bool cKniCover::isInited() const
{
    return isInited_;
}

void cKniCover::handleKni()
{
    THROW_ASSERT( kni_, "DID NOT INITED" );
    int ret = rte_kni_handle_request( kni_ );
    THROW_ASSERT( 0 == ret, "UNKNOWN ERROR" );
}

bool cKniCover::updateLink( bool val )
{
    L_DEBUG << "updateLink";
    return true;
    TA_BAD_POINTER( kni_ );
    int value = 1;
    if( !val )
    {
        value = 0;
    }
    std::string cmd = "ls /sys/devices/virtual/net/";
    L_DEBUG << system( cmd.c_str() );
    cmd = "echo 1 > /sys/devices/virtual/net/" + name() + "/carrier";
    int ret = system( cmd.c_str() );
    L_DEBUG << "LCORE" << rte_lcore_id();
    for( int i = 0; i < 20; ++i )
    {
        if( ret == 0 )
        {
            break;
        }
        ret = system( cmd.c_str() );
        sleep( 1 );
    }

    ret = rte_kni_update_link( kni_, value );
    if( ret == -1 )
    {
        stringstream ss;
        ss << "STATE:" << ( val ? "TRUE " : "FALSE" ) << " RET:" << ret;
        THROW_ERROR( ss.str() );
    }

    if( ret == 1 && ( val == true ) )
    {
        return true;
    }
    if( ret == 0 && ( val == false ) )
    {
        return true;
    }

    return false;
}

uint32_t cKniCover::sendPackets( cMbuf** buffer, uint32_t size )
{
    TA_LOGIC_ERROR( size );

    for( uint i = 0; i < size; ++i )
    {
        cMbuf* mbuf = buffer[i];
        if( mbuf->rfCount() > 1 )
        {
            L_WARN << mbuf->strSession();
        }
        L_PF_TO_KNI( mbuf, "KNI" );
    }

    uint32_t ret = rte_kni_tx_burst( kni_, (rte_mbuf**)buffer, size );
    TA_LOGIC_ERROR( ret == size );
    counters_.txPackets_ += ret;
    for( uint32_t i = ret; i < size; ++i )
    {
        NOT_IMPLEMENTED;

        //        buffer[i]->free();
        counters_.txDrop_++;
    }
    handleKni();
    return ret;
}

uint32_t cKniCover::recvPackets( cMbuf** buffer, uint32_t size )
{
    TA_LOGIC_ERROR( size );
    handleKni();
    uint32_t answer = rte_kni_rx_burst( kni_, (rte_mbuf**)buffer, size );
    if( answer )
    {

        for( uint i = 0; i < answer; ++i )
        {
            cMbuf* mbuf = buffer[i];
            mbuf->init();
            L_PF_FROM_KNI( mbuf, "  " );
        }

        counters_.rxPackets_ += answer;
    }
    return answer;
}

iReceiver* cKniCover::getReceiver()
{
    TA_BAD_POINTER( rcvs_ );
    return rcvs_;
}

iSender* cKniCover::getSender()
{
    TA_BAD_POINTER( snd_ );
    return snd_;
}

uint32_t cKniCover::index() const
{
    return index_;
}

std::string cKniCover::name() const
{
    return name_;
}

bool cKniCover::readCarrier()
{
    std::string cmd;
    cmd = "cat /sys/devices/virtual/net/" + name() + "/carrier";

    system( cmd.c_str() );
    cmd = "echo 1 > /sys/devices/virtual/net/" + name() + "/carrier";
    system( cmd.c_str() );
    return true;
}

cKniCover::~cKniCover()
{
    usleep( 1 );
    std::lock_guard<std::mutex> guard( myMutex_ );

    TA_BAD_POINTER( kni_ );
    rte_kni_release( kni_ );
    kni_ = nullptr;

    //    rte_kni_update_link
    //    rte_kni_register_handlers
}

std::string cKniCover::toStr() const
{
    stringstream ss;

    return ss.str();
}

string sKniCounters::toStr() const
{
    stringstream ss;
    ss << "RX:" << rxPackets_ << " tx:" << txPackets_ << " DROP:" << txDrop_;

    return ss.str();
}
