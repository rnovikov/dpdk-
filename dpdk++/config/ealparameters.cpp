#include "ealparameters.h"
#include <general/error_handler.h>
#define FAKE_PCI_ADDR "ffff:ff:ff.0"
using namespace std;

namespace config {


sEalParameters::sEalParameters()
{

}

void sEalParameters::load(const std::string &data)
{
	nlohmann::json js=nlohmann::json::parse(data);
	process_.load(js["process"]);
	lcores_=js.at("lcores").get<std::vector<int> >();
	memory_= js.at("memory").get<std::vector<int> >();
	loadPortsConfig(js.at("ports").get<const std::vector<nlohmann::json> >());

	channels_= js.value("channels", 4);
	rank_ = js.value("rank", 2);
}

sJsonProcess sEalParameters::getProcess() const
{
	return process_;
}

std::vector<int> sEalParameters::getLcores() const
{
	return lcores_;
}

std::vector<int> sEalParameters::getMemory() const {
	return memory_;
}

std::vector<sRealPortParam> sEalParameters::getPorts() const
{
	return ports_;
}

int sEalParameters::getChannels() const
{
	return channels_;
}

int sEalParameters::getRank() const
{
	return rank_;
}

vector<string> sEalParameters::args() const
{
	// see https://doc.dpdk.org/guides/linux_gsg/linux_eal_parameters.html
	vector<string> answer;

	answer.push_back("--lcores");
	answer.push_back(getLcoresMap());
	answer.push_back("-r");
	answer.push_back(to_string(rank_));

	answer.push_back("-n");
	answer.push_back(to_string(channels_));

	answer.push_back("--file-prefix");
	answer.push_back(process_.prefix_);

	answer.push_back("-m");
	answer.push_back(to_string(memory_[0]));

	answer.push_back("--proc-type");
	if(process_.isPrimary_)
	{
		answer.push_back("primary" );
	}
	else
	{
		answer.push_back("secondary" );
	}

	uint32_t pciPortsCount = 0;

	for(const sRealPortParam &prt :ports_)
	{
		if(prt.type_==ePhysPortConfigType::DPDK)
		{
			pciPortsCount++;
			answer.push_back("-a");
			answer.push_back(prt.pci_);
		}
	}

	if( !pciPortsCount )
	{
		answer.push_back("-a");
		answer.push_back(FAKE_PCI_ADDR);
	}

	answer.push_back( "--no-telemetry" );

	return answer;
}

string sEalParameters::getLcoresMap() const
{
	stringstream ss;

	for(uint i =0; i < lcores_.size(); ++i)
	{
		if(i)
		{
			ss<<",";
		}
		ss<<i<<"@"<<lcores_[i];
	}

	return ss.str();
}

ePhysPortConfigType sEalParameters::portTypeFromString(const std::string &val)
{
	if(val=="DPDK")
	{
		return ePhysPortConfigType::DPDK;
	}
	if(val=="PROCESS")
	{
		return ePhysPortConfigType::PROCESS;
	}
	if(val=="TASK")
	{
		return ePhysPortConfigType::TASK;
	}
	THROW_ERROR("BAD STR: \""+val+"\"");
	return ePhysPortConfigType::BAD_VAL;
}

void sEalParameters::loadPortsConfig(const std::vector<nlohmann::json> &vls)
{
	TA_ALREADY_INITED(ports_.size());
	TA_LOGIC_ERROR(vls.size());
	for(const nlohmann::json & item:vls)
	{
		sRealPortParam cfg=loadFromJson(item);
		ports_.push_back(cfg);
	}

}
sRealPortParam sEalParameters::loadFromJson(const nlohmann::json& json)
{
	sRealPortParam config;
	config.name_=json.at("name").get<string>();
	string type=json.at("type").get<string>();
	config.type_=portTypeFromString(type);
	switch (config.type_) {
	case ePhysPortConfigType::DPDK:
		config.pci_=json.at("pci").get<string>();


		break;
	default:
		TA_LOGIC_ERROR(0);
		break;
	}
	return config;
}



void sJsonProcess::load(const nlohmann::json &json)
{
	isPrimary_=json.at("isPrimary").get<bool>();
	name_=json.at("name").get<string>();
	prefix_=json.at("prefix").get<string>();
}



}
