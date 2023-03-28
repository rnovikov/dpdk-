#ifndef LOGGER_API_H
#define LOGGER_API_H
#include <logger/logger_handler.h>


#define LOG_IT( lvl, fl ) \
    if( LOG_LEVEL && ( (uint)lvl ) <= LOG_LEVEL ) \
    logger::cSimpleLoggerItem( logger::cSimpleLoggerItem::log_it, lvl, __FILE__, __FUNCTION__, __LINE__, fl )

#define L_DEBUG LOG_IT( logger::eLogLvl::DEBUG, F_LOG_FLAG_PRINT_ALL )
#define L_TRACE LOG_IT( logger::eLogLvl::TRACE, F_LOG_FLAG_PRINT_ALL )
#define L_TEST LOG_IT( logger::eLogLvl::TRACE, 0 )
#define L_LMP_ERR LOG_IT( logger::eLogLvl::TRACE, 0 )
#define L_ERR LOG_IT( logger::eLogLvl::ERR, F_LOG_FLAG_PRINT_ALL )
#define L_EMERG LOG_IT( logger::eLogLvl::EMERG, F_LOG_FLAG_PRINT_ALL )
#define LL_PF LOG_IT( logger::eLogLvl::EMERG, 0 )
#define L_UT_TRACE LOG_IT( logger::eLogLvl::DEBUG, 0 )
#define L_TEST_COUT LOG_IT( logger::eLogLvl::NO_LOG, 0 )


#endif // LOGGER_API_H
