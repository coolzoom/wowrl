/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_statusbar.h"

#include "gui/frost_frame.h"
#include "gui/frost_guimanager.h"
#include "gui/frost_texture.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str StatusBar::CLASS_NAME = "GUI::StatusBar";

StatusBar::StatusBar() : Frame(), mBarLayer_(LAYER_ARTWORK)
{
    mObjectType_ = OJBECT_TYPE_STATUSBAR;
    lType_.PushBack("StatusBar");
}

StatusBar::~StatusBar()
{
}

void StatusBar::SetMinValue( const s_float& fMin )
{
    fMinValue_ = fMin;
    FireUpdateBarTexture_();
}

void StatusBar::SetMaxValue( const s_float& fMax )
{
    fMaxValue_ = fMax;
    FireUpdateBarTexture_();
}

void StatusBar::SetMinMaxValues( const s_float& fMin, const s_float& fMax )
{
    fMinValue_ = fMin;
    fMaxValue_ = fMax;
    FireUpdateBarTexture_();
}

void StatusBar::SetValue( const s_float& fValue )
{
    fValue_ = fValue;
    fValue_.Clamp(fMinValue_, fMaxValue_);
    FireUpdateBarTexture_();
}

void StatusBar::SetBarDrawLayer( LayerType mBarLayer )
{
    mBarLayer_ = mBarLayer;
}

void StatusBar::SetBarDrawLayer( const s_str& sBarLayer )
{
    if (sBarLayer == "ARTWORK")
        mBarLayer_ = LAYER_ARTWORK;
    else if (sBarLayer == "BACKGROUND")
        mBarLayer_ = LAYER_BACKGROUND;
    else if (sBarLayer == "BORDER")
        mBarLayer_ = LAYER_BORDER;
    else if (sBarLayer == "HIGHLIGHT")
        mBarLayer_ = LAYER_HIGHLIGHT;
    else if (sBarLayer == "OVERLAY")
        mBarLayer_ = LAYER_OVERLAY;
    else
    {
        Warning(lType_.Back(),
            "Uknown layer type : \""+sBarLayer+"\". Using \"ARTWORK\"."
        );
        mBarLayer_ = LAYER_ARTWORK;
    }
}

void StatusBar::SetBarTexture( s_ptr<Texture> pBarTexture )
{
    pBarTexture_ = pBarTexture;
}

void StatusBar::SetBarColor( const Color& mBarColor )
{
    if (pBarTexture_)
    {
        mBarColor_ = mBarColor;
        pBarTexture_->SetColor(mBarColor_);
    }
}

const s_float& StatusBar::GetMinValue() const
{
    return fMinValue_;
}

const s_float& StatusBar::GetMaxValue() const
{
    return fMaxValue_;
}

const s_float& StatusBar::GetValue() const
{
    return fValue_;
}

LayerType StatusBar::GetBarDrawLayer() const
{
    return mBarLayer_;
}

s_ptr<Texture> StatusBar::GetBarTexture() const
{
    return pBarTexture_;
}

const Color& StatusBar::GetBarColor() const
{
    return mBarColor_;
}

s_ptr<Texture> StatusBar::CreateBarTexture_()
{
    if (!pBarTexture_)
    {
        pBarTexture_ = new Texture();
        pBarTexture_->SetSpecial();
        pBarTexture_->SetParent(this);
        pBarTexture_->SetDrawLayer(mBarLayer_);
    }

    return pBarTexture_;
}

void StatusBar::CreateGlue()
{
    s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
    pLua->PushString(sName_);
    lGlueList_.PushBack(
        pLua->Push<LuaStatusBar>(new LuaStatusBar(pLua->GetState()))
    );
    pLua->SetGlobal(sName_);
}

void StatusBar::Update()
{
    Frame::Update();

    if (bUpdateBarTexture_ && pBarTexture_)
    {
        s_float fCoef = (fValue_ - fMinValue_)/(fMaxValue_ - fMinValue_);

        pBarTexture_->SetRelWidth(fCoef);
        pBarTexture_->SetRelHeight(1.0f);

        bUpdateBarTexture_ = false;
    }
}

void StatusBar::FireUpdateBarTexture_()
{
    bUpdateBarTexture_ = true;
}
