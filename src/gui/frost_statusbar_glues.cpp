/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_statusbar.h"

#include "gui/frost_frame.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaStatusBar::CLASS_NAME = "GUI::LuaStatusBar";

LuaStatusBar::LuaStatusBar(lua_State* pLua) : LuaFrame(pLua)
{
    pStatusBarParent_ = s_ptr<StatusBar>::DynamicCast(pParent_);
    if (pParent_ && !pStatusBarParent_)
    {
        throw Exception(CLASS_NAME, "Dynamic cast failed !");
    }
}
