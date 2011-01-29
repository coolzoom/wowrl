/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_log.h"
#include "frost_utilsmanager.h"

using namespace std;

namespace Frost
{
    void Log( const s_str& sMessage, const s_bool& bTimeStamps, const s_uint& uiOffset )
    {
        UtilsManager::GetSingleton()->Log(sMessage, bTimeStamps, uiOffset);
    }

    void Error( const s_str& sClass, const s_str& sMessage )
    {
        s_str sHeader = "# Error # : ";
        if (sClass != "")
            sHeader += sClass + " : ";

        Log(sHeader + sMessage, true, sHeader.GetLength());
    }

    void Warning( const s_str& sClass, const s_str& sMessage )
    {
        s_str sHeader = "# Warning # : ";
        if (sClass != "")
            sHeader += sClass + " : ";

        Log(sHeader + sMessage, true, sHeader.GetLength());
    }

    const s_uint& GetLogLevel()
    {
        return UtilsManager::GetSingleton()->GetLogLevel();
    }
}
