#include "logger_api.h"
#include <iostream>
#include <preprocessor_tools/error.h>
using namespace std;
namespace logger
{
std::mutex cSimpleLoggerItem::logMTX_;

void cSimpleLoggerItem::log_it(
	eLogLvl logLvl, const string& file, const string& function, uint32_t line, const string& msg, uint32_t flags )
{
	stringstream ss;

	if( flags & F_LOG_FLAG_PRINT_FILE )
	{
		ss << file << ":";
	}
	if( flags & F_LOG_FLAG_PRINT_FUNCTION )
	{
		ss << function;
	}
	if( flags & F_LOG_FLAG_PRINT_LINE )
	{
		ss << "(" << line << ")";
	}
	if( flags )
	{
		ss << ":\"";
	}
	ss << std::string( msg );
	if( flags )
	{
		ss << "\"";
	}
	lock_guard< mutex > guard( logMTX_ );

	cout << ss.str() << std::endl;
}

cSimpleLoggerItem::cSimpleLoggerItem( loggerFunction loggerFunc, eLogLvl logLvl, const std::string& file,
	const std::string& function, uint32_t line, uint32_t flags )
	: loggerFunc_( loggerFunc ), logLvl_( logLvl ), file_( file ), function_( function ), line_( line ), flags_( flags )
{
	TA_BAD_POINTER( loggerFunc_ );
}

cSimpleLoggerItem::~cSimpleLoggerItem()
{
	loggerFunc_( logLvl_, file_, function_, line_, str(), flags_ );
}

string lvlToStr( eLogLvl level )
{
	switch( level )
	{
	case eLogLvl::NO_LOG:
		return string( "NO_LOG" );
		break;
	case eLogLvl::EMERG:
		return string( "EMERG" );
		break;
	case eLogLvl::ALERT:
		return string( "ALERT" );
		break;
	case eLogLvl::CRIT:
		return string( "CRIT" );
		break;
	case eLogLvl::ERR:
		return string( "ERR" );
		break;
	case eLogLvl::WARN:
		return string( "WARN" );
		break;
	case eLogLvl::NOTICE:
		return string( "NOTICE" );
		break;
	case eLogLvl::INFO:
		return string( "INFO" );
		break;
	case eLogLvl::DEBUG:
		return string( "DEBUG" );
		break;
	case eLogLvl::TRACE:
		return string( "TRACE" );
		break;
	default:
		throw; // can not use THROW_ASSERT in this cpp.
		break;
	}

	return string( "BAD VALUE" );
}

} // namespace logger
