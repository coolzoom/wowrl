/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_checkbutton.h"

#include "gui/frost_frame.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaCheckButton::CLASS_NAME = "GUI::LuaCheckButton";

LuaCheckButton::LuaCheckButton(lua_State* pLua) : LuaButton(pLua)
{
    pCheckButtonParent_ = s_ptr<CheckButton>::DynamicCast(pParent_);
    if (pParent_ && !pCheckButtonParent_)
    {
        throw Exception(CLASS_NAME, "Dynamic cast failed !");
    }
}
