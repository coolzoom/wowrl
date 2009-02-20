/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Lua header                */
/*                                        */
/*                                        */


#ifndef FROST_LUA_H
#define FROST_LUA_H

#include "frost.h"

namespace Frost
{
    namespace Lua
    {
        /// Allow a Lua state to use Frost's global functions
        /** \param pLua The Lua state to use
        */
        void    RegisterGlobalFuncs(s_ptr<Lua::State> pLua);

        /// Allow a Lua state to use Frost's GUI classes
        /** \param pLua The Lua state to use
        */
        void    RegisterGUIClasses(s_ptr<Lua::State> pLua);
    }

    // Global glues
    int l_GetDelta(lua_State* pLua);
    int l_GetLocale(lua_State* pLua);
    int l_GetLocalizedString(lua_State* pLua);
    int l_GetTime(lua_State* pLua);
    int l_GetTimeOfTheDay(lua_State* pLua);
    int l_GetMousePos(lua_State* pLua);
}

#endif
