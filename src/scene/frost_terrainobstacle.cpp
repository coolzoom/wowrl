/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*         TerrainObstacle source         */
/*                                        */
/*                                        */

#include "scene/frost_terrainobstacle.h"
#include "scene/frost_terrainchunk.h"
#include "scene/frost_physicsmanager.h"
#include "scene/frost_planeobstacle.h"

using namespace std;

namespace Frost
{
    TerrainObstacle::TerrainObstacle( const s_array<Triangle>& lTriangleArray,
        const s_array<s_float>& lHeightData, const s_uint& uiNX, const s_uint& uiNZ,
        const s_float& fXSize, const s_float& fZSize, const s_float& fOffX, const s_float& fOffZ,
        s_ptr<TerrainChunk> pParent ) :
        lTriangleArray_(lTriangleArray), lHeightData_(lHeightData),
        uiNX_(uiNX), uiNZ_(uiNZ), fXSize_(fXSize), fZSize_(fZSize), fOffX_(fOffX), fOffZ_(fOffZ),
        pParent_(pParent)
    {
        fTileXSize_ = fXSize_/s_float(uiNX_);
        fTileZSize_ = fZSize_/s_float(uiNZ_);

        mBoundingBox_ = pParent_->GetBoundingBox();
    }

    s_bool TerrainObstacle::IsPointInsideTriangle_( const Vector& mPoint, const Triangle& mTriangle ) const
    {
        // Note : code taken from :
        // http://www.blackpawn.com/texts/pointinpoly/default.html

        // Compute vectors
        Vector mE1 = mTriangle.mP[1] - mTriangle.mP[0];
        Vector mE2 = mTriangle.mP[2] - mTriangle.mP[0];
        Vector mP  =          mPoint - mTriangle.mP[0];

        // Compute dot products
        s_float fA = mE1*mE1;
        s_float fB = mE1*mE2;
        s_float fC = mE1*mP;
        s_float fD = mE2*mE2;
        s_float fE = mE2*mP;

        // Compute barycentric coordinates
        s_float fInvDenom = 1.0f/(fA*fD - fB*fB);
        s_float fU = (fD*fC - fB*fE)*fInvDenom;
        s_float fV = (fA*fE - fB*fC)*fInvDenom;

        // Check if point is in triangle
        return ((fU > 0.0f) && (fV > 0.0f) && (fU + fV < 1.0f));
    }

    s_float TerrainObstacle::GetSmallestRoot_( const s_float& fA, const s_float& fB, const s_float& fC ) const
    {
        s_float fDet = fB*fB - 4.0f*fA*fC;

        if (fDet < 0.0f)
            return s_float::NaN;
        else
        {
            s_float fSqrtDet = sqrt(fDet);
            if (fA < 0.0f)
                return (-fB + fSqrtDet)/(2.0f*fA);
            else
                return (-fB - fSqrtDet)/(2.0f*fA);
        }
    }

