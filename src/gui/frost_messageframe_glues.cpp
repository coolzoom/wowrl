/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_messageframe.h"

#include "gui/frost_frame.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaMessageFrame::CLASS_NAME = "GUI::LuaMessageFrame";

LuaMessageFrame::LuaMessageFrame(lua_State* pLua) : LuaFrame(pLua)
{
    pMessageFrameParent_ = s_ptr<MessageFrame>::DynamicCast(pParent_);
    if (pParent_ && !pMessageFrameParent_)
    {
        throw Exception(CLASS_NAME, "Dynamic cast failed !");
    }
}
