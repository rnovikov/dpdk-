#ifndef DPDK_TOOLS_H
#define DPDK_TOOLS_H
#include <base/base_types.h>
#include <rte_pci.h>
#include <rte_tcp.h>
#include <rte_flow.h>
#include <packet/sfivetyplesession.h>
struct rte_mempool;
struct rte_ring;
struct rte_ether_addr;
struct rte_tcp_hdr;
struct rte_eth_dev_info;
struct rte_pci_device;
struct st_rfc3550_rtp_hdr;

namespace dc_tools
{

std::string strRtpBaseHdr( st_rfc3550_rtp_hdr* rtp );
std::string strTcpHdr( rte_tcp_hdr* tcp );
struct sTcpOptions
{
    uint16_t mss_ = 0;
    uint8_t wndScale_ = 0;

    std::string toStr() const;
};
#define FLOW_MAX_PATTERN_NUMBER 8
#define FLOW_MAX_ACTION_NUMBER 4
struct sFlowItem
{

    rte_flow_attr attr_;

private:
    rte_flow_item pattern_[FLOW_MAX_PATTERN_NUMBER];

public:
    rte_flow_action action_[FLOW_MAX_ACTION_NUMBER];
    rte_flow_action_port_id tmpNamedAction;

    uint32_t currentPatternId_ = 0;

    void addEthPattern();
    void addStopPattern();
    void addIpPattern();

    rte_flow_item* getPatternPointer()
    {
        TA_LOGIC_ERROR( currentPatternId_ == BAD_UINT32_VALUE );
        return &pattern_[0];
    }

    rte_flow_item& getPatternAndIncrementIndex();

    void setQueueAction( const rte_flow_action_queue& q )
    {
        action_[0].type = RTE_FLOW_ACTION_TYPE_QUEUE;
        action_[0].conf = &q;
        action_[1].type = RTE_FLOW_ACTION_TYPE_END;
    }
    void setPaththrough()
    {
        NOT_IMPLEMENTED;
        tmpNamedAction.id = 0;
        tmpNamedAction.original = 0;
        action_[0].type = RTE_FLOW_ACTION_TYPE_PORT_ID;
        action_[0].conf = &tmpNamedAction;
        action_[1].type = RTE_FLOW_ACTION_TYPE_END;
    }
    sFlowItem( uint32_t id )
    {
        zero_struct( attr_ );
        zero_struct( pattern_ );
        zero_struct( action_ );
        zero_struct( tmpNamedAction );
        attr_.priority = id;
    }
};
std::string strMempool( rte_mempool* pool );

std::string strMacAddr( const rte_ether_addr* mac );

bool strToMac( const std::string& macIn, rte_ether_addr* macOut );
std::string strMacAddr( const uint8_t mac_addr[] );

std::string strDevInfo( rte_eth_dev_info* info );

sTcpOptions parceTcpOptions( rte_tcp_hdr* tcp );

uint64_t get_microseconds();

std::string strPciAddr( rte_pci_device* dev );

static inline uint64_t sizeToMask( uint32_t size )
{
    TA_LOGIC_ERROR( size <= ( sizeof( uint64_t ) * 8 ) ); // less or equal bitcount in uint64_t
    uint64_t answer = 0;
    for( uint i = 0; i < size; ++i )
    {
        answer <<= 1;
        answer += 1;
    }
    return answer;
}

rte_flow* make_port_rule( uint32_t port, uint32_t rxQ, const sFiveTypleSession& key );

} // namespace tools

#endif // DPDK_TOOLS_H