    s_bool TerrainObstacle::EllipsoidGoThrough(
        const Vector& mRadiusVector, const Vector& mPreviousPos,
        const Vector& mNextPos, CollisionData& rData ) const
    {
        // Note : algorithm taken from :
        // http://www.peroxide.dk/papers/collision/collision.pdf
        // Improved Collision Detection and Response,
        // by Kasper Fauerby on 25th July 2003.

        // First, transform the tested object into the obstacle's space
        // and convert world coordinates to ellipsoid coordinates
        Vector mPosition = mPreviousPos - pParent_->GetPosition();
        mPosition.ScaleDown(mRadiusVector);

        Vector mDestination = mNextPos - pParent_->GetPosition();
        mDestination.ScaleDown(mRadiusVector);

        Vector mDistance = mDestination - mPosition;
        Vector mInitialDistance = mDistance;
        s_float fDistanceSquared = mDistance.GetLengthSquared();

        Vector mDirection = mDistance;
        mDirection.Normalize();

        // Declare needed variables
        Vector  mPlaneNormal, mIntersection;
        s_float fStartTime, fEndTime;
        s_float fT, fBestT = 1.0f;
        s_float fSignedDistance, fNormDotDist;
        s_bool  bCollision, bEmbedded;
        Vector  mPointDistance, mEdge, mBaseToVertex;
        s_float fEdgeDotDist, fEdgeSquared, fEdgeDotBTV, fEdgeCollisionPoint;

        s_array<TerrainObstacle::Triangle>::const_iterator iterTriangle;
        foreach (iterTriangle, lTriangleArray_)
        {
            TerrainObstacle::Triangle mTriangle = *iterTriangle;
            mTriangle.mP[0].ScaleDown(mRadiusVector);
            mTriangle.mP[1].ScaleDown(mRadiusVector);
            mTriangle.mP[2].ScaleDown(mRadiusVector);

            // Calculate the triangle's plane and its normal
            mPlaneNormal = (mTriangle.mP[1] - mTriangle.mP[0])^(mTriangle.mP[2] - mTriangle.mP[0]);
            mPlaneNormal.Normalize();

            // Always orient the plane normal toward the object
            fSignedDistance = mPlaneNormal*(mPosition - mTriangle.mP[0]);
            if (fSignedDistance < 0.0f)
            {
                mPlaneNormal    *= -1.0f;
                fSignedDistance *= -1.0f;
            }

            fNormDotDist = mPlaneNormal*mDistance;

            bEmbedded = false;

            if (fabs(mDirection*mPlaneNormal) < 0.001f)
            {
                // The triangle's normal is perpendicular to the direction,
                // so the sphere will never come closer to the plane.
                if (fSignedDistance <= 1.0f)
                {
                    // It's distance is too short : it may collide.
                    // The sphere is embeded in the plane.
                    fStartTime = 0.0f;
                    fEndTime = 1.0f;
                    bEmbedded = true;
                }
                else
                {
                    // This plane won't collide with the sphere.
                    continue;
                }
            }
            else
            {
                // The sphere can collide the plane, but when ?
                fStartTime = (1.0f - fSignedDistance)/fNormDotDist;
                fEndTime = (-1.0f - fSignedDistance)/fNormDotDist;

                if (fStartTime > fBestT || fEndTime < 0.0f)
                {
                    // Not now.
                    continue;
                }

                fStartTime.Clamp(0.0f, 1.0f);
                fEndTime.Clamp(0.0f, 1.0f);
            }

            // Calculate the intersection of the sphere and the plane
            Vector mPlaneIntersection;
            mPlaneIntersection = mPosition - mPlaneNormal + fStartTime*mDistance;

            if (!bEmbedded && IsPointInsideTriangle_(mPlaneIntersection, mTriangle))
            {
                // Luckily, the sphere collided the plane inside the triangle.
                mIntersection = mPlaneIntersection;
                fBestT = fStartTime;
                bCollision = true;
                continue;
            }

            for (uint i = 0; i < 3; ++i)
            {
                // Calculate the intersection for each vertex
                mPointDistance = mPosition - mTriangle.mP[i];
                fT = GetSmallestRoot_(
                    fDistanceSquared,
                    2.0f*(mDistance*mPointDistance),
                    mPointDistance*mPointDistance - 1.0f
                );

                if (fT.IsInRange(0.0f, fBestT))
                {
                    mIntersection = mTriangle.mP[i];
                    fBestT = fT;
                    bCollision = true;
                }
            }

            for (uint i = 0; i < 3; ++i)
            {
                // ... for each edge
                if (i == 2)
                    mEdge = mTriangle.mP[0] - mTriangle.mP[2];
                else
                    mEdge = mTriangle.mP[i+1] - mTriangle.mP[i];

                mBaseToVertex = mTriangle.mP[i] - mPosition;
                fEdgeDotDist = mEdge*mDistance;
                fEdgeSquared = mEdge.GetLengthSquared();
                fEdgeDotBTV = mEdge*mBaseToVertex;

                fT = GetSmallestRoot_(
                    -fDistanceSquared*fEdgeSquared + fEdgeDotDist*fEdgeDotDist,
                    fEdgeSquared*2.0f*(mDistance*mBaseToVertex) - 2.0f*fEdgeDotDist*fEdgeDotBTV,
                    fEdgeSquared*(1.0f - mBaseToVertex*mBaseToVertex) + fEdgeDotBTV*fEdgeDotBTV
                );

                if (fT.IsInRange(0.0f, fBestT))
                {
                    fEdgeCollisionPoint = (fEdgeDotDist*fT - fEdgeDotBTV)/fEdgeSquared;
                    if (fEdgeCollisionPoint.IsInRange(0.0f, 1.0f))
                    {
                        mIntersection = mTriangle.mP[i] + fEdgeCollisionPoint*mEdge;
                        fBestT = fT;
                        bCollision = true;
                    }
                }
            }

            if (bCollision && (fBestT != 1.0f))
            {
                mDistance *= fBestT;
                fBestT = 1.0f;
                fDistanceSquared = mDistance.GetLengthSquared();
            }
        }

        if (bCollision)
        {
            // Calculate the new end position
            rData.mNewPosition = mPosition + fBestT*mDistance;
            rData.mNewPosition.ScaleUp(mRadiusVector);
            rData.mNewPosition += pParent_->GetPosition();

            // Transform the collision point
            rData.mCollisionPoint = mIntersection;
            rData.mCollisionPoint.ScaleUp(mRadiusVector);
            rData.mCollisionPoint += pParent_->GetPosition();

            // Calculate the world space collision normal
            rData.mPlaneNormal = mPosition + fBestT*mDistance - mIntersection;
            rData.mPlaneNormal.Normalize();
            rData.mPlaneNormal.ScaleUp(Vector(
                mRadiusVector.Y()*mRadiusVector.Z(),
                mRadiusVector.Z()*mRadiusVector.X(),
                mRadiusVector.X()*mRadiusVector.Y()
            ));
            rData.mPlaneNormal.Normalize();
        }

        return !bCollision;
    }

