/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_button.h"

#include "gui/frost_gui_frame.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaButton::CLASS_NAME = "GUI::LuaButton";

LuaButton::LuaButton(lua_State* pLua) : LuaFrame(pLua)
{
    pButtonParent_ = (Button*)(pParent_.Get());
}
