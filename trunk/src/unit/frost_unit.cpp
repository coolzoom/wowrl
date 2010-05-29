/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Unit source              */
/*                                        */
/*                                        */

#include "unit/frost_unit.h"

#include "model/frost_modelmanager.h"
#include "model/frost_model.h"
#include "model/frost_animmanager.h"
#include "scene/frost_scenemanager.h"
#include "scene/frost_node.h"
#include "scene/frost_zonemanager.h"
#include "camera/frost_cameramanager.h"
#include "camera/frost_camera.h"
#include "unit/frost_unitmanager.h"
#include "unit/frost_healthtype.h"
#include "unit/frost_powertype.h"
#include "material/frost_material.h"
#include "material/frost_decal.h"
#include "material/frost_shadermanager.h"
#include "gameplay/frost_gameplaymanager.h"

using namespace std;

namespace Frost
{
    const s_str Unit::CLASS_NAME = "Unit";

    // TODO : Unit : Implement spells
    // TODO : Unit : Implement movement (+pathfinding)
    // TODO : Unit : Implement buffs

    Unit::Unit( const s_uint& uiID, const s_str& sName ) :
        uiID_(uiID), sName_(sName), uiLevel_(s_uint::NaN)
    {
        pNode_ = SceneManager::GetSingleton()->CreateNode();
        pCamera_ = CameraManager::GetSingleton()->CreateCamera(Vector(0, 4, 5));
        pCamera_->OrbitAround(Vector(0, 2, 0));
        pCamera_->AttachTo(pNode_, false, false);
        pCamera_->CreateGlue(GameplayManager::GetSingleton()->GetLua());

        pSelectionDecal_ = s_refptr<Decal>(new Decal("Textures/UnitSelection.png"));
        pSelectionDecal_->AttachTo(pNode_, false, true);
        pSelectionDecal_->SetPosition(Vector(0, 5, 0));
        pSelectionDecal_->SetDirection(Vector(0, -1, 0));
        pSelectionDecal_->SetProjection(Decal::PROJ_ORTHOGRAPHIC);
        pSelectionDecal_->SetScale(1.5f);
        pSelectionDecal_->SetSelfIllumination(Color(0, 255, 0));
        pSelectionDecal_->Show();

        pShadowDecal_ = s_refptr<Decal>(new Decal("Textures/UnitShadow.png"));
        pShadowDecal_->AttachTo(pNode_, false, true);
        pShadowDecal_->SetPosition(Vector(0, 5, 0));
        pShadowDecal_->SetDirection(Vector(0, -1, 0));
        pShadowDecal_->SetProjection(Decal::PROJ_ORTHOGRAPHIC);
        pShadowDecal_->SetScale(1.8f);
        pShadowDecal_->Show();

        ZoneManager::GetSingleton()->AddDecalOnGround(pShadowDecal_);
    }

    Unit::~Unit()
    {
        ZoneManager::GetSingleton()->RemoveDecalFromGround(pShadowDecal_);
        if (bSelected_)
            ZoneManager::GetSingleton()->RemoveDecalFromGround(pSelectionDecal_);

        CameraManager::GetSingleton()->DeleteCamera(pCamera_);
        SceneManager::GetSingleton()->DeleteNode(pNode_);

        s_ctnr< s_ptr<LuaUnit> >::iterator iter;
        foreach (iter, lGlueList_)
        {
            (*iter)->NotifyDeleted();
        }
    }

    void Unit::SetClass(const s_str& sClassName)
    {
        mClass_ = *UnitManager::GetSingleton()->GetClassByName(sClassName);
        pHealthType_ = s_refptr<HealthTypeInstance>(new HealthTypeInstance(this, mClass_.sHealthType));
        pPowerType_ = s_refptr<PowerTypeInstance>(new PowerTypeInstance(this, mClass_.sPowerType));

        pHealthType_->On("UnitSpawn");
        pPowerType_->On("UnitSpawn");
    }

    void Unit::SetClass(const Class& mClass)
    {
        mClass_ = mClass;
        pHealthType_ = s_refptr<HealthTypeInstance>(new HealthTypeInstance(this, mClass_.sHealthType));
        pPowerType_ = s_refptr<PowerTypeInstance>(new PowerTypeInstance(this, mClass_.sPowerType));

        pHealthType_->On("UnitSpawn");
        pPowerType_->On("UnitSpawn");
    }

    const Class& Unit::GetClass() const
    {
        return mClass_;
    }

