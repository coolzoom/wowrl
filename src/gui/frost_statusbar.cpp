/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_statusbar.h"

#include "gui/frost_gui_frame.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str StatusBar::CLASS_NAME = "GUI::StatusBar";

StatusBar::StatusBar(lua_State* luaVM) : Frame(luaVM)
{
}
