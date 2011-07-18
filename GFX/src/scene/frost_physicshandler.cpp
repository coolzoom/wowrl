/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          PhysicsHandler source         */
/*                                        */
/*                                        */

#include "scene/frost_physicshandler.h"

#include "scene/frost_physicsmanager.h"
#include "scene/frost_movableobject.h"

using namespace std;

namespace Frost
{
    const s_str PhysicsHandler::CLASS_NAME = "PhysicsHandler";

    PhysicsHandler::PhysicsHandler( s_ptr<MovableObject> pParent ) :
        pParent_(pParent)
    {
    }

    PhysicsHandler::~PhysicsHandler()
    {
    }

    void PhysicsHandler::Enable()
    {
        bEnabled_ = true;
    }

    void PhysicsHandler::Disable()
    {
        bEnabled_ = false;
    }

    const s_bool& PhysicsHandler::IsEnabled() const
    {
        return bEnabled_;
    }

    s_ptr<const MovableObject> PhysicsHandler::GetParent() const
    {
        return pParent_;
    }

    s_ptr<MovableObject> PhysicsHandler::GetParent()
    {
        return pParent_;
    }

    void PhysicsHandler::SetEventReceiver( s_ptr<EventReceiver> pEventReceiver )
    {
        pEventReceiver_ = pEventReceiver;
    }

    s_ptr<EventReceiver> PhysicsHandler::GetEventReceiver()
    {
        if (pEventReceiver_)
            return pEventReceiver_;
        else
            return pParent_;
    }
}
