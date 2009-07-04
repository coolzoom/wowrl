/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Lua source               */
/*                                        */

#include "frost_utils_lua_glues.h"
#include "frost_utils_lua_function.h"
#include "frost_utils_luamanager.h"
#include "frost_utils_eventmanager.h"
#include "frost_utils_event.h"
#include "frost_utils_log.h"

using namespace std;
using namespace Frost;

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

int Lua::l_SendString( lua_State* pLua )
{
    LuaManager::GetSingleton()->GetState(pLua)->sComString += lua_tostring(pLua, 1);

    return 0;
}

int Lua::l_EmptyString( lua_State* pLua )
{
    LuaManager::GetSingleton()->GetState(pLua)->sComString = "";

    return 0;
}

int Lua::l_ConcTable( lua_State* pLua )
{
    s_str t = lua_tostring(pLua, 1);
    s_str s = LuaManager::GetSingleton()->GetState(pLua)->ConcTable(lua_tostring(pLua, 2));
    s = t+s;
    lua_pushstring(pLua, s.GetASCII().c_str());

    return 1;
}


int Lua::l_ThrowError( lua_State* pLua )
{
    Lua::Function mFunc("ThrowError", pLua);
    mFunc.Add(0, "caption", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        Log("# Error # : Lua : " + mFunc.Get(0)->GetString());

        Event e("LUA_ERROR");
        e.Add(s_var(mFunc.Get(0)->GetString()));

        EventManager::GetSingleton()->FireEvent(e);
    }

    return mFunc.Return();
}

int Lua::l_ThrowInternalError( lua_State* pLua )
{
    if (lua_isstring(pLua, -1))
    {
        s_str sError = lua_tostring(pLua, -1);
        Log("# Error # : Lua : " + sError);
        lua_pop(pLua, 1);

        Event e("LUA_ERROR");
        e.Add(sError);

        EventManager::GetSingleton()->FireEvent(e);
    }
    else
        Error("Lua", "Unhandled error.");

    return 0;
}

int Lua::l_Log( lua_State* pLua )
{
    Lua::Function mFunc("Log", pLua);
    mFunc.Add(0, "caption", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        Log("# Lua # : " + mFunc.Get(0)->GetString());
    }

    return mFunc.Return();
}

int Lua::l_RandomInt( lua_State* pLua )
{
    Lua::Function mFunc("RandomInt", pLua, 1);
    mFunc.Add(0, "min", Lua::TYPE_NUMBER);
    mFunc.Add(1, "max", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        s_int iMin = s_int(mFunc.Get(0)->GetNumber());
        s_int iMax = s_int(mFunc.Get(1)->GetNumber());

        s_int iRand = s_int::Random(iMin, iMax);
        mFunc.Push(iRand);
    }

    return mFunc.Return();
}

int Lua::l_RandomFloat( lua_State* pLua )
{
    Lua::Function mFunc("RandomFloat", pLua, 1);
    mFunc.Add(0, "min", Lua::TYPE_NUMBER);
    mFunc.Add(1, "max", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        s_float fMin = mFunc.Get(0)->GetNumber();
        s_float fMax = mFunc.Get(1)->GetNumber();

        s_float fRand = s_float::Random(fMin, fMax);
        mFunc.Push(fRand);
    }

    return mFunc.Return();
}

int Lua::l_StrReplace( lua_State* pLua )
{
    Lua::Function mFunc("StrReplace", pLua, 2);
    mFunc.Add(0, "base string", Lua::TYPE_STRING);
    mFunc.Add(1, "pattern", Lua::TYPE_STRING);
    mFunc.Add(3, "replacement", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        s_str sBase = mFunc.Get(0)->GetString();
        s_str sPattern = mFunc.Get(1)->GetString();
        s_str sReplacement = mFunc.Get(2)->GetString();
        s_uint uiCount = sBase.Replace(sPattern, sReplacement);
        mFunc.Push(sBase);
        mFunc.Push(uiCount);
    }

    return mFunc.Return();
}

int Lua::l_StrCapitalStart( lua_State* pLua )
{
    Lua::Function mFunc("StrCapitalStart", pLua, 1);
    mFunc.Add(0, "base string", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        s_str sBase = mFunc.Get(0)->GetString();
        sBase.CapitalStart(true);
        mFunc.Push(sBase);
    }

    return mFunc.Return();
}

int Lua::l_GetGlobal( lua_State* pLua )
{
    Lua::Function mFunc("GetGlobal", pLua, 1);
    mFunc.Add(0, "variable name", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        s_str sName = mFunc.Get(0)->GetString();
        lua_getglobal(pLua, sName.GetASCII().c_str());
        mFunc.NotifyPushed();
    }

    return mFunc.Return();
}

int Lua::l_DoString( lua_State* pLua )
{
    Lua::Function mFunc("RunScript", pLua);
    mFunc.Add(0, "script", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        s_str sScript = mFunc.Get(0)->GetString();
        int iError = luaL_dostring(pLua, sScript.GetASCII().c_str());
        if (iError == LUA_ERRRUN)
            l_ThrowInternalError(pLua);
        else if (iError == LUA_ERRMEM)
            Error("Lua", "Memory error.");
        else
            Error("Lua", "Unhandled error.");
    }

    return mFunc.Return();
}
/** \endcond
*/
