/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          PhysicsHandler header         */
/*                                        */
/*                                        */


#ifndef FROST_PHYSICSHANDLER_H
#define FROST_PHYSICSHANDLER_H

#include "frost.h"

namespace Frost
{
    /// Controls a MovableObject's movement with the law of physics
    /** \note This class only supports constant force field (gravity).<br>
    *         If you need more, you can always create your own handler class
    *         and use PhysicsManager::
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

        /// Adds some speed to this object.
        /** \param mAdd The ammount of speed to add
        */
        void                 AddSpeed(const Vector& mAdd);

        /// Sets the speed of this object.
        /** \param mSpeed The new speed of this object
        */
        virtual void         SetSpeed(const Vector& mSpeed);

        /// Returns the speed of this object.
        /** \return The speed of this object
        */
        const Vector&        GetSpeed() const;

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

        static const s_str CLASS_NAME;

    protected :

        s_ptr<MovableObject> pParent_;
        Vector               mSpeed_;
        s_bool               bEnabled_;
    };
}

#endif
