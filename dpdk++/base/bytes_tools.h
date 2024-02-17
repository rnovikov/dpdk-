#ifndef BYTES_TOOLS_H
#define BYTES_TOOLS_H

#include "base_defines.h"

#define fast_swap_values( x, y ) \
    ( x ) ^= ( y ); \
    ( y ) ^= ( x ); \
    ( x ) ^= ( y )



#endif // BYTES_TOOLS_H
