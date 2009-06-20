/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_slider.h"

#include "gui/frost_gui_frame.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaSlider::CLASS_NAME = "GUI::LuaSlider";

LuaSlider::LuaSlider(lua_State* pLua) : LuaFrame(pLua)
{
    pSliderParent_ = s_ptr<Slider>::DynamicCast(pParent_);
}
