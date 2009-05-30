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
#include "frost_healthtype.h"
#include "frost_powertype.h"
#include "frost_inputmanager.h"
#include "camera/frost_cameramanager.h"
#include "camera/frost_camera.h"

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
            pCharacter->CreateGlue();
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
        pCreature->CreateGlue();
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
        {
            Warning(CLASS_NAME,
                "No unit found with the ID : "+uiID+"."
            );
            return NULL;
        }
    }

    void UnitManager::UpdateUnits( const s_float& fDelta )
    {
        // Handle selection
        s_ptr<InputManager> pInputMgr = InputManager::GetSingleton();
        s_ptr<Engine>       pFrost = Engine::GetSingleton();

        Ogre::Ray mRay = CameraManager::GetSingleton()->GetMainCamera()->GetOgreCamera()->getCameraToViewportRay(
            (pInputMgr->GetMPosX()/s_float(pFrost->GetScreenWidth())).Get(),
            (pInputMgr->GetMPosY()/s_float(pFrost->GetScreenHeight())).Get()
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
                s_str sType = pMyObject->getTypeName();
                if (sType == "CHARACTER")
                {
                    pUnit = s_ptr<CharacterOgreInterface>(pMyObject)->GetCharacter();
                    break;
                }
                else if (sType == "CREATURE")
                {
                    pUnit = s_ptr<CreatureOgreInterface>(pMyObject)->GetCreature();
                    break;
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
                map< s_uint, s_ptr<Unit> >::iterator iterUnit;
                foreach (iterUnit, lSelectedUnitList_)
                {
                    iterUnit->second->NotifySelected(false);
                }
                lSelectedUnitList_.empty();

                if (pUnit)
                {
                    pUnit->NotifySelected(true);
                    lSelectedUnitList_[pUnit->GetID()] = pUnit;
                }
            }
        }

        pMouseOveredUnit_ = pUnit;

        // Update units
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

    s_ptr<const Class> UnitManager::GetClassByName( const s_str& sClassName ) const
    {
        map<s_str, Class>::const_iterator iter = lClassList_.find(sClassName);
        if (iter != lClassList_.end())
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
        map<s_str, HealthType>::const_iterator iter = lHealthTypeList_.find(sHealthTypeName);
        if (iter != lHealthTypeList_.end())
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
        map<s_str, PowerType>::const_iterator iter = lPowerTypeList_.find(sPowerTypeName);
        if (iter != lPowerTypeList_.end())
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
}
