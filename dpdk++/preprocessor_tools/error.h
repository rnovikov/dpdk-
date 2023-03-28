#ifndef ERRORS_H
#define ERRORS_H
#include <rte_branch_prediction.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdint.h>
#include <unistd.h>
#include <sched.h>
#define DPDKPP_ABORT std::abort()

#define THROW_ASSERT( x, msg ) \
    if( unlikely( bool( x ) == false ) ) \
    { \
        std::stringstream msg_curr; \
        msg_curr << "MSG[CPU:" << sched_getcpu() << "][" << gettid() << "]:" << __FILE__ << ":" << __FUNCTION__ << "(" \
                 << __LINE__ << "):\"" << std::string( msg ) << "\""; \
        std::cerr << msg_curr.str() << std::endl; \
        std::cout << msg_curr.str() << std::endl; \
        fflush( stdout ); \
        DPDKPP_ABORT; \
    };

#define THROW_ERROR(msg) THROW_ASSERT(0,msg)
#define TA_BAD_POINTER(x) THROW_ASSERT(x != nullptr,"BAD POINTER ")
#define TA_ALREADY_INITED(x) THROW_ASSERT( !x,"ALREADY INITED")




#endif // ERRORS_H
