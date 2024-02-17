#include "json_tools.h"
#include <base/base_types.h>
#include <fstream>
using namespace std;
namespace dc_tools
{

static inline std::string readFileDataWithComment(
    const std::string& path, char line_comment = '#', const std::string& splitLines = "\n" );

nlohmann::json readFromFile( const std::string& filePath, const std::string& module )
{
    TA_LOGIC_ERROR( filesystem::exists( filePath ) );
    std::string data = readFileDataWithComment( filePath );
    L_DEBUG << data;
    nlohmann::json js = nlohmann::json::parse( data );
    if( module.size() )
    {
        return js[module];
    }
    return js;
}

static inline std::string readFileDataWithComment(
    const std::string& filePath, char line_comment, const std::string& splitLines )
{
    TA_LOGIC_ERROR( filesystem::exists( filePath ) );
    ifstream currFile;
    currFile.open( filePath );

    string answer;
    string line;
    while( std::getline( currFile, line ) )
    {
        if( line[0] != line_comment )
        {
            answer += line + splitLines;
        }
    }
    return answer;
}


}
