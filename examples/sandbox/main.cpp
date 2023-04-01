#include <iostream>
#include <dpdk_cover/cdpdk.h>
#include <argparse/argparse.hpp>
#include <tools/string_tools.h>
#include <fstream>
using namespace std;

int main(int argc,char ** argv)
{
    argparse::ArgumentParser program("sandbox");
    program.add_argument("--config").required();
    program.parse_args(argc, argv);
	string config =tools::readWholeFile(program.get<string>("--config"));

	cout<<config <<endl;
    cout<<"HELLO WORLD"<<endl;
	cDpdk::init(config,argv[0]);

	cDpdk::getInstance()->initPorts();
    cout<<"ZERO ARG:"<<argv[0]<<endl;
	cout<<cDpdk::getInstance()->toStr()<<endl;
    return 0;
}
