#ifndef CDPDK_H
#define CDPDK_H
#include <string>

class cDpdk
{
public:
    static void init(const std::string & config);


private:
    static cDpdk * instance_;
    cDpdk(const std::string & config);
};

#endif // CDPDK_H
