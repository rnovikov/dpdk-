#ifndef CKNIMANAGER_H
#define CKNIMANAGER_H
#include <base/base_types.h>
#include <tools/cobjectkeeper.h>
#define MAX_KNI_COUNT 32



class cDpdk;
class cKniCover;
class cKniManager
{
public:
    static cKniManager* getInstance();
    static bool isInited();
    static void init( uint32_t maxKniCount, cDpdk& dpdk );
    static void deinit();

    /**
     * @brief createKni init kni for any purposes
     * @param name - name, how it will show in "ifconfig"cmd
     * @param macAddr default kni mac addres
     * @return
     */
    cKniCover* createKni( const std::string& name, const std::string& macAddr );
    cKniCover* getKniByName( const std::string& name ) const;
    uint32_t getKniIndexByName( const std::string& name ) const;

    void freeKniByPtr( cKniCover* kni );
    void freeKniByIndex( uint32_t index );

    void handleKni();

    DC_ALWAYS_INLINE cKniCover* getKniByIndex( uint32_t index ) const;

    std::string toStr() const;

    std::string dump() const;

protected:
    friend class cKniCover;

private:
    cKniManager( cDpdk& dpdk );
    ~cKniManager();
    cDpdk& dpdk_;

    rte_mempool* mempool_ = nullptr;
    dc_tools::cObjKeeper<cKniCover, MAX_KNI_COUNT> objKeeper_;
    uint32_t maxKniCount_ = BAD_UINT32_VALUE;

    mutable std::mutex myMutex_;
    static cKniManager* instance_;

    /// PRIVATE API Proxy Functions list
    /* Pointer to function of changing MTU */
    static int kni_change_mtu( uint16_t port_id, unsigned int new_mtu );

    /* Pointer to function of configuring network interface */
    static int kni_config_network_if( uint16_t port_id, uint8_t if_up );

    /* Pointer to function of configuring mac address */
    static int kni_config_mac_address( uint16_t port_id, uint8_t mac_addr[] );

    /* Pointer to function of configuring promiscuous mode */
    static int kni_config_promiscusity( uint16_t port_id, uint8_t to_on );

    /* Pointer to function of configuring allmulticast mode */
    static int kni_config_allmulticast( uint16_t port_id, uint8_t to_on );
};

DC_ALWAYS_INLINE cKniCover* cKniManager::getKniByIndex( uint32_t index ) const
{
    return objKeeper_.getObj( index );
}

#endif // CKNIMANAGER_H
