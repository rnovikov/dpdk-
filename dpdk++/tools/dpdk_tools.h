#ifndef DPDK_TOOLS_H
#define DPDK_TOOLS_H

#include <string>
#include <rte_ethdev.h>

struct rte_pci_addr;
namespace tools
{
	uint32_t getDpdkPortIdByPci(const std::string & pci);
	rte_eth_dev_info getDevInfo(uint32_t dpdkPortId);
	std::string pciToStr( const rte_pci_addr* pci );
}

#endif // DPDK_TOOLS_H
