/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*        MovableUnitHandler source       */
/*                                        */
/*                                        */

#include "unit/frost_movableunithandler.h"
#include "unit/frost_movableunit.h"
#include "scene/frost_physicsmanager.h"
#include "scene/frost_node.h"

using namespace std;

namespace Frost
{
    MovableUnitHandler::MovableUnitHandler( s_ptr<MovableUnit> pMovableUnit ) :
        PhysicsHandler(pMovableUnit ? pMovableUnit->GetNode() : NULL),
        pMovableUnit_(pMovableUnit), bFirstUpdate_(true), mState_(STATE_FREEFALL),
        mPreviousState_(STATE_FREEFALL), mRadius_(Vector::UNIT)
    {
        SetEventReceiver(pMovableUnit_);
    }

    MovableUnitHandler::~MovableUnitHandler()
    {
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
            mPosition_ = pParent_->GetPosition(false) - mHotSpot_;
            mPreviousState_ = mState_;
            Vector mPreviousPos = mPosition_;

            switch (mState_)
            {
                case STATE_FREEFALL :
                {
                    // Apply gravity
                    mSpeed_   += PhysicsManager::GetSingleton()->GetGravity()*fDelta;
                    mMovement_ = fDelta*(
                        mSpeed_ + PhysicsManager::GetSingleton()->GetGravity()*fDelta/2.0f
                    );

                    // Check for collisions
                    uiRecursionCounter_ = 0;
                    UpdateMovement_();

                    break;
                }
                case STATE_TERRAIN :
                {
                    mHorizontalSpeed_ = pMovableUnit_->GetMovementSpeed();
                    mMovement_ = mHorizontalSpeed_*fDelta;
                    s_float fMovement = mMovement_.GetNorm();

                    // Check for collisions
                    uiRecursionCounter_ = 0;
                    UpdateMovement_();

                    // Try to remain on the ground
                    mMovement_ = -Vector::UNIT_Y*(
                        0.001f + fMovement*tan(
                            PhysicsManager::GetSingleton()->GetMinSlidingAngle()
                        )
                    );

                    uiRecursionCounter_ = PhysicsManager::GetSingleton()->GetMaxCollisionRecursion() - 1;
                    UpdateMovement_();

                    if (mState_ == STATE_FREEFALL)
                    {
                        // The unit starts to fall, give it some impulsion
                        mSpeed_  = mHorizontalSpeed_;
                    }

                    break;
                }
            }


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

            s_ptr<Obstacle> pCurrentlyBinded = pBindedObstacle_;

            // Check for collision with all obstacles
            const s_ctnr< s_ptr<Obstacle> >& lObstacleList = PhysicsManager::GetSingleton()->GetObstacleList();
            s_ctnr< s_ptr<Obstacle> >::const_iterator iterObstacle;
            foreach (iterObstacle, lObstacleList)
            {
                if (!(*iterObstacle)->EllipsoidGoThrough(
                    mRadius_, mPosition_, mDestination, mFinalDestination, mData))
                {
                    mDestination = mData.mNewPosition;
                    pBindedObstacle_ = *iterObstacle;
                    mData_ = mData;
                    bCollision = true;
                }
            }

            mPosition_ = mDestination;

            if (bCollision)
            {
                switch (mPreviousState_)
                {
                    case STATE_TERRAIN :
                    {
                        s_float fDotProduct = mMovement_*Vector::UNIT_Y;
                        s_float fAngle = acos(mData_.mPlaneNormal*Vector::UNIT_Y);
                        if (fDotProduct == 0.0f)
                        {
                            // Horizontal movement
                            if (fAngle > PhysicsManager::GetSingleton()->GetMaxClimbingAngle())
                            {
                                // The slope is too steep, the unit can't go there.
                                // End the movement.
                                mMovement_ = Vector::ZERO;
                                mState_ = STATE_TERRAIN;
                            }
                            else
                            {
                                // The slope is fine, move as usual.
                                mMovement_ = mData_.mRemainingMovement;
                                mGroundNormal_ = mData_.mPlaneNormal;
                                mGroundIntersection_ = mData_.mCollisionPoint;
                                mState_ = STATE_TERRAIN;
                            }
                        }
                        else
                        {
                            // Gravity test
                            if (fAngle > PhysicsManager::GetSingleton()->GetMinSlidingAngle())
                            {
                                // The slope is too steep, the unit slides.
                                mSpeed_  = mHorizontalSpeed_;
                                mMovement_ = Vector::ZERO;

                                mState_ = STATE_FREEFALL;
                                pBindedObstacle_ = NULL;
                            }
                            else
                            {
                                // The slope is fine, move as usual.
                                mGroundNormal_ = mData_.mPlaneNormal;
                                mGroundIntersection_ = mData_.mCollisionPoint;
                                mMovement_ = Vector::ZERO;
                                mState_ = STATE_TERRAIN;
                            }
                        }
                        break;
                    }
                    case STATE_FREEFALL :
                    {
                        s_float fAngle = acos(mData_.mPlaneNormal*Vector::UNIT_Y);
                        if (fAngle > PhysicsManager::GetSingleton()->GetMinSlidingAngle())
                        {
                            // The slope is too steep to land, continue the free fall
                            mMovement_ = mData_.mRemainingMovement;

                            // ... and adjust the speed
                            mSpeed_ -= (mData_.mPlaneNormal*mSpeed_)*mData_.mPlaneNormal;

                            mState_ = STATE_FREEFALL;
                            pBindedObstacle_ = NULL;
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
                mState_ = STATE_FREEFALL;
                pBindedObstacle_ = NULL;
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


