#include "cdpdk.h"
#include <iostream>
#include <sstream>
#include <general/error_handler.h>
#include <rte_eal.h>

using namespace std;
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

void cDpdk::initPorts(f_port_create_fabric f_fabric)
{
	for(sj)
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
	TA_LOGIC_ERROR(ret>=0);
	delete argv_tmp;
}

