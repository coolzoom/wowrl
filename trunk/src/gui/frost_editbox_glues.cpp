/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_editbox.h"

#include "gui/frost_gui_frame.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaEditBox::CLASS_NAME = "GUI::LuaEditBox";

LuaEditBox::LuaEditBox(lua_State* pLua) : LuaFrame(pLua)
{
    pEditBoxParent_ = (EditBox*)(pParent_.Get());
}
