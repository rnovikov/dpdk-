#include "cdpdkport.h"
#include <tools/tools.h>
#include <tools/dpdk_tools.h>
#include <rte_ethdev.h>
#include "private_rss_keys.h"
cDpdkPort::cDpdkPort(uint32_t portId, const std::string &name,  const std::string &pci):cRealPort(portId,name),pci_(pci)
{
	dpdkPortId_=tools::getDpdkPortIdByPci(pci_);
}

cDpdkPort::~cDpdkPort()
{
	TA_DO_NOT_USE_IT;
}

void cDpdkPort::init()
{

	TA_NOT_IMPLEMENTED;
}

void cDpdkPort::initQueues(uint16_t rxQueues, uint16_t txQueues)
{
	TA_NOT_IMPLEMENTED;
}

sFastPathPortQueueApi cDpdkPort::getRxQueue(tQueueId qId)
{

	TA_NOT_IMPLEMENTED;
	return sFastPathPortQueueApi();
}

sFastPathPortQueueApi cDpdkPort::getTxQueue(tQueueId qId)
{

	TA_NOT_IMPLEMENTED;
	return sFastPathPortQueueApi();
}

bool cDpdkPort::setLinkState(bool isOn)
{
	if(isOn==isLinkUp_)
	{
		return true;
	}

	int ret=1;
	if(isOn)
	{
		ret= rte_eth_dev_set_link_up(dpdkPortId());
	}
	else
	{
		ret= rte_eth_dev_set_link_down(dpdkPortId());
	}

	if(ret)
	{
		return false;
	}
	isLinkUp_=true;
	return true;
}

bool cDpdkPort::setPromiscMode(bool isOn)
{
	if(isOn)
	{
		return 0==rte_eth_promiscuous_enable(dpdkPortId());
	}
	else
	{
		return 0==rte_eth_promiscuous_disable(dpdkPortId());
	}
}

bool cDpdkPort::startDevice()
{
	return 0==rte_eth_dev_start(dpdkPortId());
}

bool cDpdkPort::stopDevice()
{
	return 0==rte_eth_dev_stop(dpdkPortId());

}

rte_eth_conf cDpdkPort::getDpdkPortConf()
{

	rte_eth_conf port_conf;
	tools::zero_struct( port_conf );
	port_conf.rxmode.mq_mode = RTE_ETH_MQ_RX_RSS;
	port_conf.rx_adv_conf.rss_conf.rss_key = (uint8_t*)rss_key_default;
	port_conf.rx_adv_conf.rss_conf.rss_key_len = sizeof( rss_key_default );
	port_conf.rx_adv_conf.rss_conf.rss_hf = 0x38d34; //< stub!
	port_conf.txmode.mq_mode = RTE_ETH_MQ_TX_NONE;
	return port_conf;
}

rte_eth_rxconf cDpdkPort::getDpdkRxConf()
{
	rte_eth_rxconf answer;
	rte_eth_dev_info info = getDevInfo();

	answer = info.default_rxconf;
	return answer;
}

rte_eth_txconf cDpdkPort::getDpdkTxConf()
{
	rte_eth_txconf answer;
	rte_eth_dev_info info;
	tools::zero_struct( info );
	rte_eth_dev_info_get( dpdkPortId(), &info );
	answer = info.default_txconf;
	return answer;

}

rte_eth_dev_info cDpdkPort::getDevInfo() const
{
	return tools::getDevInfo(dpdkPortId());
}

