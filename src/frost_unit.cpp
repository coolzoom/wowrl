/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Unit source              */
/*                                        */
/*                                        */

#include "frost_unit.h"

#include "frost_modelmanager.h"
#include "frost_model.h"
#include "frost_animmanager.h"
#include "frost_scenemanager.h"
#include "frost_node.h"
#include "camera/frost_cameramanager.h"
#include "camera/frost_camera.h"
#include "frost_unitmanager.h"
#include "frost_healthtype.h"
#include "frost_powertype.h"
#include "frost_material.h"

using namespace std;

#define JUMP_COEF_1   s_double(-4.0*fJumpHeight_/(fJumpDuration_*fJumpDuration_))
#define JUMP_COEF_2   s_double(4.0*fJumpHeight_/fJumpDuration_)

namespace Frost
{
    const s_str Unit::CLASS_NAME = "Unit";

    // TODO : Unit : Implémenter les sors
    // TODO : Unit : Implémenter le mouvement (+pathfinding)
    // TODO : Unit : Implémenter la mort
    // TODO : Unit : Implémenter les buffs

    Unit::Unit( const s_uint& uiID, const s_str& sName ) :
        uiID_(uiID), sName_(sName), uiLevel_(s_uint::NaN),
        mMovementType_(MOVEMENT_NONE), mJumpMovementType_(MOVEMENT_NONE),
        mLMovementType_(LMOVEMENT_NONE), bTurn_(true), fJumpHeight_(1.4f),
        fJumpDuration_(0.9f), fForwardRunSpeed_(6.5f), fForwardWalkSpeed_(2.5f),
        fBackwardRunSpeed_(4.5f), fBackwardWalkSpeed_(2.5f), fTurnRate_(0.385f)
    {
        pNode_ = SceneManager::GetSingleton()->CreateNode();
        pCamera_ = s_ptr<Camera>(CameraManager::GetSingleton()->CreateChasingCamera(this));
    }

    Unit::~Unit()
    {
        CameraManager::GetSingleton()->DeleteCamera(pCamera_);
        SceneManager::GetSingleton()->DeleteNode(pNode_);
        pGlue_.Delete();
    }

    void Unit::SetClass(const s_str& sClassName)
    {
        mClass_ = *UnitManager::GetSingleton()->GetClassByName(sClassName);
        pHealthType_ = s_refptr<HealthTypeInstance>(new HealthTypeInstance(this, mClass_.sHealthType));
        pPowerType_ = s_refptr<PowerTypeInstance>(new PowerTypeInstance(this, mClass_.sPowerType));
    }

    void Unit::SetClass(const Class& mClass)
    {
        mClass_ = mClass;
        pHealthType_ = s_refptr<HealthTypeInstance>(new HealthTypeInstance(this, mClass_.sHealthType));
        pPowerType_ = s_refptr<PowerTypeInstance>(new PowerTypeInstance(this, mClass_.sPowerType));
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
            fHealth_ = fMaxHealth;
        if (fHealth_ < 0.0f)
            fHealth_ = 0.0f;
    }

    void Unit::FillHealthGauge()
    {
        fHealth_ = mStats_.fMaxHealth.GetValue();
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
            fPower_ = fMaxPower;
        if (fPower_ < 0.0f)
            fPower_ = 0.0f;
    }

    void Unit::FillPowerGauge()
    {
        fPower_ = mStats_.fMaxPower.GetValue();
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
        map< s_str, Characteristic<s_var> >::const_iterator iter = mStats_.lCharactList.find(sStatName);
        if (iter != mStats_.lCharactList.end())
        {
            s_var vReturn;
            if (iter->second.IsInteger())
                vReturn.SetI(iter->second.GetValueI());
            else
                vReturn.SetF(iter->second.GetValueF());

            return vReturn;
        }
        else
            return s_var(NULL);
    }

    void Unit::SetLevel( const s_uint& uiLevel )
    {
        uiLevel_ = uiLevel;
    }

    const s_uint& Unit::GetLevel() const
    {
        return uiLevel_;
    }

