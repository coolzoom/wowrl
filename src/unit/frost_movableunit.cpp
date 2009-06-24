/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           MovableUnit source           */
/*                                        */
/*                                        */

#include "unit/frost_movableunit.h"
#include "model/frost_model.h"
#include "scene/frost_node.h"
#include "model/frost_animmanager.h"
#include "unit/frost_healthtype.h"
#include "unit/frost_powertype.h"
#include "camera/frost_camera.h"

using namespace std;

#define JUMP_COEF_1   s_double(-4.0f*fJumpHeight_/(fJumpDuration_*fJumpDuration_))
#define JUMP_COEF_2   s_double(4.0f*fJumpHeight_/fJumpDuration_)

namespace Frost
{
    MovableUnit::MovableUnit( const s_uint& uiID, const s_str& sName ) : Unit(uiID, sName),
        mMovementType_(MOVEMENT_NONE), mJumpMovementType_(MOVEMENT_NONE),
        mLMovementType_(LMOVEMENT_NONE), bTurn_(true), fJumpHeight_(1.4f),
        fJumpDuration_(0.9f), fForwardRunSpeed_(6.5f), fForwardWalkSpeed_(2.5f),
        fBackwardRunSpeed_(4.5f), fBackwardWalkSpeed_(2.5f), fTurnRate_(0.385f)
    {
    }

    MovableUnit::~MovableUnit()
    {
    }

