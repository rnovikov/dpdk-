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

template <uint32_t sizeOfStruct, uint32_t minPowValue, uint16_t valRange> class cLoopStatCounter
{
public:
    ALWAYS_INLINE cLoopStatCounter();
    ALWAYS_INLINE void operator+=( const cLoopStatCounter<sizeOfStruct, minPowValue, valRange>& l );
    ALWAYS_INLINE void operator-=( const cLoopStatCounter<sizeOfStruct, minPowValue, valRange>& l );
    ALWAYS_INLINE void nextSnap( uint32_t sz, uint64_t time );
    ALWAYS_INLINE void singleSnap( uint64_t time );

    ALWAYS_INLINE bool loopsEmpty() const;
    ALWAYS_INLINE void maxTimeOnPacket( uint64_t& maxOnP );
    ALWAYS_INLINE std::string detailedStr() const;
    ALWAYS_INLINE std::string statistics() const;
    ALWAYS_INLINE std::string toStr( const std::string& name ) const;

private:
    static_assert( valRange < sizeof( uint64_t ) * 8, " " );
    static_assert( sizeOfStruct, "zero size" );
    uint64_t maxLoopTime_[sizeOfStruct];
    uint64_t loopTimes_[valRange + 1];
};

template <uint32_t sizeOfStruct, uint32_t minPowValue, uint16_t valRange>
ALWAYS_INLINE cLoopStatCounter<sizeOfStruct, minPowValue, valRange>::cLoopStatCounter()
{
    memset( this, 0, sizeof( cLoopStatCounter<sizeOfStruct, minPowValue, valRange> ) );
}

template <uint32_t sizeOfStruct, uint32_t minPowValue, uint16_t valRange>
ALWAYS_INLINE void cLoopStatCounter<sizeOfStruct, minPowValue, valRange>::operator+=(
    const cLoopStatCounter<sizeOfStruct, minPowValue, valRange>& l )
{
    for( uint i = 0; i < sizeOfStruct; ++i )
    {
        maxLoopTime_[i] += l.maxLoopTime_[i];
    }
    for( uint i = 0; i <= valRange; ++i )
    {
        loopTimes_[i] += l.loopTimes_[i];
    }
}

template <uint32_t sizeOfStruct, uint32_t minPowValue, uint16_t valRange>
ALWAYS_INLINE void cLoopStatCounter<sizeOfStruct, minPowValue, valRange>::operator-=(
    const cLoopStatCounter<sizeOfStruct, minPowValue, valRange>& l )
{
    for( uint i = 0; i < sizeOfStruct; ++i )
    {
        maxLoopTime_[i] -= l.maxLoopTime_[i];
    }
    for( uint i = 0; i <= valRange; ++i )
    {
        loopTimes_[i] -= l.loopTimes_[i];
    }
}
template <uint32_t sizeOfStruct, uint32_t minPowValue, uint16_t valRange>
ALWAYS_INLINE void cLoopStatCounter<sizeOfStruct, minPowValue, valRange>::nextSnap( uint32_t sz, uint64_t time )
{
    THROW_ASSERT(
        sz < sizeOfStruct, "size too much sz:" + std::to_string( sz ) + " def:" + std::to_string( sizeOfStruct ) );
    if( unlikely( maxLoopTime_[sz] < time ) )
    {
        maxLoopTime_[sz] = time;
    }
    int index = 64 - minPowValue - __builtin_clzll( time );
    if( unlikely( index > valRange ) )
    {
        loopTimes_[valRange]++;
    }
    else if( unlikely( index <= 0 ) )
    {
        loopTimes_[0]++;
    }
    else
    {
        loopTimes_[index]++;
    }
}

template <uint32_t sizeOfStruct, uint32_t minPowValue, uint16_t valRange>
ALWAYS_INLINE void cLoopStatCounter<sizeOfStruct, minPowValue, valRange>::singleSnap( uint64_t time )
{
    TA_LOGIC_ERROR( sizeOfStruct == 1 );
    nextSnap( 0, time );
}

