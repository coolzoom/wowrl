/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Obstacle source            */
/*                                        */
/*                                        */

#include "scene/frost_planeobstacle.h"
#include "scene/frost_physicsmanager.h"

#include <OgreSceneNode.h>

using namespace std;

namespace Frost
{
    const s_str PlaneObstacle::CLASS_NAME = "PlaneObstacle";

    PlaneObstacle::PlaneObstacle( const s_float& fXSize, const s_float& fZSize ) :
        fXSize_(fXSize), fZSize_(fZSize)
    {
        SetInitialDirection(Vector::UNIT_Y);
    }

    s_bool PlaneObstacle::PointGoThrough( const Vector& mPreviousPos, s_ptr<Vector> pNextPos ) const
    {
        return true;
    }

    s_bool PlaneObstacle::IsPointInsideQuad_( const Vector& mPoint ) const
    {
        // Note : code inspired from :
        // http://www.blackpawn.com/texts/pointinpoly/default.html

        // Compute vectors
        Vector mE1 = mP_[1] - mP_[0];
        Vector mE2 = mP_[3] - mP_[0];
        Vector mP  = mPoint - mP_[0];

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

        // Check if point is in quad
        return ((fU.IsInRange(0.0f, 1.0f) && fV.IsInRange(0.0f, 1.0f)));
    }

    s_float PlaneObstacle::GetSmallestRoot_( const s_float& fA, const s_float& fB, const s_float& fC ) const
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

