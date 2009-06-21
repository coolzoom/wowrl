/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             LuaUnit source             */
/*                                        */
/*                                        */

#include "unit/frost_unit.h"
#include "unit/frost_unitmanager.h"

#define method(widget, function) {#function, &Lua##widget::_##function}

using namespace std;

namespace Frost
{
    const s_str LuaUnitManager::CLASS_NAME = "LuaUnitManager";

    LuaUnitManager::LuaUnitManager( lua_State* pLua )
    {
        lua_newtable(pLua);
        iRef_ = luaL_ref(pLua, LUA_REGISTRYINDEX);
        pLua_ = pLua;
    }

    int LuaUnitManager::_GetSelectedUnit( lua_State* pLua )
    {
        Lua::Function mFunc("UnitManager:GetSelectedUnit", pLua);

        if (mFunc.Check())
        {
            Log("Lua : not implemented : \""+mFunc.GetName()+"\".");
        }

        return mFunc.Return();
    }

    int LuaUnitManager::_GetNearestUnit( lua_State* pLua )
    {
        Lua::Function mFunc("UnitManager:GetNearestUnit", pLua);

        if (mFunc.Check())
        {
            Log("Lua : not implemented : \""+mFunc.GetName()+"\".");
        }

        return mFunc.Return();
    }

    int LuaUnitManager::GetDataTable( lua_State * pLua )
    {
        lua_getref(pLua, iRef_);
        return 1;
    }

    const char LuaUnitManager::className[] = "Unit";
    Lunar<LuaUnitManager>::RegType LuaUnitManager::methods[] = {
        {"dt", &LuaUnitManager::GetDataTable},

        method(UnitManager, GetSelectedUnit),
        method(UnitManager, GetNearestUnit),

        {0,0}
    };
}
