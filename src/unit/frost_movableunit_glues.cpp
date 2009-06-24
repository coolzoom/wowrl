/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          LuaMovableUnit source         */
/*                                        */
/*                                        */

#include "unit/frost_movableunit.h"

#define method(widget, function) {#function, &Lua##widget::_##function}

using namespace std;

namespace Frost
{
    const s_str LuaMovableUnit::CLASS_NAME = "LuaMovableUnit";

    LuaMovableUnit::LuaMovableUnit( lua_State* pLua ) : LuaUnit(pLua)
    {
        pParentMovable_ = s_ptr<MovableUnit>::DynamicCast(pParent_);
    }

    int LuaMovableUnit::_Jump( lua_State* pLua )
    {
        Lua::Function mFunc("MovableUnit:Jump", pLua);

        pParentMovable_->Jump();

        return mFunc.Return();
    }

    int LuaMovableUnit::_SetMoveForward( lua_State* pLua )
    {
        Lua::Function mFunc("MovableUnit:SetMoveForward", pLua);
        mFunc.Add(0, "move forward", Lua::TYPE_BOOLEAN);

        if (mFunc.Check())
        {
            pParentMovable_->SetMoveForward(mFunc.Get(0)->GetBool());
        }

        return mFunc.Return();
    }

    int LuaMovableUnit::_SetMoveBackward( lua_State* pLua )
    {
        Lua::Function mFunc("MovableUnit:SetMoveBackward", pLua);
        mFunc.Add(0, "move backward", Lua::TYPE_BOOLEAN);

        if (mFunc.Check())
        {
            pParentMovable_->SetMoveBackward(mFunc.Get(0)->GetBool());
        }

        return mFunc.Return();
    }

    int LuaMovableUnit::_SetMoveLeft( lua_State* pLua )
    {
        Lua::Function mFunc("MovableUnit:SetMoveLeft", pLua);
        mFunc.Add(0, "move left", Lua::TYPE_BOOLEAN);

        if (mFunc.Check())
        {
            pParentMovable_->SetMoveLeft(mFunc.Get(0)->GetBool());
        }

        return mFunc.Return();
    }

    int LuaMovableUnit::_SetMoveRight( lua_State* pLua )
    {
        Lua::Function mFunc("MovableUnit:SetMoveRight", pLua);
        mFunc.Add(0, "move right", Lua::TYPE_BOOLEAN);

        if (mFunc.Check())
        {
            pParentMovable_->SetMoveRight(mFunc.Get(0)->GetBool());
        }

        return mFunc.Return();
    }

    int LuaMovableUnit::_ToggleTurning( lua_State* pLua )
    {
        Lua::Function mFunc("MovableUnit:ToggleTurning", pLua);

        pParentMovable_->ToggleTurning();

        return mFunc.Return();
    }

    int LuaMovableUnit::_ToggleWalking( lua_State* pLua )
    {
        Lua::Function mFunc("MovableUnit:ToggleWalking", pLua);

        pParentMovable_->ToggleWalking();

        return mFunc.Return();
    }

    const char LuaMovableUnit::className[] = "MovableUnit";
    Lunar<LuaMovableUnit>::RegType LuaMovableUnit::methods[] = {
        {"dt", &LuaMovableUnit::GetDataTable},

        // Inherited from Unit
        method(MovableUnit, AddBuff),
        method(MovableUnit, AddHealth),
        method(MovableUnit, AddPower),
        method(MovableUnit, Damage),
        method(MovableUnit, FillHealthGauge),
        method(MovableUnit, FillPowerGauge),
        method(MovableUnit, EmptyHealthGauge),
        method(MovableUnit, EmptyPowerGauge),
        method(MovableUnit, Die),
        method(MovableUnit, GetCamera),
        method(MovableUnit, GetClass),
        method(MovableUnit, GetHealthRegenRatio),
        method(MovableUnit, GetLevel),
        method(MovableUnit, GetPowerRegenRatio),
        method(MovableUnit, GetStat),
        method(MovableUnit, GetUnitType),
        method(MovableUnit, IsInCombat),
        method(MovableUnit, IsSitting),
        method(MovableUnit, RotateCamera),
        method(MovableUnit, RotateModel),
        method(MovableUnit, SetAnim),
        method(MovableUnit, SetAttacking),
        method(MovableUnit, ZoomCamera),

        method(MovableUnit, Jump),
        method(MovableUnit, SetMoveForward),
        method(MovableUnit, SetMoveBackward),
        method(MovableUnit, SetMoveLeft),
        method(MovableUnit, SetMoveRight),
        method(MovableUnit, ToggleTurning),
        method(MovableUnit, ToggleWalking),

        {0,0}
    };
}
