/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Utils header               */
/*                                        */
/*                                        */

// Base types
typedef unsigned int       uint;
typedef unsigned char      uchar;
typedef unsigned short     ushort;
typedef unsigned long      ulong;
typedef unsigned long long ullong;

typedef char string_element;

typedef int  default_int;
typedef uint default_uint;

#ifdef __GXX_EXPERIMENTAL_CXX0X__
    #define CPP_0X
#endif

#ifdef CPP_0X
    #undef __STRICT_ANSI__
    #include <initializer_list>
#endif

#include <cstddef>

/*#include <string>
#include <vector>
#include <deque>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <typeinfo>*/

