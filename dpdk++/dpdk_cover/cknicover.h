#ifndef CKNICOVER_H
#define CKNICOVER_H
#include <base/base_types.h>
#include <rte_ether.h>
struct rte_kni_ops;
struct rte_kni;
class cMbuf;

struct sKniCounters
{
    uint64_t rxPackets_ = 0;
    uint64_t txPackets_ = 0;
    uint64_t txDrop_ = 0;

    std::string toStr() const;
};

/**
 * @brief The cKniCover class
 * cover for Kernel Nerwork Interface
 *
 *
 */

class cKniCover
{
public:
    /**
     * @brief handleKni
     * get processor time to check all comands from kernel, like up, down, change mtu
     */
    void handleKni();

    uint32_t sendPackets( cMbuf** buffer, uint32_t size );
    uint32_t recvPackets( cMbuf** buffer, uint32_t size );

    iReceiver* getReceiver();
    iSender* getSender();
    uint32_t index() const;
    std::string name() const;
    /**
     * @brief setKniType
     * @param type Knis in DF has different purposes
     * @return
     */
    bool readCarrier();

    void setL3Index( uint32_t l3Index );
    std::string toStr() const;

    inline sKniCounters getCounters() const
    {
        return counters_;
    }
    inline rte_kni* kni()
    {
        return kni_;
    }

protected:
    friend class cKniManager;
    ~cKniCover();
    /// Only owner(kni manager) can use this functions!!!!
    explicit cKniCover( const std::string& name, const rte_ether_addr& macAddr );

    void init( rte_mempool* mempool, rte_kni_ops* ops, uint32_t index );
    bool updateLink( bool val );
    int changeMtu( unsigned int new_mtu );
    int configMetworkIf( uint8_t if_up );
    int configMacAddress( uint8_t mac_addr[] );
    int configPromiscusity( uint8_t to_on );
    int configAllmulticast( uint8_t to_on );

    bool isInited() const;

private:
    rte_kni* kni_ = nullptr;
    sKniCounters counters_;
    bool isInited_ = false;
    rte_mempool* mempool_ = nullptr;
    uint32_t index_ = BAD_UINT32_VALUE;
    rte_ether_addr mac_;
    std::string name_;

    iReceiver* rcvs_ = nullptr;
    iSender* snd_ = nullptr;

    mutable std::mutex myMutex_;
};

#endif // CKNICOVER_H
