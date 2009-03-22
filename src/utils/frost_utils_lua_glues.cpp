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

using namespace std;
using namespace Frost;

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
    lua_pushstring(pLua, s.c_str());

    return 1;
}


int Lua::l_ThrowError( lua_State* pLua )
{
    Lua::Function mFunc("ThrowError", pLua);
    mFunc.Add(0, "caption", Lua::TYPE_STRING, VALUE_STRING);
    if (mFunc.Check())
    {
        Log("# Error # : LUA : " + mFunc.Get(0)->GetS());

        Event e("LUA_ERROR");
        e.Add(s_var(mFunc.Get(0)->GetS()));

        EventManager::GetSingleton()->FireEvent(e);
    }

    return mFunc.Return();
}

int Lua::l_Log( lua_State* pLua )
{
    Lua::Function mFunc("Log", pLua);
    mFunc.Add(0, "caption", Lua::TYPE_STRING, VALUE_STRING);
    if (mFunc.Check())
    {
        Log("# LUA # : " + mFunc.Get(0)->GetS());
    }

    return mFunc.Return();
}

int Lua::l_RandomInt( lua_State* pLua )
{
    Lua::Function mFunc("RandomInt", pLua, 1);
    mFunc.Add(0, "min", Lua::TYPE_NUMBER, VALUE_INT);
    mFunc.Add(1, "max", Lua::TYPE_NUMBER, VALUE_INT);
    if (mFunc.Check())
    {
        s_int iMin = mFunc.Get(0)->GetI();
        s_int iMax = mFunc.Get(1)->GetI();

        s_int iRand = s_int::Random(iMin, iMax);
        mFunc.Push(Lua::ReturnValue(iRand));
    }

    return mFunc.Return();
}

int Lua::l_RandomFloat( lua_State* pLua )
{
    Lua::Function mFunc("RandomFloat", pLua, 1);
    mFunc.Add(0, "min", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(1, "max", Lua::TYPE_NUMBER, VALUE_FLOAT);
    if (mFunc.Check())
    {
        s_float fMin = mFunc.Get(0)->GetF();
        s_float fMax = mFunc.Get(1)->GetF();

        s_float fRand = s_float::Random(fMin, fMax);
        mFunc.Push(Lua::ReturnValue(fRand));
    }

    return mFunc.Return();
}

int Lua::l_StrReplace( lua_State* pLua )
{
    Lua::Function mFunc("StrReplace", pLua, 2);
    mFunc.Add(0, "base string", Lua::TYPE_STRING, VALUE_STRING);
    mFunc.Add(1, "pattern", Lua::TYPE_STRING, VALUE_STRING);
    mFunc.Add(3, "replacement", Lua::TYPE_STRING, VALUE_STRING);
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

int Lua::l_StrCapitalStart( lua_State* pLua )
{
    Lua::Function mFunc("StrCapitalStart", pLua, 1);
    mFunc.Add(0, "base string", Lua::TYPE_STRING, VALUE_STRING);
    if (mFunc.Check())
    {
        s_str sBase = mFunc.Get(0)->GetS();
        sBase.CapitalStart(true);
        mFunc.Push(Lua::ReturnValue(sBase));
    }

    return mFunc.Return();
}

int Lua::l_GetGlobal( lua_State* pLua )
{
    Lua::Function mFunc("GetGlobal", pLua, 1);
    mFunc.Add(0, "variable name", Lua::TYPE_STRING, VALUE_STRING);
    if (mFunc.Check())
    {
        s_str sName = mFunc.Get(0)->GetS();
        lua_getglobal(pLua, sName.c_str());
        mFunc.Push(Lua::ReturnValue(RETURN_OBJECT));
    }

    return mFunc.Return();
}

int Lua::l_DoString( lua_State* pLua )
{
    Lua::Function mFunc("RunScript", pLua);
    mFunc.Add(0, "script", Lua::TYPE_STRING, VALUE_STRING);
    if (mFunc.Check())
    {
        s_str sScript = mFunc.Get(0)->GetS();
        int iError = luaL_dostring(pLua, sScript.c_str());
        if (iError == LUA_ERRRUN)
            l_ThrowError(pLua);
        else if (iError == LUA_ERRMEM)
            Error("Lua", "Memory error.");
        else
            Error("Lua", "Unhandled error.");
    }

    return mFunc.Return();
}
/** \endcond
*/
