#ifndef STRING_TOOLS_H
#define STRING_TOOLS_H
#include <string>

namespace tools
{
enum class eOutNumberSystem
{
	BINARY,
	DEC,
	HEX
};







	std::string readWholeFile(const std::string & filePath,char commentSymbol='#');
	std::string strUint(uint64_t val,eOutNumberSystem st=eOutNumberSystem::DEC);
}

#endif // STRING_TOOLS_H
