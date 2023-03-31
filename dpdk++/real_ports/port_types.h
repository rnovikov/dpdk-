#ifndef PORT_TYPES_H
#define PORT_TYPES_H
#include <stdint.h>
#include <general/error_handler.h>
#include <general/base_defines.h>
#include <general/base_types.h>
#include <general/forward_declaration.h>

struct sInitRealPortParam
{
	std::string  driverName_;
	uint32_t portId_=BAD_UINT16_VALUE;
	uint32_t dpdkPortId_=BAD_UINT16_VALUE;
};

typedef iRealPort * ( *f_port_create_fabric )( const sInitRealPortParam & params  );

typedef uint16_t ( *f_dpdk_real_port_burst )( void* rxq, struct rte_mbuf** rx_pkts, uint16_t nb_pkts );

static uint16_t fake_rx_function( void* rxq, struct rte_mbuf** rx_pkts, uint16_t nb_pkts )
{
	TA_NOT_IMPLEMENTED;
	return 0;
}
static uint16_t fake_tx_function( void* txq, struct rte_mbuf** tx_pkts, uint16_t nb_pkts )
{
	TA_NOT_IMPLEMENTED;
	return 0;
}

struct sFastPathPortQueueApi
{
	f_dpdk_real_port_burst func_;
	void* data_;
	tRealPort portId_ = BAD_UINT8_VALUE;
//	void initAsRxFake( uint16_t qId, tRealPort portId );
//	void initAsTxFake( uint16_t qId, tRealPort portId );
	std::string toStr() const;
};

struct sMirrorPortData
{
	iMirrorPoint* mp_ = nullptr;
	sFastPathPortQueueApi api_;
	bool isRx_ = false;

	bool isInited() const;
	void clear();
};


struct sRealPortConfig
{

};


#endif // PORT_TYPES_H
