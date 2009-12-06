/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           LuaCreature source           */
/*                                        */
/*                                        */

#include "unit/frost_creature.h"
#include "unit/frost_unitmanager.h"

#define method(widget, function) {#function, &Lua##widget::_##function}

using namespace std;

namespace Frost
{
    const s_str LuaCreature::CLASS_NAME = "LuaCreature";

    LuaCreature::LuaCreature( lua_State* pLua ) : LuaMovableUnit(pLua)
    {
        pParentCreature_ = s_ptr<Creature>::DynamicCast(pParent_);
    }

    const char  LuaCreature::className[] = "Creature";
    const char* LuaCreature::classList[] = {"Creature", 0};
    Lunar<LuaCreature>::RegType LuaCreature::methods[] = {
        {"dt", &LuaCreature::GetDataTable},

        method(Creature, AddBuff),
        method(Creature, AddHealth),
        method(Creature, AddPower),
        method(Creature, Damage),
        method(Creature, DisableMotionBlur),
        method(Creature, FillHealthGauge),
        method(Creature, FillPowerGauge),
        method(Creature, EmptyHealthGauge),
        method(Creature, EmptyPowerGauge),
        method(Creature, EnableMotionBlur),
        method(Creature, Die),
        method(Creature, GetCamera),
        method(Creature, GetClass),
        method(Creature, GetHealthRegenRatio),
        method(Creature, GetLevel),
        method(Creature, GetPowerRegenRatio),
        method(Creature, GetStat),
        method(Creature, GetUnitType),
        method(Creature, IsInCombat),
        method(Creature, IsSitting),
        method(Creature, Jump),
        method(Creature, RotateCamera),
        method(Creature, RotateModel),
        method(Creature, SetAnim),
        method(Creature, SetAttacking),
        method(Creature, SetMoveForward),
        method(Creature, SetMoveBackward),
        method(Creature, SetMoveLeft),
        method(Creature, SetMoveRight),
        method(Creature, ToggleTurning),
        method(Creature, ToggleWalking),
        method(Creature, ZoomCamera),

        {0,0}
    };
}
