/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Obstacle header            */
/*                                        */
/*                                        */


#ifndef FROST_PLANEOBSTACLE_H
#define FROST_PLANEOBSTACLE_H

#include "frost.h"
#include "scene/frost_obstacle.h"

namespace Frost
{
    /// A simple plane obstacle.
    class PlaneObstacle : public MovableObstacle
    {
    public :

        /// Constructor.
        /** \note Creates an infinite plane.<br>
        *         When created, the plane will face toward the positive Y axis
        *         and be centered at the origin (0, 0, 0).
        */
        PlaneObstacle();

        /// Constructor.
        /** \param fXSize The X dimension of the plane
        *   \param fZSize The Z dimension of the plane
        *   \note When created, the plane will face toward the positive Y axis
        *         and be centered at the origin (0, 0, 0).
        */
        PlaneObstacle(const s_float& fXSize, const s_float& fZSize);

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

        /// Update this object's parameters.
        /** \param fDelta The time elapsed since the last call
        */
        void   Update(const s_float& fDelta);

        static const s_str CLASS_NAME;

    private :

        void UpdateData_();
        s_bool IsPointInsideQuad_(const Vector& mPoint) const;
        s_float GetSmallestRoot_(const s_float& fA, const s_float& fB, const s_float& fC) const;

        s_float            fXSize_, fZSize_;
        s_bool             bInfinite_;
        s_array<Vector, 4> mP_;
        Vector             mCenter_, mNormal_;
    };
}

#endif
