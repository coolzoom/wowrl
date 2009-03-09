/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           UnitManager source           */
/*                                        */
/*                                        */

#include "frost_unitmanager.h"

#include "frost_character.h"
#include "frost_creature.h"
#include "frost_modelmanager.h"
#include "frost_model.h"
#include "frost_materialmanager.h"
#include "frost_material.h"
#include "frost_lua.h"

using namespace std;

namespace Frost
{
    const s_str UnitManager::CLASS_NAME = "UnitManager";

    UnitManager::UnitManager()
    {
    }

    UnitManager::~UnitManager()
    {
        map< s_uint, s_ptr<Unit> >::iterator iterUnit;
        foreach (iterUnit, lUnitList_)
        {
            iterUnit->second.Delete();
        }

        LuaManager::GetSingleton()->CloseLua(pLua_);
    }

    void UnitManager::Initialize()
    {
        pLua_ = LuaManager::GetSingleton()->CreateLua();
        Lua::RegisterUnitClass(pLua_);
        Lua::RegisterGlobalFuncs(pLua_);
    }

    s_ptr<Character> UnitManager::CreateCharacter( const s_str& sName, const s_str& sRace, CharGender mGender )
    {
        s_ptr<Character> pCharacter;

        if (MAPFIND(sRace, lRaceList_))
        {
            pCharacter = new Character(uiCounter_, sName, lRaceList_[sRace], mGender);
            lUnitList_[uiCounter_] = pCharacter;
            uiCounter_++;
        }
        else
        {
            Error(CLASS_NAME, "Unkown race : \""+sRace+"\" (creating character \""+sName+"\").");
        }

        return pCharacter;
    }

    s_ptr<Creature> UnitManager::CreateCreature( const s_str& sName )
    {
        s_ptr<Creature> pCreature = new Creature(uiCounter_, sName);
        lUnitList_[uiCounter_] = pCreature;
        uiCounter_++;

        return pCreature;
    }

    void UnitManager::DeleteUnit( s_ptr<Unit> pUnit )
    {
        if (pUnit)
        {
            map< s_uint, s_ptr<Unit> >::iterator iterUnit;
            iterUnit = lUnitList_.find(pUnit->GetID());

            if (iterUnit != lUnitList_.end())
            {
                if (iterUnit->second->GetID() == pUnit->GetID())
                {
                    // Everything went fine, delete and erase from map
                    iterUnit->second.Delete();
                    lUnitList_.erase(iterUnit);
                }
            }
            else
            {
                Warning(CLASS_NAME,
                    "Trying to call DeleteUnit on an Unit that has not been created by UnitManager (ID:"+
                    pUnit->GetID()+")."
                );
            }
        }
    }

    s_ptr<Unit> UnitManager::GetUnitByID( const s_uint& uiID ) const
    {
        map< s_uint, s_ptr<Unit> >::const_iterator iterUnit = lUnitList_.find(uiID);
        if (iterUnit != lUnitList_.end())
        {
            return iterUnit->second;
        }
        else
            return NULL;
    }

    void UnitManager::UpdateUnits( const s_float& fDelta )
    {
        map< s_uint, s_ptr<Unit> >::iterator iterUnit;
        foreach (iterUnit, lUnitList_)
        {
            iterUnit->second->Update(fDelta);
        }
    }

    const std::vector<s_str>& UnitManager::GetSpellSchoolList()
    {
        return lSchoolList_;
    }
}
