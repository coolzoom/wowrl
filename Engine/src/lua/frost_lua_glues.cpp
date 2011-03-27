/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Lua source               */
/*                                        */

#include "lua/frost_lua.h"
#include "frost_engine.h"

#include <frost_inputmanager.h>
#include <frost_localemanager.h>
#include <frost_utils_lua.h>
#include <frost_utils_timemanager.h>

using namespace std;
using namespace Frost;
using namespace Frost::Lua;

/* [#] The following functions are called "glues". They aren't used in C++ but
*  in the Lua environnement. To get a description of what they do, you can refer
*  to :
*                 http://www.wowwiki.com/World_of_Warcraft_API
*
*  They aren't all described there, but the most complexe ones are. Their use is,
*  in many case, obvious.
*/

/** \cond NOT_REMOVE_FROM_DOC
*/

int Frost::l_GetLocale( lua_State* pLua )
{
    Lua::Function mFunc("GetLocale", pLua, 1);

    mFunc.Push(LocaleManager::GetSingleton()->GetLocale());

    return mFunc.Return();
}

int Frost::l_GetLocalizedString( lua_State* pLua )
{
    Lua::Function mFunc("GetLocalizedString", pLua, 1);
    mFunc.Add(0, "string id", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        s_str sID = mFunc.Get(0)->GetString();
        mFunc.Push(LocaleManager::GetSingleton()->GetLocalizedString(sID));
    }

    return mFunc.Return();
}

int Frost::l_GetTime( lua_State* pLua )
{
    Lua::Function mFunc("GetTime", pLua, 1);

    mFunc.Push(TimeManager::GetSingleton()->GetTime());

    return mFunc.Return();
}

int Frost::l_GetTimeOfTheDay( lua_State* pLua )
{
    Lua::Function mFunc("GetTimeOfTheDay", pLua, 4);

    mFunc.Push(TimeManager::GetSingleton()->GetHour());
    mFunc.Push(TimeManager::GetSingleton()->GetMinutes());
    mFunc.Push(TimeManager::GetSingleton()->GetSeconds());
    mFunc.Push(TimeManager::GetSingleton()->GetMicroseconds());

    return mFunc.Return();
}

int Frost::l_Exit( lua_State* pLua )
{
    Lua::Function mFunc("Exit", pLua);

    Engine::GetSingleton()->ShutDown();

    return mFunc.Return();
}

/** \endcond
*/