    void MovableUnit::Jump()
    {
        if (!bJumping_)
        {
            bJumping_ = true;
            mJumpTimer_.Start(true);
            if ((mLMovementType_ == LMOVEMENT_STRAFE_LEFT) ||
                (mLMovementType_ == LMOVEMENT_STRAFE_RIGHT))
                mJumpMovementType_ = MOVEMENT_FORWARD;
            else
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

    void MovableUnit::SetMovement_( Movement mMovementType )
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
                        pNode_->Yaw(0.125f - fCumuledYaw_);
                        fCumuledYaw_ = 0.125f;
                    }
                    else if (mLMovementType_ == LMOVEMENT_STRAFE_RIGHT)
                    {
                        pNode_->Yaw(-0.125f - fCumuledYaw_);
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
                        pNode_->Yaw(-0.125f - fCumuledYaw_);
                        fCumuledYaw_ = -0.125f;
                    }
                    else if (mLMovementType_ == LMOVEMENT_STRAFE_RIGHT)
                    {
                        pNode_->Yaw(-0.125f - fCumuledYaw_);
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
                            pNode_->Yaw(0.25f - fCumuledYaw_);
                            fCumuledYaw_ = 0.25f;
                        }
                        else
                        {
                            pNode_->Yaw(-0.25f - fCumuledYaw_);
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

    void MovableUnit::SetMoveForward( const s_bool& bMoveForward )
    {
        pNode_->UnlockTracking();
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

    void MovableUnit::SetMoveBackward( const s_bool& bMoveBackward )
    {
        pNode_->UnlockTracking();
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

    void MovableUnit::SetLateralMovement_( LateralMovement mLMovementType )
    {
        pNode_->UnlockTracking();
        mLMovementType_ = mLMovementType;
        switch (mLMovementType)
        {
            case LMOVEMENT_STRAFE_LEFT :
            {
                if (mMovementType_ == MOVEMENT_NONE)
                {
                    pNode_->Yaw(0.25f - fCumuledYaw_);
                    fCumuledYaw_ = 0.25f;
                    if (bWalk_)
                        pBodyModel_->GetAnimMgr()->SetAnim(ANIM_WALK, ANIM_PRIORITY_BACKGROUND);
                    else
                        pBodyModel_->GetAnimMgr()->SetAnim(ANIM_RUN, ANIM_PRIORITY_BACKGROUND);
                }
                else if (mMovementType_ == MOVEMENT_FORWARD)
                {
                    pNode_->Yaw(0.125f - fCumuledYaw_);
                    fCumuledYaw_ = 0.125f;
                }
                else if (mMovementType_ == MOVEMENT_BACKWARD)
                {
                    pNode_->Yaw(-0.125f - fCumuledYaw_);
                    fCumuledYaw_ = -0.125f;
                }
                break;
            }
            case LMOVEMENT_STRAFE_RIGHT :
            {
                if (mMovementType_ == MOVEMENT_NONE)
                {
                    pNode_->Yaw(-0.25f - fCumuledYaw_);
                    fCumuledYaw_ = -0.25f;
                    if (bWalk_)
                        pBodyModel_->GetAnimMgr()->SetAnim(ANIM_WALK, ANIM_PRIORITY_BACKGROUND);
                    else
                        pBodyModel_->GetAnimMgr()->SetAnim(ANIM_RUN, ANIM_PRIORITY_BACKGROUND);
                }
                else if (mMovementType_ == MOVEMENT_FORWARD)
                {
                    pNode_->Yaw(-0.125f - fCumuledYaw_);
                    fCumuledYaw_ = -0.125f;
                }
                else if (mMovementType_ == MOVEMENT_BACKWARD)
                {
                    pNode_->Yaw(0.125f - fCumuledYaw_);
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

    void MovableUnit::SetMoveLeft( const s_bool& bMoveLeft )
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

    void MovableUnit::SetMoveRight( const s_bool& bMoveRight )
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

    void MovableUnit::ToggleTurning()
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

    void MovableUnit::SetTurning( const s_bool& bTurn )
    {
        if (bTurn_ != bTurn)
            ToggleTurning();
    }

    void MovableUnit::StopMovement()
    {
        bMoveForward_ = bMoveBackward_ = bStrafeLeft_ = bStrafeRight_ = bTurnLeft_ = bTurnRight_ = false;
        SetMovement_(MOVEMENT_NONE);
        SetLateralMovement_(LMOVEMENT_NONE);
    }

    void MovableUnit::ToggleWalking()
    {
        bWalk_ = !bWalk_;
        SetMovement_(mMovementType_);
    }

    void MovableUnit::SetWalking( const s_bool& bWalk )
    {
        bWalk_ = bWalk;
        SetMovement_(mMovementType_);
    }

    void MovableUnit::Update( const s_float& fDelta )
    {
        if (pHealthType_)
        {
            pHealthType_->Update(fDelta);
        }
        if (pPowerType_)
        {
            pPowerType_->Update(fDelta);
        }

        UpdateMovement_(fDelta);

        if (pBodyModel_)
            pBodyModel_->Update(fDelta);
    }

    void MovableUnit::UpdateMovement_( const s_float fDelta )
    {
        pNode_->Update(fDelta);

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
                                mJumpHMovement_ += mJumpDirection_*fForwardWalkSpeed_*fDelta;
                            else
                                mJumpHMovement_ += mJumpDirection_*fForwardRunSpeed_*fDelta;

                            break;
                        }
                        case MOVEMENT_BACKWARD :
                        {
                            if (bWalk_)
                                mJumpHMovement_ -= mJumpDirection_*fForwardWalkSpeed_*fDelta;
                            else
                                mJumpHMovement_ -= mJumpDirection_*fForwardRunSpeed_*fDelta;

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
                s_float fAngle = fTurnRate_*fDelta;
                pNode_->Yaw(fAngle);
                pCamera_->Yaw(fAngle);
                break;
            }
            case LMOVEMENT_TURN_RIGHT :
            {
                s_float fAngle = -fTurnRate_*fDelta;
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
                                -fForwardWalkSpeed_*fDelta
                            ),
                            true
                        );
                    else
                        pNode_->Translate(
                            Vector(
                                0.0f,
                                0.0f,
                                -fForwardRunSpeed_*fDelta
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
                                -fForwardWalkSpeed_*fDelta
                            ),
                            true
                        );
                    else
                        pNode_->Translate(
                            Vector(
                                0.0f,
                                0.0f,
                                -fForwardRunSpeed_*fDelta
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
                            -fForwardWalkSpeed_*fDelta
                        ),
                        true
                    );
                else
                    pNode_->Translate(
                        Vector(
                            0.0f,
                            0.0f,
                            -fForwardRunSpeed_*fDelta
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
                            fBackwardWalkSpeed_*fDelta
                        ),
                        true
                    );
                else
                    pNode_->Translate(
                        Vector(
                            0.0f,
                            0.0f,
                            fBackwardRunSpeed_*fDelta
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

    void MovableUnit::CreateGlue( s_ptr<Lua::State> pLua )
    {
        pLua->PushNumber(GetID());
        lGlueList_.PushBack(
            pLua->Push<LuaMovableUnit>(new LuaMovableUnit(pLua->GetState()))
        );
        pLua->SetGlobal(GetLuaID());
    }
}
