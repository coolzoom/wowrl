/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Obstacle source            */
/*                                        */
/*                                        */

#include "scene/frost_axisalignedobstacle.h"
#include "scene/frost_physicsmanager.h"

#include <OgreSceneNode.h>

using namespace std;

namespace Frost
{
    const s_str AxisAlignedObstacle::CLASS_NAME = "AxisAlignedObstacle";

    AxisAlignedObstacle::AxisAlignedObstacle( const s_float& fXSize, const s_float& fYSize, const s_float& fZSize ) :
        fXSize_(fXSize), fYSize_(fYSize), fZSize_(fZSize)
    {
        mBoundingBox_ = AxisAlignedBox(
            Vector(-fXSize_/2.0f, -fYSize_/2.0f, -fZSize_/2.0f),
            Vector( fXSize_/2.0f,  fYSize_/2.0f,  fZSize_/2.0f)
        );
    }

    s_bool AxisAlignedObstacle::EllipsoidGoThrough(
        const Vector& mRadiusVector, const Vector& mPreviousPos,
        const Vector& mNextPos, CollisionData& rData ) const
    {
        return true;
    }

    s_bool AxisAlignedObstacle::GetRayIntersection(
        const Vector& mRayOrigin, const Vector& mRayDirection, Vector& mIntersection ) const
    {
        return false;
    }
}