    void Unit::NotifySelected( const s_bool& bSelected )
    {
        if (bSelected != bSelected_)
        {
            bSelected_ = bSelected;
            Log(sName_+" is "+bSelected_.GetAsString("", "not ")+"selected");
            if (bSelected_)
            {
                pBodyModel_->GetMaterial()->SetSelfIllumination(Color(255, 255, 255));
            }
            else
            {
                pBodyModel_->GetMaterial()->SetSelfIllumination(Color(0, 0, 0));
            }
        }
    }

    const s_bool& Unit::IsSelected() const
    {
        return bSelected_;
    }

    void Unit::Jump()
    {
        if (!bJumping_)
        {
            bJumping_ = true;
            mJumpTimer_.Start(true);
            mJumpMovementType_ = mMovementType_;
            mJumpHMovement_ = Vector::ZERO;

            if (pBodyModel_)
            {
                pBodyModel_->GetAnimMgr()->SetSpeed(1.0f);
                pBodyModel_->GetAnimMgr()->SetAnim(ANIM_JUMP_START);
                pBodyModel_->GetAnimMgr()->SetAnim(ANIM_JUMP, ANIM_PRIORITY_BACKGROUND);
                mJumpPosition_ = pNode_->GetPosition();
                mJumpDirection_ = pNode_->GetDirection();
            }
        }
    }

    void Unit::SetMovement_( Movement mMovementType )
    {
        mMovementType_ = mMovementType;
        if (!bJumping_)
        {
            switch (mMovementType)
            {
                case MOVEMENT_FORWARD :
                {
                    if (mLMovementType_ == LMOVEMENT_STRAFE_LEFT)
                    {
                        pNode_->Yaw(0.125f-fCumuledYaw_);
                        fCumuledYaw_ = 0.125f;
                    }
                    else if (mLMovementType_ == LMOVEMENT_STRAFE_RIGHT)
                    {
                        pNode_->Yaw(-0.125f-fCumuledYaw_);
                        fCumuledYaw_ = -0.125f;
                    }
                    else
                    {
                        pBodyModel_->GetAnimMgr()->SetSpeed(1.0f);
                        if (bWalk_)
                            pBodyModel_->GetAnimMgr()->SetAnim(ANIM_WALK, ANIM_PRIORITY_BACKGROUND);
                        else
                            pBodyModel_->GetAnimMgr()->SetAnim(ANIM_RUN, ANIM_PRIORITY_BACKGROUND);
                    }

                    break;
                }
                case MOVEMENT_BACKWARD :
                {
                    if (mLMovementType_ == LMOVEMENT_STRAFE_LEFT)
                    {
                        pNode_->Yaw(-0.125f-fCumuledYaw_);
                        fCumuledYaw_ = -0.125f;
                    }
                    else if (mLMovementType_ == LMOVEMENT_STRAFE_RIGHT)
                    {
                        pNode_->Yaw(-0.125f-fCumuledYaw_);
                        fCumuledYaw_ = 0.125f;
                    }

                    if (bWalk_)
                    {
                        pBodyModel_->GetAnimMgr()->SetSpeed(1.0f);
                    }
                    else
                    {
                        pBodyModel_->GetAnimMgr()->SetSpeed(fBackwardRunSpeed_/fBackwardWalkSpeed_);
                    }
                    pBodyModel_->GetAnimMgr()->SetAnim(ANIM_WALKBACKWARDS, ANIM_PRIORITY_BACKGROUND);

                    break;
                }
                case MOVEMENT_NONE :
                {
                    if ((mLMovementType_ == LMOVEMENT_STRAFE_LEFT) ||
                        (mLMovementType_ == LMOVEMENT_STRAFE_RIGHT))
                    {
                        if (mLMovementType_ == LMOVEMENT_STRAFE_LEFT)
                        {
                            pNode_->Yaw(0.25f-fCumuledYaw_);
                            fCumuledYaw_ = 0.25f;
                        }
                        else
                        {
                            pNode_->Yaw(-0.25f-fCumuledYaw_);
                            fCumuledYaw_ = -0.25f;
                        }
                        pBodyModel_->GetAnimMgr()->SetSpeed(1.0f);
                        if (bWalk_)
                            pBodyModel_->GetAnimMgr()->SetAnim(ANIM_WALK, ANIM_PRIORITY_BACKGROUND);
                        else
                            pBodyModel_->GetAnimMgr()->SetAnim(ANIM_RUN, ANIM_PRIORITY_BACKGROUND);
                    }
                    else
                    {
                        pBodyModel_->GetAnimMgr()->SetSpeed(1.0f);
                        pBodyModel_->GetAnimMgr()->SetAnim(ANIM_STAND, ANIM_PRIORITY_BACKGROUND);
                    }

                    break;
                }
            }
        }
    }

