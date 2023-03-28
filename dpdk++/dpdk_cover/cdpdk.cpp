#include "cdpdk.h"
#include <iostream>
using namespace std;
cDpdk * cDpdk::instance_=nullptr;

void cDpdk::init(const std::string &config)
{

}

cDpdk::cDpdk(const std::string &config)
{
	cout<<config<<endl;
}
