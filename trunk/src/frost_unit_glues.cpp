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
            Log("Lua : not implemented : \""+mFunc.GetName()+"\".");
        }

        return mFunc.Return();
    }

    int LuaUnit::_AddHealth( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:AddHealth", pLua);
        mFunc.Add(0, "health add", Lua::TYPE_NUMBER, VALUE_FLOAT);

        if (mFunc.Check())
        {
            pParent_->AddHealth(mFunc.Get(0)->GetF());
        }

        return mFunc.Return();
    }

    int LuaUnit::_AddPower( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:AddPower", pLua);
        mFunc.Add(0, "power add", Lua::TYPE_NUMBER, VALUE_FLOAT);

        if (mFunc.Check())
        {
            pParent_->AddPower(mFunc.Get(0)->GetF());
        }

        return mFunc.Return();
    }

    int LuaUnit::_Damage( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:Damage", pLua);

        if (mFunc.Check())
        {
            Log("Lua : not implemented : \""+mFunc.GetName()+"\".");
        }

        return mFunc.Return();
    }

    int LuaUnit::_FillHealthGauge( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:FillHealthGauge", pLua);

        if (mFunc.Check())
        {
            pParent_->FillHealthGauge();
        }

        return mFunc.Return();
    }

    int LuaUnit::_FillPowerGauge( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:FillPowerGauge", pLua);

        if (mFunc.Check())
        {
            pParent_->FillPowerGauge();
        }

        return mFunc.Return();
    }

    int LuaUnit::_EmptyHealthGauge( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:EmptyHealthGauge", pLua);

        if (mFunc.Check())
        {
            pParent_->EmptyHealthGauge();
        }

        return mFunc.Return();
    }

    int LuaUnit::_EmptyPowerGauge( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:EmptyPowerGauge", pLua);

        if (mFunc.Check())
        {
            pParent_->EmptyPowerGauge();
        }

        return mFunc.Return();
    }

    int LuaUnit::_Die( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:Die", pLua);

        if (mFunc.Check())
        {
            pParent_->Die();
        }

        return mFunc.Return();
    }

    int LuaUnit::_GetClass( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:GetClass", pLua, 1);

        if (mFunc.Check())
        {
            mFunc.Push(pParent_->GetClass().sName);
        }

        return mFunc.Return();
    }

    int LuaUnit::_GetHealthRegenRatio( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:GetHealthRegenRatio", pLua, 1);

        if (mFunc.Check())
        {
            mFunc.Push(pParent_->GetHealthRegenRatio());
        }

        return mFunc.Return();
    }

    int LuaUnit::_GetLevel( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:GetLevel", pLua, 1);

        if (mFunc.Check())
        {
            mFunc.Push(pParent_->GetLevel());
        }

        return mFunc.Return();
    }

    int LuaUnit::_GetPowerRegenRatio( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:GetPowerRegenRatio", pLua, 1);

        if (mFunc.Check())
        {
            mFunc.Push(pParent_->GetPowerRegenRatio());
        }

        return mFunc.Return();
    }

    int LuaUnit::_GetStat( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:GetStat", pLua, 1);
        mFunc.Add(0, "stat name", Lua::TYPE_STRING, VALUE_STRING);

        if (mFunc.Check())
        {
            mFunc.Push(pParent_->GetStat(mFunc.Get(0)->GetS()));
        }

        return mFunc.Return();
    }

    int LuaUnit::_GetUnitType( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:SetAnim", pLua, 1);

        if (mFunc.Check())
        {
            mFunc.Push(s_str("UNIT"));
        }

        return mFunc.Return();
    }

    int LuaUnit::_IsInCombat( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:IsInCombat", pLua, 1);

        if (mFunc.Check())
        {
            mFunc.Push(s_bool(false));
        }

        return mFunc.Return();
    }

    int LuaUnit::_IsSitting( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:IsSitting", pLua, 1);

        if (mFunc.Check())
        {
            mFunc.Push(s_bool(false));
        }

        return mFunc.Return();
    }

    int LuaUnit::_SetAnim( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:SetAnim", pLua);

        if (mFunc.Check())
        {
            Log("Lua : not implemented : \""+mFunc.GetName()+"\".");
        }

        return mFunc.Return();
    }

    int LuaUnit::_SetAttacking( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:SetAttacking", pLua);

        if (mFunc.Check())
        {
            Log("Lua : not implemented : \""+mFunc.GetName()+"\".");
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
        method(Unit, AddHealth),
        method(Unit, AddPower),
        method(Unit, Damage),
        method(Unit, FillHealthGauge),
        method(Unit, FillPowerGauge),
        method(Unit, EmptyHealthGauge),
        method(Unit, EmptyPowerGauge),
        method(Unit, Die),
        method(Unit, GetClass),
        method(Unit, GetHealthRegenRatio),
        method(Unit, GetLevel),
        method(Unit, GetPowerRegenRatio),
        method(Unit, GetStat),
        method(Unit, GetUnitType),
        method(Unit, IsInCombat),
        method(Unit, IsSitting),
        method(Unit, SetAnim),
        method(Unit, SetAttacking),

        {0,0}
    };
}