    void Unit::AddHealth( const s_float& fHealthAdd )
    {
        fHealth_ += fHealthAdd;
        s_float fMaxHealth = mStats_.fMaxHealth.GetValue();
        if (fHealth_ > fMaxHealth)
        {
            fHealth_ = fMaxHealth;
            pHealthType_->On("Max");
        }
        if (fHealth_ < 0.0f)
        {
            fHealth_ = 0.0f;
            pHealthType_->On("Min");
        }
    }

    void Unit::FillHealthGauge()
    {
        fHealth_ = mStats_.fMaxHealth.GetValue();
    }

    void Unit::EmptyHealthGauge()
    {
        fHealth_ = 0.0f;
    }

    const s_float& Unit::GetHealth() const
    {
        return fHealth_;
    }

    s_float Unit::GetMaxHealth() const
    {
        return mStats_.fMaxHealth.GetValue();
    }

    s_float Unit::GetHealthRegenRatio() const
    {
        return mStats_.fHealthRegenRatio.GetValue();
    }

    void Unit::AddPower( const s_float& fPowerAdd )
    {
        fPower_ += fPowerAdd;
        s_float fMaxPower = mStats_.fMaxPower.GetValue();
        if (fPower_ > fMaxPower)
        {
            fPower_ = fMaxPower;
            pPowerType_->On("Max");
        }
        if (fPower_ < 0.0f)
        {
            fPower_ = 0.0f;
            pPowerType_->On("Min");
        }
    }

    void Unit::FillPowerGauge()
    {
        fPower_ = mStats_.fMaxPower.GetValue();
    }

    void Unit::EmptyPowerGauge()
    {
        fPower_ = 0.0f;
    }

    const s_float& Unit::GetPower() const
    {
        return fPower_;
    }

    s_float Unit::GetMaxPower() const
    {
        return mStats_.fMaxPower.GetValue();
    }

    s_float Unit::GetPowerRegenRatio() const
    {
        return mStats_.fPowerRegenRatio.GetValue();
    }

    void Unit::Die()
    {
        if (!bDead_)
        {
            bDead_ = true;

            pHealthType_->On("UnitDeath");
            pPowerType_->On("UnitDeath");

            pBodyModel_->GetAnimMgr()->SetAnim(ANIM_DEATH, ANIM_PRIORITY_BACKGROUND);
        }
    }

    const s_bool& Unit::IsDead()
    {
        return bDead_;
    }

    void Unit::Resurrect()
    {
        if (bDead_)
        {
            bDead_ = false;

            pBodyModel_->GetAnimMgr()->SetAnim(ANIM_STAND, ANIM_PRIORITY_BACKGROUND);
            pBodyModel_->GetAnimMgr()->Play();
        }
    }

    s_bool Unit::IsHostile( s_ptr<Unit> pReference ) const
    {
        // NOTE : Not yet implemented (Unit::IsHostile).
        return false;
    }

    void Unit::SetStat( const s_str& sStatName, const s_int& iValue )
    {
        mStats_.lCharactList[sStatName].SetBaseValue(iValue);
    }

    void Unit::SetStat( const s_str& sStatName, const s_float& fValue )
    {
        mStats_.lCharactList[sStatName].SetBaseValue(fValue);
    }

    s_var Unit::GetStat( const s_str& sStatName ) const
    {
        s_map< s_str, Characteristic<s_var> >::const_iterator iter = mStats_.lCharactList.Get(sStatName);
        if (iter != mStats_.lCharactList.End())
        {
            s_var vReturn;
            if (iter->second.IsInteger())
                vReturn = iter->second.GetValueI();
            else
                vReturn = iter->second.GetValueF();

            return vReturn;
        }
        else
        {
            Error(CLASS_NAME,
                "Unkown stat : \""+sStatName+"\" for "+sName_+". Returning zero."
            );
            return s_int(0);
        }
    }

    void Unit::SetLevel( const s_uint& uiLevel )
    {
        uiLevel_ = uiLevel;
    }

    const s_uint& Unit::GetLevel() const
    {
        return uiLevel_;
    }

    void Unit::Highlight( const s_bool& bHighlighted )
    {
        if (bHighlighted != bHighlighted_)
        {
            bHighlighted_ = bHighlighted;
            if (bHighlighted_)
            {
                pBodyModel_->GetMaterial()->SetSelfIllumination(Color(128, 128, 128));
            }
            else
            {
                pBodyModel_->GetMaterial()->SetSelfIllumination(Color(0, 0, 0));
            }
        }
    }

