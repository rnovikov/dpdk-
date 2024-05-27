#ifndef BASE_TYPES_H
#define BASE_TYPES_H
#include <string>
#include <stdint.h>





typedef uint8_t tRealPort;
typedef uint16_t tVirtualPort;
typedef uint16_t tQueueId;
typedef uint16_t tVlanId;
typedef uint8_t tVrfId;

enum class ePhysPortConfigType
{
	BAD_VAL=0,
	DPDK,
	PROCESS,
	TASK
};

struct sRealPortParam
{
	std::string pci_;
	ePhysPortConfigType type_;
//	ePhysPortConfigOwner owner_;
	std::string  driverName_="";
	std::string name_;
};


#endif // BASE_TYPES_H
