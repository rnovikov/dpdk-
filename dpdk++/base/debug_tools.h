#ifndef DEBUG_TOOLS_H
#define DEBUG_TOOLS_H
#include <sstream>
#include <mutex>
#include <rte_cycles.h>
#include "base_defines.h"
#include "bytes_tools.h"
#include <string.h>
#include <logger/csimplelogger.h>
/// todo hide logger realization


#define LOG_IT_TO_COUT( lvl ) \
    if( ( (uint)lvl ) <= LOG_LEVEL ) \
    dc_logger::cSimpleLogger( dc_logger::cSimpleLogger::log_it_to_cout, __FILE__, __FUNCTION__, __LINE__ )

#define L_INIT LOG_IT_TO_COUT( 0 ) << "INIT:"
#define L_EMERG LOG_IT_TO_COUT( 0 ) << "EMERG:"
#define L_TEST_INFO LOG_IT_TO_COUT( 1 ) << "TEST:"
#define L_ERROR LOG_IT_TO_COUT( 2 ) << "ERROR:"
#define L_WARN LOG_IT_TO_COUT( 3 ) << "WARN:"
#define L_CUDA LOG_IT_TO_COUT( 4 ) << "CUDA:"
#define L_INFO LOG_IT_TO_COUT( 6 ) << "INFO:"
#define L_TRACE LOG_IT_TO_COUT( 7 ) << "TRACE:"
#define L_DEBUG LOG_IT_TO_COUT( 8 ) << "DEBUG:"
#define L_NOTICE LOG_IT_TO_COUT( 9 ) << "NOTICE:"





#define THROW_ASSERT( x, msg ) \
    if( unlikely( !( x ) ) ) \
    { \
        L_EMERG << msg; \
        fflush( stdout ); \
        std::abort(); \
    }
#define THROW_ERROR( msg ) THROW_ASSERT( 0, msg )

#define TA_DPDK_THREAD \
    if( 1 ) \
    { \
        int lcore = rte_lcore_id(); \
        if( lcore < 0 || lcore > 128 ) \
            NOT_IMPLEMENTED; \
    }

#define TA_ALREADY_INITED( x ) THROW_ASSERT( !x, "ALREADY INITED" );
#define TA_BAD_POINTER( x ) THROW_ASSERT( x, "BAD POINTER" );
#define THROW_HERE THROW_ASSERT( 0, "NOT_EXPECTED" )
#define NOT_IMPLEMENTED THROW_ASSERT( 0, "NOT IMPLEMENTED YET" )
#define TA_LOGIC_ERROR( x ) THROW_ASSERT( ( x ), "LOGIC ERROR" )


template <class T> void zero_struct( T& obj )
{
    memset( &obj, 0, sizeof( T ) );
}

static DC_ALWAYS_INLINE uint64_t get_cycles()
{
    return rte_rdtsc() ;
}

static DC_ALWAYS_INLINE uint64_t get_hz()
{
    return rte_get_tsc_hz() ;
}
#endif // DEBUG_TOOLS_H
