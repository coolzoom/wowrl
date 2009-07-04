/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_editbox.h"

#include "gui/frost_frame.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaEditBox::CLASS_NAME = "GUI::LuaEditBox";

LuaEditBox::LuaEditBox(lua_State* pLua) : LuaFrame(pLua)
{
    pEditBoxParent_ = s_ptr<EditBox>::DynamicCast(pParent_);
    if (pParent_ && !pEditBoxParent_)
    {
        Error(CLASS_NAME, "Dynamic cast failed !");
    }
}
