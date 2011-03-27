/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               XXX header               */
/*                                        */
/*                                        */


#ifndef FROST_EVENTMANAGER_H
#define FROST_EVENTMANAGER_H

#include "frost_utils_types.h"
#include "frost_utils_manager.h"
#ifdef USING_THREADS
 #include "frost_utils_thread.h"
#endif

namespace Frost
{
    class EventReceiver;
    class Event;

    /// Manages events and their responses
    class EventManager : public Manager<EventManager>
    {
    friend class Manager<EventManager>;
    public :

        /// Enables an EventReceiver's reaction to an event.
        /** \param pReceiver The EventReceiver to consider
        *   \param sEvent    The name of the event it should react to
        */
        void RegisterEvent(s_ptr<EventReceiver> pReceiver, const s_str& sEvent);

        /// Disables an EventReceiver's reaction to an event.
        /** \param pReceiver The EventReceiver to consider
        *   \param sEvent    The name of the event it shouldn't react to anymore
        */
        void UnregisterEvent(s_ptr<EventReceiver> pReceiver, const s_str& sEvent);

        /// Disables all events connected to the provided EventReceiver.
        /** \param pReceiver The EventReceiver to disable
        */
        void UnregisterReceiver(s_ptr<EventReceiver> pReceiver);

        /// Tells this manager an Event as occured.
        /** \param mEvent The Event which has occured
        *   \note All EventReceivers registred to react to this Event
        *         will be told the Event has occured by calling
        *         EventReceiver::OnEvent().
        */
        void FireEvent(const Event& mEvent);

        /// Tells this manager to clear the fired Events list.
        void FrameEnded();

        /// Enables/disables debug output in the log file.
        void ToggleDebugOutput();

        static const s_str CLASS_NAME;

    protected :

        /// Default constructor
        /** \note This is a singleton class. That means that there can
        *         only be a single instance at the same time. That's why
        *         the constructor is not in public scope.<br>
        *         If you need to get a pointer to that unique instance,
        *         call EventManager::GetSingleton().<br>
        *         Note that, if GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close
        *         your program by calling EventManager::Delete() (this is
        *         automatically done by Engine).
        */
        EventManager();

        /// Destructor
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        ~EventManager();

        /// Copy constructor
        EventManager(const EventManager& mMgr);

        /// Assignment operator
        EventManager& operator = (const EventManager& mMgr);

    private :

        s_multimap< s_str, s_ptr<EventReceiver> > lReceiverList_;
        s_ctnr<s_str>                             lFiredEventList_;

        s_bool bDebugOutput_;

        #ifdef USING_THREADS
        Mutex mMutex_;
        #endif
    };

}

#endif
