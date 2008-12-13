/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           EventManager source          */
/*                                        */
/*                                        */

#include "frost_eventmanager.h"
#include "frost_event.h"
#include "frost_eventreceiver.h"

using namespace std;

namespace Frost
{
    const s_str EventManager::CLASS_NAME = "EventManager";

    EventManager::EventManager()
    {
    }

    EventManager::~EventManager()
    {
    }

    void EventManager::RegisterEvent( s_ptr<EventReceiver> pReceiver, const s_str& sEventName )
    {
        if (MAPFIND(sEventName, lReceiverList_))
        {
            // This event is already registered to one or more EventReceivers.
            // Now, check our EventReceiver isn't one of them :

            pair< multimap< s_str, s_ptr<EventReceiver> >::iterator,
                  multimap< s_str, s_ptr<EventReceiver> >::iterator > mIterPair;
            mIterPair = lReceiverList_.equal_range(sEventName);
            multimap< s_str, s_ptr<EventReceiver> >::iterator iterReceiver;
            for (iterReceiver = mIterPair.first; iterReceiver != mIterPair.second; iterReceiver++)
            {
                if (iterReceiver->second == pReceiver)
                {
                    Warning(CLASS_NAME,
                        "Event \""+sEventName+"\" is already registered to that EventReceiver "+
                        "(EventReceiver : "+s_str(s_int((int)(long)pReceiver.Get()))+").");
                    return;
                }
            }
        }

        lReceiverList_.insert(pair< s_str, s_ptr<EventReceiver> >(sEventName, pReceiver));
    }

    void EventManager::UnregisterEvent( s_ptr<EventReceiver> pReceiver, const s_str& sEventName )
    {
        if (MAPFIND(sEventName, lReceiverList_))
        {
            // This event is already registered to one or more EventReceivers.
            // Now, check our EventReceiver is one of them :

            pair< multimap< s_str, s_ptr<EventReceiver> >::iterator,
                  multimap< s_str, s_ptr<EventReceiver> >::iterator > mIterPair;
            mIterPair = lReceiverList_.equal_range(sEventName);
            multimap< s_str, s_ptr<EventReceiver> >::iterator iterReceiver;
            for (iterReceiver = mIterPair.first; iterReceiver != mIterPair.second; iterReceiver++)
            {
                if (iterReceiver->second == pReceiver)
                {
                    lReceiverList_.erase(iterReceiver);
                    return;
                }
            }
        }

        Warning(CLASS_NAME,
            "Event \""+sEventName+"\" is not registered to that EventReceiver "+
            "(EventReceiver : "+s_str(s_int((int)(long)pReceiver.Get()))+").");
    }

    void EventManager::FireEvent( const Event& mEvent )
    {
        if (MAPFIND(mEvent.GetName(), lReceiverList_))
        {
            // This event is registered to one or more EventReceivers.
            // Check if this event should only be fired once per frame.
            if (!VECTORFIND(mEvent.GetName(), lFiredEventList_))
            {
                // Now, tell all these EventReceivers that this Event has occured.
                pair< multimap< s_str, s_ptr<EventReceiver> >::iterator,
                      multimap< s_str, s_ptr<EventReceiver> >::iterator > mIterPair;
                mIterPair = lReceiverList_.equal_range(mEvent.GetName());
                multimap< s_str, s_ptr<EventReceiver> >::iterator iterReceiver;
                for (iterReceiver = mIterPair.first; iterReceiver != mIterPair.second; iterReceiver++)
                {
                    iterReceiver->second->OnEvent(mEvent);
                }

                if (!mEvent.IsOncePerFrame())
                    lFiredEventList_.push_back(mEvent.GetName());
            }
        }
    }

    void EventManager::FrameEnded()
    {
        lFiredEventList_.clear();
    }
}
