/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Obstacle source            */
/*                                        */
/*                                        */

#include "scene/frost_boxobstacle.h"
#include "scene/frost_physicsmanager.h"

#include <OgreSceneNode.h>

using namespace std;

namespace Frost
{
    const s_str BoxObstacle::CLASS_NAME = "BoxObstacle";

    BoxObstacle::BoxObstacle( const s_float& fXSize, const s_float& fYSize, const s_float& fZSize ) :
        fXSize_(fXSize), fYSize_(fYSize), fZSize_(fZSize)
    {
        mBoundingBox_ = AxisAlignedBox(
            Vector(-fXSize_/2.0f, -fYSize_/2.0f, -fZSize_/2.0f),
            Vector( fXSize_/2.0f,  fYSize_/2.0f,  fZSize_/2.0f)
        );
    }

    s_bool BoxObstacle::PointGoThrough( const Vector& mPreviousPos, s_ptr<Vector> pNextPos ) const
    {
        return true;
    }

    s_bool BoxObstacle::EllipsoidGoThrough(
        const Vector& mRadiusVector, const Vector& mPreviousPos,
        const Vector& mNextPos, const Vector& mFinalPos, CollisionData& rData ) const
    {
        return true;
    }

    s_bool BoxObstacle::GetRayIntersection(
        const Vector& mRayOrigin, const Vector& mRayDirection, Vector& mIntersection ) const
    {
        return false;
    }
}
