#include "ealparameters.h"
#include <preprocessor_tools/error.h>
#define FAKE_PCI_ADDR "ffff:ff:ff.0"
using namespace std;
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

std::vector<sJsonRealPortConfig> sEalParameters::getPorts() const
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

	for(const sJsonRealPortConfig &prt :ports_)
	{
		if(prt.type_==ePhysPortConfigType::DPDK)
		{
			pciPortsCount++;
			answer.push_back("-a");
			answer.push_back("0000:"+prt.pci_);
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

ePhysPortConfigType sJsonRealPortConfig::portTypeFromString(const std::string &val)
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
		sJsonRealPortConfig cfg;
		cfg.loadFromJson(item);
		ports_.push_back(cfg);
	}

}

void sJsonRealPortConfig::loadFromJson(const nlohmann::json &item)
{
	name_=item.at("name").get<string>();
	string type=item.at("type").get<string>();
	type_=portTypeFromString(type);
	switch (type_) {
	case ePhysPortConfigType::DPDK:
		pci_=item.at("pci").get<string>();
		driver_=item.at("driver").get<string>();

		break;
	default:
		TA_LOGIC_ERROR(0);
		break;
	}


}

void sJsonProcess::load(const nlohmann::json &json)
{
	isPrimary_=json.at("isPrimary").get<bool>();
	name_=json.at("name").get<string>();
	prefix_=json.at("prefix").get<string>();
}
