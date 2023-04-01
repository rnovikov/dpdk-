#ifndef EALPARAMETERS_H
#define EALPARAMETERS_H

#include <nlohmann/json.hpp>
#include <general/base_types.h>

namespace config {


//enum class ePhysPortConfigOwner
//{

//};



struct sJsonProcess {
	bool isPrimary_;
	std::string name_;
	std::string prefix_;
	void load(const nlohmann::json& json);
};

struct sEalParameters {
public:
	// Constructor that takes a JSON object
	sEalParameters();
	void load(const std::string & data);

	// Getter functions for all parameters

	sJsonProcess getProcess()const;
	std::vector<int> getLcores() const;
	std::vector<int> getMemory() const;
	std::vector<sRealPortParam> getPorts() const;
	int getChannels() const;
	int getRank() const;

	std::vector<std::string> args()const;

	std::string getLcoresMap()const;

private:
	ePhysPortConfigType portTypeFromString(const std::string & val);
	sRealPortParam loadFromJson(const nlohmann::json& json);
	// Struct representing the "process" field
	void loadPortsConfig(const std::vector<nlohmann::json> & vls );
	sJsonProcess process_;

	std::vector<int> lcores_;
	std::vector<int> memory_;
	std::vector<sRealPortParam> ports_;
	int channels_;
	int rank_;
};

}

#endif // EALPARAMETERS_H