    void Unit::SetMoveForward( const s_bool& bMoveForward )
    {
        if (bMoveForward)
        {
            bMoveForward_ = bMoveForward;
            if (mMovementType_ != MOVEMENT_BACKWARD)
            {
                SetMovement_(MOVEMENT_FORWARD);
            }
            else
            {
                SetMovement_(MOVEMENT_NONE);
            }
        }
        else
        {
            if ( bMoveForward_ && (mMovementType_ == MOVEMENT_NONE) )
            {
                SetMovement_(MOVEMENT_BACKWARD);
            }
            else
            {
                SetMovement_(MOVEMENT_NONE);
            }
            bMoveForward_ = bMoveForward;
        }
    }

    void Unit::SetMoveBackward( const s_bool& bMoveBackward )
    {
        if (bMoveBackward)
        {
            bMoveBackward_ = bMoveBackward;
            if (mMovementType_ != MOVEMENT_FORWARD)
            {
                SetMovement_(MOVEMENT_BACKWARD);
            }
            else
            {
                SetMovement_(MOVEMENT_NONE);
            }
        }
        else
        {
            if ( bMoveBackward_ && (mMovementType_ == MOVEMENT_NONE) )
            {
                SetMovement_(MOVEMENT_FORWARD);
            }
            else
            {
                SetMovement_(MOVEMENT_NONE);
            }
            bMoveBackward_ = bMoveBackward;
        }
    }

    void Unit::SetLateralMovement_( LateralMovement mLMovementType )
    {
        mLMovementType_ = mLMovementType;
        switch (mLMovementType)
        {
            case LMOVEMENT_STRAFE_LEFT :
            {
                if (mMovementType_ == MOVEMENT_NONE)
                {
                    pNode_->Yaw(0.25f-fCumuledYaw_);
                    fCumuledYaw_ = 0.25f;
                    if (bWalk_)
                        pBodyModel_->GetAnimMgr()->SetAnim(ANIM_WALK, ANIM_PRIORITY_BACKGROUND);
                    else
                        pBodyModel_->GetAnimMgr()->SetAnim(ANIM_RUN, ANIM_PRIORITY_BACKGROUND);
                }
                else if (mMovementType_ == MOVEMENT_FORWARD)
                {
                    pNode_->Yaw(0.125f-fCumuledYaw_);
                    fCumuledYaw_ = 0.125f;
                }
                else if (mMovementType_ == MOVEMENT_BACKWARD)
                {
                    pNode_->Yaw(-0.125f-fCumuledYaw_);
                    fCumuledYaw_ = -0.125f;
                }
                break;
            }
            case LMOVEMENT_STRAFE_RIGHT :
            {
                if (mMovementType_ == MOVEMENT_NONE)
                {
                    pNode_->Yaw(-0.25f-fCumuledYaw_);
                    fCumuledYaw_ = -0.25f;
                    if (bWalk_)
                        pBodyModel_->GetAnimMgr()->SetAnim(ANIM_WALK, ANIM_PRIORITY_BACKGROUND);
                    else
                        pBodyModel_->GetAnimMgr()->SetAnim(ANIM_RUN, ANIM_PRIORITY_BACKGROUND);
                }
                else if (mMovementType_ == MOVEMENT_FORWARD)
                {
                    pNode_->Yaw(-0.125f-fCumuledYaw_);
                    fCumuledYaw_ = -0.125f;
                }
                else if (mMovementType_ == MOVEMENT_BACKWARD)
                {
                    pNode_->Yaw(0.125f-fCumuledYaw_);
                    fCumuledYaw_ = 0.125f;
                }
                break;
            }
            case LMOVEMENT_TURN_LEFT :
            {
                if (mMovementType_ == MOVEMENT_NONE)
                {
                    pBodyModel_->GetAnimMgr()->SetAnim(ANIM_SHUFFLE_LEFT, ANIM_PRIORITY_BACKGROUND);
                }
                break;
            }
            case LMOVEMENT_TURN_RIGHT :
            {
                if (mMovementType_ == MOVEMENT_NONE)
                {
                    pBodyModel_->GetAnimMgr()->SetAnim(ANIM_SHUFFLE_RIGHT, ANIM_PRIORITY_BACKGROUND);
                }
                break;
            }
            case LMOVEMENT_NONE :
            {
                pNode_->Yaw(-fCumuledYaw_);
                fCumuledYaw_ = 0.0f;
                if (mMovementType_ == MOVEMENT_NONE)
                {
                    pBodyModel_->GetAnimMgr()->SetAnim(ANIM_STAND, ANIM_PRIORITY_BACKGROUND);
                }
                break;
            }
        }
    }

