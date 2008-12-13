/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_layeredregion.h"

#include "gui/frost_gui_uiobject.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LayeredRegion::CLASS_NAME = "GUI::LayeredRegion";

LayeredRegion::LayeredRegion(lua_State* luaVM) : UIObject(luaVM)
{
}

LayerType LayeredRegion::GetDrawLayer()
{

}

void LayeredRegion::SetDrawLayer(LayerType mLayer)
{

}
