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
        mFunc.Add(0, "flags", Lua::TYPE_STRING);

        if (mFunc.Check())
        {
            s_str sFlags = mFunc.Get(0)->GetString();
            s_bool bAlive, bDead;
            s_bool bFriendly, bHostile;
            s_ctnr<s_str> lWords = sFlags.Cut(",");
            s_ctnr<s_str>::iterator iterWord;
            foreach (iterWord, lWords)
            {
                iterWord->Trim(' ');
                iterWord->ToUpper();
                if (*iterWord == "ALIVE")
                    bAlive = true;
                else if (*iterWord == "DEAD")
                    bDead = true;
                else if (*iterWord == "FRIENDLY")
                    bFriendly = true;
                else if (*iterWord == "HOSTILE")
                    bHostile = true;
                else
                    Warning(mFunc.GetName(),
                        "Unknown flag : \""+*iterWord+"\". Ignored."
                    );
            }

            s_ptr<Unit> pUnit;

            const s_map< s_uint, s_ptr<Unit> >& lUnitList = UnitManager::GetSingleton()->GetSelectedUnitList();
            s_map< s_uint, s_ptr<Unit> >::const_iterator iterUnit;
            foreach (iterUnit, lUnitList)
            {
                if ((bAlive && !iterUnit->second->IsDead()) ||
                    (bDead && iterUnit->second->IsDead()) ||
                    (bAlive == bDead))
                {
                    if ((bFriendly && !iterUnit->second->IsHostile()) ||
                        (bHostile && iterUnit->second->IsHostile()) ||
                        (bFriendly == bHostile))
                    {
                        pUnit = iterUnit->second;
                        break;
                    }
                }
            }

            if (pUnit)
            {
                pUnit->PushOnLua(mFunc.GetState());
            }
            else
                mFunc.GetState()->PushNil();
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
