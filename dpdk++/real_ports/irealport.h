#ifndef IREALPORT_H
#define IREALPORT_H
#include <string>
#include <real_ports/port_types.h>
#include <tools/ctableview.h>




// rte_eth
struct sPortStat
{


	void exportToRow(tools::cTableRow& row);
};
class iMirrorPoint;

class iRealPort
{
public:
	iRealPort(uint32_t portId,const std::string & name);
	virtual ~iRealPort();
	virtual void init()=0;
	virtual void initQueues(uint16_t rxQueues,uint16_t txQueues)=0;
	virtual bool setLinkState( bool isOn ) = 0;
	virtual bool setPromiscMode( bool isOn ) = 0;

	virtual bool startDevice() = 0;
	virtual bool stopDevice() = 0;


	virtual sFastPathPortQueueApi getRxQueue(tQueueId qId)=0;
	virtual sFastPathPortQueueApi getTxQueue(tQueueId qId)=0;

	uint16_t getRxQueuesCount() const;
	uint16_t getTxQueuesCount() const;



	inline uint32_t portId()const {return portId_;}

	virtual bool setRxMirrorPoint( iMirrorPoint* point, uint32_t queueId )=0;
	virtual bool setTxMirrorPoint( iMirrorPoint* point, uint32_t queueId )=0;
	static iRealPort * port_fabric_default(uint32_t id,const sRealPortParam & port);


	void exportToRow(tools::cTableRow & row,const std::vector<std::string> & headers);


	std::string name()const{return name_;}
private:
	uint32_t portId_=BAD_UINT32_VALUE;
	std::string name_;



};

#endif // IREALPORT_H
