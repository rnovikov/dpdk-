#ifndef ICORETHREAD_H
#define ICORETHREAD_H
#include <base/base_types.h>

class iCoreThread
{
public:
    iCoreThread();

    virtual ~iCoreWorker()
    {
    }
    virtual void initOnCore() = 0;

    DC_ALWAYS_INLINE
    virtual std::string getResultJson() const = 0;
    virtual std::string name() const = 0;

    void loop();
    void stop();
    uint32_t lcoreId() const
    {
        return lcoreId_;
    }

    bool isDone() const;
    bool isInited() const;


protected  :
    virtual void onDone() {  }
    virtual uint64_t doIteration() = 0;


private:
    uint64_t minLoopTime_ = 0;
    volatile bool isWork_ = true;
    uint32_t delay_ = 0;
    bool isInited_ = false;
    bool isDone_ = false;

    uint32_t lcoreId_ = BAD_UINT32_VALUE;
    uint64_t iterationCounter_ = 0;

};

#endif // CCORETHREAD_H
