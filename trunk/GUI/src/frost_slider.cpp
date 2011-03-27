/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_slider.h"

#include "frost_frame.h"
#include "frost_guimanager.h"
#include "frost_texture.h"

#include <frost_utils_event.h>

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str Slider::CLASS_NAME = "GUI::Slider";

Slider::Slider(s_ptr<GUIManager> pManager) : Frame(pManager), mOrientation_(ORIENT_VERTICAL), mThumbLayer_(LAYER_OVERLAY)
{
    lType_.PushBack("Slider");

    bAllowClicksOutsideThumb_ = true;
}

Slider::~Slider()
{
}

s_str Slider::Serialize( const s_str& sTab ) const
{
    return Frame::Serialize(sTab);
}

s_bool Slider::CanUseScript( const s_str& sScriptName ) const
{
    if (Frame::CanUseScript(sScriptName))
        return true;
    else if (sScriptName == "OnValueChanged")
        return true;
    else
        return false;
}

void Slider::CopyFrom( s_ptr<UIObject> pObj )
{
    Frame::CopyFrom(pObj);

    s_ptr<Slider> pSlider = s_ptr<Slider>::DynamicCast(pObj);

    if (pSlider)
    {
        this->SetValueStep(pSlider->GetValueStep());
        this->SetMinValue(pSlider->GetMinValue());
        this->SetMaxValue(pSlider->GetMaxValue());
        this->SetValue(pSlider->GetValue());
        this->SetThumbDrawLayer(pSlider->GetThumbDrawLayer());
        this->SetOrientation(pSlider->GetOrientation());
        this->SetAllowClicksOutsideThumb(pSlider->AreClicksOutsideThumbAllowed());

        s_ptr<Texture> pThumb = pSlider->GetThumbTexture();
        if (pThumb)
        {
            this->CreateThumbTexture_();
            if (this->IsVirtual())
                pThumbTexture_->SetVirtual();
            pThumbTexture_->SetName(pThumb->GetName());
            if (!pManager_->AddUIObject(pThumbTexture_))
            {
                Warning(lType_.Back(),
                    "Trying to add \""+pThumb->GetName()+"\" to \""+sName_+"\",\n"
                    "but its name was already taken : \""+pThumbTexture_->GetName()+"\". Skipped."
                );
                pThumbTexture_.Delete();
            }
            else
            {
                if (!IsVirtual())
                    pThumbTexture_->CreateGlue();
                this->AddRegion(pThumbTexture_);
                pThumbTexture_->CopyFrom(pThumb);
                pThumbTexture_->NotifyLoaded();
            }
        }
    }
}

void Slider::OnEvent( const Event& mEvent )
{
    Frame::OnEvent(mEvent);

    if (bIsMouseEnabled_ && pManager_->IsEnabled())
    {
        if (mEvent.GetName() == "MOUSE_PRESSED")
        {
            if (bMouseInThumb_)
            {
                pManager_->StartMoving(
                    pThumbTexture_, pThumbTexture_->ModifyPoint(ANCHOR_CENTER),
                    mOrientation_ == ORIENT_HORIZONTAL ? Vector::CONSTRAINT_X : Vector::CONSTRAINT_Y
                );
                bThumbMoved_ = true;
            }
            else if (bMouseInFrame_ && bAllowClicksOutsideThumb_)
            {
                s_float fValue;
                if (mOrientation_ == ORIENT_HORIZONTAL)
                {
                    s_float fOffset = s_float(iMousePosX_ - lBorderList_[BORDER_LEFT]);
                    fValue = fOffset/s_float(uiAbsWidth_);
                    SetValue(fValue*(fMaxValue_ - fMinValue_) + fMinValue_);
                }
                else
                {
                    s_float fOffset = s_float(iMousePosY_ - lBorderList_[BORDER_TOP]);
                    fValue = fOffset/s_float(uiAbsHeight_);
                    SetValue(fValue*(fMaxValue_ - fMinValue_) + fMinValue_);
                }

                if (pThumbTexture_)
                {
                    s_float fCoef = (fValue_ - fMinValue_)/(fMaxValue_ - fMinValue_);

                    s_ptr<Anchor> pAnchor = pThumbTexture_->ModifyPoint(ANCHOR_CENTER);
                    if (mOrientation_ == ORIENT_HORIZONTAL)
                        pAnchor->SetAbsOffset(s_int(s_float(uiAbsWidth_)*fCoef), 0);
                    else
                        pAnchor->SetAbsOffset(0, s_int(s_float(uiAbsHeight_)*fCoef));

                    pManager_->StartMoving(
                        pThumbTexture_, pThumbTexture_->ModifyPoint(ANCHOR_CENTER),
                        mOrientation_ == ORIENT_HORIZONTAL ? Vector::CONSTRAINT_X : Vector::CONSTRAINT_Y
                    );
                    bThumbMoved_ = true;
                }
            }
        }
        else if (mEvent.GetName() == "MOUSE_RELEASED")
        {
            if (pThumbTexture_)
            {
                pManager_->StopMoving(pThumbTexture_);
                bThumbMoved_ = false;
            }
        }
    }
}

