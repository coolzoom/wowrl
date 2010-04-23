/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Obstacle header            */
/*                                        */
/*                                        */


#ifndef FROST_OBSTACLE_H
#define FROST_OBSTACLE_H

#include "frost.h"
#include "scene/frost_movableobject.h"

namespace Frost
{
    /// Contains returned collision data.
    struct CollisionData
    {
        /// The best available position
        Vector mNewPosition;
        /// The collision point
        Vector mCollisionPoint;
        /// The collision plane normal
        Vector mPlaneNormal;
    };

    /// A generic obstacle (abstract).
    /** This class should be inherited, see MeshObstacle for example.
    */
    class Obstacle
    {
    public :

        /// Constructor.
        Obstacle();

        /// Destructor.
        virtual ~Obstacle();

        /// Check for collision for an ellipsoidal object.
        /** \param mRadiusVector The radius vector of the object
        *   \param mPreviousPos  The initial position of the object (center of the ellipsoid)
        *   \param mNextPos      The wanted position of the object
        *   \param[out] rData    The collision data (output)
        *   \return 'true' if there was no collision
        *   \note The collision algorithm of PhysicsManager is recursive, so this function
        *         can be called several times per frame for the same object.<br>
        *         mNextPos is where you want your object to be <b>for this recursion
        *         iteration</b> only.<br>
        *         mFinalPos is where it should have been <b>at the end of the frame</b> if
        *         no collision had occured. This one is used to compute the object's speed
        *         and return a proper collision reaction.<br>
        *         Once it has finished detecting collision, this function returns a data
        *         structure that holds collision informations.<br>
        *         The most important one is mRemainingMovement : it tells the global collision
        *         algorithm that this object can still move after the collision (sliding).
        */
        virtual s_bool EllipsoidGoThrough(
            const Vector& mRadiusVector, const Vector& mPreviousPos,
            const Vector& mNextPos, CollisionData& rData
        ) const = 0;

        /// Checks if a ray intersects this Obstacle.
        /** \param mRayOrigin         The start point of the ray
        *   \param mRayDirection      The direction on which to cast the ray
        *   \param[out] mIntersection The intersection point
        *   \return 'true' if the ray has intersected the Obstacle
        */
        virtual s_bool GetRayIntersection(
            const Vector& mRayOrigin, const Vector& mRayDirection,
            Vector& mIntersection
        ) const = 0;

        /// Checks if the provided point is inside this Obstacle's bouding box.
        /** \param mPoint The point to test
        *   \return 'true' if the provided point is inside this Obstacle's one
        */
        virtual s_bool IsInBoundingBox(const Vector& mPoint) const;

        /// Checks if the provided box intersects this Obstacle's bouding box.
        /** \param mBox The bounding box to test
        *   \return 'true' if the provided box intersects this Obstacle's one
        */
        virtual s_bool IsInBoundingBox(const AxisAlignedBox& mBox) const;

        /// Sets this Obstacle as active.
        /** \param bIsActive 'true' to enable this obstacle
        */
        void           SetActive(const s_bool& bIsActive);

        /// Checks if this Obstacle is active.
        /** \return 'true' if this obstacle is active
        */
        const s_bool&  IsActive() const;

        /// Update this object's parameters.
        /** \param fDelta The time elapsed since the last call
        *   \note Automatically called by PhysicsManager.
        */
        virtual void   Update(const s_float& fDelta);

        static const s_str CLASS_NAME;

    protected :

        s_bool         bIsActive_;
        AxisAlignedBox mBoundingBox_;
    };

    /// A movable obstacle (abstract).
    /** Allows obstacles to be moved.
    */
    class MovableObstacle : public Obstacle,  public MovableObject
    {
    public :

        /// Constructor.
        MovableObstacle();

        /// Destructor.
        virtual ~MovableObstacle();

        /// Checks if the provided point is inside this Obstacle's bouding box.
        /** \param mPoint The point to test
        *   \return 'true' if the provided point is inside this Obstacle's one
        */
        virtual s_bool IsInBoundingBox(const Vector& mPoint) const;

        /// Checks if the provided box intersects this Obstacle's bouding box.
        /** \param mBox The bounding box to test
        *   \return 'true' if the provided box intersects this Obstacle's one
        */
        virtual s_bool IsInBoundingBox(const AxisAlignedBox& mBox) const;

        /// Tells this obstacle that it is now static.
        /** \param bIsStatic 'true' to make this obstacle static
        *   \note A static obstacle might consume less CPU because
        *         it will not need to update its position.
        */
        void SetStatic(const s_bool& bIsStatic);

        /// Checks if this obstacle is static.
        /** \return 'true' if this obstacle is static
        */
        const s_bool& IsStatic() const;

        /// Updates this MovableObstacle once.
        /** \note Only needed if the obstacle is static,
        *         you may need to update it at least once.<br>
        *         If not implemented by the derived class, this
        *         function has no effect.
        */
        virtual void ForceUpdate();

        /// Update this object's parameters.
        /** \param fDelta The time elapsed since the last call
        */
        virtual void  Update(const s_float& fDelta);

        static const s_str CLASS_NAME;

    protected :

        s_bool bIsStatic_;
    };
}

#endif
