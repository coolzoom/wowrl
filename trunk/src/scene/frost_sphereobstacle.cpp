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
        mBoundingBox_ = AxisAlignedBox(
            Vector(-fRadius_, -fRadius_, -fRadius_),
            Vector( fRadius_,  fRadius_,  fRadius_)
        );
    }

    s_bool SphereObstacle::EllipsoidGoThrough(
        const Vector& mRadiusVector, const Vector& mPreviousPos,
        const Vector& mNextPos, CollisionData& rData ) const
    {
        return true;
    }

    s_bool SphereObstacle::GetRayIntersection(
        const Vector& mRayOrigin, const Vector& mRayDirection, Vector& mIntersection ) const
    {
        return false;
    }
}