template <uint32_t sizeOfStruct, uint32_t minPowValue, uint16_t valRange>
ALWAYS_INLINE bool cLoopStatCounter<sizeOfStruct, minPowValue, valRange>::loopsEmpty() const
{
    for( uint i = 0; i < sizeOfStruct; ++i )
    {
        if( maxLoopTime_[i] )
        {
            return false;
        }
    }
    for( int i = 0; i <= valRange; ++i )
    {
        if( loopTimes_[i] )
        {
            return false;
        }
    }
    return true;
}

template <uint32_t sizeOfStruct, uint32_t minPowValue, uint16_t valRange>
ALWAYS_INLINE void cLoopStatCounter<sizeOfStruct, minPowValue, valRange>::maxTimeOnPacket( uint64_t& maxOnP )
{
    for( uint32_t i = 1; i < sizeOfStruct; ++i )
    {
        uint64_t v = maxLoopTime_[i] / i;
        if( maxOnP < v )
        {
            maxOnP = v;
        }
    }
}

template <uint32_t sizeOfStruct, uint32_t minPowValue, uint16_t valRange>
ALWAYS_INLINE std::string cLoopStatCounter<sizeOfStruct, minPowValue, valRange>::detailedStr() const
{
    if( loopsEmpty() )
    {
        return std::string();
    }
    std::stringstream ss;
    uint64_t maxTime = 0;
    for( uint32_t i = 0; i < sizeOfStruct; ++i )
    {
        if( maxLoopTime_[i] )
        {
            if( maxLoopTime_[i] > maxTime )
            {
                maxTime = maxLoopTime_[i];
            }
            ss << "pkts:" << std::setw( 5 ) << i << " maxTime:" << std::setw( 10 ) << maxLoopTime_[i] << std::endl;
        }
    }
    ss << "totalMax:" << maxTime << std::endl;
    ss << statistics();
    return ss.str();
}

template <uint32_t sizeOfStruct, uint32_t minPowValue, uint16_t valRange>
ALWAYS_INLINE std::string cLoopStatCounter<sizeOfStruct, minPowValue, valRange>::statistics() const

{
    std::stringstream ss;
    uint64_t cnt = 1;
    cnt <<= minPowValue;
    uint64_t currentPow = valRange;
    while( loopTimes_[currentPow] == 0 )
    {
        if( !currentPow )
        {
            break;
        }
        currentPow--;
    }
    ss << "val from " << std::setw( 10 ) << 1 << " to " << std::setw( 10 ) << cnt << " : " << loopTimes_[0]
       << std::endl;
    for( uint i = 1; i <= currentPow; ++i )
    {
        cnt <<= 1;
        if( loopTimes_[i] )
        {
            ss << "val from " << std::setw( 10 ) << ( ( cnt >> 1 ) + 1 ) << " to " << std::setw( 10 ) << cnt << " : "
               << loopTimes_[i] << std::endl;
        }
    }
    return ss.str();
}

template <uint32_t sizeOfStruct, uint32_t minPowValue, uint16_t valRange>
ALWAYS_INLINE std::string cLoopStatCounter<sizeOfStruct, minPowValue, valRange>::toStr( const std::string& name ) const
{
    if( loopsEmpty() )
    {
        return std::string();
    }
    std::stringstream ss;
    uint64_t maxTime = 0;
    uint32_t onPkts = 0;
    for( uint32_t i = 0; i < sizeOfStruct; ++i )
    {
        if( maxLoopTime_[i] > maxTime )
        {
            maxTime = maxLoopTime_[i];
            onPkts = i;
        }
    }
    ss << name << " maxTime:" << maxTime << "  (" << onPkts << ")" << std::endl;
    ss << statistics() << std::endl;
    return ss.str();
}

using MemoryPoolCounter = cLoopStatCounter<1, 10, 30>;

}

#endif // PERFORMANCE_COUNTERS_H