    void Unit::SetMoveLeft( const s_bool& bMoveLeft )
    {
        if (bTurn_)
        {
            bStrafeLeft_ = bStrafeRight_ = false;
            if (bMoveLeft)
            {
                bTurnLeft_ = bMoveLeft;
                if (mLMovementType_ != LMOVEMENT_TURN_RIGHT)
                {
                    SetLateralMovement_(LMOVEMENT_TURN_LEFT);
                }
                else
                {
                    SetLateralMovement_(LMOVEMENT_NONE);
                }
            }
            else
            {
                if ( bTurnLeft_ && (mLMovementType_ == LMOVEMENT_NONE) )
                {
                    SetLateralMovement_(LMOVEMENT_TURN_RIGHT);
                }
                else
                {
                    SetLateralMovement_(LMOVEMENT_NONE);
                }
                bTurnLeft_ = bMoveLeft;
            }
        }
        else
        {
            bTurnLeft_ = bTurnRight_ = false;
            if (bMoveLeft)
            {
                bStrafeLeft_ = bMoveLeft;
                if (mLMovementType_ != LMOVEMENT_STRAFE_RIGHT)
                {
                    SetLateralMovement_(LMOVEMENT_STRAFE_LEFT);
                }
                else
                {
                    SetLateralMovement_(LMOVEMENT_NONE);
                }
            }
            else
            {
                if ( bStrafeLeft_ && (mLMovementType_ == LMOVEMENT_NONE) )
                {
                    SetLateralMovement_(LMOVEMENT_STRAFE_RIGHT);
                }
                else
                {
                    SetLateralMovement_(LMOVEMENT_NONE);
                }
                bStrafeLeft_ = bMoveLeft;
            }
        }
    }

    void Unit::SetMoveRight( const s_bool& bMoveRight )
    {
        if (bTurn_)
        {
            bStrafeLeft_ = bStrafeRight_ = false;
            if (bMoveRight)
            {
                bTurnRight_ = bMoveRight;
                if (mLMovementType_ != LMOVEMENT_TURN_LEFT)
                {
                    SetLateralMovement_(LMOVEMENT_TURN_RIGHT);
                }
                else
                {
                    SetLateralMovement_(LMOVEMENT_NONE);
                }
            }
            else
            {
                if ( bTurnRight_ && (mLMovementType_ == LMOVEMENT_NONE) )
                {
                    SetLateralMovement_(LMOVEMENT_TURN_LEFT);
                }
                else
                {
                    SetLateralMovement_(LMOVEMENT_NONE);
                }
                bTurnRight_ = bMoveRight;
            }
        }
        else
        {
            bTurnLeft_ = bTurnRight_ = false;
            if (bMoveRight)
            {
                bStrafeRight_ = bMoveRight;
                if (mLMovementType_ != LMOVEMENT_STRAFE_LEFT)
                {
                    SetLateralMovement_(LMOVEMENT_STRAFE_RIGHT);
                }
                else
                {
                    SetLateralMovement_(LMOVEMENT_NONE);
                }
            }
            else
            {
                if ( bStrafeRight_ && (mLMovementType_ == LMOVEMENT_NONE) )
                {
                    SetLateralMovement_(LMOVEMENT_STRAFE_LEFT);
                }
                else
                {
                    SetLateralMovement_(LMOVEMENT_NONE);
                }
                bStrafeRight_ = bMoveRight;
            }
        }
    }

