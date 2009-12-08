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

StatusBar::StatusBar() : Frame(), mOrientation_(ORIENT_HORIZONTAL), mBarLayer_(LAYER_ARTWORK)
{
    lType_.PushBack("StatusBar");
}

StatusBar::~StatusBar()
{
}

s_bool StatusBar::CanUseScript( const s_str& sScriptName ) const
{
    if ((sScriptName == "OnValueChanged") ||
        (sScriptName == "OnDragStart") ||
        (sScriptName == "OnDragStop") ||
        (sScriptName == "OnEnter") ||
        (sScriptName == "OnEvent") ||
        (sScriptName == "OnHide") ||
        (sScriptName == "OnKeyDown") ||
        (sScriptName == "OnKeyUp") ||
        (sScriptName == "OnLeave") ||
        (sScriptName == "OnLoad") ||
        (sScriptName == "OnMouseDown") ||
        (sScriptName == "OnMouseUp") ||
        (sScriptName == "OnMouseWheel") ||
        (sScriptName == "OnReceiveDrag") ||
        (sScriptName == "OnShow") ||
        (sScriptName == "OnSizeChanged") ||
        (sScriptName == "OnUpdate"))
        return true;
    else
        return false;
}

void StatusBar::CopyFrom( s_ptr<UIObject> pObj )
{
    Frame::CopyFrom(pObj);

    s_ptr<StatusBar> pStatusBar = s_ptr<StatusBar>::DynamicCast(pObj);

    if (pStatusBar)
    {
        this->SetMinValue(pStatusBar->GetMinValue());
        this->SetMaxValue(pStatusBar->GetMaxValue());
        this->SetValue(pStatusBar->GetValue());
        this->SetBarDrawLayer(pStatusBar->GetBarDrawLayer());

        CreateBarTexture_();

        Anchor mAnchor(pBarTexture_, ANCHOR_BOTTOMLEFT, "$parent", ANCHOR_BOTTOMLEFT);

        pBarTexture_->CopyFrom(pStatusBar->GetBarTexture());
        pBarTexture_->ClearAllPoints();
        pBarTexture_->SetPoint(mAnchor);

        GUIManager::GetSingleton()->AddUIObject(pBarTexture_);
    }
}

void StatusBar::SetMinValue( const s_float& fMin )
{
    if (fMin != fMinValue_)
    {
        fMinValue_ = fMin;
        if (fMinValue_ > fMaxValue_) fMinValue_ = fMaxValue_;
        fValue_.Clamp(fMinValue_, fMaxValue_);
        FireUpdateBarTexture_();
    }
}

void StatusBar::SetMaxValue( const s_float& fMax )
{
    if (fMax != fMaxValue_)
    {
        fMaxValue_ = fMax;
        if (fMaxValue_ < fMaxValue_) fMaxValue_ = fMinValue_;
        fValue_.Clamp(fMinValue_, fMaxValue_);
        FireUpdateBarTexture_();
    }
}

void StatusBar::SetMinMaxValues( const s_float& fMin, const s_float& fMax )
{
    if (fMin != fMinValue_ || fMax != fMaxValue_)
    {
        fMinValue_ = s_float::Min(fMin, fMax);
        fMaxValue_ = s_float::Max(fMin, fMax);
        fValue_.Clamp(fMinValue_, fMaxValue_);
        FireUpdateBarTexture_();
    }
}

void StatusBar::SetValue( const s_float& fValue )
{
    if (fValue != fValue_)
    {
        fValue_ = fValue;
        fValue_.Clamp(fMinValue_, fMaxValue_);
        FireUpdateBarTexture_();
    }
}

void StatusBar::SetBarDrawLayer( LayerType mBarLayer )
{
    mBarLayer_ = mBarLayer;
    pBarTexture_->SetDrawLayer(mBarLayer_);
}

void StatusBar::SetBarDrawLayer( const s_str& sBarLayer )
{
    if (pBarTexture_)
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
        pBarTexture_->SetDrawLayer(mBarLayer_);
    }
}

void StatusBar::SetBarTexture( s_ptr<Texture> pBarTexture )
{
    pBarTexture_ = pBarTexture;
    pBarTexture_->ClearAllPoints();
    pBarTexture_->SetPoint(Anchor(pBarTexture_, ANCHOR_BOTTOMLEFT, sName_, ANCHOR_BOTTOMLEFT));
    FireUpdateBarTexture_();
}

void StatusBar::SetBarColor( const Color& mBarColor )
{
    if (pBarTexture_)
    {
        mBarColor_ = mBarColor;
        pBarTexture_->SetColor(mBarColor_);
    }
}

void StatusBar::SetOrientation( const Orientation& mOrient )
{
    if (mOrient != mOrientation_)
    {
        mOrientation_ = mOrient;
        FireUpdateBarTexture_();
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

StatusBar::Orientation StatusBar::GetOrientation() const
{
    return mOrientation_;
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
    if (bVirtual_)
    {
        s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
        pLua->NewTable();
        pLua->SetGlobal(sLuaName_);
    }
    else
    {
        s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
        pLua->PushString(sName_);
        lGlueList_.PushBack(
            pLua->Push<LuaStatusBar>(new LuaStatusBar(pLua->GetState()))
        );
        pLua->SetGlobal(sLuaName_);
    }
}

void StatusBar::Update()
{
    Frame::Update();

    if (bUpdateBarTexture_ && pBarTexture_)
    {
        s_float fCoef = (fValue_ - fMinValue_)/(fMaxValue_ - fMinValue_);

        if (mOrientation_ == ORIENT_HORIZONTAL)
        {
            pBarTexture_->SetRelWidth(fCoef);
            pBarTexture_->SetRelHeight(1.0f);
        }
        else
        {
            pBarTexture_->SetRelWidth(1.0f);
            pBarTexture_->SetRelHeight(fCoef);
        }

        bUpdateBarTexture_ = false;
    }
}

void StatusBar::FireUpdateBarTexture_()
{
    bUpdateBarTexture_ = true;
}
