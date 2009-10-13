/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          PhysicsHandler header         */
/*                                        */
/*                                        */


#ifndef FROST_PHYSICSHANDLER_H
#define FROST_PHYSICSHANDLER_H

#include "frost.h"
#include "scene/frost_obstacle.h"

namespace Frost
{
    /// Controls a MovableObject's movement (abstract)
    /** This class must be derived and handle collision by itself.
    */
    class PhysicsHandler
    {
    public :

        /// Constructor.
        /** \param pParent The object to handle
        */
        PhysicsHandler(s_ptr<MovableObject> pParent);

        /// Destructor.
        virtual ~PhysicsHandler();

        /// Enables controls of the MovableObject.
        /** \note Disabled by default.
        */
        void                 Enable();

        /// Disables controls of the MovableObject.
        /** \note Disabled by default.
        */
        void                 Disable();

        /// Checks if the MovableObject is under control.
        /** \return 'true' if the MovableObject is under control
        *   \note Disabled by default.
        */
        const s_bool&        IsEnabled();

        /// Returns this handler's parent.
        /** \return this handler's parent
        */
        s_ptr<MovableObject> GetParent();

        /// Sets the object that will receive physics events.
        /** \param pEventReceiver The event receiver to use
        */
        void                 SetEventReceiver(s_ptr<EventReceiver> pEventReceiver);

        /// Returns the object that should receive physics events.
        /** \return The object that should receive physics events
        *   \note If no event receiver has been set, this function
        *         returns the associated MovableObject.
        */
        s_ptr<EventReceiver> GetEventReceiver();

        /// Updates this handler.
        /** Checks for collision and moves the associated movable object.
        *   \param fDelta The time elapsed since the last call
        *   \note Automatically called by PhysicsManager if this handler
        *         has been registered.
        */
        virtual void         Update(const s_float& fDelta) = 0;

        static const s_str CLASS_NAME;

    protected :

        s_ptr<MovableObject> pParent_;
        s_ptr<EventReceiver> pEventReceiver_;
        s_bool               bEnabled_;
    };
}

#endif