    void Unit::ToggleTurning()
    {
        bTurn_ = !bTurn_;
        fCumuledYaw_ = 0.0f;
        switch (mLMovementType_)
        {
            case LMOVEMENT_STRAFE_LEFT :
            {
                bTurnLeft_ = true;
                bStrafeLeft_ = false;
                SetLateralMovement_(LMOVEMENT_TURN_LEFT);
                break;
            }
            case LMOVEMENT_STRAFE_RIGHT :
            {
                bTurnRight_ = true;
                bStrafeRight_ = false;
                SetLateralMovement_(LMOVEMENT_TURN_RIGHT);
                break;
            }
            case LMOVEMENT_TURN_LEFT :
            {
                bStrafeLeft_ = true;
                bTurnLeft_ = false;
                SetLateralMovement_(LMOVEMENT_STRAFE_LEFT);
                break;
            }
            case LMOVEMENT_TURN_RIGHT :
            {
                bStrafeRight_ = true;
                bTurnRight_ = false;
                SetLateralMovement_(LMOVEMENT_STRAFE_RIGHT);
                break;
            }
            case LMOVEMENT_NONE :
            {
                if (bStrafeLeft_ || bStrafeRight_)
                {
                    bTurnLeft_ = bTurnRight_ = true;
                    bStrafeLeft_ = bStrafeRight_ = false;
                }
                else if (bTurnLeft_ || bTurnRight_)
                {
                    bStrafeLeft_ = bStrafeRight_ = false;
                    bTurnLeft_ = bTurnRight_ = true;
                }

                break;
            }
        }
    }

    void Unit::SetTurning( const s_bool& bTurn )
    {
        //if ( (bTurn_ && !bTurn) || (!bTurn_ && bTurn) )
        if (bTurn_ != bTurn)
            ToggleTurning();
    }

    void Unit::StopMovement()
    {
        bMoveForward_ = bMoveBackward_ = bStrafeLeft_ = bStrafeRight_ = bTurnLeft_ = bTurnRight_ = false;
        SetMovement_(MOVEMENT_NONE);
        SetLateralMovement_(LMOVEMENT_NONE);
    }

    void Unit::ToggleWalking()
    {
        bWalk_ = !bWalk_;
        SetMovement_(mMovementType_);
    }

    void Unit::SetWalking( const s_bool& bWalk )
    {
        bWalk_ = bWalk;
        SetMovement_(mMovementType_);
    }

    s_refptr<Model> Unit::GetBodyModel()
    {
        return pBodyModel_;
    }

    s_ptr<Node> Unit::GetNode()
    {
        return pNode_;
    }

    void Unit::ZoomCamera( const s_float& fZoom )
    {
        pCamera_->Translate(Vector::UNIT_Z*fZoom, true);
    }

    void Unit::RotateCamera( const s_float& fYaw, const s_float& fPitch )
    {
        pCamera_->Yaw(fYaw);
        pCamera_->Pitch(fPitch);

        bCameraMovedAlone_ = true;
    }

    void Unit::RotateModel( const s_float& fYaw, const s_float& fPitch )
    {
        if (bCameraMovedAlone_)
        {
            Vector mDirection = pCamera_->GetDirection(false);
            mDirection.Y(0.0f);
            pNode_->SetDirection(mDirection);
            bCameraMovedAlone_ = false;
        }

        pNode_->Yaw(fYaw);
        pCamera_->Yaw(fYaw);
        pCamera_->Pitch(fPitch);
    }

    Vector Unit::GetPosition() const
    {
        return pNode_->GetPosition(false);
    }

