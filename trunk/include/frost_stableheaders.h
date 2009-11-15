/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Stable headers              */
/*                                        */
/*  ## : All headers here are either      */
/*  unlikely to change or often included  */
/*  and should be pre-compiled.           */
/*                                        */
/*                                        */


#ifndef FROST_STABLEHEADERS_H
#define FROST_STABLEHEADERS_H

// Ogre
#include <OgrePrerequisites.h>

// OIS
#include <OISPrereqs.h>

// S(T)L
#include <cmath>
#include <string>
#include <map>
#ifdef MSVC
    #include <unordered_map>
#else
    #include <tr1/unordered_map>
#endif
#include <vector>
#include <deque>
#include <list>
#include <sstream>
#include <fstream>
#include <ios>

// Utils
#include <frost_utils.h>

// XML
#include <frost_xml.h>

// Frost
#include "frost_macros.h"
#include "frost_engine.h"

#endif
