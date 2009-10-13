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
}
