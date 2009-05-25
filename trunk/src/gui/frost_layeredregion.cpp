/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_layeredregion.h"

#include "gui/frost_gui_uiobject.h"
#include "gui/frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LayeredRegion::CLASS_NAME = "GUI::LayeredRegion";

LayeredRegion::LayeredRegion() : UIObject()
{
    mObjectType_ = OJBECT_TYPE_LAYEREDREGION;
    lType_.push_back("LayeredRegion");
}

LayerType LayeredRegion::GetDrawLayer()
{
    return mLayer_;
}

void LayeredRegion::SetDrawLayer(LayerType mLayer)
{
    mLayer_ = mLayer;
}

void LayeredRegion::SetDrawLayer(const s_str& sLayer)
{
    if (sLayer == "ARTWORK")
        mLayer_ = LAYER_ARTWORK;
    else if (sLayer == "BACKDROUND")
        mLayer_ = LAYER_BACKGROUND;
    else if (sLayer == "BORDER")
        mLayer_ = LAYER_BORDER;
    else if (sLayer == "HIGHLIGHT")
        mLayer_ = LAYER_HIGHLIGHT;
    else if (sLayer == "OVERLAY")
        mLayer_ = LAYER_OVERLAY;
    else
    {
        Warning(lType_.back(),
            "Uknown layer type : \""+sLayer+"\". Using \"ARTWORK\"."
        );
        mLayer_ = LAYER_ARTWORK;
    }
}
