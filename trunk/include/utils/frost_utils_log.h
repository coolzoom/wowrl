/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */


#ifndef FROST_UTILS_LOG_H
#define FROST_UTILS_LOG_H

#include "frost_utils.h"
#include "frost_utils_types.h"

namespace Frost
{
    void Log(const s_str& sMessage, const s_bool& bTimeStamps = true);
    void Error(const s_str& sClass, const s_str& sMessage);
    void Warning(const s_str& sClass, const s_str& sMessage);
}

#endif
