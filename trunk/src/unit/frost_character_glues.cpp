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

    LuaCharacter::LuaCharacter( lua_State* pLua ) : LuaUnit(pLua)
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

    const char LuaCharacter::className[] = "Character";
    Lunar<LuaCharacter>::RegType LuaCharacter::methods[] = {
        {"dt", &LuaCharacter::GetDataTable},

        method(Character, AddBuff),
        method(Character, AddHealth),
        method(Character, AddPower),
        method(Character, Damage),
        method(Character, FillHealthGauge),
        method(Character, FillPowerGauge),
        method(Character, EmptyHealthGauge),
        method(Character, EmptyPowerGauge),
        method(Character, Die),
        method(Character, GetClass),
        method(Character, GetHealthRegenRatio),
        method(Character, GetLevel),
        method(Character, GetPowerRegenRatio),
        method(Character, GetStat),
        method(Character, GetUnitType),
        method(Character, IsInCombat),
        method(Character, IsSitting),
        method(Character, SetAnim),
        method(Character, SetAttacking),

        method(Character, GetRace),

        {0,0}
    };
}
