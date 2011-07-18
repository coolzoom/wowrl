/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_layeredregion.h"

#include "frost_uiobject.h"
#include "frost_frame.h"
#include "frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LayeredRegion::CLASS_NAME = "GUI::LayeredRegion";

LayeredRegion::LayeredRegion(s_ptr<GUIManager> pManager) : Region(pManager)
{
    lType_.PushBack("LayeredRegion");
}

LayeredRegion::~LayeredRegion()
{
}

s_str LayeredRegion::Serialize( const s_str& sTab ) const
{
    s_str sStr = Region::Serialize(sTab);

    sStr << sTab << "  # Layer       : ";
    switch (mLayer_)
    {
        case LAYER_BACKGROUND : sStr << "BACKGROUND\n"; break;
        case LAYER_BORDER : sStr << "BORDER\n"; break;
        case LAYER_ARTWORK : sStr << "ARTWORK\n"; break;
        case LAYER_OVERLAY : sStr << "OVERLAY\n"; break;
        case LAYER_HIGHLIGHT : sStr << "HIGHLIGHT\n"; break;
        case LAYER_SPECIALHIGH : sStr << "SPECIALHIGH\n"; break;
        default : sStr << "<error>\n"; break;
    };

    return sStr;
}

void LayeredRegion::CreateGlue()
{
    s_ptr<Lua::State> pLua = pManager_->GetLua();
    pLua->PushString(sName_);
    lGlueList_.PushBack(pLua->PushNew<LuaLayeredRegion>());
    pLua->SetGlobal(sName_);
}

void LayeredRegion::SetParent( s_ptr<UIObject> pParent )
{
    pFrameParent_ = s_ptr<Frame>::DynamicCast(pParent);

    UIObject::SetParent(pParent);
}

void LayeredRegion::Show()
{
    if (!bIsShown_)
    {
        bIsShown_ = true;
        NotifyRendererNeedRedraw();
    }
}

void LayeredRegion::Hide()
{
    if (bIsShown_)
    {
        bIsShown_ = false;
        NotifyRendererNeedRedraw();
    }
}

s_bool LayeredRegion::IsVisible() const
{
    return pParent_->IsVisible() && bIsShown_;
}

LayerType LayeredRegion::GetDrawLayer()
{
    return mLayer_;
}

void LayeredRegion::SetDrawLayer( LayerType mLayer )
{
    if (mLayer_ != mLayer)
    {
        mLayer_ = mLayer;
        NotifyRendererNeedRedraw();
        pFrameParent_->FireBuildLayerList();
    }
}

void LayeredRegion::SetDrawLayer( const s_str& sLayer )
{
    LayerType mLayer;
    if (sLayer == "ARTWORK")
        mLayer = LAYER_ARTWORK;
    else if (sLayer == "BACKGROUND")
        mLayer = LAYER_BACKGROUND;
    else if (sLayer == "BORDER")
        mLayer = LAYER_BORDER;
    else if (sLayer == "HIGHLIGHT")
        mLayer = LAYER_HIGHLIGHT;
    else if (sLayer == "OVERLAY")
        mLayer = LAYER_OVERLAY;
    else
    {
        Warning(lType_.Back(),
            "Uknown layer type : \""+sLayer+"\". Using \"ARTWORK\"."
        );
        mLayer = LAYER_ARTWORK;
    }

    if (mLayer_ != mLayer)
    {
        mLayer_ = mLayer;
        NotifyRendererNeedRedraw();
        pFrameParent_->FireBuildLayerList();
    }
}

void LayeredRegion::NotifyRendererNeedRedraw() const
{
    if (!bVirtual_)
    {
        if (pRenderer_)
            pRenderer_->FireRedraw();
        else if (pFrameParent_)
        {
            pFrameParent_->NotifyRendererNeedRedraw();
        }
    }
}
