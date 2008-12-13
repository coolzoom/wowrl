/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_scrollingmessageframe.h"

#include "gui/frost_gui_frame.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str ScrollingMessageFrame::CLASS_NAME = "GUI::ScrollingMessageFrame";

ScrollingMessageFrame::ScrollingMessageFrame(lua_State* luaVM) : Frame(luaVM)
{
}
