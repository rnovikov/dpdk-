#ifndef CDPDKPORT_H
#define CDPDKPORT_H
#include <real_ports/crealport.h>
#include <rte_ethdev.h>

class cDpdkPort:public cRealPort
{
public:
	cDpdkPort(uint32_t portId,const std::string &name,const std::string & pci);
	virtual ~cDpdkPort();
	virtual void init();
	virtual void initQueues(uint16_t rxQueues,uint16_t txQueues);
	virtual sFastPathPortQueueApi getRxQueue(tQueueId qId);
	virtual sFastPathPortQueueApi getTxQueue(tQueueId qId);
	virtual bool setLinkState( bool isOn ) ;
	virtual bool setPromiscMode( bool isOn ) ;

	virtual bool startDevice();
	virtual bool stopDevice() ;
	inline uint32_t dpdkPortId()const {return dpdkPortId_;}
protected:
	virtual rte_eth_conf getDpdkPortConf();
	virtual rte_eth_rxconf getDpdkRxConf();
	virtual rte_eth_txconf getDpdkTxConf();

	rte_eth_dev_info getDevInfo()const;
private:
	std::string pci_;
	uint32_t dpdkPortId_=BAD_UINT32_VALUE;
	bool isDeviceStarted_=false;
	bool isLinkUp_=false;

//	static rte_eth_dev_info getDevInfo(uint32_t dpdkId);
};

#endif // CDPDKPORT_H
