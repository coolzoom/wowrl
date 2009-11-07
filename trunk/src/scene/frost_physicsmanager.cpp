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

    PhysicsManager::PhysicsManager() :
        mGravity_(0, -9.81f, 0), uiMaxCollisionRecursion_(5u),
        fMinSlidingAngle_(0.125f), fMaxClimbingAngle_(0.125f)
    {
    }

    PhysicsManager::~PhysicsManager()
    {
        Log("Closing "+CLASS_NAME+"...");
    }

    s_bool PhysicsManager::ReadConfig()
    {
        s_ptr<Engine> pEngine = Engine::GetSingleton();
        if (pEngine->IsConstantDefined("MaxCollisionRecursion"))
            uiMaxCollisionRecursion_ = pEngine->GetUIntConstant("MaxCollisionRecursion");

        if (pEngine->IsConstantDefined("MinSlidingAngle"))
            fMinSlidingAngle_ = pEngine->GetFloatConstant("MinSlidingAngle");

        if (pEngine->IsConstantDefined("MaxClimbingAngle"))
            fMaxClimbingAngle_ = pEngine->GetFloatConstant("MaxClimbingAngle");

        return true;
    }

    void PhysicsManager::RegisterHandler( s_ptr<PhysicsHandler> pHandler )
    {
        if (pHandler->GetParent())
            lHandlerList_[pHandler->GetParent()] = pHandler;
        else
        {
            Warning(CLASS_NAME,
                "Trying to register a PhysicsHandler with no parent."
            );
        }
    }

    void PhysicsManager::RemoveHandler( s_ptr<PhysicsHandler> pHandler )
    {
        if (pHandler && pHandler->GetParent())
        {
            s_map< s_ptr<MovableObject>, s_ptr<PhysicsHandler> >::iterator iterHandler;
            iterHandler = lHandlerList_.Get(pHandler->GetParent());

            if (iterHandler != lHandlerList_.End())
                lHandlerList_.Erase(iterHandler);
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

    void PhysicsManager::SetMinSlidingAngle( const s_float& fMinSlidingAngle )
    {
        fMinSlidingAngle_ = fMinSlidingAngle;
    }

    const s_float& PhysicsManager::GetMinSlidingAngle() const
    {
        return fMinSlidingAngle_;
    }

    void PhysicsManager::SetMaxClimbingAngle( const s_float& fMaxClimbingAngle )
    {
        fMaxClimbingAngle_ = fMaxClimbingAngle;
    }

    const s_float& PhysicsManager::GetMaxClimbingAngle() const
    {
        return fMaxClimbingAngle_;
    }

    const s_uint& PhysicsManager::GetMaxCollisionRecursion() const
    {
        return uiMaxCollisionRecursion_;
    }

    void PhysicsManager::AddObstacle( s_ptr<Obstacle> pObstacle )
    {
        if (pObstacle)
        {
            if (!lObstacleList_.Find(pObstacle))
            {
                lObstacleList_.PushBack(pObstacle);
                pObstacle->SetActive(true);
            }
            else
            {
                Warning(CLASS_NAME,
                    "Trying to add an obstacle that is already in the scene."
                );
            }
        }
    }

    void PhysicsManager::RemoveObstacle( s_ptr<Obstacle> pObstacle )
    {
        if (pObstacle)
        {
            s_ctnr< s_ptr<Obstacle> >::iterator iterObstacle;
            iterObstacle = lObstacleList_.Get(pObstacle);

            if (iterObstacle != lObstacleList_.End())
            {
                pObstacle->SetActive(false);
                lObstacleList_.Erase(iterObstacle);
            }
        }
    }

    void PhysicsManager::ClearObstacles()
    {
        s_ctnr< s_ptr<Obstacle> >::iterator iterObstacle;
        foreach (iterObstacle, lObstacleList_)
        {
            (*iterObstacle)->SetActive(false);
        }
        lObstacleList_.Clear();
    }

    const s_ctnr< s_ptr<Obstacle> >& PhysicsManager::GetObstacleList() const
    {
        return lObstacleList_;
    }

    void PhysicsManager::Update( const s_float& fDelta )
    {
        s_ctnr< s_ptr<Obstacle> >::iterator iterObstacle;
        foreach (iterObstacle, lObstacleList_)
        {
            (*iterObstacle)->Update(fDelta);
        }

        s_map< s_ptr<MovableObject>, s_ptr<PhysicsHandler> >::iterator iterHandler;
        foreach (iterHandler, lHandlerList_)
        {
            iterHandler->second->Update(fDelta);
        }
    }
}
