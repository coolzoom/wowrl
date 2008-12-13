/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_editbox.h"

#include "gui/frost_gui_frame.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str EditBox::CLASS_NAME = "GUI::EditBox";

EditBox::EditBox(lua_State* luaVM) : Frame(luaVM)
{
}