void StepValue( s_float& fValue, const s_float& fStep )
{
    // Makes the value a multiple of the step :
    // fValue = N*fStep, where N is an integer.
    if (!fStep.IsNull())
        fValue = s_float::Round(fValue/fStep)*fStep;
}

void Slider::SetMinValue( const s_float& fMin )
{
    if (fMin != fMinValue_)
    {
        fMinValue_ = fMin;
        if (fMinValue_ > fMaxValue_) fMinValue_ = fMaxValue_;
        else StepValue(fMinValue_, fValueStep_);

        if (fValue_ < fMinValue_)
        {
            fValue_ = fMinValue_;
            lQueuedEventList_.PushBack("ValueChanged");
        }

        FireUpdateThumbTexture_();
    }
}

void Slider::SetMaxValue( const s_float& fMax )
{
    if (fMax != fMaxValue_)
    {
        fMaxValue_ = fMax;
        if (fMaxValue_ < fMaxValue_) fMaxValue_ = fMinValue_;
        else StepValue(fMaxValue_, fValueStep_);

        if (fValue_ > fMaxValue_)
        {
            fValue_ = fMaxValue_;
            lQueuedEventList_.PushBack("ValueChanged");
        }

        FireUpdateThumbTexture_();
    }
}

void Slider::SetMinMaxValues( const s_float& fMin, const s_float& fMax )
{
    if (fMin != fMinValue_ || fMax != fMaxValue_)
    {
        fMinValue_ = s_float::Min(fMin, fMax);
        fMaxValue_ = s_float::Max(fMin, fMax);
        StepValue(fMinValue_, fValueStep_);
        StepValue(fMaxValue_, fValueStep_);
        if (fValue_ > fMaxValue_ || fValue_ < fMinValue_)
        {
            fValue_.Clamp(fMinValue_, fMaxValue_);
            lQueuedEventList_.PushBack("ValueChanged");
        }

        FireUpdateThumbTexture_();
    }
}

void Slider::SetValue( const s_float& fValue, const s_bool& bSilent )
{
    if (fValue != fValue_)
    {
        fValue_ = fValue;
        fValue_.Clamp(fMinValue_, fMaxValue_);
        StepValue(fValue_, fValueStep_);
        if (!bSilent)
            lQueuedEventList_.PushBack("ValueChanged");

        FireUpdateThumbTexture_();
    }
}

void Slider::SetValueStep( const s_float& fValueStep )
{
    if (fValueStep_ != fValueStep)
    {
        fValueStep_ = fValueStep;

        StepValue(fMinValue_, fValueStep_);
        StepValue(fMaxValue_, fValueStep_);
        s_float fOldValue = fValue_;
        StepValue(fValue_,    fValueStep_);
        fValue_.Clamp(fMinValue_, fMaxValue_);
        if (fValue_ != fOldValue)
            lQueuedEventList_.PushBack("ValueChanged");

        FireUpdateThumbTexture_();
    }
}

void Slider::SetThumbTexture( s_ptr<Texture> pTexture )
{
    pThumbTexture_ = pTexture;
    pThumbTexture_->ClearAllPoints();
    pThumbTexture_->SetPoint(Anchor(
        pThumbTexture_, ANCHOR_CENTER, sName_,
        mOrientation_ == ORIENT_HORIZONTAL ? ANCHOR_LEFT : ANCHOR_TOP
    ));

    FireUpdateThumbTexture_();
}

void Slider::SetOrientation( Orientation mOrientation )
{
    if (mOrientation != mOrientation_)
    {
        mOrientation_ = mOrientation;
        if (pThumbTexture_)
        {
            pThumbTexture_->SetPoint(Anchor(
                pThumbTexture_, ANCHOR_TOPLEFT, sName_,
                mOrientation_ == ORIENT_HORIZONTAL ? ANCHOR_LEFT : ANCHOR_TOP
            ));
        }

        FireUpdateThumbTexture_();
    }
}

void Slider::SetThumbDrawLayer( LayerType mThumbLayer )
{
    mThumbLayer_ = mThumbLayer;
    if (pThumbTexture_)
        pThumbTexture_->SetDrawLayer(mThumbLayer_);
}

void Slider::SetThumbDrawLayer( const s_str& sThumbLayer )
{
    if (sThumbLayer == "ARTWORK")
        mThumbLayer_ = LAYER_ARTWORK;
    else if (sThumbLayer == "BACKGROUND")
        mThumbLayer_ = LAYER_BACKGROUND;
    else if (sThumbLayer == "BORDER")
        mThumbLayer_ = LAYER_BORDER;
    else if (sThumbLayer == "HIGHLIGHT")
        mThumbLayer_ = LAYER_HIGHLIGHT;
    else if (sThumbLayer == "OVERLAY")
        mThumbLayer_ = LAYER_OVERLAY;
    else
    {
        Warning(lType_.Back(),
            "Uknown layer type : \""+sThumbLayer+"\". Using \"OVERLAY\"."
        );
        mThumbLayer_ = LAYER_OVERLAY;
    }

    if (pThumbTexture_)
        pThumbTexture_->SetDrawLayer(mThumbLayer_);
}

