/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             LuaUnit source             */
/*                                        */
/*                                        */

#include "unit/frost_unit.h"
#include "unit/frost_unitmanager.h"
#include <camera/frost_camera.h>

#define method(widget, function) {#function, &Lua##widget::_##function}

using namespace std;

namespace Frost
{
    const s_str LuaUnit::CLASS_NAME = "LuaUnit";

    LuaUnit::LuaUnit( lua_State* pLua )
    {
        uiID_ = (uint)lua_tonumber(pLua, -1);

        lua_newtable(pLua);
        iRef_ = luaL_ref(pLua, LUA_REGISTRYINDEX);
        pLua_ = pLua;

        pParent_ = UnitManager::GetSingleton()->GetUnitByID(uiID_);
        sName_ = pParent_->GetName();
    }

    void LuaUnit::NotifyDeleted()
    {
        pParent_ = nullptr;
    }

    s_bool LuaUnit::CheckParent_()
    {
        if (!pParent_)
        {
            Warning(sName_+" ("+uiID_+")", "This unit has been deleted and can no longer be used.");
            return false;
        }

        return true;
    }

    int LuaUnit::_AddBuff( lua_State* pLua )
    {
        if (!CheckParent_())
            return 0;

        Lua::Function mFunc("Unit:AddBuff", pLua);

        if (mFunc.Check())
        {
            // NOTE : Not yet implemented (Unit:AddBuff).
            Log("Lua : not implemented : \""+mFunc.GetName()+"\".");
        }

        return mFunc.Return();
    }

    int LuaUnit::_AddHealth( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:AddHealth", pLua);
        mFunc.Add(0, "health add", Lua::TYPE_NUMBER);

        if (mFunc.Check())
        {
            pParent_->AddHealth(mFunc.Get(0)->GetNumber());
        }

        return mFunc.Return();
    }

    int LuaUnit::_AddPower( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:AddPower", pLua);
        mFunc.Add(0, "power add", Lua::TYPE_NUMBER);

        if (mFunc.Check())
        {
            pParent_->AddPower(mFunc.Get(0)->GetNumber());
        }

        return mFunc.Return();
    }

    int LuaUnit::_Damage( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:Damage", pLua);

        if (mFunc.Check())
        {
            // NOTE : Not yet implemented (Unit:Damage).
            Log("Lua : not implemented : \""+mFunc.GetName()+"\".");
        }

        return mFunc.Return();
    }

    int LuaUnit::_DisableMotionBlur( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:DisableMotionBlur", pLua);

        if (mFunc.Check())
        {
            pParent_->DisableMotionBlur();
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

    int LuaUnit::_EnableMotionBlur( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:EnableMotionBlur", pLua);

        if (mFunc.Check())
        {
            pParent_->EnableMotionBlur();
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
        mFunc.Add(0, "stat name", Lua::TYPE_STRING);

        if (mFunc.Check())
        {
            mFunc.Push(pParent_->GetStat(mFunc.Get(0)->GetString()));
        }

        return mFunc.Return();
    }

    int LuaUnit::_GetUnitType( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:GetUnitType", pLua, 1);

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
            // NOTE : Not yet implemented (Unit:IsInCombat).
            mFunc.Push(s_bool(false));
        }

        return mFunc.Return();
    }

    int LuaUnit::_IsSitting( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:IsSitting", pLua, 1);

        if (mFunc.Check())
        {
            // NOTE : Not yet implemented (Unit:IsSitting).
            mFunc.Push(s_bool(false));
        }

        return mFunc.Return();
    }

    int LuaUnit::_SetAnim( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:SetAnim", pLua);

        if (mFunc.Check())
        {
            // NOTE : Not yet implemented (Unit:SetAnim).
            Log("Lua : not implemented : \""+mFunc.GetName()+"\".");
        }

        return mFunc.Return();
    }

    int LuaUnit::_SetAttacking( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:SetAttacking", pLua);

        if (mFunc.Check())
        {
            // NOTE : Not yet implemented (Unit:SetAttacking).
            Log("Lua : not implemented : \""+mFunc.GetName()+"\".");
        }

        return mFunc.Return();
    }

    int LuaUnit::_SetPlayerControlled( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:SetPlayerControlled", pLua);
        mFunc.Add(0, "player controlled", Lua::TYPE_BOOLEAN);

        if (mFunc.Check())
        {
            pParent_->SetPlayerControlled(mFunc.Get(0)->GetBool());
        }

        return mFunc.Return();
    }

    int LuaUnit::_GetCamera( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:GetCamera", pLua, 1);

        if (pParent_->GetCamera())
        {
            pParent_->GetCamera()->PushOnLua(mFunc.GetState());
            mFunc.NotifyPushed();
        }
        else
        {
            mFunc.PushNil();
        }

        return mFunc.Return();
    }

    int LuaUnit::_RotateCamera( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:RotateCamera", pLua);
        mFunc.Add(0, "yaw", Lua::TYPE_NUMBER);
        mFunc.Add(1, "pitch", Lua::TYPE_NUMBER);

        if (mFunc.Check())
        {
            pParent_->RotateCamera(
                mFunc.Get(0)->GetNumber(),
                mFunc.Get(1)->GetNumber()
            );
        }

        return mFunc.Return();
    }

    int LuaUnit::_RotateModel( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:RotateModel", pLua);
        mFunc.Add(0, "yaw", Lua::TYPE_NUMBER);
        mFunc.Add(1, "pitch", Lua::TYPE_NUMBER);

        if (mFunc.Check())
        {
            pParent_->RotateModel(
                mFunc.Get(0)->GetNumber(),
                mFunc.Get(1)->GetNumber()
            );
        }

        return mFunc.Return();
    }

    int LuaUnit::_ZoomCamera( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:ZoomCamera", pLua);
        mFunc.Add(0, "zoom", Lua::TYPE_NUMBER);

        if (mFunc.Check())
        {
            pParent_->ZoomCamera(mFunc.Get(0)->GetNumber());
        }

        return mFunc.Return();
    }

    int LuaUnit::GetDataTable( lua_State * pLua )
    {
        lua_getref(pLua, iRef_);
        return 1;
    }

    const char  LuaUnit::className[] = "Unit";
    const char* LuaUnit::classList[] = {"Unit",
        "MovableUnit", "Character", "Creature", 0
    };
    Lunar<LuaUnit>::RegType LuaUnit::methods[] = {
        {"dt", &LuaUnit::GetDataTable},

        method(Unit, AddBuff),
        method(Unit, AddHealth),
        method(Unit, AddPower),
        method(Unit, Damage),
        method(Unit, DisableMotionBlur),
        method(Unit, FillHealthGauge),
        method(Unit, FillPowerGauge),
        method(Unit, EmptyHealthGauge),
        method(Unit, EmptyPowerGauge),
        method(Unit, EnableMotionBlur),
        method(Unit, Die),
        method(Unit, GetCamera),
        method(Unit, GetClass),
        method(Unit, GetHealthRegenRatio),
        method(Unit, GetLevel),
        method(Unit, GetPowerRegenRatio),
        method(Unit, GetStat),
        method(Unit, GetUnitType),
        method(Unit, IsInCombat),
        method(Unit, IsSitting),
        method(Unit, RotateCamera),
        method(Unit, RotateModel),
        method(Unit, SetAnim),
        method(Unit, SetAttacking),
        method(Unit, SetPlayerControlled),
        method(Unit, ZoomCamera),

        {0,0}
    };
}
