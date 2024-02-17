#ifndef ICORETHREAD_H
#define ICORETHREAD_H
#include <base/base_types.h>
class cThreadManager;
class iCoreThread
{
public:
    iCoreThread();

    virtual ~iCoreThread() {   }
    virtual void initOnCore() = 0;
    virtual std::string getResultJson() const = 0;
    virtual std::string name() const = 0;

    DC_ALWAYS_INLINE uint64_t iteration()
    {
        uint64_t answer=doIteration();
        return answer;
    }

    virtual std::string toStr()const;
protected:
    friend class cThreadManager;
    virtual uint64_t doIteration() = 0;

    void setManagerParams(uint32_t lcoreId){ lcoreId_ = lcoreId;}
public:


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
