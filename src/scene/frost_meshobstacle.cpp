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

    MeshObstacle::MeshObstacle( const s_array<Triangle>& lTriangleArray )
    {
        lTriangleArray_ = lTriangleArray;
    }

    MeshObstacle::~MeshObstacle()
    {
    }

    s_bool MeshObstacle::PointGoThrough( const Vector& mPreviousPos, s_ptr<Vector> pNextPos ) const
    {
        // TODO : Rewrite this code : not precise and not working
        Vector mDirection = *pNextPos - mPreviousPos;
        s_float fMaxDistance = mDirection.GetNorm();
        mDirection /= fMaxDistance;

        // Algorithm taken from :
        // http://www.graphics.cornell.edu/pubs/1997/MT97.pdf

        Vector  mE1, mE2, mTemp, mP1Dir;
        Vector  mCollisionPoint = *pNextPos;
        Vector  mOffset;
        s_float fDet, fInvDet, fU, fV, fDistance;
        s_float fShortestDistance = fMaxDistance;
        s_float fPrevDistance, fNextDistance;
        s_bool  bCollision;

        // Test each triangle of the list
        s_array<Triangle>::const_iterator iterTriangle;
        foreach (iterTriangle, lTriangleArray_)
        {
            // Get the direction from the origin of the ray to the first vertex
            mP1Dir = mPreviousPos - iterTriangle->mP[0];

            fPrevDistance = mP1Dir*iterTriangle->mNormal;
            fNextDistance = (*pNextPos - iterTriangle->mP[0])*iterTriangle->mNormal;

            if (fPrevDistance*fNextDistance < 0.0f)
            {
                if (fPrevDistance < 0.0f)
                    mOffset = 0.00001f*iterTriangle->mNormal;
                else
                    mOffset = -0.00001f*iterTriangle->mNormal;

                // Get the base of the triangle (local 2D space)
                mE1 = iterTriangle->mP[1] - iterTriangle->mP[0];
                mE2 = iterTriangle->mP[2] - iterTriangle->mP[0];

                // Calculate the determinant
                mTemp = mDirection^mE2;
                fDet = mE1*mTemp;

                if (fDet > -0.000001f && fDet < 0.000001f)
                {
                    // The ray is contained inside the triangle's plane. Skip it.
                    continue;
                }

                fInvDet = 1.0f/fDet;

                // U and V are the coordinates of the intersection point in the local 2D space
                // Calculate U
                fU = fInvDet*(mP1Dir*mTemp);

                if (fU < 0.0f || fU > 1.0f)
                {
                    // By definition, U and V are positive (U > 0), and U+V < 1 (U < 1).
                    // This U doesn't meet the conditions : the intersection is inside the
                    // triangle's plane, but not inside the triangle itself. Skip it.
                    continue;
                }

                // Calculate V
                mTemp = mP1Dir^mE1;
                fV = fInvDet*(mDirection*mTemp);

                if (fV < 0.0f || fU + fV > 1.0f)
                {
                    // Again, V must be positive and U+V < 1, so this V doesn't meet the
                    // conditions. Skip this triangle.
                    continue;
                }

                // We know the intersection is inside the triangle, now let's calculate
                // it's position from the ray's point of view :
                // intersection = origin + distance*direction
                fDistance = fInvDet*(mE2*mTemp);
                if (fDistance > 0.000001f)
                {
                    // Distance is positive : the intersection point is in the good direction.
                    // We keep on searching for a smaller distance, in case there are
                    // "overlapping" triangles.
                    if (fDistance < fShortestDistance)
                    {
                        fShortestDistance = fDistance-0.00001f;
                        mCollisionPoint = ((1.0f-fU-fV)*iterTriangle->mP[0] + fU*iterTriangle->mP[1] + fV*iterTriangle->mP[2]) - mOffset;
                    }
                    bCollision = true;
                    continue;
                }
                else
                {
                    // Distance is negative : the intersection point is "behind" the ray.
                    // It's not an acceptable intersection. Skip this triangle.
                    continue;
                }
            }
        }

        if (mCollisionPoint != *pNextPos)
        {
            *pNextPos = mCollisionPoint;
        }

        return !bCollision;
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
        const Vector& mNextPos, const Vector& mFinalPos, CollisionData& rData ) const
    {
        // Note : algorithm taken from :
        // http://www.peroxide.dk/papers/collision/collision.pdf
        // Improved Collision Detection and Response,
        // by Kasper Fauerby on 25th July 2003.

        Ogre::Matrix4 mTransform;
        mTransform.makeTransform(
            pNode_->_getDerivedPosition(),
            Vector::FrostToOgre(mRadiusVector),
            pNode_->_getDerivedOrientation()
        );

        Ogre::Matrix4 mOrientationTransform(-pNode_->_getDerivedOrientation());

        Ogre::Matrix4 mInverse;
        mInverse.makeInverseTransform(
            pNode_->_getDerivedPosition(),
            Vector::FrostToOgre(mRadiusVector),
            pNode_->_getDerivedOrientation()
        );

        // First, transform the tested object into the obstacle's space
        // and convert world coordinates to ellipsoid coordinates
        Vector mPosition = mInverse*mPreviousPos;
        Vector mDestination = mInverse*mNextPos;
        Vector mInitialDestination = mInverse*mFinalPos;

        Vector mDistance = mDestination - mPosition;
        s_float fDistanceSquared = mDistance.GetLengthSquared();
        Vector mDirection = mDistance;
        mDirection.Normalize();
        Vector mInitialDistance = mDistance;

        // FIXME : The ellipsoid can't be transformed.
        // Transform the collision mesh instead...
        Vector mTransformedRadiusVector = mOrientationTransform*mRadiusVector;

        // Declare needed variables
        Vector mPlaneNormal, mIntersection;
        s_float fStartTime, fEndTime;
        s_float fT, fBestT = 1.0f;
        s_float fSignedDistance, fNormDotDist;
        s_bool bCollision, bEmbedded;
        Vector mPointDistance, mEdge, mBaseToVertex;
        s_float fEdgeDotDist, fEdgeSquared, fEdgeDotBTV, fEdgeCollisionPoint;

        s_array<MeshObstacle::Triangle>::const_iterator iterTriangle;
        foreach (iterTriangle, lTriangleArray_)
        {
            MeshObstacle::Triangle mTriangle = *iterTriangle;
            mTriangle.mP[0].ScaleDown(mTransformedRadiusVector);
            mTriangle.mP[1].ScaleDown(mTransformedRadiusVector);
            mTriangle.mP[2].ScaleDown(mTransformedRadiusVector);

            // Calculate the triangle's plane and its normal
            mPlaneNormal = (mTriangle.mP[1] - mTriangle.mP[0])^(mTriangle.mP[2] - mTriangle.mP[0]);
            mPlaneNormal.Normalize();

            fSignedDistance = mPlaneNormal*(mPosition - mTriangle.mP[0]);
            if (fSignedDistance > 0.0f)
            {
                // Always orient the plane normal toward the object
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
                    -fDistanceSquared*fEdgeSquared  + fEdgeDotDist*fEdgeDotDist,
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

            if (bCollision)
            {
                mDistance *= fBestT;
                fBestT = 1.0f;
                fDistanceSquared = mDistance.GetLengthSquared();
            }
        }

        if (bCollision)
        {
            // Calculate the new end position
            rData.mNewPosition = mTransform*(
                mPosition + fBestT*mDistance - 0.0001f*mInitialDistance.GetUnit()
            );

            // Transform the collision point
            rData.mCollisionPoint = mTransform*mIntersection;

            // Calculate the sliding movement
            mPlaneNormal = mPosition + fBestT*mDistance - mIntersection;
            mPlaneNormal.Normalize();

            mDistance = mNextPos - mPreviousPos;
            mDistance.Normalize();

            // Calculate the world space collision normal
            rData.mPlaneNormal = mPlaneNormal;
            rData.mPlaneNormal.ScaleUp(Vector(
                mRadiusVector.Y()*mRadiusVector.Z(),
                mRadiusVector.Z()*mRadiusVector.X(),
                mRadiusVector.X()*mRadiusVector.Y()
            ));
            rData.mPlaneNormal.Normalize();

            fSignedDistance = mPlaneNormal*(mInitialDestination - mIntersection);
            rData.mRemainingMovement = mInitialDestination - fSignedDistance*mPlaneNormal - mIntersection;
            if (!rData.mRemainingMovement.IsNull())
            {
                rData.mRemainingMovement = mOrientationTransform*rData.mRemainingMovement;
                rData.mRemainingMovement.ScaleUp(mTransformedRadiusVector);
                rData.mRemainingMovement.Normalize();
                rData.mRemainingMovement *= (mFinalPos - rData.mNewPosition).GetNorm();
            }
        }

        return !bCollision;
    }

    s_bool MeshObstacle::GetRayIntersection(
        const Vector& mRayOrigin, const Vector& mRayDirection, Vector& mIntersection ) const
    {
        return false;
    }
}
