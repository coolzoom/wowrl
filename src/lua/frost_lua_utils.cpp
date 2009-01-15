/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Lua source               */
/*                                        */

#include "frost_lua.h"

using namespace std;
using namespace Frost;
using namespace Frost::Lua;

/* [#] The following functions are shortcuts to the LUA C API. They are very
*  usefull when dealing with tables, and allow default value.
*/
s_int Lua::GetGlobalInt( const s_str& sName, const s_bool& bCritical, const s_int& iDefaultValue )
{
    lua_State* pLua = Engine::GetSingleton()->GetLua();
    s_int i;
    lua_getglobal(pLua, sName.c_str());
    if (lua_isnil(pLua, -1))
    {
        lua_pop(pLua, 1);
        if (bCritical)
        {
            PrintError(pLua, "Missing " + sName + " attribute");
            i = iDefaultValue;
        }
        else
            i = iDefaultValue;
    }
    else if (!lua_isnumber(pLua, -1))
    {
        lua_pop(pLua, 1);
        PrintError(pLua, "\"" + sName + "\" is expected to be a number");
        i = iDefaultValue;
    }
    else
    {
        i = (int)lua_tonumber(pLua, -1);
        lua_pop(pLua, 1);
    }
    return i;
}

s_float Lua::GetGlobalFloat( const s_str& sName, const s_bool& bCritical, const s_float& fDefaultValue )
{
    lua_State* pLua = Engine::GetSingleton()->GetLua();
    s_float f;
    lua_getglobal(pLua, sName.c_str());
    if (lua_isnil(pLua, -1))
    {
        lua_pop(pLua, 1);
        if (bCritical)
        {
            PrintError(pLua, "Missing " + sName + " attribute");
            f = fDefaultValue;
        }
        else
            f = fDefaultValue;
    }
    else if (!lua_isnumber(pLua, -1))
    {
        lua_pop(pLua, 1);
        PrintError(pLua, "\"" + sName + "\" is expected to be a number");
        f = fDefaultValue;
    }
    else
    {
        f = lua_tonumber(pLua, -1);
        lua_pop(pLua, 1);
    }
    return f;
}

s_str Lua::GetGlobalString( const s_str& sName, const s_bool& bCritical, const s_str& sDefaultValue )
{
    lua_State* pLua = Engine::GetSingleton()->GetLua();
    s_str s;
    lua_getglobal(pLua, sName.c_str());
    if (lua_isnil(pLua, -1))
    {
        lua_pop(pLua, 1);
        if (bCritical)
        {
            PrintError(pLua, "Missing " + sName + " attribute");
            s = sDefaultValue;
        }
        else
            s = sDefaultValue;
    }
    else if (!lua_isstring(pLua, -1))
    {
        lua_pop(pLua, 1);
        PrintError(pLua, "\"" + sName + "\" is expected to be a string");
        s = sDefaultValue;
    }
    else
    {
        s = lua_tostring(pLua, -1);
        lua_pop(pLua, 1);
    }
    return s;
}

s_bool Lua::GetGlobalBool( const s_str& sName, const s_bool& bCritical, const s_bool& bDefaultValue )
{
    lua_State* pLua = Engine::GetSingleton()->GetLua();
    s_bool b;
    lua_getglobal(pLua, sName.c_str());
    if (lua_isnil(pLua, -1))
    {
        lua_pop(pLua, 1);
        if (bCritical)
        {
            PrintError(pLua, "Missing " + sName + " attribute");
            b = bDefaultValue;
        }
        else
            b = bDefaultValue;
    }
    else if (!lua_isboolean(pLua, -1))
    {
        lua_pop(pLua, 1);
        PrintError(pLua, "\"" + sName + "\" is expected to be a bool");
        b = bDefaultValue;
    }
    else
    {
        b = lua_toboolean(pLua, -1);
        lua_pop(pLua, 1);
    }
    return b;
}

s_int Lua::GetFieldInt( const s_str& sName, const s_bool& bCritical, const s_int& iDefaultValue, const s_bool& bSetValue, lua_State* pLua )
{
    s_int i;
    if (pLua == NULL) pLua = Engine::GetSingleton()->GetLua();
    lua_getfield(pLua, -1, sName.c_str());
    if (lua_isnil(pLua, -1))
    {
        lua_pop(pLua, 1);
        if (bCritical)
            PrintError(pLua, "Missing " + sName + " attribute");
        else if (bSetValue)
        {
            SetFieldInt(sName, iDefaultValue, pLua);
            i = iDefaultValue;
        }
        else
            i = iDefaultValue;
    }
    else if (!lua_isnumber(pLua, -1))
    {
        PrintError(pLua, "Field is expected to be a number");
        lua_pop(pLua, 1);
        i = iDefaultValue;
    }
    else
    {
        i = (int)lua_tonumber(pLua, -1);
        lua_pop(pLua, 1);
    }
    return i;
}

