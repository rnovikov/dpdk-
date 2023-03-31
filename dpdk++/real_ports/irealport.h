#ifndef IREALPORT_H
#define IREALPORT_H
#include <string>
#include <port_types.h>




class iRealPort
{
public:

	virtual void init(uint16_t rxQueues,uint16_t txQueues)=0;
	virtual bool setLinkState( bool isOn ) = 0;
	virtual bool setPromiscMode( bool isOn ) = 0;

	virtual bool startDevice() = 0;
	virtual bool stopDevice() = 0;


	virtual sFastPathPortQueueApi getRxQueue(tQueueId qId)=0;
	virtual sFastPathPortQueueApi getTxQueue(tQueueId qId)=0;

	uint16_t getRxQueuesCount() const;
	uint16_t getTxQueuesCount() const;

	bool setRxMirrorPoint( iMirrorPoint* point, uint32_t queueId );
	bool setTxMirrorPoint( iMirrorPoint* point, uint32_t queueId );

	virtual uint32_t portId()const =0;
	virtual uint32_t dpdkPortId()const =0;



	static iRealPort * port_fabric_default(const sInitRealPortParam & port);

};

#endif // IREALPORT_H
