/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*         Frost interface header         */
/*                                        */
/*                                        */

#ifndef FROST_ENGINE_GLUES_H
#define FROST_ENGINE_GLUES_H

#include "frost.h"

namespace Frost
{
    /** \cond NOT_REMOVE_FROM_DOC
    */

    // Engine Lua glue
    class LuaEngine
    {
    public :

        LuaEngine(lua_State* luaVM);

        int _GetBackgroundColor(lua_State*);
        int _LoadZone(lua_State*);
        int _LoadZoneFile(lua_State*);
        int _ToggleWireframeView(lua_State*);
        int _ToggleShading(lua_State*);
        int _SetBackgroundColor(lua_State*);
        int _UnloadZone(lua_State*);

        // Lunar function
        int GetDataTable(lua_State *L);

        static const char className[];
        static const char* classList[];
        static Lunar<LuaEngine>::RegType methods[];
        static const s_str CLASS_NAME;

    protected :

        s_ptr<Engine> pEngine_;

        lua_State* pLua_;
        int        iRef_;
    };

    /** \endcond
    */
}

#endif
