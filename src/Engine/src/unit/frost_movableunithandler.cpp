/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*        MovableUnitHandler source       */
/*                                        */
/*                                        */

#include "unit/frost_movableunithandler.h"

#include "unit/frost_movableunit.h"
#include "unit/frost_unitmanager.h"
#include "scene/frost_zonemanager.h"
#include "scene/frost_zone.h"
#include <scene/frost_physicsmanager.h>
#include <scene/frost_node.h>

#include <frost_utils_event.h>

using namespace std;

namespace Frost
{
    MovableUnitHandler::MovableUnitHandler( s_ptr<MovableUnit> pMovableUnit ) :
        PhysicsHandler(pMovableUnit ? pMovableUnit->GetNode() : nullptr),
        pMovableUnit_(pMovableUnit), mRadius_(Vector::UNIT), mBoundingBox_(-mRadius_, mRadius_),
        bFirstUpdate_(true), mState_(STATE_FREEFALL), mPreviousState_(STATE_FREEFALL),
        fTimeSinceLastUpdate_(1.0f/PhysicsManager::GetSingleton()->GetCollisionDetectionRate())
    {
        SetEventReceiver(pMovableUnit_);
    }

    MovableUnitHandler::~MovableUnitHandler()
    {
    }

    void MovableUnitHandler::Enable()
    {
        PhysicsHandler::Enable();

        fTimeSinceLastUpdate_ = 1.0f/PhysicsManager::GetSingleton()->GetCollisionDetectionRate();
    }

    void MovableUnitHandler::SetSpeed( const Vector& mSpeed )
    {
        mSpeed_ = mSpeed;
        mState_ = STATE_FREEFALL;
    }

    const Vector& MovableUnitHandler::GetSpeed() const
    {
        return mSpeed_;
    }

    void MovableUnitHandler::SetEllipsoidRadius( const Vector& mRadius )
    {
        mRadius_ = mRadius;
        mBoundingBox_ = AxisAlignedBox(-mRadius, mRadius);
    }

    const Vector& MovableUnitHandler::GetEllipsoidRadius() const
    {
        return mRadius_;
    }

    void MovableUnitHandler::SetHotSpot( const Vector& mHotSpot )
    {
        mHotSpot_ = mHotSpot;
    }

    const Vector& MovableUnitHandler::GetHotSpot() const
    {
        return mHotSpot_;
    }

    const Vector& MovableUnitHandler::GetGroundNormal() const
    {
        return mGroundNormal_;
    }

    const Vector& MovableUnitHandler::GetGroundIntersection() const
    {
        return mGroundIntersection_;
    }

