/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */


#ifndef FROST_UTILS_LOG_H
#define FROST_UTILS_LOG_H

#include "frost_utils_types.h"

namespace Frost
{
    void          Log(const s_str& sMessage, const s_bool& bTimeStamps, const s_uint& uiOffset);
    const s_uint& GetLogLevel();

    template<class T> void Log(const T& mObject, const s_bool& bTimeStamps = true, const s_uint& uiOffset = 0)
    {
        Log(s_str::Convert(mObject), bTimeStamps, uiOffset);
    }

    template<uint N, class T> void Log(const T& mObject, const s_bool& bTimeStamps = true, const s_uint& uiOffset = 0)
    {
        if (GetLogLevel() >= N)
            Log(s_str::Convert(mObject), bTimeStamps, uiOffset);
    }

    void Error(const s_str& sClass, const s_str& sMessage);
    void Warning(const s_str& sClass, const s_str& sMessage);
}

#endif
