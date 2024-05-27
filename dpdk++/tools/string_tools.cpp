#include "string_tools.h"
#include <general/error_handler.h>
#include <fstream>
using namespace std;

namespace tools {

string readWholeFile(const string &filePath,char commentSymbol)
{
	string line;
	string answer;
	ifstream myfile(filePath);
	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
		{
			if(line.size())
			{
				if(line[0]!= commentSymbol)
				{
					answer+=line;
				}
			}
		}
		myfile.close();

	}
	else
	{
		TA_LOGIC_ERROR(0);
	}

	return answer;
}

string strUint(uint64_t val, eOutNumberSystem st)
{
	switch (st) {
	case eOutNumberSystem::DEC:
		return to_string(val);
		break;
	default:
		TA_NOT_IMPLEMENTED;
		break;
	}
	return string("");
}

}
