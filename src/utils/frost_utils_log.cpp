/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_log.h"
#include "frost_utilsmanager.h"

using namespace std;

namespace Frost
{
    void Log( const s_str& sMessage, const s_bool& bTimeStamps )
    {
        UtilsManager::GetSingleton()->Log(sMessage, bTimeStamps);
    }

    void Error( const s_str& sClass, const s_str& sMessage )
    {
        if (sClass != "")
            Log("# Error # : " + sClass + " : " + sMessage);
        else
            Log("# Error # : " + sMessage);
    }

    void Warning( const s_str& sClass, const s_str& sMessage )
    {
        if (sClass != "")
            Log("# Warning # : " + sClass + " : " + sMessage);
        else
            Log("# Warning # : " + sMessage);
    }
}
