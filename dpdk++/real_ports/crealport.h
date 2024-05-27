#ifndef CREALPORT_H
#define CREALPORT_H

#include <real_ports/irealport.h>
#include <array>
///
/// \brief The cRealPort class
/// Abstraction layer for mirror function and software RSS features
///
class cRealPort:public iRealPort
{
public:
	cRealPort(uint32_t portId,const std::string &name);


	virtual bool setRxMirrorPoint( iMirrorPoint* point, uint32_t queueId ) final override;
	virtual bool setTxMirrorPoint( iMirrorPoint* point, uint32_t queueId ) final override;

private:
	uint32_t rxQueues_=BAD_UINT32_VALUE;
	uint32_t txQueues_=BAD_UINT32_VALUE;

//	MAX_QUEUES_PER_PORT


};

#endif // CREALPORT_H
