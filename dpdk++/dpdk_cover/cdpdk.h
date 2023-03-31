#ifndef CDPDK_H
#define CDPDK_H
#include <string>
#include <config/ealparameters.h>
#include <general/forward_declaration.h>
#include <general/base_types.h>

#include <real_ports/irealport.h>


struct sDpdkConfig
{
	std::string zeroArg_;
	std::string jsonConfigData_;

};

class cDpdk
{
public:
	static void init(const std::string & data, const std::string &zeroArg);
	static cDpdk * getInstance();
	std::string toStr()const;
	std::vector < iRealPort *> getRealPorts();

	void initPorts(f_port_create_fabric f_fabric=iRealPort::port_fabric_default);
	void startAllPorts();

private:
    static cDpdk * instance_;
	cDpdk();
	void internalInit(const std::string &data,const std::string & zeroArg);


	std::vector <iRealPort *> portsArray_;
	std::string zeroArg_;

	sEalParameters params_;
};

#endif // CDPDK_H
