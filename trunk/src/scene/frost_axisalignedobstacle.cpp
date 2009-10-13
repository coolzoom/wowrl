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
    }

    s_bool AxisAlignedObstacle::PointGoThrough( const Vector& mPreviousPos, s_ptr<Vector> pNextPos ) const
    {
        return true;
    }

    s_bool AxisAlignedObstacle::EllipsoidGoThrough(
        const Vector& mRadiusVector, const Vector& mPreviousPos,
        const Vector& mNextPos, const Vector& mFinalPos, CollisionData& rData ) const
    {
        return true;
    }
}
