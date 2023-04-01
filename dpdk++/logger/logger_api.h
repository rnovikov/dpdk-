#ifndef LOGGER_API_H
#define LOGGER_API_H

#include <sstream>
#include <mutex>
#define F_LOG_FLAG_PRINT_FUNCTION 0x1
#define F_LOG_FLAG_PRINT_LINE 0x2
#define F_LOG_FLAG_PRINT_FILE 0x4
#define F_LOG_FLAG_PRINT_ALL ( F_LOG_FLAG_PRINT_FUNCTION | F_LOG_FLAG_PRINT_FILE | F_LOG_FLAG_PRINT_LINE )

namespace logger
{
///
/// \brief The eLogLvl enum
/// For KERNEL LOG SYSTEM CAPABILITY except 2 cases
/// NO_LOG - total remove log
/// TRACE - Additional DEBUG
///
enum class eLogLvl : uint32_t
{
	NO_LOG = 0,
	EMERG = 1,
	ALERT = 2,
	CRIT = 3,
	ERR = 4,
	WARN = 5,
	NOTICE = 6,
	INFO = 7,
	DEBUG = 8,
	TRACE = 9,
	BAD_VALUE
};

static std::string lvlToStr( eLogLvl level );

typedef void ( *loggerFunction )( eLogLvl logLvl, const std::string& file, const std::string& function, uint32_t line,
	const std::string& msg, uint32_t flags );

class cSimpleLoggerItem : public std::stringstream
{
public:
	explicit cSimpleLoggerItem( loggerFunction loggerFunc, eLogLvl logLvl, const std::string& file,
		const std::string& function, uint32_t line, uint32_t flags );

	~cSimpleLoggerItem();

	static void log_it( eLogLvl logLvl, const std::string& file, const std::string& function, uint32_t line,
		const std::string& msg, uint32_t flags );

private:
	eLogLvl logLvl_ = eLogLvl::BAD_VALUE;
	std::string file_;
	std::string function_;
	uint32_t line_;
	loggerFunction loggerFunc_;
	uint32_t flags_;
	//    std::stringstream ss_;
	static std::mutex logMTX_;
};

} // namespace logger



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

#define L_COUT LOG_IT(logger::eLogLvl::EMERG,F_LOG_FLAG_PRINT_ALL)
#define L_DEV LOG_IT(logger::eLogLvl::EMERG,F_LOG_FLAG_PRINT_ALL)


#endif // LOGGER_API_H
