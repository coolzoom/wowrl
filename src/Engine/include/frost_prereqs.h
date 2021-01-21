/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#include <frost.h>

namespace Frost
{
    /* SCENE *********************************************/
    // scene/frost_doodad.h
    class  Doodad;

    // scene/frost_zone.h
    class  Zone;

    // scene/frost_zonemanager.h
    class  ZoneManager;

    /* SPELL *********************************************/
    // spell/frost_spell.h
    class  Spell;

    /* UNIT **********************************************/
    // unit/frost_character.h
    struct Race;
    class  Character;
    class  LuaCharacter;

    // unit/frost_creature.h
    class  Creature;
    class  LuaCreature;

    // unit/frost_healthtype.h
    struct HealthType;
    class  HealthTypeInstance;

    // unit/frost_movableunit.h
    class  MovableUnit;
    class  LuaMovableUnit;

    // unit/frost_movableunithandler.h
    class  MovableUnitHandler;

    // unit/frost_powertype.h
    struct PowerType;
    class  PowerTypeInstance;

    // unit/frost_stats.h
    template<class T>
    class  Bonus;
    struct BonusSet;
    template<class T>
    class  Characteristic;
    struct Stats;

    // unit/frost_unit.h
    struct Class;
    class  Unit;
    class  LuaUnit;

    // unit/frost_unitmanager.h
    class  UnitManager;
    class  LuaUnitManager;
}
