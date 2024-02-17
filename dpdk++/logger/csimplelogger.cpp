#include "csimplelogger.h"
#include <rte_branch_prediction.h>
#include <rte_byteorder.h>
#include <rte_cycles.h>
#include <rte_lcore.h>
#include <rte_bitops.h>
#include <iostream>
#include <iomanip>
#include <unistd.h>
using namespace std;
namespace dc_logger {


std::mutex cSimpleLogger::logMTX_;
cSimpleLogger::cSimpleLogger( loggerFunction logFunc, const string& file, const std::string& function, uint32_t line )
    : logFunc_( logFunc )
{
    //(*this)<<file<<"|";
    ( *this ) << setw( 20 ) << function << "(" << setw( 4 ) << line << ")";
}

cSimpleLogger::~cSimpleLogger()
{
    logFunc_( str() );
}

void cSimpleLogger::log_it_to_cout( const std::string& msg )
{
    lock_guard<mutex> guard( logMTX_ );
    fflush( stdout );
    int lcore = rte_lcore_id();
    if( lcore > 128 )
    {
        lcore = -1;
    }
    int cpu_id = sched_getcpu();
    int pid = getpid();
    int tid = gettid();

    cout << "LCPT[" << lcore << "," << cpu_id << "," << pid << "," << tid << "]:" << msg << endl;
    fflush( stdout );
}
} // namespace logger
