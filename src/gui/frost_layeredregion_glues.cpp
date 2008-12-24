/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_layeredregion.h"

#include "gui/frost_gui_uiobject.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaLayeredRegion::CLASS_NAME = "GUI::LuaLayeredRegion";

LuaLayeredRegion::LuaLayeredRegion(lua_State* pLua) : LuaUIObject(pLua)
{
    pLayeredRegionParent_ = (LayeredRegion*)(pParent_.Get());
}
