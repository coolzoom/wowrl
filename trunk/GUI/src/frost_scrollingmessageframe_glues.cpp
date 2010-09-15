/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_scrollingmessageframe.h"

#include "frost_frame.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaScrollingMessageFrame::CLASS_NAME = "GUI::LuaScrollingMessageFrame";

LuaScrollingMessageFrame::LuaScrollingMessageFrame(lua_State* pLua) : LuaFrame(pLua)
{
    pSMFParent_ = s_ptr<ScrollingMessageFrame>::DynamicCast(pParent_);
    if (pParent_ && !pSMFParent_)
    {
        throw Exception(CLASS_NAME, "Dynamic cast failed !");
    }
}
