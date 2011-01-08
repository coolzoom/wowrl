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
#include "unit/frost_movableunithandler.h"

using namespace std;

namespace Frost
{
    MovableUnit::MovableUnit( const s_uint& uiID, const s_str& sName ) : Unit(uiID, sName),
        bTurn_(true), bFalling_(false), fForwardRunSpeed_(6.5f), fForwardWalkSpeed_(2.5f),
        fBackwardRunSpeed_(4.5f), fBackwardWalkSpeed_(2.5f), fTurnRate_(0.385f)
    {
        pHandler_ = new MovableUnitHandler(this);
        PhysicsManager::GetSingleton()->RegisterHandler(pHandler_);
    }

    MovableUnit::~MovableUnit()
    {
        PhysicsManager::GetSingleton()->RemoveHandler(pHandler_);
        pHandler_.Delete();
    }

    void MovableUnit::Jump()
    {
        if (!bFalling_)
        {
            bFalling_ = true;

            pHandler_->SetSpeed(mMovementSpeed_ + Vector(0, 7.2f, 0));

            if (pBodyModel_->HasAnimation())
            {
                pBodyModel_->GetAnimMgr()->SetAnim(ANIM_JUMP_START);
                pBodyModel_->GetAnimMgr()->SetAnim(ANIM_JUMP, ANIM_PRIORITY_BACKGROUND);
            }
        }
    }

    void MovableUnit::SetMoveForward( const s_bool& bMove )
    {
        pNode_->UnlockTracking();

        mMovementDirection_.Z() += (bMove ? -1.0f : +1.0f);
    }

    void MovableUnit::SetMoveBackward( const s_bool& bMove )
    {
        pNode_->UnlockTracking();

        mMovementDirection_.Z() += (bMove ? +1.0f : -1.0f);
    }

    void MovableUnit::SetMoveLeft( const s_bool& bMove )
    {
        pNode_->UnlockTracking();

        mMovementDirection_.X() += (bMove ? -1.0f : +1.0f);
    }

