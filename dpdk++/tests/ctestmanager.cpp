#include "ctestmanager.h"
#include <threads/cthreadmanager.h>
namespace dc_bench {
volatile bool cTestManager::isWork_ = true;
using namespace std;
cTestManager::cTestManager( const std::string& workDir, cDpdk& dpdk )
{

}

cTestManager::~cTestManager()
{
    for( auto& [key, value] : tests_ )
    {
        L_DEBUG << key << " has value " << value;
        delete value;
    }
}

bool cTestManager::hasTest( const std::string& name )
{
    return getTestByName( name ) != nullptr;
}

bool cTestManager::isDaemon( const std::string& name )
{
    return getTestByName( name )->isDaemonBased();
}

void cTestManager::setParams( const sTestParams& testParams )
{
    TA_LOGIC_ERROR( testParams.isInited_ );
    testParams_ = testParams;
    if( signal( SIGINT, cTestManager::signalHandler ) == SIG_ERR )
    {
        NOT_IMPLEMENTED;
    }
}

void cTestManager::doTest(const std::string& name)
{

    TA_LOGIC_ERROR( testParams_.isInited_ );
    L_TEST_INFO << name;
    iTest* test = getTestByName( name );
    if( test )
    {
        runTest( test );

    }
    else
    {
        L_EMERG << name;
        NOT_IMPLEMENTED;
    }
    L_TEST_INFO << "TEST DONE";
}

std::vector<string> cTestManager::testsList() const
{
    NOT_IMPLEMENTED;
    std::vector<string> answer;
    for( const auto& [key, value] : tests_ )
    {
        L_TEST_INFO << key << " has value " << value;
        answer.push_back( value->name() );
    }
    return answer;
}

void cTestManager::signalHandler( int sigNo )
{
    L_EMERG << "SIGNAL RECEIVED:" << sigNo;
    L_EMERG << "STOPPING TEST  ";
    TA_LOGIC_ERROR( sigNo == SIGINT );
    isWork_ = false;
}

iTest* cTestManager::getTestByName( const string& name )
{
    iTest* answer = nullptr;
    if( tests_.find( name ) != tests_.end() )
    {
        answer = tests_[name];
    }
    return answer;
}

void cTestManager::runTest( tests::iTest* test )
{
    TA_BAD_POINTER( test );
    cThreadManager::getInstance()->setMain( new cTestWorker( test ) );

    L_TEST_INFO << "TEST: " << test->name();
    L_TEST_INFO << test->description();
    test->setParams( &testParams_ );
    //    test->init();
    L_TEST_INFO << "STARTING TEST.... ";
    cCoreManager::getInstance()->main_loop();
    //    test->init();
    //    test->doTest();

    L_TEST_INFO << test->getResultJson();
}

void cTestManager::appendTest( tests::iTest* test )
{
    TA_LOGIC_ERROR( tests_.find( test->name() ) == tests_.end() );
    tests_[test->name()] = test;
}
}
