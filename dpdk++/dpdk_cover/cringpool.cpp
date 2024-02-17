#include "cringpool.h"


cRingPool::cRingPool(const std::string &n, const std::vector<cFrameBuffer *> &arr):name_(n),startArray_(arr)
{

}

bool cRingPool::initPool(uint64_t elemSize, uint64_t elemCount, uint64_t perCoreCache)
{
    TA_LOGIC_ERROR(startArray_.size());
    NOT_IMPLEMENTED;
    return false;
}

uint32_t cRingPool::payloadSize() const
{

}
