/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Lua source               */
/*                                        */

#include "frost_lua.h"

#include "frost_inputmanager.h"
#include "frost_localemanager.h"
#include "frost_timemanager.h"
#include "frost_eventmanager.h"
#include "frost_event.h"

using namespace std;
using namespace Frost;
using namespace Frost::Lua;

/* [#] The following functions are called "glues". They aren't used in C++ but
*  in the LUA environnement. To get a description of what they do, you can refer
*  to :
*                 http://www.wowwiki.com/World_of_Warcraft_API
*
*  They aren't all described there, but the most complexe ones are. Their use is,
*  in many case, obvious.
*/

/** \cond NOT_REMOVE_FROM_DOC
*/

int Frost::l_SendString( lua_State* pLua )
{
    Engine::sLuaComString += lua_tostring(pLua, 1);

    return 0;
}

int Frost::l_EmptyString( lua_State* pLua )
{
    Engine::sLuaComString = "";

    return 0;
}

int Frost::l_ConcTable( lua_State* pLua )
{
    s_str t = lua_tostring(pLua, 1);
    s_str s = ConcTable(pLua, lua_tostring(pLua, 2));
    s = t+s;
    lua_pushstring(pLua, s.c_str());

    return 1;
}


int Frost::l_ThrowError( lua_State* pLua )
{
    Lua::Function mFunc("l_ThrowError", pLua);
    mFunc.Add(0, "caption", LUA_TSTRING, VALUE_STRING);
    if (mFunc.Check())
    {
        Log("# Error # : LUA : " + mFunc.Get(0)->GetS());

        Event e("LUA_ERROR");
        e.Add(s_var(mFunc.Get(0)->GetS()));

        EventManager::GetSingleton()->FireEvent(e);
    }

    return mFunc.Return();
}

int Frost::l_Log( lua_State* pLua )
{
    Lua::Function mFunc("Log", pLua);
    mFunc.Add(0, "caption", LUA_TSTRING, VALUE_STRING);
    if (mFunc.Check())
    {
        Log("# LUA # : " + mFunc.Get(0)->GetS());
    }

    return mFunc.Return();
}

int Frost::l_RandomInt( lua_State* pLua )
{
    Lua::Function mFunc("RandomInt", pLua, 1);
    mFunc.Add(0, "min", LUA_TNUMBER, VALUE_INT);
    mFunc.Add(1, "max", LUA_TNUMBER, VALUE_INT);
    if (mFunc.Check())
    {
        s_int iMin = mFunc.Get(0)->GetI();
        s_int iMax = mFunc.Get(1)->GetI();

        s_int iRand = s_int::Rand(iMin, iMax);
        mFunc.Push(Lua::ReturnValue(iRand));
    }

    return mFunc.Return();
}

int Frost::l_RandomFloat( lua_State* pLua )
{
    Lua::Function mFunc("RandomFloat", pLua, 1);
    mFunc.Add(0, "min", LUA_TNUMBER, VALUE_FLOAT);
    mFunc.Add(1, "max", LUA_TNUMBER, VALUE_FLOAT);
    if (mFunc.Check())
    {
        s_float fMin = mFunc.Get(0)->GetF();
        s_float fMax = mFunc.Get(1)->GetF();

        s_float fRand = s_float::Rand(fMin, fMax);
        mFunc.Push(Lua::ReturnValue(fRand));
    }

    return mFunc.Return();
}

int Frost::l_StrReplace( lua_State* pLua )
{
    Lua::Function mFunc("StrReplace", pLua, 2);
    mFunc.Add(0, "base string", LUA_TSTRING, VALUE_STRING);
    mFunc.Add(1, "pattern", LUA_TSTRING, VALUE_STRING);
    mFunc.Add(3, "replacement", LUA_TSTRING, VALUE_STRING);
    if (mFunc.Check())
    {
        s_str sBase = mFunc.Get(0)->GetS();
        s_str sPattern = mFunc.Get(1)->GetS();
        s_str sReplacement = mFunc.Get(2)->GetS();
        s_uint uiCount = sBase.Replace(sPattern, sReplacement);
        mFunc.Push(Lua::ReturnValue(sBase));
        mFunc.Push(Lua::ReturnValue(uiCount));
    }

    return mFunc.Return();
}

int Frost::l_StrCapitalStart( lua_State* pLua )
{
    Lua::Function mFunc("StrCapitalStart", pLua, 1);
    mFunc.Add(0, "base string", LUA_TSTRING, VALUE_STRING);
    if (mFunc.Check())
    {
        s_str sBase = mFunc.Get(0)->GetS();
        sBase.CapitalStart(true);
        mFunc.Push(Lua::ReturnValue(sBase));
    }

    return mFunc.Return();
}

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
    mFunc.Add(0, "string id", LUA_TSTRING, VALUE_STRING);
    if (mFunc.Check())
    {
        s_str sID = mFunc.Get(0)->GetS();
        mFunc.Push(Lua::ReturnValue(LocaleManager::GetSingleton()->GetLocalizedString(sID, "UI")));
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

int Frost::l_GetGlobal( lua_State* pLua )
{
    Lua::Function mFunc("GetGlobal", pLua, 1);
    mFunc.Add(0, "variable name", LUA_TSTRING, VALUE_STRING);
    if (mFunc.Check())
    {
        s_str sName = mFunc.Get(0)->GetS();
        lua_getglobal(pLua, sName.c_str());
        mFunc.Push(Lua::ReturnValue(RETURN_OBJECT));
    }

    return mFunc.Return();
}

int Frost::l_GetMousePos( lua_State* pLua )
{
    Lua::Function mFunc("GetMousePos", pLua, 2);

    mFunc.Push(Lua::ReturnValue(InputManager::GetSingleton()->GetMPosX()));
    mFunc.Push(Lua::ReturnValue(InputManager::GetSingleton()->GetMPosY()));

    return mFunc.Return();
}

int Frost::l_DoString( lua_State* pLua )
{
    Lua::Function mFunc("RunScript", pLua);
    mFunc.Add(0, "string", LUA_TSTRING, VALUE_STRING);
    if (mFunc.Check())
    {
        s_str sScript = mFunc.Get(0)->GetS();
        int iError = luaL_dostring(pLua, sScript.c_str());
        if (iError) l_ThrowError(pLua);
    }

    return mFunc.Return();
}
/** \endcond
*/
