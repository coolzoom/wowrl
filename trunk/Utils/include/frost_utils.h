/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Utils header               */
/*                                        */
/*                                        */

/* This is the file you should include if you
*  want to use the Utils library.
*/

#ifndef FROST_UTILS_H
#define FROST_UTILS_H

#include "frost_utils_stdhelper.h"

#include "frost_utils_config.h"
#include "frost_utils_types.h"
#include "frost_utils_exception.h"
#include "frost_utils_var.h"

#include "frost_utils_math.h"

#include "frost_utils_log.h"

/*#include "frost_utilsmanager.h"

#include "frost_utils_color.h"
#include "frost_utils_vector.h"
#include "frost_utils_axisalignedbox.h"
#include "frost_utils_point.h"

#include "frost_utils_event.h"
#include "frost_utils_eventreceiver.h"
#include "frost_utils_eventmanager.h"

#include "frost_utils_timers.h"
#include "frost_utils_profiler.h"
#include "frost_utils_timemanager.h"*/

namespace Frost
{
    class UtilsManager;

    class Color;
    class Vector;
    class AxisAlignedBox;

    class Event;
    class EventReceiver;
    class EventManager;

    class Directory;
    class File;

    class PeriodicTimer;
    class Timer;
    class TimeManager;

    namespace Lua
    {
        class State;
        class Function;
    }

    class LuaManager;
}

struct lua_State;

#endif