    const s_bool& Unit::IsHighlighted() const
    {
        return bHighlighted_;
    }

    void Unit::NotifySelected( const s_bool& bSelected )
    {
        if (bSelected != bSelected_)
        {
            bSelected_ = bSelected;
            if (bSelected_)
                ZoneManager::GetSingleton()->AddDecalOnGround(pSelectionDecal_);
            else
                ZoneManager::GetSingleton()->RemoveDecalFromGround(pSelectionDecal_);
        }
    }

    const s_bool& Unit::IsSelected() const
    {
        return bSelected_;
    }

    void Unit::Teleport( const Vector& mDestination )
    {
        pNode_->SetPosition(mDestination);
    }

    void Unit::LookAtUnit( s_ptr<Unit> pUnit )
    {
        if (pUnit)
            pNode_->LookAt(pUnit->GetNode());
        else
            pNode_->UnlockTracking();
    }

    void Unit::LookAtObject( s_ptr<MovableObject> pObject )
    {
        if (pObject)
            pNode_->LookAt(pObject);
        else
            pNode_->UnlockTracking();
    }

    void Unit::LookAt( const Vector& mPosition )
    {
        pNode_->SetDirection(mPosition - pNode_->GetPosition(false));
    }

    s_wptr<Model> Unit::GetBodyModel()
    {
        return pBodyModel_;
    }

    s_ptr<Node> Unit::GetNode()
    {
        return pNode_;
    }

    s_ptr<Camera> Unit::GetCamera()
    {
        return pCamera_;
    }

    void Unit::RotateModel( const s_float& fYaw, const s_float& fPitch )
    {
        if (bCameraMovedAlone_)
        {
            Vector mDirection = pCamera_->GetDirection(false);
            mDirection.Y() = 0.0f;
            pNode_->SetDirection(mDirection);
            bCameraMovedAlone_ = false;
        }

        pNode_->Yaw(fYaw);
        pCamera_->Yaw(fYaw);
        pCamera_->Pitch(fPitch);
    }

    void Unit::RotateCamera( const s_float& fYaw, const s_float& fPitch )
    {
        pCamera_->Yaw(fYaw);
        pCamera_->Pitch(fPitch);

        bCameraMovedAlone_ = true;
    }

    void Unit::ZoomCamera( const s_float& fZoom )
    {
        pCamera_->Translate(Vector::UNIT_Z*fZoom, true);
    }

    Vector Unit::GetPosition() const
    {
        return pNode_->GetPosition(false);
    }

    void Unit::EnableMotionBlur()
    {
        if (!bMotionBlurEnabled_ && ShaderManager::GetSingleton()->IsMotionBlurEnabled())
        {
            bMotionBlurEnabled_ = true;
            pBodyModel_->SetCustomShaderParameter(0, Ogre::Vector4(1,1,1,1));
        }
    }

    void Unit::DisableMotionBlur()
    {
        if (bMotionBlurEnabled_ && ShaderManager::GetSingleton()->IsMotionBlurEnabled())
        {
            bMotionBlurEnabled_ = false;
            pBodyModel_->SetCustomShaderParameter(0, Ogre::Vector4(0,0,0,0));
        }
    }

    const s_bool& Unit::IsMotionBlurEnabled() const
    {
        return bMotionBlurEnabled_;
    }

    const s_str& Unit::GetName() const
    {
        return sName_;
    }

    const s_uint& Unit::GetID() const
    {
        return uiID_;
    }

    s_str Unit::GetLuaID() const
    {
        return "U_"+uiID_;
    }

    void Unit::PushOnLua( s_ptr<Lua::State> pLua ) const
    {
        pLua->PushGlobal(GetLuaID());
        pLua->SetGlobal("unit");
        pLua->PushNil();
        pLua->SetGlobal("character");
        pLua->PushNil();
        pLua->SetGlobal("creature");
    }

    void Unit::CreateGlue( s_ptr<Lua::State> pLua )
    {
        pLua->PushNumber(GetID());
        lGlueList_.PushBack(pLua->PushNew<LuaUnit>());
        pLua->SetGlobal(GetLuaID());
    }

    void Unit::OnEvent(const Event& mEvent)
    {
    }

    void Unit::Update( const s_float& fDelta )
    {
        if (pHealthType_)
        {
            pHealthType_->Update(fDelta);
        }
        if (pPowerType_)
        {
            pPowerType_->Update(fDelta);
        }

        if (pBodyModel_)
            pBodyModel_->Update(fDelta);
    }
}
