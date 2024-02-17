#ifndef CSIMPLELOGGER_H
#define CSIMPLELOGGER_H
#include <string>
#include <mutex>
#include <sstream>
namespace dc_logger{
typedef void ( *loggerFunction )( const std::string& msg );

class cSimpleLogger: public std::stringstream
{
public:
    cSimpleLogger();

    explicit cSimpleLogger( loggerFunction logFunc, const std::string& file, const std::string& function, uint32_t line );

    ~cSimpleLogger() override;
    static void log_it_to_cout( const std::string& msg );

private:
    // std out mutex for readability
    static std::mutex logMTX_;
    loggerFunction logFunc_ = nullptr;
};

} // namespace logger

#endif // CSIMPLELOGGER_H
