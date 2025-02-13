#ifndef PERFORMANCE_COUNTERS_H
#define PERFORMANCE_COUNTERS_H
#include <base/debug_tools.h>

namespace dc_perf {

#include <iomanip>
#include <nlohmann/json.hpp>
struct sPerfCounterItem
{
    sPerfCounterItem()
    {
        clear();
    }

    bool isEmpty() const;

    uint64_t cycles_ = 0;
    uint64_t ops_ = 0;

    DC_ALWAYS_INLINE void addSingle( uint64_t cyc );
    DC_ALWAYS_INLINE void addMultiple( uint64_t packets, uint64_t cyc );

    void operator+=( const sPerfCounterItem& item );
    void operator-=( const sPerfCounterItem& item );
    void clear();
    std::string detailedStr( uint32_t index = 1, uint64_t totalCycles = 0, uint64_t totalPackets = 0 ) const;

    nlohmann::json toJson( ) const;


    nlohmann::json toNlJson()const
    {
         nlohmann::json answer;
         answer["ops"]=ops_;
         answer["cycles"]=cycles_;
         return answer;
    }
};

inline bool sPerfCounterItem::isEmpty() const
{
    return ops_ == 0;
}

DC_ALWAYS_INLINE void sPerfCounterItem::addSingle( uint64_t cyc )
{
    cycles_ += cyc;
    ops_++;
}

DC_ALWAYS_INLINE void sPerfCounterItem::addMultiple( uint64_t packets, uint64_t cyc )
{
    THROW_ASSERT( packets, "zero packets " );
    THROW_ASSERT( cyc, "zero cycles " );
    //    THROW_ASSERT(packets < cyc,"Cycles less than packets");
    cycles_ += cyc;
    ops_ += packets;
}

template <uint32_t sizeOfStruct> struct sBurstCounter
{
    sBurstCounter()
    {
        clear();
    }

    void clear();
    bool isEmpty() const;
    sPerfCounterItem simplify() const;
    void ALWAYS_INLINE add( uint64_t packets, uint64_t cycles );
    constexpr uint32_t size() const
    {
        return array_.size();
    }
    std::string toStr( const std::string& name ) const;

    void operator+=( const sBurstCounter<sizeOfStruct>& item );
    void operator-=( const sBurstCounter<sizeOfStruct>& item );

private:
    std::array<sPerfCounterItem, sizeOfStruct + 1> array_;
};

template <uint32_t sizeOfStruct> void sBurstCounter<sizeOfStruct>::add( uint64_t packets, uint64_t cycles )
{
    THROW_ASSERT( packets <= sizeOfStruct, "BAD USAGE" );
    array_[packets].addSingle( cycles );
}

template <uint32_t sizeOfStruct>
ALWAYS_INLINE void sBurstCounter<sizeOfStruct>::operator+=( const sBurstCounter<sizeOfStruct>& item )
{
    for( uint i = 0; i < array_.size(); ++i )
    {
        array_[i] += item.array_[i];
    }
}

template <uint32_t sizeOfStruct>
ALWAYS_INLINE void sBurstCounter<sizeOfStruct>::operator-=( const sBurstCounter<sizeOfStruct>& item )
{
    for( uint i = 0; i < array_.size(); ++i )
    {
        array_[i] -= item.array_[i];
    }
}

template <uint32_t sizeOfStruct> inline sPerfCounterItem sBurstCounter<sizeOfStruct>::simplify() const
{
    sPerfCounterItem answer;

    for( uint i = 1; i < size(); ++i )
    {
        if( array_[i].ops_ )
        {
            answer.addMultiple( array_[i].ops_ * i, array_[i].cycles_ );
        }
    }

    return answer;
}

template <uint32_t sizeOfStruct> inline std::string sBurstCounter<sizeOfStruct>::toStr( const std::string& name ) const
{
    std::stringstream ss;
    NOT_IMPLEMENTED;
    return ss.str();
}


}

#endif // PERFORMANCE_COUNTERS_H
