/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_scrollframe.h"

#include "gui/frost_frame.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaScrollFrame::CLASS_NAME = "GUI::LuaScrollFrame";

LuaScrollFrame::LuaScrollFrame(lua_State* pLua) : LuaFrame(pLua)
{
    pScrollFrameParent_ = s_ptr<ScrollFrame>::DynamicCast(pParent_);
    if (pParent_ && !pScrollFrameParent_)
    {
        throw Exception(CLASS_NAME, "Dynamic cast failed !");
    }
}
