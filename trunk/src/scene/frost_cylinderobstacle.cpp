/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Obstacle source            */
/*                                        */
/*                                        */

#include "scene/frost_cylinderobstacle.h"
#include "scene/frost_physicsmanager.h"

#include <OgreSceneNode.h>

using namespace std;

namespace Frost
{
    const s_str CylinderObstacle::CLASS_NAME = "CylinderObstacle";

    CylinderObstacle::CylinderObstacle( const s_float& fRadius, const s_float& fHeight ) :
        fRadius_(fRadius), fHeight_(fHeight)
    {
        mBoundingBox_ = AxisAlignedBox(
            Vector(-fRadius_, -fHeight/2.0f, -fRadius_),
            Vector( fRadius_,  fHeight/2.0f,  fRadius_)
        );
    }

    s_bool CylinderObstacle::PointGoThrough( const Vector& mPreviousPos, s_ptr<Vector> pNextPos ) const
    {
        return true;
    }

    s_bool CylinderObstacle::EllipsoidGoThrough(
        const Vector& mRadiusVector, const Vector& mPreviousPos,
        const Vector& mNextPos, const Vector& mFinalPos, CollisionData& rData ) const
    {
        return true;
    }

    s_bool CylinderObstacle::GetRayIntersection(
        const Vector& mRayOrigin, const Vector& mRayDirection, Vector& mIntersection ) const
    {
        return false;
    }
}
