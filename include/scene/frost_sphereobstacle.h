/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Obstacle header            */
/*                                        */
/*                                        */


#ifndef FROST_SPHEREOBSTACLE_H
#define FROST_SPHEREOBSTACLE_H

#include "frost.h"
#include "scene/frost_obstacle.h"

namespace Frost
{
    /// A simple sphere obstacle.
    class SphereObstacle : public MovableObstacle
    {
    public :

        /// Constructor.
        /** \param fRadius The radius of the sphere
        *   \note When created, the sphere will be centered at the origin
        *         (0, 0, 0).
        */
        SphereObstacle(const s_float& fRadius);

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

        static const s_str CLASS_NAME;

    private :

        s_float fRadius_;
    };
}

#endif