    void MovableUnit::SetMoveRight( const s_bool& bMove )
    {
        pNode_->UnlockTracking();

        mMovementDirection_.X() += (bMove ? +1.0f : -1.0f);
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

    const Vector& MovableUnit::GetMovementSpeed() const
    {
        return mMovementSpeed_;
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
            pHealthType_->Update(fDelta);

        if (pPowerType_)
            pPowerType_->Update(fDelta);

        UpdateMovement_(fDelta);

        if (pBodyModel_)
            pBodyModel_->Update(fDelta);

        bNextFalling_ = true;
    }

    void MovableUnit::OnEvent( const Event& mEvent )
    {
        Unit::OnEvent(mEvent);

        if (mEvent.GetName() == "PHYSICS_COLLISION")
        {
            bFalling_ = false;

            if (pBodyModel_->HasAnimation())
            {
                if (!bWalk_)
                {
                    if (mMovementDirection_.Z().IsNull() && mMovementDirection_.X().IsNull())
                        pBodyModel_->GetAnimMgr()->SetAnim(ANIM_JUMP_END);
                    else if (mMovementDirection_.Z() < 0.0f || !mMovementDirection_.X().IsNull())
                        pBodyModel_->GetAnimMgr()->SetAnim(ANIM_JUMP_LAND_RUN);
                }

                if (mMovementDirection_.Z().IsNull())
                    pBodyModel_->GetAnimMgr()->SetAnim(ANIM_STAND, ANIM_PRIORITY_BACKGROUND);
                else
                {
                    if (mMovementDirection_.Z() < 0.0f)
                    {
                        if (bWalk_)
                            pBodyModel_->GetAnimMgr()->SetAnim(ANIM_WALK, ANIM_PRIORITY_BACKGROUND);
                        else
                            pBodyModel_->GetAnimMgr()->SetAnim(ANIM_RUN, ANIM_PRIORITY_BACKGROUND);
                    }
                    else
                    {
                        if (bWalk_)
                            pBodyModel_->GetAnimMgr()->SetAnim(ANIM_WALKBACKWARDS, ANIM_PRIORITY_BACKGROUND);
                        else
                            pBodyModel_->GetAnimMgr()->SetAnim(ANIM_WALKBACKWARDS, ANIM_PRIORITY_BACKGROUND,
                                fBackwardRunSpeed_/fBackwardWalkSpeed_
                            );
                    }
                }
            }
        }

        if (mEvent.GetName() == "PHYSICS_START_FREE_FALL")
        {
            if (pBodyModel_->HasAnimation())
                pBodyModel_->GetAnimMgr()->SetAnim(ANIM_JUMP, ANIM_PRIORITY_BACKGROUND);

            bFalling_ = true;
        }
    }

    void MovableUnit::SetYaw_( const s_float& fNewYaw )
    {
        if (fCumuledYaw_ != fNewYaw)
        {
            pNode_->Yaw(fNewYaw - fCumuledYaw_);
            fCumuledYaw_ = fNewYaw;
        }
    }

    void MovableUnit::Yaw_( const s_float& fYaw )
    {
        pNode_->Yaw(fYaw);
        fCumuledYaw_ += fYaw;
    }

    void MovableUnit::UpdateMovement_( const s_float& fDelta )
    {
        pNode_->Update(fDelta);

        if (pHandler_->IsEnabled())
        {
            if ( !pBodyModel_->HasAnimation() || (pBodyModel_->GetAnimMgr()->GetAnimID() != ANIM_JUMP_END) )
            {
                Vector mDir = mMovementDirection_;
                mMovementSpeed_ = Vector::ZERO;

                if ( (bTurn_ && !mDir.Z().IsNull()) || (!bTurn_ && !mDir.IsNull()) )
                {
                    if (!bTurn_)
                        mDir.Normalize();

                    s_float fMovementSpeed;
                    if (bFalling_)
                        fMovementSpeed = 0.5f;
                    else
                        fMovementSpeed = 1.0f;

                    s_float fNewYaw;
                    if (mDir.Z() > 0.0f)
                    {
                        // Moving backward
                        mMovementSpeed_.Z() = 1.0f;
                        if (bWalk_)
                        {
                            if (!bFalling_ && pBodyModel_->HasAnimation())
                                pBodyModel_->GetAnimMgr()->SetAnim(ANIM_WALKBACKWARDS, ANIM_PRIORITY_BACKGROUND);
                            fMovementSpeed *= fBackwardWalkSpeed_;
                        }
                        else
                        {
                            if (!bFalling_ && pBodyModel_->HasAnimation())
                                pBodyModel_->GetAnimMgr()->SetAnim(
                                    ANIM_WALKBACKWARDS, ANIM_PRIORITY_BACKGROUND,
                                    fBackwardRunSpeed_/fBackwardWalkSpeed_
                                );
                            fMovementSpeed *= fBackwardRunSpeed_;
                        }

                        if (!bTurn_)
                            fNewYaw = atan2(-mDir.X(), -mDir.Z()) - 0.5f;
                    }
                    else
                    {
                        // Moving forward
                        mMovementSpeed_.Z() = -1.0f;
                        if (bWalk_)
                        {
                            if (!bFalling_ && pBodyModel_->HasAnimation())
                                pBodyModel_->GetAnimMgr()->SetAnim(ANIM_WALK, ANIM_PRIORITY_BACKGROUND);
                            fMovementSpeed *= fForwardWalkSpeed_;
                        }
                        else
                        {
                            if (!bFalling_ && pBodyModel_->HasAnimation())
                                pBodyModel_->GetAnimMgr()->SetAnim(ANIM_RUN, ANIM_PRIORITY_BACKGROUND);
                            fMovementSpeed *= fForwardRunSpeed_;
                        }
                        if (!bTurn_)
                            fNewYaw = atan2(-mDir.X(), -mDir.Z());
                    }

                    if (bTurn_)
                        RotateModel(-mDir.X()*fTurnRate_*fDelta, 0.0f);
                    else
                        SetYaw_(fNewYaw);

                    mMovementSpeed_.Z() *= fMovementSpeed;
                }
                else
                {
                    if (bTurn_ && !mDir.X().IsNull())
                    {
                        Yaw_(-mDir.X()*fTurnRate_*fDelta);
                        if (pBodyModel_->HasAnimation())
                        {
                            pBodyModel_->GetAnimMgr()->SetAnim(
                                mDir.X() < 0.0f ? ANIM_SHUFFLE_LEFT : ANIM_SHUFFLE_RIGHT,
                                ANIM_PRIORITY_BACKGROUND
                            );
                        }
                    }
                    else
                    {
                        if (!bTurn_)
                            SetYaw_(0.0f);

                        if (!bFalling_ && pBodyModel_->HasAnimation())
                            pBodyModel_->GetAnimMgr()->SetAnim(ANIM_STAND, ANIM_PRIORITY_BACKGROUND);
                    }
                }

                s_bool bMovement;
                if (bFalling_)
                {
                    mMovementSpeed_ = Vector::ZERO;
                }
                else
                {
                    if (!mMovementSpeed_.IsNull())
                    {
                        mMovementSpeed_ = pNode_->Transform(mMovementSpeed_);
                        bMovement = true;
                    }
                }
            }
        }
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

    s_ptr<MovableUnitHandler> MovableUnit::GetPhysicsHandler()
    {
        return pHandler_;
    }

    void MovableUnit::RotateModel( const s_float& fYaw, const s_float& fPitch )
    {
        if (bCameraMovedAlone_)
            fCumuledYaw_ = 0.0f;

        Unit::RotateModel(fYaw, fPitch);
    }

    void MovableUnit::CreateGlue( s_ptr<Lua::State> pLua )
    {
        pLua->PushNumber(GetID());
        lGlueList_.PushBack(pLua->PushNew<LuaMovableUnit>());
        pLua->SetGlobal(GetLuaID());
    }
}
