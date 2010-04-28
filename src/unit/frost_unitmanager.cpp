/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           UnitManager source           */
/*                                        */
/*                                        */

#include "unit/frost_unitmanager.h"

#include "unit/frost_character.h"
#include "unit/frost_creature.h"
#include "model/frost_modelmanager.h"
#include "model/frost_model.h"
#include "material/frost_materialmanager.h"
#include "material/frost_material.h"
#include "lua/frost_lua.h"
#include "unit/frost_healthtype.h"
#include "unit/frost_powertype.h"
#include "frost_inputmanager.h"
#include "gameplay/frost_gameplaymanager.h"

using namespace std;

namespace Frost
{
    const s_str UnitManager::CLASS_NAME = "UnitManager";

    UnitManager::UnitManager() : fMaxClimbingAngle_(0.125f)
    {
        Log("Creating "+CLASS_NAME+".");
    }

    UnitManager::~UnitManager()
    {
        Log("Closing "+CLASS_NAME+"...");
        s_map< s_uint, s_ptr<Unit> >::iterator iterUnit;
        foreach (iterUnit, lUnitList_)
        {
            iterUnit->second.Delete();
        }

        s_ctnr< s_ptr<LuaUnitManager> >::iterator iter;
        foreach (iter, lGlueList_)
        {
            iter->Delete();
        }

        LuaManager::GetSingleton()->CloseLua(pLua_);
    }

    void UnitManager::Initialize()
    {
        pLua_ = LuaManager::GetSingleton()->CreateLua();
        Lua::RegisterUnitClass(pLua_);
        Lua::RegisterGlobalFuncs(pLua_);
    }

    s_bool UnitManager::ReadConfig()
    {
        s_ptr<Engine> pEngine = Engine::GetSingleton();
        if (pEngine->IsConstantDefined("MaxClimbingAngle"))
            fMaxClimbingAngle_ = pEngine->GetFloatConstant("MaxClimbingAngle");

        return true;
    }

    void UnitManager::SetMaxClimbingAngle( const s_float& fMaxClimbingAngle )
    {
        fMaxClimbingAngle_ = fMaxClimbingAngle;
    }

    const s_float& UnitManager::GetMaxClimbingAngle() const
    {
        return fMaxClimbingAngle_;
    }

    s_ptr<Character> UnitManager::CreateCharacter( const s_str& sName, const s_str& sRace, Character::Gender mGender )
    {
        s_ptr<Character> pCharacter;

        if (lRaceList_.Find(sRace))
        {
            pCharacter = new Character(uiCounter_, sName, lRaceList_[sRace], mGender);
            lUnitList_[uiCounter_] = pCharacter;
            pCharacter->CreateGlue(pLua_);
            pCharacter->CreateGlue(GameplayManager::GetSingleton()->GetLua());
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
        pCreature->CreateGlue(pLua_);
        pCreature->CreateGlue(GameplayManager::GetSingleton()->GetLua());
        uiCounter_++;

        return pCreature;
    }

    void UnitManager::DeleteUnit( s_ptr<Unit> pUnit )
    {
        if (pUnit)
        {
            s_map< s_uint, s_ptr<Unit> >::iterator iterUnit;
            iterUnit = lUnitList_.Get(pUnit->GetID());

            if (iterUnit != lUnitList_.End())
            {
                if (iterUnit->second->GetID() == pUnit->GetID())
                {
                    // Everything went fine, delete and erase from map
                    iterUnit->second.Delete();
                    lUnitList_.Erase(iterUnit);
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
        s_map< s_uint, s_ptr<Unit> >::const_iterator iterUnit = lUnitList_.Get(uiID);
        if (iterUnit != lUnitList_.End())
        {
            return iterUnit->second;
        }
        else
        {
            Warning(CLASS_NAME,
                "No unit found with the ID : "+uiID+"."
            );
            return nullptr;
        }
    }

    const s_map< s_uint, s_ptr<Unit> >& UnitManager::GetUnitList() const
    {
        return lUnitList_;
    }

    const s_map< s_uint, s_ptr<Unit> >& UnitManager::GetSelectedUnitList() const
    {
        return lSelectedUnitList_;
    }

    void UnitManager::NotifyMouseOveredUnit( s_ptr<Unit> pUnit )
    {
        pMouseOveredUnit_ = pUnit;
    }

    void UnitManager::UpdateUnits( const s_float& fDelta )
    {
        // Single click selection
        if (InputManager::GetSingleton()->MouseIsReleased(MOUSE_LEFT))
        {
            if (InputManager::GetSingleton()->GetMouseDownDuration(MOUSE_LEFT) < 0.2f)
            {
                s_map< s_uint, s_ptr<Unit> >::iterator iterUnit;
                foreach (iterUnit, lSelectedUnitList_)
                {
                    iterUnit->second->NotifySelected(false);
                }
                lSelectedUnitList_.IsEmpty();

                if (pMouseOveredUnit_)
                {
                    pMouseOveredUnit_->NotifySelected(true);
                    lSelectedUnitList_[pMouseOveredUnit_->GetID()] = pMouseOveredUnit_;
                }
            }
        }

        // Update units
        s_map< s_uint, s_ptr<Unit> >::iterator iterUnit;
        foreach (iterUnit, lUnitList_)
        {
            iterUnit->second->Update(fDelta);
        }
    }

    const s_ctnr<s_str>& UnitManager::GetSpellSchoolList()
    {
        return lSchoolList_;
    }

    s_ptr<const Class> UnitManager::GetClassByName( const s_str& sClassName ) const
    {
        s_map<s_str, Class>::const_iterator iter = lClassList_.Get(sClassName);
        if (iter != lClassList_.End())
        {
            return &(iter->second);
        }
        else
        {
            Warning(CLASS_NAME,
                "No Class found with the name \""+sClassName+"\"."
            );
            return nullptr;
        }
    }

    s_ptr<const HealthType> UnitManager::GetHealthTypeByName( const s_str& sHealthTypeName ) const
    {
        s_map<s_str, HealthType>::const_iterator iter = lHealthTypeList_.Get(sHealthTypeName);
        if (iter != lHealthTypeList_.End())
        {
            return &(iter->second);
        }
        else
        {
            Warning(CLASS_NAME,
                "No HealthType found with the name \""+sHealthTypeName+"\"."
            );
            return nullptr;
        }
    }

    s_ptr<const PowerType> UnitManager::GetPowerTypeByName( const s_str& sPowerTypeName ) const
    {
        s_map<s_str, PowerType>::const_iterator iter = lPowerTypeList_.Get(sPowerTypeName);
        if (iter != lPowerTypeList_.End())
        {
            return &(iter->second);
        }
        else
        {
            Warning(CLASS_NAME,
                "No PowerType found with the name \""+sPowerTypeName+"\"."
            );
            return nullptr;
        }
    }

    void UnitManager::ParseData()
    {
        ParseSpellSchools_();
        ParseCharacterModels_();
        ParseRaces_();
        ParseClasses_();
        ParseHealthTypes_();
        ParsePowerTypes_();
    }

    s_ptr<Lua::State> UnitManager::GetLua()
    {
        return pLua_;
    }

    void UnitManager::CreateGlue( s_ptr<Lua::State> pLua )
    {
        lGlueList_.PushBack(
            pLua->Push<LuaUnitManager>(new LuaUnitManager(pLua->GetState()))
        );
        pLua->SetGlobal("UnitManager");
    }
}
