#ifndef ITEST_H
#define ITEST_H
#include <base/base_types.h>
#include <tools/string_tools.h>

namespace dc_bench {

struct sTestParams
{

    std::string additionalParameter_;
    bool isInited_ = false;
};

class iTest
{
public:
    iTest( cDpdk& dp );
    virtual ~iTest()
    {
    }
    void setParams( sTestParams* params )
    {
        TA_LOGIC_ERROR( params->isInited_ );
        params_ = params;
    }
    void doTest();
    virtual bool isUnit() const
    {
        return true;
    }
    virtual bool isDaemonBased() const
    {
        return false;
    }

    uint64_t ops() const
    {
        return ops_;
    }
    uint64_t testTime() const
    {
        return testTime_;
    }

    virtual std::string name() const = 0;
    virtual void init() = 0;
    virtual void setDone() = 0;
    virtual std::string getResultJson() const
    {
        return "";
    }

    virtual std::string description() const
    {
        return "";
    }

protected:
    virtual uint64_t run() = 0;
    cDpdk& dpdk_;
    sTestParams* params_ = nullptr;
    uint32_t testedWrks_ = 1;

    bool isDone_ = false;
    uint64_t testTime_ = 0;
    uint64_t ops_ = 0;
};

class iApp : public iTest
{
public:
    iApp( cDpdk& dp, const std::string& dm ) : iTest( dp ), dm_( dm )
    {
    }
    virtual void init() final override
    {
        TA_LOGIC_ERROR( params_->additionalParameter_.size() );
        VecStr vc = dc_tools::splitToVector( params_->additionalParameter_, dm_ );
        internal_init( vc );
    }

private:
    virtual void internal_init( const VecStr& params ) = 0;

    std::string dm_;
};
}

#endif // ITEST_H
