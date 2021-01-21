/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*        MovableUnitHandler header       */
/*                                        */
/*                                        */

#ifndef FROST_MOVABLEUNITHANDLER_H
#define FROST_MOVABLEUNITHANDLER_H

#include "frost_prereqs.h"
#include <scene/frost_physicshandler.h>

namespace Frost
{
    /// Physics handler to control units.
    class MovableUnitHandler : public PhysicsHandler
    {
    public :

        enum State
        {
            STATE_FREEFALL,
            STATE_TERRAIN
        };

        /// Constructor.
        /** \param pMovableUnit The movable unit to control
        */
        MovableUnitHandler(s_ptr<MovableUnit> pMovableUnit);

        /// Destructor.
        virtual ~MovableUnitHandler();

        /// Enables controls of the MovableObject.
        /** \note Disabled by default.
        */
        virtual void  Enable();

        /// Sets the handler's speed.
        /** \param mSpeed The new speed
        */
        void          SetSpeed(const Vector& mSpeed);

        /// Returns the handler's speed.
        /** \return The handler's speed
        */
        const Vector& GetSpeed() const;

        /// Sets the radius of the ellipsoid bounding volume.
        /** \param mRadius The radius
        */
        void          SetEllipsoidRadius(const Vector& mRadius);

        /// Returns the radius of the ellipsoid bounding volume.
        /** \return The radius of the ellipsoid bounding volume
        */
        const Vector& GetEllipsoidRadius() const;

        /// Sets the offset of the unit's position from its bounding volume.
        /** \param mHotSpot The offset
        */
        void          SetHotSpot(const Vector& mHotSpot);

        /// Returns the offset of the unit's position from its bounding volume.
        /** \return The offset of the unit's position from its bounding volume
        */
        const Vector& GetHotSpot() const;

        /// Returns the normal of the ground if any.
        /** \return The normal of the ground if any
        *   \note If the unit is in the "free fall" state, the value returned
        *         by this function is irrelevant.
        */
        const Vector& GetGroundNormal() const;

        /// Returns the position of the collision point if any.
        /** \return The position of the collision point if any
        *   \note If the unit is in the "free fall" state, the value returned
        *         by this function is irrelevant.
        */
        const Vector& GetGroundIntersection() const;

        /// Updates this handler.
        /** Checks for collision and moves the associated movable object.
        *   \param fDelta The time elapsed since the last call
        *   \note Automatically called by PhysicsManager if this handler
        *         has been registered.
        */
        void          Update(const s_float& fDelta);

    protected :

        void SendEvents_();
        void UpdateMovement_();

        s_ptr<MovableUnit> pMovableUnit_;

        Vector         mPosition_;
        Vector         mHotSpot_;
        Vector         mRadius_;
        AxisAlignedBox mBoundingBox_;

        Vector mSpeed_;
        Vector mMovement_, mHorizontalSpeed_;
        Vector mLastSpeed_;
        Vector mInterpMovement_;
        s_bool bFirstUpdate_;
        State  mState_, mPreviousState_;
        s_bool bGravityCheck_;
        s_bool bClimbing_;

        s_uint          uiRecursionCounter_;
        s_ptr<Obstacle> pBindedObstacle_;
        Vector          mGroundNormal_;
        Vector          mGroundIntersection_;
        CollisionData   mData_;

        s_float fTimeSinceLastUpdate_;
    };
}

#endif
