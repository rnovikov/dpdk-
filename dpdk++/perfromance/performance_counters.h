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

}

#endif // PERFORMANCE_COUNTERS_H
