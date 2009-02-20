/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_layeredregion.h"

#include "gui/frost_gui_uiobject.h"
#include "frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LayeredRegion::CLASS_NAME = "GUI::LayeredRegion";

LayeredRegion::LayeredRegion() : UIObject()
{
}

LayerType LayeredRegion::GetDrawLayer()
{
    return LAYER_ARTWORK;
}

void LayeredRegion::SetDrawLayer(LayerType mLayer)
{

}
