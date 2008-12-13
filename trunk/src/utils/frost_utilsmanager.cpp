/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utilsmanager.h"

using namespace std;

namespace Frost
{
    const s_str UtilsManager::CLASS_NAME = "UtilsManager";

    UtilsManager::UtilsManager()
    {
        pLogFunction_ = NULL;
    }

    UtilsManager::~UtilsManager()
    {

    }

    void UtilsManager::SetLogFunction( LogFunction pFunc )
    {
        pLogFunction_ = pFunc;
    }

    void UtilsManager::Log( const s_str& sMessage, const s_bool& bTimeStamps )
    {
        if (pLogFunction_)
            pLogFunction_(sMessage, bTimeStamps);
    }
}



