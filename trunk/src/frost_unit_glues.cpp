/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             LuaUnit source             */
/*                                        */
/*                                        */

#include "frost_unit.h"
#include "frost_unitmanager.h"

#define method(widget, function) {#function, &Lua##widget::_##function}

using namespace std;

namespace Frost
{
    const s_str LuaUnit::CLASS_NAME = "LuaUnit";

    LuaUnit::LuaUnit( lua_State* pLua )
    {
        s_uint uiID = (uint)lua_tonumber(pLua, 1);

        lua_newtable(pLua);
        iRef_ = luaL_ref(pLua, LUA_REGISTRYINDEX);
        pLua_ = pLua;

        pParent_ = UnitManager::GetSingleton()->GetUnitByID(uiID);
    }

    int LuaUnit::_AddBuff( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:AddBuff", pLua);

        if (mFunc.Check())
        {

        }

        return mFunc.Return();
    }

    int LuaUnit::_Damage( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:Damage", pLua);

        if (mFunc.Check())
        {

        }

        return mFunc.Return();
    }

    int LuaUnit::_SetAnim( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:SetAnim", pLua);

        if (mFunc.Check())
        {

        }

        return mFunc.Return();
    }

    int LuaUnit::_SetAttacking( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:SetAttacking", pLua);

        if (mFunc.Check())
        {

        }

        return mFunc.Return();
    }

    int LuaUnit::GetDataTable( lua_State * pLua )
    {
        lua_getref(pLua, iRef_);
        return 1;
    }

    const char LuaUnit::className[] = "Unit";
    Lunar<LuaUnit>::RegType LuaUnit::methods[] = {
        {"dt", &LuaUnit::GetDataTable},

        method(Unit, AddBuff),
        method(Unit, Damage),
        method(Unit, SetAnim),
        method(Unit, SetAttacking),

        {0,0}
    };
}
