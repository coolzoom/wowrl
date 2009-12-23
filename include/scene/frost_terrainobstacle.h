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
        *   \param lHeightData    The indexed height data
        *   \param uiNX           The number of vertice on the X axis
        *   \param uiNZ           The number of vertice on the Z axis
        *   \param fXSize         The size of the terrain on the X axis
        *   \param fZSize         The size of the terrain on the Z axis
        *   \param fOffX          The X offset to return to local coordinates
        *   \param fOffZ          The Z offset to return to local coordinates
        *   \param pParent        The associated chunk
        */
        TerrainObstacle(
            const s_array<Triangle>& lTriangleArray,
            const s_array<s_float>& lHeightData,
            const s_uint& uiNX, const s_uint& uiNZ,
            const s_float& fXSize, const s_float& fZSize,
            const s_float& fOffX, const s_float& fOffZ,
            s_ptr<TerrainChunk> pParent
        );

        /// \copybrief Frost::Obstacle::PointGoThrough()
        /** \copydetails Frost::Obstacle::PointGoThrough()
        */
        s_bool  PointGoThrough(const Vector& mPreviousPos, s_ptr<Vector> pNextPos) const;

        /// \copybrief Frost::Obstacle::EllipsoidGoThrough()
        /** \copydetails Frost::Obstacle::EllipsoidGoThrough()
        */
        s_bool  EllipsoidGoThrough(
            const Vector& mRadiusVector, const Vector& mPreviousPos,
            const Vector& mNextPos, const Vector& mFinalPos,
            CollisionData& rData
        ) const;

        /// \copybrief Frost::Obstacle::GetRayIntersection()
        /** \copydetails Frost::Obstacle::GetRayIntersection()
        */
        s_bool  GetRayIntersection(
            const Vector& mRayOrigin, const Vector& mRayDirection,
            Vector& mIntersection
        ) const;

        /// Returns the height of a point in the 2D space (X, Z).
        /** \param fX The X coordinate
        *   \param fZ The Z coordinate
        *   \return The height of this point
        */
        s_float GetPointHeight(const s_float& fX, const s_float& fZ) const;

    private :

        s_bool  IsPointInsideTriangle_(const Vector& mPoint, const Triangle& mTriangle) const;
        s_float GetSmallestRoot_(const s_float& fA, const s_float& fB, const s_float& fC) const;

        s_array<Triangle>   lTriangleArray_;
        s_array<s_float>    lHeightData_;
        s_uint              uiNX_, uiNZ_;
        s_float             fXSize_, fZSize_;
        s_float             fOffX_, fOffZ_;
        s_float             fTileXSize_, fTileZSize_;
        s_ptr<TerrainChunk> pParent_;
    };
}

#endif
