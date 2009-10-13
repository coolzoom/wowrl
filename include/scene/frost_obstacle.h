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
        /// The remaining movement (direction calculated from collision plane)
        Vector mRemainingMovement;
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

        /// Check for collision between the two provided points.
        /** \param mPreviousPos The point to start from
        *   \param pNextPos     The point you need to reach
        *   \return 'true' if no collision happened
        *   \note pNextPos is also used to store the collision point
        */
        virtual s_bool PointGoThrough(const Vector& mPreviousPos, s_ptr<Vector> pNextPos) const = 0;

        /// Check for collision for an ellipsoidal object.
        /** \param mRadiusVector The radius vector of the object
        *   \param mPreviousPos  The initial position of the object (center of the ellipsoid)
        *   \param mNextPos      The wanted position of the object
        *   \param mFinalPos     The initial wanted position of the object
        *   \param rData         The collision data (output)
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
            const Vector& mNextPos, const Vector& mFinalPos,
            CollisionData& rData
        ) const = 0;

        /// Sets this obstacle as active.
        /** \param bIsActive 'true' to enable this obstacle
        */
        void          SetActive(const s_bool& bIsActive);

        /// Checks if this obstacle is active.
        /** \return 'true' if this obstacle is active
        */
        const s_bool& IsActive() const;

        /// Update this object's parameters.
        /** \param fDelta The time elapsed since the last call
        *   \note Automatically called by PhysicsManager.
        */
        virtual void  Update(const s_float& fDelta);

        static const s_str CLASS_NAME;

    protected :

        s_bool bIsActive_;
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