    void MovableUnitHandler::Update( const s_float& fDelta )
    {
        if (IsEnabled())
        {
            // The collision detection code is not executed for
            // every frame (30 times per second at most).
            // This prevents too small numbers from appearing when
            // experiencing very high number of rendered frames per
            // second.
            // Frames for which the collision code is avoided use
            // the last computed movement to translate the unit,
            // regardless of collisions (to smooth out the movement).
            // This "fake" movement is of course cancelled out when
            // real collision detection is done.

            s_float fNewDelta;

            if (PhysicsManager::GetSingleton()->IsCollisionDetectionFixedRate())
            {
                fTimeSinceLastUpdate_ += fDelta;
                if (fTimeSinceLastUpdate_ < 1.0f/PhysicsManager::GetSingleton()->GetCollisionDetectionRate())
                {
                    // Resort to "fake" movement : no check for collisions
                    pParent_->Translate(mLastSpeed_*fDelta);
                    mInterpMovement_ += mLastSpeed_*fDelta;
                    return;
                }

                // Real collision detection
                fNewDelta = fTimeSinceLastUpdate_;
                fTimeSinceLastUpdate_ = 0;

                // First cancel the "fake" movement
                pParent_->Translate(-mInterpMovement_);
                mInterpMovement_ = Vector::ZERO;
            }
            else
                fNewDelta = fDelta;

            mPosition_ = pParent_->GetPosition(false) - mHotSpot_;
            mPreviousState_ = mState_;
            Vector mPreviousPos = mPosition_;

            switch (mState_)
            {
                case STATE_FREEFALL :
                {
                    // Apply gravity
                    mSpeed_   += PhysicsManager::GetSingleton()->GetGravity()*fNewDelta;
                    mMovement_ = fNewDelta*(
                        mSpeed_ + PhysicsManager::GetSingleton()->GetGravity()*fNewDelta/2.0f
                    );

                    // Check for collisions
                    uiRecursionCounter_ = 0;
                    UpdateMovement_();

                    break;
                }
                case STATE_TERRAIN :
                {
                    mHorizontalSpeed_ = pMovableUnit_->GetMovementSpeed();
                    mMovement_ = mHorizontalSpeed_*fNewDelta;
                    //s_float fMovement = mMovement_.GetNorm();

                    // Check for collisions
                    uiRecursionCounter_ = 0;
                    UpdateMovement_();

                    // Try to remain on the ground using a "gravity check"
                    // If the terrain under the unit is too far away (more
                    // than 0.5 world units), the unit will fall.
                    mMovement_ = -Vector::UNIT_Y*0.5f;
                    //mMovement_ = -0.5*Vector::UNIT_Y*(0.001f + fMovement*tan(UnitManager::GetSingleton()->GetMaxClimbingAngle()));

                    bGravityCheck_ = true;

                    // We only need one recursion here
                    uiRecursionCounter_ = PhysicsManager::GetSingleton()->GetMaxCollisionRecursion() - 1;
                    UpdateMovement_();

                    bGravityCheck_ = false;

                    break;
                }
            }

            // Calculate the averaged speed of this frame's movement
            mLastSpeed_ = (mPosition_ - mPreviousPos)/fNewDelta;

            // Move the unit
            pParent_->Translate(mPosition_ - mPreviousPos);

            // Tell the unit what's going on
            SendEvents_();
        }
    }

