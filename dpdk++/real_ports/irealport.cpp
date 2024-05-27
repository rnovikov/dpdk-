#include "irealport.h"
#include "cdpdkport.h"
#include <logger/logger_api.h>


iRealPort::iRealPort(uint32_t portId, const std::string &name)
	:portId_(portId),name_(name)
{

}

iRealPort::~iRealPort()
{
	TA_DO_NOT_USE_IT;
}

iRealPort *iRealPort::port_fabric_default(uint32_t id,  const sRealPortParam &port)
{
	iRealPort * answer=nullptr;
	switch (port.type_) {
	case ePhysPortConfigType::DPDK:
	{
		L_DEV<<port.driverName_;
		if(port.driverName_=="net_e1000_igb")
		{
			answer= new cDpdkPort(id,port.name_,port.pci_);
		}

	}break;

	default:
		TA_NOT_IMPLEMENTED;
		break;
	}
	TA_BAD_POINTER(answer);

	return answer;
}

void iRealPort::exportToRow(tools::cTableRow &row, const std::vector<std::string> &headers)
{
	for ( const std::string & item: headers)
	{



	}
	TA_NOT_IMPLEMENTED;
}
