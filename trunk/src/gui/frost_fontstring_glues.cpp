/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_fontstring.h"

#include "gui/frost_gui_layeredregion.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaFontString::CLASS_NAME = "GUI::LuaFontString";

LuaFontString::LuaFontString(lua_State* pLua) : LuaLayeredRegion(pLua)
{
    pFontStringParent_ = (FontString*)(pParent_.Get());
}
