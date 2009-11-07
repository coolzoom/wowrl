/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_slider.h"

#include "gui/frost_frame.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaSlider::CLASS_NAME = "GUI::LuaSlider";

LuaSlider::LuaSlider(lua_State* pLua) : LuaFrame(pLua)
{
    pSliderParent_ = s_ptr<Slider>::DynamicCast(pParent_);
    if (pParent_ && !pSliderParent_)
    {
        throw Exception(CLASS_NAME, "Dynamic cast failed !");
    }
}
