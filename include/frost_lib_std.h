/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              STD headers               */
/*                                        */
/*                                        */

/* Warning : This is a precompiled header.
*            Modifying it will trigger a
*            full rebuild !
*/

#ifndef FROST_STD_H
#define FROST_STD_H

#include <math.h>
#include <string>
#include <map>
#if FROST_LINUX
    #include <tr1/unordered_map>
#else
    #include <ext/hash_map>
#endif
#include <vector>
#include <deque>
#include <list>
#include <sstream>
#include <fstream>
#include <ios>

#endif