const s_float& Slider::GetMinValue() const
{
    return fMinValue_;
}

const s_float& Slider::GetMaxValue() const
{
    return fMaxValue_;
}

const s_float& Slider::GetValue() const
{
    return fValue_;
}

const s_float& Slider::GetValueStep() const
{
    return fValueStep_;
}

s_ptr<Texture> Slider::GetThumbTexture() const
{
    return pThumbTexture_;
}

Slider::Orientation Slider::GetOrientation() const
{
    return mOrientation_;
}

LayerType Slider::GetThumbDrawLayer() const
{
    return mThumbLayer_;
}

void Slider::SetAllowClicksOutsideThumb( const s_bool& bAllow )
{
    bAllowClicksOutsideThumb_ = bAllow;
}

const s_bool& Slider::AreClicksOutsideThumbAllowed()
{
    return bAllowClicksOutsideThumb_;
}

s_ptr<Texture> Slider::CreateThumbTexture_()
{
    if (!pThumbTexture_)
    {
        pThumbTexture_ = new Texture(pManager_);
        pThumbTexture_->SetSpecial();
        pThumbTexture_->SetParent(this);
        pThumbTexture_->SetDrawLayer(mThumbLayer_);
    }

    return pThumbTexture_;
}

s_bool Slider::IsInFrame( const s_int& iX, const s_int& iY ) const
{
    if (bAllowClicksOutsideThumb_)
    {
        if (pThumbTexture_)
            return Frame::IsInFrame(iX, iY) || pThumbTexture_->IsInRegion(iX, iY);
        else
            return Frame::IsInFrame(iX, iY);
    }
    else
    {
        if (pThumbTexture_)
            return pThumbTexture_->IsInRegion(iX, iY);
        else
            return false;
    }
}

void Slider::NotifyMouseInFrame( const s_bool& bMouseInFrame, const s_int& iX, const s_int& iY )
{
    if (bAllowClicksOutsideThumb_)
        Frame::NotifyMouseInFrame(bMouseInFrame, iX, iY);

    bMouseInThumb_ = (bMouseInFrame && pThumbTexture_ && pThumbTexture_->IsInRegion(iX, iY));
}

void Slider::Update()
{
    if ( (bUpdateThumbTexture_ || bThumbMoved_) && pThumbTexture_ )
    {
        if ((mOrientation_ == ORIENT_HORIZONTAL && uiAbsWidth_.IsValid()) ||
            (mOrientation_ == ORIENT_VERTICAL   && uiAbsHeight_.IsValid()))
        {
            s_float fOldValue = fValue_;

            if (bThumbMoved_)
            {
                if (mOrientation_ == ORIENT_HORIZONTAL)
                    fValue_ = s_float(pThumbTexture_->GetPoint(ANCHOR_CENTER)->GetAbsOffsetX())/s_float(uiAbsWidth_);
                else
                    fValue_ = s_float(pThumbTexture_->GetPoint(ANCHOR_CENTER)->GetAbsOffsetY())/s_float(uiAbsHeight_);

                fValue_ *= (fMaxValue_ - fMinValue_);
                fValue_ += fMinValue_;
                fValue_.Clamp(fMinValue_, fMaxValue_);
                StepValue(fValue_, fValueStep_);

                if (fValue_ != fOldValue)
                    lQueuedEventList_.PushBack("ValueChanged");
            }

            s_float fCoef = (fValue_ - fMinValue_)/(fMaxValue_ - fMinValue_);

            s_ptr<Anchor> pAnchor = pThumbTexture_->ModifyPoint(ANCHOR_CENTER);
            if (mOrientation_ == ORIENT_HORIZONTAL)
                pAnchor->SetAbsOffset(s_int(s_float(uiAbsWidth_)*fCoef), 0);
            else
                pAnchor->SetAbsOffset(0, s_int(s_float(uiAbsHeight_)*fCoef));


            bUpdateThumbTexture_ = false;
        }
    }

    Frame::Update();
}

void Slider::FireUpdateBorders()
{
    Frame::FireUpdateBorders();
    FireUpdateThumbTexture_();
}

void Slider::CreateGlue()
{
    if (bVirtual_)
    {
        s_ptr<Lua::State> pLua = pManager_->GetLua();
        pLua->PushNumber(uiID_);
        lGlueList_.PushBack(pLua->PushNew<LuaVirtualGlue>());
        pLua->SetGlobal(sLuaName_);
    }
    else
    {
        s_ptr<Lua::State> pLua = pManager_->GetLua();
        pLua->PushString(sLuaName_);
        lGlueList_.PushBack(pLua->PushNew<LuaSlider>());
        pLua->SetGlobal(sLuaName_);
    }
}

void Slider::FireUpdateThumbTexture_()
{
    bUpdateThumbTexture_ = true;
}
