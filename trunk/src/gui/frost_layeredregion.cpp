/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_layeredregion.h"

#include "gui/frost_uiobject.h"
#include "gui/frost_guimanager.h"

#include "material/frost_materialmanager.h"
#include "gui/frost_sprite.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LayeredRegion::CLASS_NAME = "GUI::LayeredRegion";

LayeredRegion::LayeredRegion() : UIObject()
{
    mObjectType_ = OJBECT_TYPE_LAYEREDREGION;
    lType_.PushBack("LayeredRegion");
}

void LayeredRegion::Render()
{
}

void LayeredRegion::CreateGlue()
{
    s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
    pLua->PushString(sName_);
    lGlueList_.PushBack(
        pLua->Push<LuaLayeredRegion>(new LuaLayeredRegion(pLua->GetState()))
    );
    pLua->SetGlobal(sName_);
}

s_bool LayeredRegion::IsInRegion( const s_int& iX, const s_int& iY )
{
    return (iX.IsInRange(lBorderList_[BORDER_LEFT],lBorderList_[BORDER_RIGHT]) &&
            iY.IsInRange(lBorderList_[BORDER_TOP], lBorderList_[BORDER_BOTTOM]));
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
    else if (sLayer == "BACKGROUND")
        mLayer_ = LAYER_BACKGROUND;
    else if (sLayer == "BORDER")
        mLayer_ = LAYER_BORDER;
    else if (sLayer == "HIGHLIGHT")
        mLayer_ = LAYER_HIGHLIGHT;
    else if (sLayer == "OVERLAY")
        mLayer_ = LAYER_OVERLAY;
    else
    {
        Warning(lType_.Back(),
            "Uknown layer type : \""+sLayer+"\". Using \"ARTWORK\"."
        );
        mLayer_ = LAYER_ARTWORK;
    }
}
