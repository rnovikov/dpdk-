#ifndef STRING_TOOLS_H
#define STRING_TOOLS_H
#include <base/base_types.h>



namespace dc_tools
{

uint32_t ipFromStr( const std::string& ip );

VecStr splitToVector( std::string str, const std::string& delimiter );
std::string vecToString( VecStr s, const std::string& delemiter );


}
#endif // STRING_TOOLS_H
