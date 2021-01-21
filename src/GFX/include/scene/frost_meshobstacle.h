/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Obstacle header            */
/*                                        */
/*                                        */


#ifndef FROST_MESHOBSTACLE_H
#define FROST_MESHOBSTACLE_H

#include "frost.h"
#include "scene/frost_obstacle.h"

#include <OgreMatrix4.h>

namespace Frost
{
    /// Arbitrary mesh obstacle.
    /** This class can detect collision into any kind of
    *   triangle array. Because it is very generic, it is
    *   also very expensive : almost no optimizations are
    *   possible.
    *   \note 1) Whenever possible, derive from Obstacle and
    *         implement your own optimized solution.
    */
    class MeshObstacle : public MovableObstacle
    {
    public :

        /// Contains necessary informations to detect collisions.
        struct Triangle
        {
            s_array<Vector, 3> mP;
            Vector mNormal;
        };

        /// Constructor.
        /** \param lTriangleArray The triangle list
        *   \param mBoundingBox   The bounding box of this obstacle
        */
        MeshObstacle(const s_array<Triangle>& lTriangleArray, const AxisAlignedBox& mBoundingBox);

        /// Destructor.
        ~MeshObstacle();

        /// \copybrief Frost::Obstacle::EllipsoidGoThrough()
        /** \copydetails Frost::Obstacle::EllipsoidGoThrough()
        */
        s_bool EllipsoidGoThrough(
            const Vector& mRadiusVector, const Vector& mPreviousPos,
            const Vector& mNextPos, CollisionData& rData
        ) const;

        /// \copybrief Frost::Obstacle::GetRayIntersection()
        /** \copydetails Frost::Obstacle::GetRayIntersection()
        */
        s_bool GetRayIntersection(
            const Vector& mRayOrigin, const Vector& mRayDirection,
            Vector& mIntersection
        ) const;

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
        virtual void Update(const s_float& fDelta);

        static const s_str CLASS_NAME;

    private :

        s_bool  IsPointInsideTriangle_(const Vector& mPoint, const Triangle& mTriangle) const;
        s_float GetSmallestRoot_(const s_float& fA, const s_float& fB, const s_float& fC) const;

        s_array<Triangle>         lTriangleArray_;
        mutable s_array<Triangle> lTransformedTriangleArray_;

        mutable Ogre::Matrix4 mTransform_;
    };
}

#endif
