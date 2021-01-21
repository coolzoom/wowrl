/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Obstacle source            */
/*                                        */
/*                                        */

#include "scene/frost_meshobstacle.h"
#include "scene/frost_physicsmanager.h"

#include <OgreSceneNode.h>

using namespace std;

namespace Frost
{
    const s_str MeshObstacle::CLASS_NAME = "MeshObstacle";

    MeshObstacle::MeshObstacle( const s_array<Triangle>& lTriangleArray, const AxisAlignedBox& mBoundingBox )
    {
        lTriangleArray_ = lTriangleArray;
        lTransformedTriangleArray_ = lTriangleArray_;
        mBoundingBox_ = mBoundingBox;
        mTransform_ = Ogre::Matrix4::IDENTITY;
    }

    MeshObstacle::~MeshObstacle()
    {
    }

    s_bool MeshObstacle::IsPointInsideTriangle_( const Vector& mPoint, const Triangle& mTriangle ) const
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

    s_float MeshObstacle::GetSmallestRoot_( const s_float& fA, const s_float& fB, const s_float& fC ) const
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

    s_bool MeshObstacle::EllipsoidGoThrough(
        const Vector& mRadiusVector, const Vector& mPreviousPos,
        const Vector& mNextPos, CollisionData& rData ) const
    {
        // Note : algorithm taken from :
        // http://www.peroxide.dk/papers/collision/collision.pdf
        // Improved Collision Detection and Response,
        // by Kasper Fauerby on 25th July 2003.

        // First convert world coordinates to ellipsoid coordinates
        Vector mPosition = mPreviousPos;
        mPosition.ScaleDown(mRadiusVector);

        Vector mDestination = mNextPos;
        mDestination.ScaleDown(mRadiusVector);

        Vector mDistance = mDestination - mPosition;
        s_float fDistanceSquared = mDistance.GetLengthSquared();

        Vector mDirection = mDistance;
        mDirection.Normalize();

        // Declare needed variables
        Vector mPlaneNormal, mIntersection;
        s_float fStartTime, fEndTime;
        s_float fT, fBestT = 1.0f;
        s_float fSignedDistance, fNormDotDist;
        s_bool bCollision, bEmbedded;
        Vector mPointDistance, mEdge, mBaseToVertex;
        s_float fEdgeDotDist, fEdgeSquared, fEdgeDotBTV, fEdgeCollisionPoint;

        s_array<MeshObstacle::Triangle>::const_iterator iterTriangle;
        foreach (iterTriangle, lTransformedTriangleArray_)
        {
            MeshObstacle::Triangle mTriangle = *iterTriangle;
            mTriangle.mP[0].ScaleDown(mRadiusVector);
            mTriangle.mP[1].ScaleDown(mRadiusVector);
            mTriangle.mP[2].ScaleDown(mRadiusVector);

            // Calculate the triangle's plane and its normal
            mPlaneNormal = (mTriangle.mP[1] - mTriangle.mP[0])^(mTriangle.mP[2] - mTriangle.mP[0]);
            mPlaneNormal.Normalize();

            // Discard faces that are backfacing
            fSignedDistance = mPlaneNormal*(mPosition - mTriangle.mP[0]);
            if (fSignedDistance < 0.0f)
                continue;

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
                fEdgeSquared = mEdge.GetLengthSquared();
                fEdgeDotDist = mEdge*mDistance;
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
        }

        if (bCollision)
        {
            // Calculate the new end position
            rData.mNewPosition = mPosition + fBestT*0.99f*mDistance;
            rData.mNewPosition.ScaleUp(mRadiusVector);

            // Transform the collision point
            rData.mCollisionPoint = mIntersection;
            rData.mCollisionPoint.ScaleUp(mRadiusVector);

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

    s_bool MeshObstacle::GetRayIntersection(
        const Vector& mRayOrigin, const Vector& mRayDirection, Vector& mIntersection ) const
    {
        return false;
    }

    void MeshObstacle::ForceUpdate()
    {
        Ogre::Matrix4 mTransform = pNode_->_getFullTransform();

        if (mTransform != mTransform_)
        {
            mTransform_ = mTransform;
            s_array<MeshObstacle::Triangle>::iterator iterTriangle;
            foreach (iterTriangle, lTransformedTriangleArray_)
            {
                iterTriangle->mP[0] = mTransform_ * iterTriangle->mP[0];
                iterTriangle->mP[1] = mTransform_ * iterTriangle->mP[1];
                iterTriangle->mP[2] = mTransform_ * iterTriangle->mP[2];
            }
        }
    }

    void MeshObstacle::Update( const s_float& fDelta )
    {
        MovableObstacle::Update(fDelta);

        if (!bIsStatic_)
        {
            Ogre::Matrix4 mTransform = pNode_->_getFullTransform();

            if (mTransform != mTransform_)
            {
                mTransform_ = mTransform;
                s_array<MeshObstacle::Triangle>::iterator iterTriangle;
                foreach (iterTriangle, lTransformedTriangleArray_)
                {
                    iterTriangle->mP[0] = mTransform_ * iterTriangle->mP[0];
                    iterTriangle->mP[1] = mTransform_ * iterTriangle->mP[1];
                    iterTriangle->mP[2] = mTransform_ * iterTriangle->mP[2];
                }
            }
        }
    }
}
