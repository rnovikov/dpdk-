#ifndef CI350PHYSPORT_H
#define CI350PHYSPORT_H
#include <real_ports/cdpdkport.h>

class cI350PhysPort:public cDpdkPort
{
public:
	cI350PhysPort(uint32_t portId,const std::string & name,const std::string & pci);
};

#endif // CI350PHYSPORT_H