    s_bool TerrainObstacle::GetRayIntersection(
        const Vector& mRayOrigin, const Vector& mRayDirection, Vector& mIntersection ) const
    {
        Vector mPoint;

        // First see if the ray intersects the bounding box
        if (!mBoundingBox_.Contains(mRayOrigin))
        {
            // The ray origin is not inside the box, we'll have to cast it
            Vector mIntersection;
            if (!mBoundingBox_.GetRayIntersection(mRayOrigin, mRayDirection, mIntersection))
                return false;

            // We can move the ray origin to the intersection
            mPoint = mIntersection + mRayDirection*0.001f - pParent_->GetPosition();
        }
        else
            mPoint = mRayOrigin - pParent_->GetPosition();

        Vector mIteration = mRayDirection*0.01f;
        s_float fDist = mPoint.Y() - GetPointHeight(mPoint.X(), mPoint.Z());
        AxisAlignedBox mBox = pParent_->GetBoundingBox(true);
        while (mBox.Contains(mPoint))
        {
            // Move further...
            mPoint += mIteration;

            s_float fNewDist = mPoint.Y() - GetPointHeight(mPoint.X(), mPoint.Z());
            if (fNewDist*fDist < 0.0f)
            {
                // The ray has crossed the terrain
                mIntersection = mPoint - mIteration + pParent_->GetPosition();
                return true;
            }

            fDist = fNewDist;
        }

        return false;
    }

    s_float TerrainObstacle::GetPointHeight( const s_float& fX, const s_float& fZ ) const
    {
        s_float fNormalizedX = (fX/fXSize_ + fOffX_)*s_float(uiNX_-1);
        s_float fNormalizedZ = (fZ/fZSize_ + fOffZ_)*s_float(uiNZ_-1);

        // Calculate the quad on which this point is
        s_float fXMin = s_float::RoundDown(fNormalizedX);
        s_float fZMin = s_float::RoundDown(fNormalizedZ);
        s_float fXMax = s_float::RoundUp(fNormalizedX);
        s_float fZMax = s_float::RoundUp(fNormalizedZ);

        s_uint uiXMin = s_uint(fXMin);
        s_uint uiZMin = s_uint(fZMin);
        s_uint uiXMax = s_uint(fXMax);
        s_uint uiZMax = s_uint(fZMax);

        s_float fLocalX = (fNormalizedX - fXMin);
        s_float fLocalZ = (fNormalizedZ - fZMin);

        s_float fH1 = lHeightData_[uiXMin*uiNZ_ + uiZMin];
        s_float fH4 = lHeightData_[uiXMax*uiNZ_ + uiZMax];

        // A terrain quad is constructed this way :
        //
        //  Z
        //  ^   2____________4
        //  :   |           *|
        //  :   | "up"    *  |
        //  :   |       *    |
        //  :   |     *      |
        //  :   |   *        |
        //  :   | *   "down" |
        //  :   1____________3
        //  :
        //  :................> X
        //
        // Depending on where we are, we should use either the "up" OR the "down" triangle.
        // We can't make a weighted average of four vertices' heights because the quad is
        // not necessarily planar.

        if (fLocalX < fLocalZ)
        {
            // Use the "up" triangle (vertex 2)
            s_float fH2 = lHeightData_[uiXMin*uiNZ_ + uiZMax];

            // Blend the 3 heights
            return fH1 + (fH4 - fH2)*fLocalX + (fH2 - fH1)*fLocalZ;
        }
        else
        {
            // Use the "down" triangle (vertex 3)
            s_float fH3 = lHeightData_[uiXMax*uiNZ_ + uiZMin];

            // Blend the 3 heights
            return fH1 + (fH4 - fH3)*fLocalZ + (fH3 - fH1)*fLocalX;
        }
    }
}
