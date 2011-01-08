/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Obstacle source            */
/*                                        */
/*                                        */

#include "scene/frost_obstacle.h"
#include "scene/frost_physicsmanager.h"

#include <OgreSceneNode.h>

using namespace std;

namespace Frost
{
    const s_str Obstacle::CLASS_NAME = "Obstacle";
    const s_str MovableObstacle::CLASS_NAME = "MovableObstacle";

    Obstacle::Obstacle()
    {
    }

    Obstacle::~Obstacle()
    {
        PhysicsManager::GetSingleton()->RemoveObstacle(this);
    }

    void Obstacle::SetActive( const s_bool& bIsActive )
    {
        bIsActive_ = bIsActive;
    }

    const s_bool& Obstacle::IsActive() const
    {
        return bIsActive_;
    }

    void Obstacle::Update( const s_float& fDelta )
    {
    }

    s_bool Obstacle::IsInBoundingBox( const Vector& mPoint ) const
    {
        return mBoundingBox_.Contains(mPoint);
    }

    s_bool Obstacle::IsInBoundingBox( const AxisAlignedBox& mBox ) const
    {
        return mBoundingBox_.Contains(mBox);
    }

    MovableObstacle::MovableObstacle()
    {
    }

    MovableObstacle::~MovableObstacle()
    {
    }

    s_bool MovableObstacle::IsInBoundingBox( const Vector& mPoint ) const
    {
        return mTransformedBoundingBox_.Contains(mPoint);
    }

    s_bool MovableObstacle::IsInBoundingBox( const AxisAlignedBox& mBox ) const
    {
        return mTransformedBoundingBox_.Contains(mBox);
    }

    void MovableObstacle::ForceUpdate()
    {
    }

    void MovableObstacle::Update( const s_float& fDelta )
    {
        MovableObject::Update(fDelta);

        mTransformedBoundingBox_ = AxisAlignedBox();
        Vector mScale = GetScale(false);
        Vector mPos = GetPosition(false);

        for (int i = 0; i < 8; ++i)
        {
            Vector mVec = Transform(mBoundingBox_[i]);
            mVec.ScaleUp(mScale);
            mVec += mPos;

            if (mTransformedBoundingBox_.IsInfinite())
            {
                mTransformedBoundingBox_ = AxisAlignedBox(mVec, mVec);
            }
            else
            {
                mTransformedBoundingBox_.Include(mVec);
            }
        }
    }
}
