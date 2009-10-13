/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Obstacle source            */
/*                                        */
/*                                        */

#include "scene/frost_sphereobstacle.h"
#include "scene/frost_physicsmanager.h"

#include <OgreSceneNode.h>

using namespace std;

namespace Frost
{
    const s_str SphereObstacle::CLASS_NAME = "SphereObstacle";

    SphereObstacle::SphereObstacle( const s_float& fRadius ) :
        fRadius_(fRadius)
    {
    }

    s_bool SphereObstacle::PointGoThrough( const Vector& mPreviousPos, s_ptr<Vector> pNextPos ) const
    {
        // A = mPreviousPos
        // B = *pNextPos
        // O = GetPosition(false) (position of the circle)

        // First get the distance from the line (AB) to O
        Vector mDirection = *pNextPos - mPreviousPos;
        mDirection.Normalize();
        Vector mPosition = GetPosition(false);
        Vector mTemp = mPosition - mPreviousPos;
        s_float fDistance = (mTemp^mDirection).GetNorm();

        if (fDistance >= fRadius_)
        {
            // The distance is greater than the sphere's radius, no collision.
            return true;
        }
        else
        {
            // Then we calculate the position of the closest point of (AB) from O, which
            // we will call M.
            Vector mClosestPoint = mPreviousPos + (mTemp*mDirection)*mDirection;

            if (((mPreviousPos - mClosestPoint)*(*pNextPos - mClosestPoint) > 0.0f) &&
                ((*pNextPos - mPosition).GetNorm() > fRadius_))
            {
                // AM is in the same direction than BM.
                // A and B are outside the sphere, so there is no collision.
                return true;
            }
            else
            {
                // B is inside the sphere, so there is a collision.
                s_float fCollision = sqrt(fRadius_*fRadius_ - fDistance*fDistance);
                Vector mCollisionPoint = mClosestPoint - fCollision*mDirection;

                // We compensate the lost movement
                /*Vector mLostMovement = *pNextPos - mCollisionPoint;
                Vector mNormal = mCollisionPoint - mPosition;
                mNormal.Normalize();
                Vector mTangent1 = mNormal^mDirection;
                Vector mTangent2 = mTangent1^mNormal;
                s_float fLostMovement = mLostMovement*mTangent2;
                mNormal.Rotate(fLostMovement/fRadius_, mTangent1);*/
                *pNextPos = mClosestPoint - fCollision*mDirection;
                return false;
            }
        }
    }

    s_bool SphereObstacle::EllipsoidGoThrough(
        const Vector& mRadiusVector, const Vector& mPreviousPos,
        const Vector& mNextPos, const Vector& mFinalPos, CollisionData& rData ) const
    {
        return true;
    }
}
