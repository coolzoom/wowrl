/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           EventManager source          */
/*                                        */
/*                                        */

#include "frost_utils_eventmanager.h"
#include "frost_utils_event.h"
#include "frost_utils_eventreceiver.h"
#include "frost_utils_stdhelper.h"
#include "frost_utils_log.h"

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
        if (lReceiverList_.Find(sEventName))
        {
            // This event is already registered to one or more EventReceivers.
            // Now, check our EventReceiver isn't one of them :

            s_multimap< s_str, s_ptr<EventReceiver> >::range mRange;
            mRange = lReceiverList_.EqualRange(sEventName);
            s_multimap< s_str, s_ptr<EventReceiver> >::iterator iterReceiver;
            foreach (iterReceiver, mRange)
            {
                if (iterReceiver->second == pReceiver)
                {
                    Warning(CLASS_NAME,
                        "Event \""+sEventName+"\" is already registered to this EventReceiver "+
                        "(EventReceiver : "+pReceiver+")."
                    );
                    return;
                }
            }
        }

        lReceiverList_.Insert(sEventName, pReceiver);
    }

    void EventManager::UnregisterEvent( s_ptr<EventReceiver> pReceiver, const s_str& sEventName )
    {
        if (lReceiverList_.Find(sEventName))
        {
            // This event is already registered to one or more EventReceivers.
            // Now, check our EventReceiver is one of them :

            s_multimap< s_str, s_ptr<EventReceiver> >::range mRange;
            mRange = lReceiverList_.EqualRange(sEventName);
            s_multimap< s_str, s_ptr<EventReceiver> >::iterator iterReceiver;
            foreach (iterReceiver, mRange)
            {
                if (iterReceiver->second == pReceiver)
                {
                    lReceiverList_.Erase(iterReceiver);
                    return;
                }
            }
        }

        Warning(CLASS_NAME,
            "Event \""+sEventName+"\" is not registered to this EventReceiver "+
            "(EventReceiver : "+pReceiver+")."
        );
    }

    void EventManager::UnregisterReceiver( s_ptr<EventReceiver> pReceiver )
    {
        s_ctnr<s_multimap< s_str, s_ptr<EventReceiver> >::iterator> lDeleteList;

        s_multimap< s_str, s_ptr<EventReceiver> >::iterator iterReceiver;
        foreach (iterReceiver, lReceiverList_)
        {
            if (iterReceiver->second == pReceiver)
                lDeleteList.PushBack(iterReceiver);
        }

        s_ctnr<s_multimap< s_str, s_ptr<EventReceiver> >::iterator>::iterator iterDelete;
        foreach (iterDelete, lDeleteList)
        {
            lReceiverList_.Erase(*iterDelete);
        }
    }

    void EventManager::FireEvent( const Event& mEvent )
    {
        if (lReceiverList_.Find(mEvent.GetName()))
        {
            // This event is registered to one or more EventReceivers.
            // Check if this event should only be fired once per frame.
            if (!lFiredEventList_.Find(mEvent.GetName()))
            {
                if (bDebugOutput_)
                    Log("# "+CLASS_NAME+" # : "+mEvent.GetName()+" is being fired...");

                // Now, tell all these EventReceivers that this Event has occured.
                s_multimap< s_str, s_ptr<EventReceiver> >::range mRange;
                mRange = lReceiverList_.EqualRange(mEvent.GetName());
                s_multimap< s_str, s_ptr<EventReceiver> >::iterator iterReceiver;
                foreach (iterReceiver, mRange)
                {
                    iterReceiver->second->OnEvent(mEvent);
                }

                if (mEvent.IsOncePerFrame())
                    lFiredEventList_.PushBack(mEvent.GetName());

                if (bDebugOutput_)
                    Log("# "+CLASS_NAME+" # : "+mEvent.GetName()+" has been fired.");
            }
        }
    }

    void EventManager::FrameEnded()
    {
        lFiredEventList_.Clear();
    }

    void EventManager::ToggleDebugOutput()
    {
        bDebugOutput_ = !bDebugOutput_;
    }
}
