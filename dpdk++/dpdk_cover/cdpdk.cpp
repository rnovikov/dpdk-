#include "cdpdk.h"
#include <iostream>
#include <sstream>
#include <general/error_handler.h>
#include <rte_eal.h>
#include <rte_ethdev.h>
#include <tools/dpdk_tools.h>
#include <logger/logger_api.h>
using namespace std;
using namespace config;

cDpdk * cDpdk::instance_=nullptr;

void cDpdk::init(const std::string &data,const std::string & zeroArg )
{
	TA_ALREADY_INITED(instance_);
	instance_=new cDpdk;
	TA_BAD_POINTER(instance_);
	instance_->internalInit(data,zeroArg);
}

cDpdk *cDpdk::getInstance()
{
	TA_BAD_POINTER(instance_);
	return instance_;
}

string cDpdk::toStr() const
{
	stringstream ss;
	for(const string & item:params_.args())
	{
		ss<<item<<", ";
	}
	ss<<endl;

	return ss.str();

}

std::vector<iRealPort *> cDpdk::getRealPorts()const
{
	return portsArray_;
}

void cDpdk::initPorts(f_port_create_fabric f_fabric)
{
	TA_ALREADY_INITED(portsArray_.size());

	for(sRealPortParam item:params_.getPorts())
	{
		iRealPort * realPortItem=nullptr;
		if(item.type_==ePhysPortConfigType::DPDK)
		{
			uint32_t dpdkDevPort=tools::getDpdkPortIdByPci(item.pci_);
			TA_LOGIC_ERROR(dpdkDevPort!=BAD_UINT32_VALUE);
			L_COUT<<dpdkDevPort;
			rte_eth_dev_info info=tools::getDevInfo(dpdkDevPort);
			item.driverName_=info.driver_name;
			realPortItem=f_fabric(portsArray_.size(),item);


		}
		else
		{
			TA_NOT_IMPLEMENTED;
		}
		TA_BAD_POINTER(realPortItem);
		portsArray_.push_back(realPortItem);
	}

}

cDpdk::cDpdk()
{

}

void cDpdk::internalInit(const std::string &data, const std::string &zeroArg)
{
	params_.load(data);
	vector<string> dpdkArgs;
	dpdkArgs.push_back(zeroArg);
	for (const string & item:params_.args())
	{
		dpdkArgs.push_back(item);
	}

	const char ** argv_tmp=new const char *[dpdkArgs.size()];
	for (uint i =0; i < dpdkArgs.size(); ++i)
	{
		argv_tmp[i]=dpdkArgs[i].c_str();
	}
	int ret=rte_eal_init(dpdkArgs.size(),(char **)argv_tmp);
	L_DEV<<ret;
	TA_LOGIC_ERROR(ret>=0);
	delete argv_tmp;
}

