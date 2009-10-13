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

    MovableObstacle::MovableObstacle()
    {
    }

    MovableObstacle::~MovableObstacle()
    {
    }

    void MovableObstacle::ForceUpdate()
    {
    }

    void MovableObstacle::Update( const s_float& fDelta )
    {
        MovableObject::Update(fDelta);
    }
}
