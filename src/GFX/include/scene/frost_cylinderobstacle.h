/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Obstacle header            */
/*                                        */
/*                                        */


#ifndef FROST_CYLINDEROBSTACLE_H
#define FROST_CYLINDEROBSTACLE_H

#include "frost.h"
#include "scene/frost_obstacle.h"

namespace Frost
{
    /// A simple cylinder obstacle.
    class CylinderObstacle : public MovableObstacle
    {
    public :

        /// Constructor.
        /** \param fRadius The radius of the cylinder
        *   \param fHeight The height of the cylinder
        *   \note When created, the cylinder will be aligned with the Y
        *         axis and centered at the origin (0, 0, 0).
        */
        CylinderObstacle(const s_float& fRadius, const s_float& fHeight);

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
        s_float fHeight_;
    };
}

#endif
