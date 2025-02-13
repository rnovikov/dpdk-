#include "performance_counters.h"
#include <iomanip>
#include <tools/tools.h>

using namespace std;
using namespace dc_perf;

void sPerfCounterItem::operator+=( const sPerfCounterItem& item )
{
    cycles_ += item.cycles_;
    ops_ += item.ops_;
}

void sPerfCounterItem::operator-=( const sPerfCounterItem& item )
{
    NOT_IMPLEMENTED;
}
void sPerfCounterItem::clear()
{
    cycles_ = 0;
    ops_ = 0;
}

string sPerfCounterItem::detailedStr( uint32_t index, uint64_t totalCycles, uint64_t totalPackets ) const
{
    stringstream ss;
    NOT_IMPLEMENTED;
    return ss.str();
}

std::string sPerfCounterItem::toStr( uint64_t hz ) const
{
    stringstream ss;

    double cyclesOnOperation = cycles_;
    cyclesOnOperation /= ops_;
    ss << "cps:" << cyclesOnOperation << "   " << endl;

    if( hz )
    {
        cyclesOnOperation = hz / cyclesOnOperation;
        ss << " ops in one second: " << (uint64_t)cyclesOnOperation << endl;
    }

    return ss.str();
}

string sPerfCounterItem::toJson( const std::string& name ) const
{
    stringstream ss;
    ss << addQuote( name ) << ":{";
    ss << addQuote( "ops" ) << ":" << to_string( ops_ ) << ",";
    ss << addQuote( "cycles" ) << ":" << to_string( cycles_ );
    ss << "}";
    return ss.str();
}
SimpleTableRow sPerfCounterItem::toRow( const std::string& name ) const
{
    SimpleTableRow answer;

    uint64_t hz = get_hz();
    TA_LOGIC_ERROR( hz );
    answer.push_back( name );
    uint64_t opsInSec = 0;
    uint64_t totalTimeMs = 0;
    uint64_t cycOnOper = 0;

    double cyclesOnOperation = cycles_;
    if( ops_ && cycles_ )
    {
        cyclesOnOperation /= ops_;
        cycOnOper = cyclesOnOperation;

        if( cyclesOnOperation )
        {
            cyclesOnOperation = hz / cyclesOnOperation;
        }
        opsInSec = cyclesOnOperation;
        totalTimeMs = ( cycOnOper * ops_ * 1000 ) / hz;
    }
    else
    {
        if( cycles_ )
        {
            NOT_IMPLEMENTED;
        }
        cycOnOper = 0;
    }
    //    ss<<"cps:"<<cyclesOnOperation<< "   "<<endl;

    answer.push_back( to_string( opsInSec ) );
    answer.push_back( to_string( totalTimeMs ) );
    answer.push_back( to_string( cycOnOper ) );
    answer.push_back( to_string( ops_ ) );
    answer.push_back( to_string( cycles_ ) );

    return answer;
}

SimpleTableRow sPerfCounterItem::headers()
{
    SimpleTableRow answer;
    answer.push_back( "NAME" );
    answer.push_back( "OPS_IN_SEC" );
    answer.push_back( "TOTAL_TIME ms" );

    answer.push_back( "Cycles on OPS" );
    answer.push_back( "TOTAL_OPERATIONS" );
    answer.push_back( "TOTAL_CYCLES" );

    return answer;
}