    s_ptr<Camera> Unit::GetCamera()
    {
        return pCamera_;
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

    void Unit::CreateGlue()
    {
        s_ptr<Lua::State> pLua = UnitManager::GetSingleton()->GetLua();
        pLua->PushNumber(GetID());
        LuaUnit* pNewGlue;
        pGlue_ = pNewGlue = new LuaUnit(pLua->GetState());
        Lunar<LuaUnit>::push(pLua->GetState(), pNewGlue);
        pLua->SetGlobal(GetLuaID());
    }

    void Unit::OnEvent(const Event& mEvent)
    {

    }

    void Unit::Update( const s_float& fDelta )
    {
        if (pHealthType_)
            pHealthType_->Update(fDelta);
        if (pPowerType_)
            pPowerType_->Update(fDelta);

        UpdateMovement_(fDelta);

        if (pBodyModel_)
            pBodyModel_->Update(fDelta);
    }

    void Unit::UpdateMovement_( const s_float fDelta )
    {
        if (bJumping_)
        {
            if (pBodyModel_)
            {
                if (mJumpTimer_.GetElapsed() > s_double(fJumpDuration_))
                {
                    bJumping_ = false;
                    pNode_->SetPosition(mJumpPosition_+mJumpHMovement_);
                    switch (mMovementType_)
                    {
                        case MOVEMENT_FORWARD :
                        {
                            if (bWalk_)
                                pBodyModel_->GetAnimMgr()->SetAnim(ANIM_WALK, ANIM_PRIORITY_BACKGROUND);
                            else
                            {
                                pBodyModel_->GetAnimMgr()->SetAnim(ANIM_JUMP_LAND_RUN);
                                pBodyModel_->GetAnimMgr()->SetAnim(ANIM_RUN, ANIM_PRIORITY_BACKGROUND);
                            }
                            break;
                        }
                        case MOVEMENT_BACKWARD :
                        {
                            if (!bWalk_)
                                pBodyModel_->GetAnimMgr()->SetSpeed(fBackwardRunSpeed_/fBackwardWalkSpeed_);
                            pBodyModel_->GetAnimMgr()->SetAnim(ANIM_WALKBACKWARDS, ANIM_PRIORITY_BACKGROUND);
                            break;
                        }
                        case MOVEMENT_NONE :
                        {
                            switch (mLMovementType_)
                            {
                                case LMOVEMENT_TURN_LEFT :
                                {
                                    pBodyModel_->GetAnimMgr()->SetAnim(ANIM_JUMP_END);
                                    pBodyModel_->GetAnimMgr()->SetAnim(ANIM_SHUFFLE_LEFT, ANIM_PRIORITY_BACKGROUND);
                                    break;
                                }
                                case LMOVEMENT_TURN_RIGHT :
                                {
                                    pBodyModel_->GetAnimMgr()->SetAnim(ANIM_JUMP_END);
                                    pBodyModel_->GetAnimMgr()->SetAnim(ANIM_SHUFFLE_RIGHT, ANIM_PRIORITY_BACKGROUND);
                                    break;
                                }
                                case LMOVEMENT_STRAFE_LEFT :
                                case LMOVEMENT_STRAFE_RIGHT :
                                {
                                    if (bWalk_)
                                        pBodyModel_->GetAnimMgr()->SetAnim(ANIM_WALK, ANIM_PRIORITY_BACKGROUND);
                                    else
                                    {
                                        pBodyModel_->GetAnimMgr()->SetAnim(ANIM_JUMP_LAND_RUN);
                                        pBodyModel_->GetAnimMgr()->SetAnim(ANIM_RUN, ANIM_PRIORITY_BACKGROUND);
                                    }
                                    break;
                                }
                                case LMOVEMENT_NONE :
                                {
                                    pBodyModel_->GetAnimMgr()->SetAnim(ANIM_JUMP_END);
                                    pBodyModel_->GetAnimMgr()->SetAnim(ANIM_STAND, ANIM_PRIORITY_BACKGROUND);
                                    break;
                                }
                            }
                            break;

                        }
                    }
                }
                else
                {
                    switch (mJumpMovementType_)
                    {
                        case MOVEMENT_FORWARD :
                        {
                            if (bWalk_)
                                mJumpHMovement_ += mJumpDirection_ *
                                                   s_float(s_double(fForwardWalkSpeed_) *
                                                   TimeManager::GetSingleton()->GetDelta());
                            else
                                mJumpHMovement_ += mJumpDirection_ *
                                                   s_float(s_double(fForwardRunSpeed_) *
                                                   TimeManager::GetSingleton()->GetDelta());

                            break;
                        }
                        case MOVEMENT_BACKWARD :
                        {
                            if (bWalk_)
                                mJumpHMovement_ -= mJumpDirection_ *
                                                   s_float(s_double(fBackwardWalkSpeed_) *
                                                   TimeManager::GetSingleton()->GetDelta());
                            else
                                mJumpHMovement_ -= mJumpDirection_ *
                                                   s_float(s_double(fBackwardRunSpeed_) *
                                                   TimeManager::GetSingleton()->GetDelta());

                            break;
                        }
                        case MOVEMENT_NONE :
                        {
                            break;
                        }
                    }

                    pNode_->SetPosition(Vector(
                        mJumpPosition_.X() + mJumpHMovement_.X(),
                        mJumpPosition_.Y() +
                        s_float(JUMP_COEF_1*mJumpTimer_.GetElapsed()*mJumpTimer_.GetElapsed() +
                        JUMP_COEF_2*mJumpTimer_.GetElapsed()),
                        mJumpPosition_.Z() + mJumpHMovement_.Z()
                    ));
                }
            }
        }

        switch (mLMovementType_)
        {
            case LMOVEMENT_TURN_LEFT :
            {
                s_float fAngle = s_float(s_double(fTurnRate_)*TimeManager::GetSingleton()->GetDelta());
                pNode_->Yaw(fAngle);
                pCamera_->Yaw(fAngle);
                break;
            }
            case LMOVEMENT_TURN_RIGHT :
            {
                s_float fAngle = s_float(s_double(-fTurnRate_)*TimeManager::GetSingleton()->GetDelta());
                pNode_->Yaw(fAngle);
                pCamera_->Yaw(fAngle);
                break;
            }
            case LMOVEMENT_STRAFE_LEFT :
            {
                if (mMovementType_ == MOVEMENT_NONE)
                {
                    if (bWalk_)
                        pNode_->Translate(
                            Vector(
                                0.0f,
                                0.0f,
                                -s_float(s_double(fForwardWalkSpeed_) *
                                TimeManager::GetSingleton()->GetDelta())
                            ),
                            true
                        );
                    else
                        pNode_->Translate(
                            Vector(
                                0.0f,
                                0.0f,
                                -s_float(s_double(fForwardRunSpeed_) *
                                TimeManager::GetSingleton()->GetDelta())
                            ),
                            true
                        );
                }
                break;
            }
            case LMOVEMENT_STRAFE_RIGHT :
            {
                if (mMovementType_ == MOVEMENT_NONE)
                {
                    if (bWalk_)
                        pNode_->Translate(
                            Vector(
                                0.0f,
                                0.0f,
                                -s_float(s_double(fForwardWalkSpeed_) *
                                TimeManager::GetSingleton()->GetDelta())
                            ),
                            true
                        );
                    else
                        pNode_->Translate(
                            Vector(
                                0.0f,
                                0.0f,
                                -s_float(s_double(fForwardRunSpeed_) *
                                TimeManager::GetSingleton()->GetDelta())
                            ),
                            true
                        );
                }
                break;
            }
            case LMOVEMENT_NONE :
            {
                break;
            }
        }

        switch (mMovementType_)
        {
            case MOVEMENT_FORWARD :
            {
                if (bWalk_)
                    pNode_->Translate(
                        Vector(
                            0.0f,
                            0.0f,
                            -s_float(s_double(fForwardWalkSpeed_) *
                            TimeManager::GetSingleton()->GetDelta())
                        ),
                        true
                    );
                else
                    pNode_->Translate(
                        Vector(
                            0.0f,
                            0.0f,
                            -s_float(s_double(fForwardRunSpeed_) *
                            TimeManager::GetSingleton()->GetDelta())
                        ),
                        true
                    );

                break;
            }
            case MOVEMENT_BACKWARD :
            {
                if (bWalk_)
                    pNode_->Translate(
                        Vector(
                            0.0f,
                            0.0f,
                            s_float(s_double(fBackwardWalkSpeed_) *
                            TimeManager::GetSingleton()->GetDelta())
                        ),
                        true
                    );
                else
                    pNode_->Translate(
                        Vector(
                            0.0f,
                            0.0f,
                            s_float(s_double(fBackwardRunSpeed_) *
                            TimeManager::GetSingleton()->GetDelta())
                        ),
                        true
                    );

                break;
            }
            case MOVEMENT_NONE :
            {
                break;
            }
        }
    }
}