    void MovableUnitHandler::UpdateMovement_()
    {
        if ((!mMovement_.IsNull()) &&
            (uiRecursionCounter_ < PhysicsManager::GetSingleton()->GetMaxCollisionRecursion()))
        {
            Vector mDestination = mPosition_ + mMovement_;
            Vector mFinalDestination = mDestination;

            CollisionData mData;
            s_bool bCollision;

            //s_ptr<Obstacle> pCurrentlyBinded = pBindedObstacle_;
            AxisAlignedBox mTempBox = mBoundingBox_ + mPosition_;

            // Check for collision with all obstacles
            const s_ctnr< s_ptr<Obstacle> >& lObstacleList = PhysicsManager::GetSingleton()->GetObstacleList();
            s_ctnr< s_ptr<Obstacle> >::const_iterator iterObstacle;
            foreach (iterObstacle, lObstacleList)
            {
                s_ptr<Obstacle> pObs = (*iterObstacle);
                // First check if the two bounding boxes intersect
                if (pObs->IsInBoundingBox(mTempBox))
                {
                    // Then do the true collision detection
                    if (!pObs->EllipsoidGoThrough(mRadius_, mPosition_, mDestination, mData))
                    {
                        mDestination = mData.mNewPosition;
                        pBindedObstacle_ = pObs;
                        mData_ = mData;
                        bCollision = true;
                    }
                }
            }

            mPosition_ = mDestination;

            if (bCollision)
            {
                switch (mPreviousState_)
                {
                    case STATE_TERRAIN :
                    {
                        if (bGravityCheck_)
                        {
                            mMovement_ = Vector::ZERO;
                            mState_ = STATE_TERRAIN;
                        }
                        else
                        {
                            s_float fAngle = acos(mData_.mPlaneNormal.Y());
                            if (fAngle < UnitManager::GetSingleton()->GetMaxClimbingAngle())
                            {
                                // The slope is fine, move as usual.
                                mGroundNormal_ = mData_.mPlaneNormal;
                                mGroundIntersection_ = mData_.mCollisionPoint;

                                // Calculate remaning movement
                                mMovement_ = mFinalDestination - mPosition_;
                                mMovement_.Y() = 0.0f;
                                mMovement_.Y() = - mMovement_*mData_.mPlaneNormal/mData_.mPlaneNormal.Y();
                                mMovement_.Normalize();
                                mMovement_ *= (mFinalDestination - mPosition_).GetNorm();

                                mState_ = STATE_TERRAIN;
                            }
                            else
                            {
                                Vector mHorizontalMovement = mMovement_; mHorizontalMovement.Y() = 0.0f;
                                bClimbing_ = (mHorizontalMovement*mData_.mPlaneNormal < 0.0f);

                                if (bClimbing_)
                                {
                                    // The slope is too steep, the unit can't go there.
                                    mMovement_ = mData_.mPlaneNormal^Vector::UNIT_Y;
                                    mMovement_.Normalize();
                                    mMovement_ = ((mFinalDestination - mPosition_)*mMovement_)*mMovement_;

                                    mState_ = STATE_TERRAIN;
                                }
                                else
                                {
                                    // The slope is too steep, the unit slides.
                                    mSpeed_  = mHorizontalSpeed_;
                                    mMovement_ = Vector::ZERO;

                                    mState_ = STATE_FREEFALL;
                                    pBindedObstacle_ = nullptr;

                                    // Give it some initial impulsion
                                    mSpeed_ = mHorizontalSpeed_*0.5f;
                                }
                            }
                        }
                        break;
                    }
                    case STATE_FREEFALL :
                    {
                        s_float fAngle = acos(mData_.mPlaneNormal.Y());
                        if (fAngle > UnitManager::GetSingleton()->GetMaxClimbingAngle())
                        {
                            // The slope is too steep to land, continue the free fall
                            mMovement_ = mFinalDestination - mPosition_;
                            mMovement_ -= (mData.mPlaneNormal*mMovement_)*mData.mPlaneNormal;
                            if (!mMovement_.IsNull())
                            {
                                mMovement_.Normalize();
                                mMovement_ *= (mFinalDestination - mPosition_).GetNorm();
                            }

                            // ... and adjust the speed
                            mSpeed_ -= (mData_.mPlaneNormal*mSpeed_)*mData_.mPlaneNormal;

                            mState_ = STATE_FREEFALL;
                            pBindedObstacle_ = nullptr;
                        }
                        else
                        {
                            // The slope is gentle enough to land, stop movement
                            mMovement_ = Vector::ZERO;
                            mSpeed_ = Vector::ZERO;
                            mState_ = STATE_TERRAIN;
                        }
                        break;
                    }
                }

                ++uiRecursionCounter_;
                UpdateMovement_();
            }
            else
            {
                if (bGravityCheck_)
                {
                    mMovement_ = Vector::ZERO;

                    // The unit starts to fall, give it some initial impulsion
                    mSpeed_ = mHorizontalSpeed_;
                }

                mState_ = STATE_FREEFALL;
                pBindedObstacle_ = nullptr;
            }
        }
    }

    void MovableUnitHandler::SendEvents_()
    {
        if (bFirstUpdate_)
        {
            switch (mState_)
            {
                case STATE_FREEFALL :
                {
                    Event mEvent("PHYSICS_START_FREE_FALL");
                    GetEventReceiver()->OnEvent(mEvent);
                    break;
                }
                case STATE_TERRAIN :
                {
                    Event mEvent("PHYSICS_COLLISION");
                    GetEventReceiver()->OnEvent(mEvent);
                    break;
                }
            }

            bFirstUpdate_ = false;
        }
        else if (mState_ != mPreviousState_)
        {
            if (mPreviousState_ == STATE_FREEFALL && mState_ == STATE_TERRAIN)
            {
                Event mEvent("PHYSICS_COLLISION");
                GetEventReceiver()->OnEvent(mEvent);
            }
            else if (mPreviousState_ == STATE_TERRAIN && mState_ == STATE_FREEFALL)
            {
                Event mEvent("PHYSICS_START_FREE_FALL");
                GetEventReceiver()->OnEvent(mEvent);
            }
        }
    }
}


