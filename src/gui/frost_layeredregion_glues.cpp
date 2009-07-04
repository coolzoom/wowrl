/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_layeredregion.h"

#include "gui/frost_uiobject.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaLayeredRegion::CLASS_NAME = "GUI::LuaLayeredRegion";

LuaLayeredRegion::LuaLayeredRegion(lua_State* pLua) : LuaUIObject(pLua)
{
    pLayeredRegionParent_ = s_ptr<LayeredRegion>::DynamicCast(pParent_);
    if (pParent_ && !pLayeredRegionParent_)
    {
        Error(CLASS_NAME, "Dynamic cast failed !");
    }
}
