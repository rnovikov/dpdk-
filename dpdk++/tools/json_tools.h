#ifndef JSON_TOOLS_H
#define JSON_TOOLS_H
#include <nlohmann/json.hpp>

namespace dc_tools
{
nlohmann::json readFromFile( const std::string& filePath, const std::string& module = "" );

}

#endif // JSON_TOOLS_H
