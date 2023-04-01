#include "dpdk_tools.h"
#include <rte_ethdev.h>
#include <rte_bus_pci.h>
#include <rte_pci.h>
#include <bus_pci_driver.h>
#include <general/base_defines.h>
#include <logger/logger_api.h>
#include <tools/tools.h>
#include <rte_pci.h>
#include <string>
#include <iomanip>
#include <rte_bus_pci.h>
#include <general/error_handler.h>
using namespace std;
namespace tools {
rte_eth_dev_info getDevInfo(uint32_t dpdkPortId)
{
	rte_eth_dev_info answer;
	zero_struct(answer);
	rte_eth_dev_info_get(dpdkPortId,&answer);


	return answer;
}

string pciToStr( const rte_pci_addr* pci )
{
	stringstream ss;
	ss << setfill( '0' ) << setw( 4 ) << hex << pci->domain << ":" << setw( 2 ) << ( (uint32_t)pci->bus ) << ":"
	   << setw( 2 ) << ( (uint32_t)pci->devid ) << "." << ( (uint32_t)pci->function );
	return ss.str();
}

uint32_t getDpdkPortIdByPci(const string &pci)
{
	uint32_t ports_count = rte_eth_dev_count_avail();

	for(uint i =0; i < ports_count;++i)
	{
		rte_eth_dev_info info=getDevInfo(i);
		rte_pci_device* pci_dev = RTE_DEV_TO_PCI( info.device );
		std::string curPci = tools::pciToStr( &pci_dev->addr );
		L_COUT<<"COMPARE "<<curPci<<" -- "<<pci<<endl;
		if(curPci==pci)
		{
			return i;
		}

	}






	TA_NOT_IMPLEMENTED;

//	rte_eth_dev_info dev_info;
//	tools::zero_struct( dev_info );
//	rte_eth_dev_info_get( i, &dev_info );
//	rte_pci_device* pci_dev = RTE_DEV_TO_PCI( dev_info.device );
	return BAD_UINT32_VALUE;

}
}
