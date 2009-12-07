/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_colorselect.h"

#include "gui/frost_frame.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaColorSelect::CLASS_NAME = "GUI::LuaColorSelect";

LuaColorSelect::LuaColorSelect(lua_State* pLua) : LuaFrame(pLua)
{
    pColorSelectParent_ = s_ptr<ColorSelect>::DynamicCast(pParent_);
    if (pParent_ && !pColorSelectParent_)
    {
        throw Exception(CLASS_NAME, "Dynamic cast failed !");
    }
}
