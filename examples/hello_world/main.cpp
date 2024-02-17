#include <iostream>
#include <dpdk_cover/cdpdk.h>
#include <tools/json_tools.h>
#include <tools/cpcapreader.h>
#include <argparse/argparse.hpp>


using namespace std;
int main(int argc,char ** argv)
{
    std::string zeroArg=argv[0];
    L_INIT<<"ZERO_ARG:"<<zeroArg;
    argparse::ArgumentParser program("dc_hello_world");
    program.add_argument("--dpdk_conf");
    program.add_argument("--pcap_path");
    program.parse_args(argc, argv);
    string confPath=program.get<string>("--dpdk_conf");
    string pcapPath=program.get<string>("--pcap_path");

    nlohmann::json js = dc_tools::readFromFile( confPath );
    uint32_t ret=0;



    cDpdk dpdk(true);
    ret=dpdk.init(js,zeroArg);

    rte_mempool * pool=dpdk.getDefaultPool();
    L_EMERG<<ret;
    L_DEBUG<<pcapPath;
    dc_tools::cPcapReader reader;

    reader.init(pcapPath,pool);






    L_DEBUG<<"HELLO";




    return ret;




}
