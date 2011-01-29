/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utilsmanager.h"
#include "frost_utils_timemanager.h"

using namespace std;

#ifndef USING_OGRE
#ifdef WIN32
    #include <sys/timeb.h>
#else
    #include <sys/time.h>
#endif
#endif

namespace Frost
{
    const s_str UtilsManager::CLASS_NAME = "UtilsManager";

    UtilsManager::UtilsManager()
    {
        pLogFunction_ = nullptr;
    }

    UtilsManager::~UtilsManager()
    {
    }

    void UtilsManager::Initialize()
    {
        // Calling srand(time(nullptr)) can result in similar results
        // because time()'s resolution is too low. Here we use a
        // high precision timer.
        srand(TimeManager::GetSingleton()->GetMicroseconds().Get());

        // In GCC, the first rand() is round(3.25*seed) + 38
        // To get rid of this effect, we just call rand() several times.
        rand(); rand(); rand(); rand();
    }

    void UtilsManager::SetLogFunction( LogFunction pFunc )
    {
        pLogFunction_ = pFunc;
    }

    void UtilsManager::Log( const s_str& sMessage, const s_bool& bTimeStamps, const s_uint& uiOffset )
    {
        if (pLogFunction_)
            pLogFunction_(sMessage, bTimeStamps, uiOffset);
    }

    void UtilsManager::SetLogLevel( const s_uint& uiLevel )
    {
        uiLogLevel_ = uiLevel;
    }

    const s_uint& UtilsManager::GetLogLevel() const
    {
        return uiLogLevel_;
    }
}



