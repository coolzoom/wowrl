/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          PhysicsManager source         */
/*                                        */
/*                                        */

#include "scene/frost_physicsmanager.h"

#include "scene/frost_physicshandler.h"
#include "scene/frost_movableobject.h"
#include "scene/frost_obstacle.h"

using namespace std;

namespace Frost
{
    const s_str PhysicsManager::CLASS_NAME = "PhysicsManager";

    PhysicsManager::PhysicsManager() : mGravity_(0, -9.81f, 0)
    {
    }

    PhysicsManager::~PhysicsManager()
    {
        Log("Closing "+CLASS_NAME+"...");
        s_map< s_ptr<MovableObject>, s_ptr<PhysicsHandler> >::iterator iterHandler;
        foreach (iterHandler, lHandlerList_)
        {
            iterHandler->second.Delete();
        }

        s_map< s_uint, s_ptr<Obstacle> >::iterator iterObstacle;
        foreach (iterObstacle, lObstacleList_)
        {
            iterObstacle->second.Delete();
        }
    }

    s_ptr<PhysicsHandler> PhysicsManager::CreateHandler( s_ptr<MovableObject> pObj )
    {
        s_ptr<PhysicsHandler> pHandler;

        s_map< s_ptr<MovableObject>, s_ptr<PhysicsHandler> >::iterator iterHandler;
        iterHandler = lHandlerList_.Get(pObj);
        if (iterHandler == lHandlerList_.End())
        {
            pHandler = new PhysicsHandler(pObj);
            lHandlerList_[pObj] = pHandler;
        }
        else
        {
            pHandler = lHandlerList_[pObj];
        }

        return pHandler;
    }

    void PhysicsManager::RegisterHandler( s_ptr<PhysicsHandler> pHandler )
    {
        if (pHandler->GetParent())
            lHandlerList_[pHandler->GetParent()] = pHandler;
        else
        {
            Warning(CLASS_NAME,
                "Trying to register a PhysicsHandler with no parent"
            );
        }
    }

    void PhysicsManager::DeleteHandler( s_ptr<PhysicsHandler> pHandler )
    {
        if (pHandler && pHandler->GetParent())
        {
            s_map< s_ptr<MovableObject>, s_ptr<PhysicsHandler> >::iterator iterHandler;
            iterHandler = lHandlerList_.Get(pHandler->GetParent());

            if (iterHandler != lHandlerList_.End())
            {
                iterHandler->second.Delete();
                lHandlerList_.Erase(iterHandler);

                return;
            }

            Warning(CLASS_NAME,
                "Trying to call DeleteHandler on a Handler that has not been created "
                "by (or registered to) PhysicsManager (parent ID:"+pHandler->GetParent()->GetID()+")."
            );
        }
    }

    void PhysicsManager::SetGravity( const Vector& mGravity )
    {
        mGravity_ = mGravity;
    }

    const Vector& PhysicsManager::GetGravity() const
    {
        return mGravity_;
    }

    s_ptr<Obstacle> PhysicsManager::AddObstacle( s_ptr<Plane> pPlane )
    {
        return NULL;
    }

    s_ptr<Obstacle> PhysicsManager::AddObstacle( s_ptr<Model> pModel )
    {
        return NULL;
    }

    void PhysicsManager::RemoveObstacle( s_ptr<Obstacle> pObstacle )
    {
        if (pObstacle)
        {
            s_map< s_uint, s_ptr<Obstacle> >::iterator iterObstacle;
            iterObstacle = lObstacleList_.Get(pObstacle->GetID());

            if (iterObstacle != lObstacleList_.End())
            {
                if (iterObstacle->second->GetID() == pObstacle->GetID())
                {
                    // Everything went fine, delete, erase from map and return
                    iterObstacle->second.Delete();
                    lObstacleList_.Erase(iterObstacle);

                    return;
                }
            }

            Warning(CLASS_NAME,
                "Trying to call RemoveObstacle on an Obstacle that has not been created "
                "by PhysicsManager (ID:"+pObstacle->GetID()+")."
            );
        }
    }

    void PhysicsManager::ClearObstacles()
    {
        s_map< s_uint, s_ptr<Obstacle> >::iterator iterObstacle;
        foreach (iterObstacle, lObstacleList_)
        {
            iterObstacle->second.Delete();
        }
        lObstacleList_.Clear();
    }

    void PhysicsManager::UpdateHandlers( const s_float& fDelta )
    {
        s_map< s_ptr<MovableObject>, s_ptr<PhysicsHandler> >::iterator iterHandler;
        foreach (iterHandler, lHandlerList_)
        {
            if (iterHandler->second->IsEnabled())
            {
                // Acceleration is "constant"
                const Vector& mAcceleration = mGravity_;

                // Update speed
                iterHandler->second->AddSpeed(mAcceleration*fDelta);

                // Translate
                Vector mMovement = iterHandler->second->GetSpeed()*fDelta +
                                   mAcceleration*fDelta*fDelta/2.0f;
                iterHandler->first->Translate(mMovement, true);

                // TODO : Check collisions
            }
        }
    }
}
