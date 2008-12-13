/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_slider.h"

#include "gui/frost_gui_frame.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str Slider::CLASS_NAME = "GUI::Slider";

Slider::Slider(lua_State* luaVM) : Frame(luaVM)
{
}
