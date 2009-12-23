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
        */
        MeshObstacle(const s_array<Triangle>& lTriangleArray);

        /// Destructor.
        ~MeshObstacle();

        /// \copybrief Frost::Obstacle::PointGoThrough()
        /** \copydetails Frost::Obstacle::PointGoThrough()
        */
        s_bool PointGoThrough(const Vector& mPreviousPos, s_ptr<Vector> pNextPos) const;

        /// \copybrief Frost::Obstacle::EllipsoidGoThrough()
        /** \copydetails Frost::Obstacle::EllipsoidGoThrough()
        */
        s_bool EllipsoidGoThrough(
            const Vector& mRadiusVector, const Vector& mPreviousPos,
            const Vector& mNextPos, const Vector& mFinalPos,
            CollisionData& rData
        ) const;

        /// \copybrief Frost::Obstacle::GetRayIntersection()
        /** \copydetails Frost::Obstacle::GetRayIntersection()
        */
        s_bool GetRayIntersection(
            const Vector& mRayOrigin, const Vector& mRayDirection,
            Vector& mIntersection
        ) const;

        static const s_str CLASS_NAME;

    private :

        s_bool  IsPointInsideTriangle_(const Vector& mPoint, const Triangle& mTriangle) const;
        s_float GetSmallestRoot_(const s_float& fA, const s_float& fB, const s_float& fC) const;

        s_array<Triangle> lTriangleArray_;
    };
}

#endif
