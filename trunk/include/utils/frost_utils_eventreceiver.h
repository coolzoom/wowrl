/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          EventReceiver header          */
/*                                        */
/*                                        */


#ifndef FROST_EVENTRECEIVER_H
#define FROST_EVENTRECEIVER_H

#include "frost_utils.h"
#include "frost_utils_types.h"

namespace Frost
{
    class Event;

    /// Abstract interface for event handling
    /** All classes which should react to some events
    *   should inherit from this class.<br>
    *   They will automatically react to Events thanks
    *   to the EventManager.
    */
    class EventReceiver
    {
    public :

        /// Constructor.
        EventReceiver();

        /// Destructor.
        virtual ~EventReceiver();

        /// Called whenever an Event occurs.
        /** \param mEvent The Event which has occured
        *   \note Only registered events will cause this
        *         function to be called.
        */
        virtual void OnEvent(const Event& mEvent) = 0;

        /// Enables reaction to an Event.
        /** \param sEventName The name of the Event this class should
        *                     react to
        */
        virtual void RegisterEvent(const s_str& sEventName);

        /// Disables reaction to an Event.
        /** \param sEventName The name of the Event this class shouldn't
        *                     react to anymore
        */
        virtual void UnregisterEvent(const s_str& sEventName);

        static const s_str CLASS_NAME;
    };
}

#endif
