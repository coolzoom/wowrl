/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          LuaCharacter source           */
/*                                        */
/*                                        */

#include "unit/frost_character.h"
#include "unit/frost_unitmanager.h"

#define method(widget, function) {#function, &Lua##widget::_##function}

using namespace std;

namespace Frost
{
    const s_str LuaCharacter::CLASS_NAME = "LuaCharacter";

    LuaCharacter::LuaCharacter( lua_State* pLua ) : LuaMovableUnit(pLua)
    {
        pParentCharacter_ = s_ptr<Character>::DynamicCast(pParent_);
    }

    int LuaCharacter::_GetRace( lua_State* pLua )
    {
        Lua::Function mFunc("Unit:GetRace", pLua, 1);

        if (mFunc.Check())
        {
            mFunc.Push(pParentCharacter_->GetRace().sName);
        }

        return mFunc.Return();
    }

    const char  LuaCharacter::className[] = "Character";
    const char* LuaCharacter::classList[] = {"Character", 0};
    Lunar<LuaCharacter>::RegType LuaCharacter::methods[] = {
        {"dt", &LuaCharacter::GetDataTable},

        method(Character, AddBuff),
        method(Character, AddHealth),
        method(Character, AddPower),
        method(Character, Damage),
        method(Character, DisableMotionBlur),
        method(Character, FillHealthGauge),
        method(Character, FillPowerGauge),
        method(Character, EmptyHealthGauge),
        method(Character, EmptyPowerGauge),
        method(Character, EnableMotionBlur),
        method(Character, Die),
        method(Character, GetCamera),
        method(Character, GetClass),
        method(Character, GetHealthRegenRatio),
        method(Character, GetLevel),
        method(Character, GetPowerRegenRatio),
        method(Character, GetStat),
        method(Character, GetUnitType),
        method(Character, IsInCombat),
        method(Character, IsSitting),
        method(Character, Jump),
        method(Character, RotateCamera),
        method(Character, RotateModel),
        method(Character, SetAnim),
        method(Character, SetAttacking),
        method(Character, SetMoveForward),
        method(Character, SetMoveBackward),
        method(Character, SetMoveLeft),
        method(Character, SetMoveRight),
        method(Character, ToggleTurning),
        method(Character, ToggleWalking),
        method(Character, ZoomCamera),

        method(Character, GetRace),

        {0,0}
    };
}
