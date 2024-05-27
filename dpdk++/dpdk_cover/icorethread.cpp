#include "icorethread.h"
#include <general/error_handler.h>
#include <rte_lcore.h>



iCoreThread::iCoreThread(uint32_t coreIndex)
{

}

volatile bool iCoreThread::isInit() const
{
	return isInited_;
}

void iCoreThread::run()
{
	rte_eal_remote_launch(launch_f, this, index_);
}

int iCoreThread::launch_f(void * arg)
{
	TA_LOGIC_ERROR(rte_lcore_id()==index_);
	return ((iCoreThread *)arg)->loop();

}

int iCoreThread::loop()
{
	init();
	setInited();

	return 0;
}
