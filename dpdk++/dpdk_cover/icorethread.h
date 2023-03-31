#ifndef ICORETHREAD_H
#define ICORETHREAD_H
#include <general/base_defines.h>


class iCoreThread
{
public:
	iCoreThread(uint32_t  coreIndex);
	void stop();
	volatile bool isInit()const;
	void run();
	virtual void init()=0;
	virtual uint64_t iteration()=0;
private:
	static int launch_f(void *);
	int loop();
	uint32_t index_=BAD_UINT32_VALUE;
	volatile bool isWork_=true;
	void setInited(){isInited_=true;}
	volatile bool isInited_=false;

};

#endif // ICORETHREAD_H
