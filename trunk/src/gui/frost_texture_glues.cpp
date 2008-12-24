/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_texture.h"

#include "gui/frost_gui_layeredregion.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaTexture::CLASS_NAME = "GUI::LuaTexture";

LuaTexture::LuaTexture(lua_State* pLua) : LuaLayeredRegion(pLua)
{
    pTextureParent_ = (Texture*)(pParent_.Get());
}
