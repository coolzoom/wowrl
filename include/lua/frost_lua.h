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
        /// Allows a Lua::State to use Frost's global functions
        /** \param pLua The Lua state to use
        */
        void RegisterGlobalFuncs(s_ptr<Lua::State> pLua);

        /// Allows a Lua::State to use Frost's GUI classes
        /** \param pLua The Lua state to use
        */
        void RegisterGUIClasses(s_ptr<Lua::State> pLua);

        /// Allows a Lua::State to use Frost's Unit class
        /** \param pLua The Lua state to use
        */
        void RegisterUnitClass(s_ptr<Lua::State> pLua);

        /// Allows a Lua::State to use Frost's MovableObject class
        /** \param pLua The Lua state to use
        */
        void RegisterMovableObjectClass(s_ptr<Lua::State> pLua);

        /// Allows a Lua::State to use Frost's Gameplay class
        /** \param pLua The Lua state to use
        */
        void RegisterGameplayClass(s_ptr<Lua::State> pLua);

        /// Allows a Lua::State to use the lua Vector class
        /** \param pLua The Lua state to use
        */
        void RegisterVectorClass(s_ptr<Lua::State> pLua);

        /// Allows a Lua::State to use the lua Vector class
        /** \param pLua The Lua state to use
        */
        void RegisterKeyCodes(s_ptr<Lua::State> pLua);
    }

    // Global glues
    int l_GetDelta(lua_State* pLua);
    int l_GetLocale(lua_State* pLua);
    int l_GetLocalizedString(lua_State* pLua);
    int l_GetTime(lua_State* pLua);
    int l_GetTimeOfTheDay(lua_State* pLua);
    int l_GetMousePos(lua_State* pLua);
    int l_Exit(lua_State* pLua);
}

#endif
