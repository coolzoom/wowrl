/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_scrollingmessageframe.h"

#include "gui/frost_gui_frame.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaScrollingMessageFrame::CLASS_NAME = "GUI::LuaScrollingMessageFrame";

LuaScrollingMessageFrame::LuaScrollingMessageFrame(lua_State* pLua) : LuaFrame(pLua)
{
    pSMFParent_ = s_ptr<ScrollingMessageFrame>::DynamicCast(pParent_);
}
