/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Lua header                */
/*                                        */
/*                                        */


#ifndef FROST_LUA_GLUES_H
#define FROST_LUA_GLUES_H

#include "frost_prereqs.h"

namespace Frost
{
namespace Lua
{
    // Engine
    int _EnableMouseDecal(lua_State* pLua);
    int _GetMouseDecalColor(lua_State* pLua);
    int _GetMouseDecalPosition(lua_State* pLua);
    int _SetMouseDecalColor(lua_State* pLua);
    int _SetMouseDecalTexture(lua_State* pLua);

    void RegisterNewEngineFuncs(s_ptr<Lua::State> pLua);

    // Editor
    int _AddDoodad(lua_State* pLua);
    int _GetCurrentZoneFile(lua_State* pLua);
    int _GetModelFile(lua_State* pLua);
    int _GetModelMaterial(lua_State* pLua);
    int _GetZoneModelList(lua_State* pLua);
    int _IsModelLoaded(lua_State* pLua);
    int _RegisterNewModel(lua_State* pLua);
    int _IsZoneLoaded(lua_State* pLua);
    int _IsZoneSaved(lua_State* pLua);
    int _LoadZone(lua_State* pLua);
    int _LoadZoneFile(lua_State* pLua);
    int _NewZone(lua_State* pLua);
    int _NotifyDoodadPositioned(lua_State* pLua);
    int _SaveZone(lua_State* pLua);
    int _SetModelMaterial(lua_State* pLua);
    int _UnloadZone(lua_State* pLua);

    void RegisterNewEditorFuncs(s_ptr<Lua::State> pLua);
}
}

#endif
