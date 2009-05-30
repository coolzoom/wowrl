/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Lua source               */
/*                                        */

#include "lua/frost_lua.h"

#include "frost_inputmanager.h"
#include "frost_localemanager.h"

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

int Frost::l_GetDelta( lua_State* pLua )
{
    Lua::Function mFunc("GetDelta", pLua, 1);

    mFunc.Push(Lua::ReturnValue(TimeManager::GetSingleton()->GetDelta()));

    return mFunc.Return();
}

int Frost::l_GetLocale( lua_State* pLua )
{
    Lua::Function mFunc("GetLocale", pLua, 1);

    mFunc.Push(Lua::ReturnValue(LocaleManager::GetSingleton()->GetLocale()));

    return mFunc.Return();
}

int Frost::l_GetLocalizedString( lua_State* pLua )
{
    Lua::Function mFunc("GetLocalizedString", pLua, 1);
    mFunc.Add(0, "string id", Lua::TYPE_STRING, VALUE_STRING);
    if (mFunc.Check())
    {
        s_str sID = mFunc.Get(0)->GetS();
        mFunc.Push(Lua::ReturnValue(LocaleManager::GetSingleton()->GetLocalizedString(sID)));
    }

    return mFunc.Return();
}

int Frost::l_GetTime( lua_State* pLua )
{
    Lua::Function mFunc("GetTime", pLua, 1);

    mFunc.Push(Lua::ReturnValue(TimeManager::GetSingleton()->GetTime()));

    return mFunc.Return();
}

int Frost::l_GetTimeOfTheDay( lua_State* pLua )
{
    Lua::Function mFunc("GetTimeOfTheDay", pLua, 3);

    mFunc.Push(Lua::ReturnValue(TimeManager::GetSingleton()->GetHour()));
    mFunc.Push(Lua::ReturnValue(TimeManager::GetSingleton()->GetMinutes()));
    mFunc.Push(Lua::ReturnValue(TimeManager::GetSingleton()->GetSeconds()));

    return mFunc.Return();
}

int Frost::l_GetMousePos( lua_State* pLua )
{
    Lua::Function mFunc("GetMousePos", pLua, 2);

    mFunc.Push(Lua::ReturnValue(InputManager::GetSingleton()->GetMPosX()));
    mFunc.Push(Lua::ReturnValue(InputManager::GetSingleton()->GetMPosY()));

    return mFunc.Return();
}

/** \endcond
*/
