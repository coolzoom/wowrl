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
#include "camera/frost_cameramanager.h"
#include "camera/frost_camera.h"
#include "gameplay/frost_gameplaymanager.h"

#include <OgreRay.h>
#include <OgreSceneManager.h>

using namespace std;

namespace Frost
{
    const s_str UnitManager::CLASS_NAME = "UnitManager";

    UnitManager::UnitManager()
    {
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
            return NULL;
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

    void UnitManager::UpdateUnits( const s_float& fDelta )
    {
        // Handle selection
        s_ptr<InputManager> pInputMgr = InputManager::GetSingleton();
        s_ptr<Engine>       pFrost = Engine::GetSingleton();

        Ogre::Ray mRay = CameraManager::GetSingleton()->GetMainCamera()->GetOgreCamera()->getCameraToViewportRay(
            (pInputMgr->GetMousePosX()/s_float(pFrost->GetScreenWidth())).Get(),
            (pInputMgr->GetMousePosY()/s_float(pFrost->GetScreenHeight())).Get()
        );
        s_ptr<Ogre::RaySceneQuery> pRayQuery = pFrost->GetOgreSceneManager()->createRayQuery(mRay);

        pRayQuery->setSortByDistance(true);
        Ogre::RaySceneQueryResult& mRes = pRayQuery->execute();
        Ogre::RaySceneQueryResult::iterator iter = mRes.begin();
        s_ptr<Unit> pUnit;
        if (iter != mRes.end())
        {
            foreach (iter, mRes)
            {
                s_ptr<Ogre::MovableObject> pObject = iter->movable;
                s_ptr<Ogre::UserDefinedObject> pMyObject = pObject->getUserObject();
                if (pMyObject)
                {
                    s_str sType = pMyObject->getTypeName();
                    if (sType == "CHARACTER")
                    {
                        pUnit = s_ptr<CharacterOgreInterface>::DynamicCast(pMyObject)->GetCharacter();
                        break;
                    }
                    else if (sType == "CREATURE")
                    {
                        pUnit = s_ptr<CreatureOgreInterface>::DynamicCast(pMyObject)->GetCreature();
                        break;
                    }
                }
            }
        }
        pFrost->GetOgreSceneManager()->destroyQuery(pRayQuery.Get());

        if (pMouseOveredUnit_ != pUnit)
        {
            if (pMouseOveredUnit_)
                pMouseOveredUnit_->NotifyHighlighted(false);
            if (pUnit)
                pUnit->NotifyHighlighted(true);
        }

        // Single click selection
        if (pInputMgr->MouseIsReleased(MOUSE_LEFT))
        {
            if (pInputMgr->GetMouseDownDuration(MOUSE_LEFT) < 0.2f)
            {
                s_map< s_uint, s_ptr<Unit> >::iterator iterUnit;
                foreach (iterUnit, lSelectedUnitList_)
                {
                    iterUnit->second->NotifySelected(false);
                }
                lSelectedUnitList_.IsEmpty();

                if (pUnit)
                {
                    pUnit->NotifySelected(true);
                    lSelectedUnitList_[pUnit->GetID()] = pUnit;
                }
            }
        }

        pMouseOveredUnit_ = pUnit;

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
            return NULL;
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
            return NULL;
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
            return NULL;
        }
    }

    s_bool UnitManager::ParseData()
    {
        if (!ParseSpellSchools_())
            return false;

        if (!ParseCharacterModels_())
            return false;

        if (!ParseRaces_())
            return false;

        if (!ParseClasses_())
            return false;

        if (!ParseHealthTypes_())
            return false;

        if (!ParsePowerTypes_())
            return false;

        return true;
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
