#include "crealport.h"

cRealPort::cRealPort(uint32_t portId, const std::string &name)
	:iRealPort(portId,name)
{

}

bool cRealPort::setRxMirrorPoint(iMirrorPoint *point, uint32_t queueId)
{
	TA_NOT_IMPLEMENTED;
	return true;
}

bool cRealPort::setTxMirrorPoint(iMirrorPoint *point, uint32_t queueId)
{
	TA_NOT_IMPLEMENTED;
	return true;
}
