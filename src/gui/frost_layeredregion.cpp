/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_layeredregion.h"

#include "gui/frost_uiobject.h"
#include "gui/frost_frame.h"
#include "gui/frost_guimanager.h"

#include "material/frost_materialmanager.h"
#include "gui/frost_sprite.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LayeredRegion::CLASS_NAME = "GUI::LayeredRegion";

LayeredRegion::LayeredRegion() : Region()
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
    s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
    pLua->PushString(sName_);
    lGlueList_.PushBack(
        pLua->Push<LuaLayeredRegion>(new LuaLayeredRegion(pLua->GetState()))
    );
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
    mLayer_ = mLayer;
}

void LayeredRegion::SetDrawLayer( const s_str& sLayer )
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

void LayeredRegion::NotifyRendererNeedRedraw()
{
    if (!bVirtual_)
    {
        if (pRenderer_)
            pRenderer_->FireRedraw();
        else if (pFrameParent_)
            GUIManager::GetSingleton()->FireRedraw(pFrameParent_->GetFrameStrata());
    }
}
