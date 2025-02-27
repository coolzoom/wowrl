/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Obstacle header            */
/*                                        */
/*                                        */


#ifndef FROST_AXISALIGNEDOBSTACLE_H
#define FROST_AXISALIGNEDOBSTACLE_H

#include "frost.h"
#include "scene/frost_obstacle.h"

namespace Frost
{
    /// A simple axis aligned obstacle.
    /** \note Rotation will have no effect on this one.
    */
    class AxisAlignedObstacle : public MovableObstacle
    {
    public :

        /// Constructor.
        /** \param fXSize The dimension of the box on the X axis
        *   \param fYSize The dimension of the box on the Y axis
        *   \param fZSize The dimension of the box on the Z axis
        *   \note When created, the axis aligned box will be centered
        *         at the origin.
        */
        AxisAlignedObstacle(const s_float& fXSize, const s_float& fYSize, const s_float& fZSize);

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

        s_float fXSize_, fYSize_, fZSize_;
    };
}

#endif