s_float Lua::GetFieldFloat( const s_str& sName, const s_bool& bCritical, const s_float& fDefaultValue, const s_bool& bSetValue, lua_State* pLua )
{
    s_float f;
    if (pLua == NULL) pLua = Engine::GetSingleton()->GetLua();
    lua_getfield(pLua, -1, sName.c_str());
    if (lua_isnil(pLua, -1))
    {
        lua_pop(pLua, 1);
        if (bCritical)
            PrintError(pLua, "Missing " + sName + " attribute");
        else if (bSetValue)
        {
            SetFieldFloat(sName, fDefaultValue, pLua);
            f = fDefaultValue;
        }
        else
            f = fDefaultValue;
    }
    else if (!lua_isnumber(pLua, -1))
    {
        PrintError(pLua, "Field is expected to be a number");
        lua_pop(pLua, 1);
        f = fDefaultValue;
    }
    else
    {
        f = lua_tonumber(pLua, -1);
        lua_pop(pLua, 1);

    }
    return f;
}

s_str Lua::GetFieldString( const s_str& sName, const s_bool& bCritical, const s_str& sDefaultValue, const s_bool& bSetValue, lua_State* pLua )
{
    s_str s;
    if (pLua == NULL) pLua = Engine::GetSingleton()->GetLua();
    lua_getfield(pLua, -1, sName.c_str());
    if (lua_isnil(pLua, -1))
    {
        lua_pop(pLua, 1);
        if (bCritical)
            PrintError(pLua, "Missing " + sName + " attribute");
        else if (bSetValue)
        {
            SetFieldString(sName, sDefaultValue, pLua);
            s = sDefaultValue;
        }
        else
            s = sDefaultValue;
    }
    else if (!lua_isstring(pLua, -1))
    {
        PrintError(pLua, "Field is expected to be a string");
        lua_pop(pLua, 1);
        s = sDefaultValue;
    }
    else
    {
        s = lua_tostring(pLua, -1);
        lua_pop(pLua, 1);
    }
    return s;
}

s_bool Lua::GetFieldBool( const s_str& sName, const s_bool& bCritical, const s_bool& bDefaultValue, const s_bool& bSetValue, lua_State* pLua )
{
    s_bool b;
    if (pLua == NULL) pLua = Engine::GetSingleton()->GetLua();
    lua_getfield(pLua, -1, sName.c_str());
    if (lua_isnil(pLua, -1))
    {
        lua_pop(pLua, 1);
        if (bCritical)
            PrintError(pLua, "Missing " + sName + " attribute");
        else if (bSetValue)
        {
            SetFieldBool(sName, bDefaultValue, pLua);
            b = bDefaultValue;
        }
        else
            b = bDefaultValue;
    }
    else if (!lua_isboolean(pLua, -1))
    {
        PrintError(pLua, "Field is expected to be a bool");
        lua_pop(pLua, 1);
        b = bDefaultValue;
    }
    else
    {
        b = lua_toboolean(pLua, -1);
        lua_pop(pLua, 1);
    }
    return b;
}

void Lua::SetFieldInt( const s_str& sName, const s_int& iValue, lua_State* pLua )
{
    if (pLua == NULL) pLua = Engine::GetSingleton()->GetLua();
    lua_pushstring(pLua, sName.c_str());
    lua_pushnumber(pLua, iValue);
    lua_settable(pLua, -3);
}

void Lua::SetFieldFloat( const s_str& sName, const s_float& fValue, lua_State* pLua )
{
    if (pLua == NULL) pLua = Engine::GetSingleton()->GetLua();
    lua_pushstring(pLua, sName.c_str());
    lua_pushnumber(pLua, fValue);
    lua_settable(pLua, -3);
}

void Lua::SetFieldString( const s_str& sName, const s_str& sValue, lua_State* pLua )
{
    if (pLua == NULL) pLua = Engine::GetSingleton()->GetLua();
    lua_pushstring(pLua, sName.c_str());
    lua_pushstring(pLua, sValue.c_str());
    lua_settable(pLua, -3);
}

void Lua::SetFieldBool( const s_str& sName, const s_bool& bValue, lua_State* pLua )
{
    if (pLua == NULL) pLua = Engine::GetSingleton()->GetLua();
    lua_pushstring(pLua, sName.c_str());
    lua_pushboolean(pLua, bValue);
    lua_settable(pLua, -3);
}

void Lua::SetIFieldInt( const s_int& iID, const s_int& iValue, lua_State* pLua )
{
    if (pLua == NULL) pLua = Engine::GetSingleton()->GetLua();
    lua_pushnumber(pLua, iID);
    lua_pushnumber(pLua, iValue);
    lua_settable(pLua, -3);
}

void Lua::SetIFieldFloat( const s_int& iID, const s_float& fValue, lua_State* pLua )
{
    if (pLua == NULL) pLua = Engine::GetSingleton()->GetLua();
    lua_pushnumber(pLua, iID);
    lua_pushnumber(pLua, fValue);
    lua_settable(pLua, -3);
}

void Lua::SetIFieldString( const s_int& iID, const s_str& sValue, lua_State* pLua )
{
    if (pLua == NULL) pLua = Engine::GetSingleton()->GetLua();
    lua_pushnumber(pLua, iID);
    lua_pushstring(pLua, sValue.c_str());
    lua_settable(pLua, -3);
}

void Lua::SetIFieldBool( const s_int& iID, const s_bool& bValue, lua_State* pLua )
{
    if (pLua == NULL) pLua = Engine::GetSingleton()->GetLua();
    lua_pushnumber(pLua, iID);
    lua_pushboolean(pLua, bValue);
    lua_settable(pLua, -3);
}
