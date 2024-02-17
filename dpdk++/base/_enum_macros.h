#ifndef _ENUM_MACROS_H
#define _ENUM_MACROS_H



#define CHECK_ENUM( e, x ) \
    if( val == e::x ) \
    { \
        return #x; \
    }

#define ENUM_TO_STR( f, e ) \
    for( int i = 1; i < (int)e::MAX_VALUE; ++i ) \
    { \
        if( f( (e)i ) == val ) \
        { \
            return (e)i; \
        } \
    }







#endif // _ENUM_MACROS_H
