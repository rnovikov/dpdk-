#include "string_tools.h"
#include <packet/network_tools.h>
using namespace std;

namespace dc_tools
{

string vecToString( VecStr s, const std::string& delemiter )
{
    stringstream ss;
    uint32_t size = s.size();
    for( uint i = 0; i < size; ++i )
    {
        if( i )
        {
            ss << delemiter;
        }
        ss << s[i];
    }
    return ss.str();
}

VecStr splitToVector(std::string str, const std::string &delimiter)
{
    size_t pos = str.find( delimiter );
    size_t initialPos = 0;
    VecStr answer;

    // Decompose statement
    while( pos != std::string::npos )
    {
        answer.push_back( str.substr( initialPos, pos - initialPos ) );
        initialPos = pos + delimiter.size();

        pos = str.find( delimiter, initialPos );
    }

    // Add the last one
    answer.push_back( str.substr( initialPos, std::min( pos, str.size() ) - initialPos + delimiter.size() ) );
    return answer;

}















}
