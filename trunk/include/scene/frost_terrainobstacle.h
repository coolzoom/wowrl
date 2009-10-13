/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*         TerrainObstacle header         */
/*                                        */
/*                                        */

#ifndef FROST_TERRAINOBSTACLE_H
#define FROST_TERRAINOBSTACLE_H

#include "frost.h"
#include "scene/frost_obstacle.h"

namespace Frost
{
    /// Terrain optimized obstacle.
    /** \note This is not a movable obstacle.
    */
    class TerrainObstacle : public Obstacle
    {
    public :

        /// Contains necessary informations to detect collisions.
        struct Triangle
        {
            s_array<Vector, 3> mP;
        };

        /// Constructor.
        /** \param lTriangleArray The triangle array
        *   \param pParent        The associated chunk
        */
        TerrainObstacle(const s_array<Triangle>& lTriangleArray, s_ptr<TerrainChunk> pParent);

        /// \copybrief Frost::Obstacle::PointGoThrough()
        /** \copydetails Frost::Obstacle::PointGoThrough()
        */
        s_bool  PointGoThrough(const Vector& mPreviousPos, s_ptr<Vector> pNextPos) const;

        /// \copybrief Frost::Obstacle::EllipsoidGoThrough()
        /** \copydetails Frost::Obstacle::EllipsoidGoThrough()
        */
        s_bool EllipsoidGoThrough(
            const Vector& mRadiusVector, const Vector& mPreviousPos,
            const Vector& mNextPos, const Vector& mFinalPos,
            CollisionData& rData
        ) const;

        /// Returns the height of a point in the 2D space (X, Z).
        /** \param fX The X coordinate
        *   \param fZ The Z coordinate
        *   \return The height of this point
        */
        s_float GetPointHeight(const s_float& fX, const s_float& fZ);

    private :

        s_bool  IsPointInsideTriangle_(const Vector& mPoint, const Triangle& mTriangle) const;
        s_float GetSmallestRoot_(const s_float& fA, const s_float& fB, const s_float& fC) const;

        s_array<Triangle>   lTriangleArray_;
        s_ptr<TerrainChunk> pParent_;
    };
}

#endif
