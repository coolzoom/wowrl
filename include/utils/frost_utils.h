/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Utils header               */
/*                                        */
/*                                        */

// Base types
typedef unsigned int   uint;
typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned long  ulong;

#ifndef FROST_UTILS_H
#define FROST_UTILS_H

#ifdef USE_UNICODE
    #define STRING(X) L##X
#else
    #define STRING(X) X
#endif

#include <string>
#include <vector>
#include <deque>
#include <fstream>
#include <sstream>
#include <map>



#endif
