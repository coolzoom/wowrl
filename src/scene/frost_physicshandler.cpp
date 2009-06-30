/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          PhysicsHandler source         */
/*                                        */
/*                                        */

#include "scene/frost_physicshandler.h"

#include "scene/frost_physicsmanager.h"

using namespace std;

namespace Frost
{
    const s_str PhysicsHandler::CLASS_NAME = "PhysicsHandler";

    PhysicsHandler::PhysicsHandler( s_ptr<MovableObject> pParent ) :
        pParent_(pParent), mSpeed_()
    {
    }

    PhysicsHandler::~PhysicsHandler()
    {
    }

    void PhysicsHandler::AddSpeed( const Vector& mAdd )
    {
        mSpeed_ += mAdd;
    }

    void PhysicsHandler::SetSpeed( const Vector& mSpeed )
    {
        mSpeed_ = mSpeed;
    }

    const Vector& PhysicsHandler::GetSpeed() const
    {
        return mSpeed_;
    }

    void PhysicsHandler::Enable()
    {
        bEnabled_ = true;
    }

    void PhysicsHandler::Disable()
    {
        bEnabled_ = false;
    }

    const s_bool& PhysicsHandler::IsEnabled()
    {
        return bEnabled_;
    }

    s_ptr<MovableObject> PhysicsHandler::GetParent()
    {
        return pParent_;
    }
}
