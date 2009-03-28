/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Lua glues header            */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_LUA_GLUES_H
#define FROST_UTILS_LUA_GLUES_H

#include "frost_utils_lua_state.h"

namespace Frost
{
    namespace Lua
    {
        int l_RandomInt(lua_State* pLua);
        int l_RandomFloat(lua_State* pLua);
        int l_StrReplace(lua_State* pLua);
        int l_StrCapitalStart(lua_State* pLua);
        int l_GetGlobal(lua_State* pLua);
        int l_DoString(lua_State* pLua);

        int l_ThrowError(lua_State* pLua);
        int l_ThrowInternalError(lua_State* pLua);
        int l_Log(lua_State* pLua);
        int l_SendString(lua_State* pLua);
        int l_EmptyString(lua_State* pLua);
        int l_ConcTable(lua_State* pLua);
    }
}

#endif
