/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          EventReceiver source          */
/*                                        */
/*                                        */

#include "frost_utils_eventreceiver.h"
#include "frost_utils_eventmanager.h"

using namespace std;

namespace Frost
{
    const s_str EventReceiver::CLASS_NAME = "EventReceiver";

    EventReceiver::EventReceiver()
    {
    }

    EventReceiver::~EventReceiver()
    {
        EventManager::GetSingleton()->UnregisterReceiver(this);
    }

    void EventReceiver::RegisterEvent( const s_str& sEventName )
    {
        EventManager::GetSingleton()->RegisterEvent(this, sEventName);
    }

    void EventReceiver::UnregisterEvent( const s_str& sEventName )
    {
        EventManager::GetSingleton()->UnregisterEvent(this, sEventName);
    }
}



