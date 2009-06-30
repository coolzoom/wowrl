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
#include "scene/frost_physicsmanager.h"
#include "scene/frost_physicshandler.h"

using namespace std;

#define GRAVITY 9.81f

namespace Frost
{
    MovableUnit::MovableUnit( const s_uint& uiID, const s_str& sName ) : Unit(uiID, sName),
        bTurn_(true), fJumpDuration_(0.9f),
        fForwardRunSpeed_(6.5f), fForwardWalkSpeed_(2.5f),
        fBackwardRunSpeed_(4.5f), fBackwardWalkSpeed_(2.5f), fTurnRate_(0.385f)
    {
        pHandler_ = PhysicsManager::GetSingleton()->CreateHandler(pNode_);
    }

    MovableUnit::~MovableUnit()
    {
        PhysicsManager::GetSingleton()->DeleteHandler(pHandler_);
    }

    void MovableUnit::Jump()
    {
        if (!bJumping_)
        {
            bJumping_ = true;

            // Initial jump speed
            //fJumpVSpeed_ = GRAVITY*fJumpDuration_/2.0f;

            // Cumulated jump height (reaches zero when the unit has touched the ground again)
            // Note : This variable is needed because there is no collision detection for now.
            // Later, it'll be useless.
            //fCumuledJumpHeight_ = 0.0f;

            // The maximum jump height is :
            // fMaxJumpHeight = GRAVITY*(fJumpDuration_^2)/8.0f;
            // ... (that is 0.99m with the default parameters)
            // and is reached at t=fJumpDuration_/2.0f

            EnablePhysics();
            pHandler_->SetSpeed(Vector(0, 4.4145f, 0));

            pBodyModel_->GetAnimMgr()->SetAnim(ANIM_JUMP_START);
            pBodyModel_->GetAnimMgr()->SetAnim(ANIM_JUMP, ANIM_PRIORITY_BACKGROUND);
        }
    }

    void MovableUnit::SetMoveForward( const s_bool& bMove )
    {
        pNode_->UnlockTracking();

        mMovementDirection_.ZR() += (bMove ? -1.0f : +1.0f);
    }

    void MovableUnit::SetMoveBackward( const s_bool& bMove )
    {
        pNode_->UnlockTracking();

        mMovementDirection_.ZR() += (bMove ? +1.0f : -1.0f);
    }

    void MovableUnit::SetMoveLeft( const s_bool& bMove )
    {
        pNode_->UnlockTracking();

        mMovementDirection_.XR() += (bMove ? -1.0f : +1.0f);
    }

    void MovableUnit::SetMoveRight( const s_bool& bMove )
    {
        pNode_->UnlockTracking();

        mMovementDirection_.XR() += (bMove ? +1.0f : -1.0f);
    }

    void MovableUnit::ToggleTurning()
    {
        bTurn_ = !bTurn_;
    }

    void MovableUnit::SetTurning( const s_bool& bTurn )
    {
        bTurn_ = bTurn;
    }

    void MovableUnit::StopMovement()
    {
        mMovementDirection_ = Vector::ZERO;
    }

    void MovableUnit::ToggleWalking()
    {
        bWalk_ = !bWalk_;
    }

    void MovableUnit::SetWalking( const s_bool& bWalk )
    {
        bWalk_ = bWalk;
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

    void MovableUnit::SetYaw_( const s_float& fNewYaw )
    {
        if (fCumuledYaw_ != fNewYaw)
        {
            pNode_->Yaw(fNewYaw - fCumuledYaw_);
            fCumuledYaw_ = fNewYaw;
        }
    }

    void MovableUnit::UpdateMovement_( const s_float& fDelta )
    {
        pNode_->Update(fDelta);

        Vector mDir = mMovementDirection_;
        Vector mMovement;

        if (!mDir.IsNull())
        {
            mDir.Normalize();

            s_float fMovementSpeed;
            s_float fNewYaw;
            if (mDir.Z() > 0.0f)
            {
                // Moving backward
                mMovement.ZR() = 1.0f;
                if (bWalk_)
                {
                    if (!bJumping_)
                        pBodyModel_->GetAnimMgr()->SetAnim(ANIM_WALKBACKWARDS, ANIM_PRIORITY_BACKGROUND);
                    fMovementSpeed = fBackwardWalkSpeed_;
                }
                else
                {
                    if (!bJumping_)
                        pBodyModel_->GetAnimMgr()->SetAnim(
                            ANIM_WALKBACKWARDS, ANIM_PRIORITY_BACKGROUND,
                            fBackwardRunSpeed_/fBackwardWalkSpeed_
                        );
                    fMovementSpeed = fBackwardRunSpeed_;
                }
                fNewYaw = atan2(-mDir.X(), -mDir.Z()) - 0.5f;
            }
            else
            {
                // Moving forward
                mMovement.ZR() = -1.0f;
                if (bWalk_)
                {
                    if (!bJumping_)
                        pBodyModel_->GetAnimMgr()->SetAnim(ANIM_WALK, ANIM_PRIORITY_BACKGROUND);
                    fMovementSpeed = fForwardWalkSpeed_;
                }
                else
                {
                    if (!bJumping_)
                        pBodyModel_->GetAnimMgr()->SetAnim(ANIM_RUN, ANIM_PRIORITY_BACKGROUND);
                    fMovementSpeed = fForwardRunSpeed_;
                }
                fNewYaw = atan2(-mDir.X(), -mDir.Z());
            }

            SetYaw_(fNewYaw);
            mMovement.ZR() *= fMovementSpeed*fDelta;
        }
        else
        {
            if (!bJumping_)
                pBodyModel_->GetAnimMgr()->SetAnim(ANIM_STAND, ANIM_PRIORITY_BACKGROUND);

            SetYaw_(0);
        }

        /*if (bJumping_)
        {
            // y(t+dt) = y(t) + speed(t)*dt + (acceleration(t)*dt^2)/2.0f
            // dy = y(t+dt) - y(t)
            s_float fDY = fJumpVSpeed_*fDelta - GRAVITY*fDelta*fDelta/2.0f;
            fCumuledJumpHeight_ += fDY;

            // speed(t+dt) = speed(t) + acceleration*dt
            fJumpVSpeed_ -= GRAVITY*fDelta;

            if (fCumuledJumpHeight_ < 0.0f)
            {
                mMovement.Y(fDY - fCumuledJumpHeight_);
                bJumping_ = false;

                if (!bWalk_)
                {
                    if (mMovement.Z() < 0.0f)
                    {
                        pBodyModel_->GetAnimMgr()->SetAnim(ANIM_JUMP_LAND_RUN);
                    }
                    else if (mMovement.Z().IsNull())
                    {
                        pBodyModel_->GetAnimMgr()->SetAnim(ANIM_JUMP_END);
                    }
                }
            }
            else
                mMovement.Y(fDY);
        }*/

        pNode_->Translate(mMovement, true);
    }

    void MovableUnit::EnablePhysics()
    {
        pHandler_->Enable();
    }

    void MovableUnit::DisablePhysics()
    {
        pHandler_->Disable();
    }

    s_bool MovableUnit::IsPhysicsEnabled() const
    {
        return (pHandler_->IsEnabled());
    }

    void MovableUnit::ForceOnGround()
    {

    }

    s_ptr<PhysicsHandler> MovableUnit::GetPhysicsHandler()
    {
        return pHandler_;
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