    s_bool PlaneObstacle::EllipsoidGoThrough(
        const Vector& mRadiusVector, const Vector& mPreviousPos,
        const Vector& mNextPos, const Vector& mFinalPos, CollisionData& rData ) const
    {
        // Note : algorithm inspired from :
        // http://www.peroxide.dk/papers/collision/collision.pdf
        // Improved Collision Detection and Response,
        // by Kasper Fauerby on 25th July 2003.

        // Transform coordinates to ellipsoid space
        Vector mPosition = mPreviousPos;
        mPosition.ScaleDown(mRadiusVector);

        Vector mDestination = mNextPos;
        mDestination.ScaleDown(mRadiusVector);

        Vector mDistance = mDestination - mPosition;
        s_float fDistanceSquared = mDistance.GetLengthSquared();
        Vector mDirection = mDistance;
        mDirection.Normalize();

        s_array<Vector, 4> mTransformedPoints(mP_);
        for (uint ui = 0; ui < 4; ++ui)
            mTransformedPoints[ui].ScaleDown(mRadiusVector);

        Vector mAdjustedNormal = (mTransformedPoints[1] - mTransformedPoints[0]) ^
                                 (mTransformedPoints[3] - mTransformedPoints[0]);

        mAdjustedNormal.Normalize();

        Vector mOrigin = mCenter_;
        mOrigin.ScaleDown(mRadiusVector);

        // Always orient the plane normal toward the object
        s_float fSignedDistance = mAdjustedNormal*(mPosition - mOrigin);
        if (fSignedDistance < 0.0f)
        {
            mAdjustedNormal *= -1.0f;
            fSignedDistance *= -1.0f;
        }

        s_float fNormDotDist = mAdjustedNormal*mDistance;

        s_bool bEmbedded;

        s_float fStartTime, fEndTime;
        if (fabs(mDirection*mAdjustedNormal) < 0.001f)
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
                return true;
            }
        }
        else
        {
            // The sphere can collide the plane, but when ?
            fStartTime = (1.0f - fSignedDistance)/fNormDotDist;
            fEndTime = (-1.0f - fSignedDistance)/fNormDotDist;

            if (fStartTime > 1.0f || fEndTime < 0.0f)
            {
                // Not now.
                return true;
            }

            fStartTime.Clamp(0.0f, 1.0f);
            fEndTime.Clamp(0.0f, 1.0f);
        }

        // Calculate the intersection of the sphere and the plane
        Vector mPlaneIntersection = mPosition - mAdjustedNormal + fStartTime*mDistance;
        Vector mWorldIntersection = mPlaneIntersection;
        mWorldIntersection.ScaleUp(mRadiusVector);

        if (!bEmbedded && IsPointInsideQuad_(mWorldIntersection))
        {
            // The sphere collided the plane inside the defined quad.
            // Calculate the new end position
            rData.mNewPosition = mPosition + fStartTime*mDistance - 0.0001f*mDistance.GetUnit();
            rData.mNewPosition.ScaleUp(mRadiusVector);

            // Transform the collision point
            rData.mCollisionPoint = mWorldIntersection;

            mDistance.ScaleUp(mRadiusVector);
            mDistance.Normalize();

            rData.mPlaneNormal = mNormal_;

            fSignedDistance = mAdjustedNormal*(mDestination - mPlaneIntersection);
            rData.mRemainingMovement = mDestination - fSignedDistance*mAdjustedNormal - mPlaneIntersection;
            if (!rData.mRemainingMovement.IsNull())
            {
                rData.mRemainingMovement.ScaleUp(mRadiusVector);
                rData.mRemainingMovement.Normalize();
                rData.mRemainingMovement *= (mFinalPos - rData.mNewPosition).GetNorm();
            }

            return false;
        }

        Vector mPointDistance, mIntersection;
        s_float fT;
        s_float fBestT = 1.0f;
        s_bool bCollision;

        for (uint i = 0; i < 4; ++i)
        {
            // Calculate the intersection for each vertex
            mPointDistance = mPosition - mTransformedPoints[i];
            fT = GetSmallestRoot_(
                fDistanceSquared,
                2.0f*(mDistance*mPointDistance),
                mPointDistance*mPointDistance - 1.0f
            );

            if (fT.IsInRange(0.0f, fBestT))
            {
                mIntersection = mTransformedPoints[i];
                fBestT = fT;
                bCollision = true;
            }
        }

        Vector mEdge, mBaseToVertex;
        s_float fEdgeDotDist, fEdgeSquared, fEdgeDotBTV;
        s_float fEdgeCollisionPoint;

        for (uint i = 0; i < 4; ++i)
        {
            // ... for each edge
            if (i == 3)
                mEdge = mTransformedPoints[0] - mTransformedPoints[3];
            else
                mEdge = mTransformedPoints[i+1] - mTransformedPoints[i];

            mBaseToVertex = mTransformedPoints[i] - mPosition;
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
                    mIntersection = mTransformedPoints[i] + fEdgeCollisionPoint*mEdge;
                    fBestT = fT;
                    bCollision = true;
                }
            }
        }

        if (bCollision)
        {
            // The sphere collided the plane.
            // Calculate the new end position
            rData.mNewPosition = mPosition + fBestT*mDistance - 0.0001f*mDistance.GetUnit();
            rData.mNewPosition.ScaleUp(mRadiusVector);

            // Transform the collision point
            rData.mCollisionPoint = mIntersection;
            rData.mCollisionPoint.ScaleUp(mRadiusVector);

            // Calculate the world space collision normal
            rData.mPlaneNormal = mPosition + fBestT*mDistance - mIntersection;
            rData.mPlaneNormal.ScaleUp(Vector(
                mRadiusVector.Y()*mRadiusVector.Z(),
                mRadiusVector.Z()*mRadiusVector.X(),
                mRadiusVector.X()*mRadiusVector.Y()
            ));
            rData.mPlaneNormal.Normalize();

            // Calculate the remaining movement
            fSignedDistance = rData.mPlaneNormal*(mPreviousPos - mFinalPos);
            rData.mRemainingMovement = mFinalPos - mPreviousPos;
            rData.mRemainingMovement += fSignedDistance*rData.mPlaneNormal;
            if (!rData.mRemainingMovement.IsNull())
            {
                rData.mRemainingMovement.Normalize();
                rData.mRemainingMovement *= (mFinalPos - rData.mNewPosition).GetNorm();
            }

            return false;
        }

        return true;
    }

    void PlaneObstacle::Update( const s_float& fDelta )
    {
        MovableObstacle::Update(fDelta);

        UpdateData_();
    }

    void PlaneObstacle::UpdateData_()
    {
        mCenter_ = GetPosition(false);

        const Ogre::Matrix4& mTransform = pNode_->_getFullTransform();

        mP_[0] = mTransform*Vector(-fXSize_/2.0f, 0.0f, -fZSize_/2.0f);
        mP_[1] = mTransform*Vector( fXSize_/2.0f, 0.0f, -fZSize_/2.0f);
        mP_[2] = mTransform*Vector( fXSize_/2.0f, 0.0f,  fZSize_/2.0f);
        mP_[3] = mTransform*Vector(-fXSize_/2.0f, 0.0f,  fZSize_/2.0f);

        mNormal_ = mTransform*Vector(0.0f, 1.0f, 0.0f) - mCenter_;
    }
}
