/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           LuaCreature source           */
/*                                        */
/*                                        */

#include "frost_creature.h"
#include "frost_unitmanager.h"

#define method(widget, function) {#function, &Lua##widget::_##function}

using namespace std;

namespace Frost
{
    const s_str LuaCreature::CLASS_NAME = "LuaCreature";

    LuaCreature::LuaCreature( lua_State* pLua ) : LuaUnit(pLua)
    {
        pParentCreature_ = s_ptr<Creature>(pParent_);
    }

    const char LuaCreature::className[] = "Creature";
    Lunar<LuaCreature>::RegType LuaCreature::methods[] = {
        {"dt", &LuaCreature::GetDataTable},

        method(Creature, AddBuff),
        method(Creature, AddHealth),
        method(Creature, AddPower),
        method(Creature, Damage),
        method(Creature, FillHealthGauge),
        method(Creature, FillPowerGauge),
        method(Creature, GetClass),
        method(Creature, GetHealthRegenRatio),
        method(Creature, GetLevel),
        method(Creature, GetPowerRegenRatio),
        method(Creature, GetStat),
        method(Creature, GetUnitType),
        method(Creature, IsInCombat),
        method(Creature, IsSitting),
        method(Creature, SetAnim),
        method(Creature, SetAttacking),

        {0,0}
    };
}
