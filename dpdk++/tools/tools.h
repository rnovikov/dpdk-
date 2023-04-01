#ifndef TOOLS_H
#define TOOLS_H
#include <string.h>
namespace tools
{

template <class T>
void zero_struct(T& item)
{
	memset(&item,0,sizeof(item));
}









}



#endif // TOOLS_H
