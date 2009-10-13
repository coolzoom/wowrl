/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Obstacle header            */
/*                                        */
/*                                        */


#ifndef FROST_BOXOBSTACLE_H
#define FROST_BOXOBSTACLE_H

#include "frost.h"
#include "scene/frost_obstacle.h"

namespace Frost
{
    /// A simple box obstacle.
    class BoxObstacle : public MovableObstacle
    {
    public :

        /// Constructor.
        /** \param fXSize The dimension of the box on the X axis
        *   \param fYSize The dimension of the box on the Y axis
        *   \param fZSize The dimension of the box on the Z axis
        *   \note When created, the box will be centered at the origin.
        */
        BoxObstacle(const s_float& fXSize, const s_float& fYSize, const s_float& fZSize);

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

        static const s_str CLASS_NAME;

    private :

        s_float fXSize_, fYSize_, fZSize_;
    };
}

#endif
