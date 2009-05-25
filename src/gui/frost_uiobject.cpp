/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_uiobject.h"

#include "gui/frost_gui_frame.h"
#include "gui/frost_guimanager.h"
#include "frost_material.h"
#include "frost_spritemanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str UIObject::CLASS_NAME = "GUI::UIObject";

UIObject::UIObject()
{
    mObjectType_ = OJBECT_TYPE_UIOBJECT;
    lType_.push_back("UIObject");

    fAlpha_ = 1.0f;
    uiAbsWidth_ = s_uint::NaN;
    uiAbsHeight_ = s_uint::NaN;
    bIsWidthAbs_ = true;
    bIsHeightAbs_ = true;
    fRelWidth_ = s_float::NaN;
    fRelHeight_ = s_float::NaN;
    bIsShown_ = true;
    bUpdateBorders_ = true;
    uiID_ = s_uint::NaN;
    bReady_ = true;
}

UIObject::~UIObject()
{
    GUIManager::GetSingleton()->RemoveUIObject(this);
    pGlue_.Delete();
}

s_ptr<LuaUIObject> UIObject::GetGlue()
{
    return pGlue_;
}

s_refptr<Material> UIObject::GetMaterial()
{
    return pMaterial_;
}

s_str UIObject::Serialize( const s_str& sTab ) const
{
    s_str sStr;

    sStr << sTab << "  # Name        : "        << sName_ << " ("+bReady_.GetAsString("ready", "not ready")+")\n";
    sStr << sTab << "  # ID          : "        << uiID_ << "\n";
    sStr << sTab << "  # Type        : "        << lType_.back() << "\n";
    if (pParent_)
    sStr << sTab << "  # Parent      : "  << pParent_->GetName() << "\n";
    else
    sStr << sTab << "  # Parent      : none\n";
    sStr << sTab << "  # Num anchors : " << s_int((int)lAnchorList_.size()) << "\n";
    if (!lAnchorList_.empty())
    {
        sStr << sTab << "  |-###\n";
        map<AnchorPoint, Anchor>::const_iterator iterAnchor;
        foreach (iterAnchor, lAnchorList_)
        {
            sStr << iterAnchor->second.Serialize(sTab);
            sStr << sTab << "  |-###\n";
        }
    }
    sStr << sTab << "  # Borders :\n";
    sStr << sTab << "  |-###\n";
    sStr << sTab << "  |   # left   : " << lBorderList_[BORDER_LEFT] << "\n";
    sStr << sTab << "  |   # top    : " << lBorderList_[BORDER_TOP] << "\n";
    sStr << sTab << "  |   # right  : " << lBorderList_[BORDER_RIGHT] << "\n";
    sStr << sTab << "  |   # bottom : " << lBorderList_[BORDER_BOTTOM] << "\n";
    sStr << sTab << "  |-###\n";
    sStr << sTab << "  # Alpha       : "       << fAlpha_ << "\n";
    sStr << sTab << "  # Shown       : "       << bIsShown_ << "\n";
    sStr << sTab << "  # Abs width   : "  << uiAbsWidth_ << "\n";
    sStr << sTab << "  # Abs height  : "  << uiAbsHeight_ << "\n";
    sStr << sTab << "  # Rel width   : "  << fRelWidth_ << "\n";
    sStr << sTab << "  # Rel height  : "  << fRelHeight_ << "\n";

    return sStr;
}

void UIObject::CopyFrom( s_ptr<UIObject> pObj )
{
    this->SetAlpha(pObj->GetAlpha());
    this->SetShown(pObj->IsShown());
    if (pObj->IsWidthAbsolute())
        this->SetAbsWidth(pObj->GetAbsWidth());
    else
        this->SetRelWidth(pObj->GetRelWidth());
    if (pObj->IsHeightAbsolute())
        this->SetAbsHeight(pObj->GetAbsHeight());
    else
        this->SetRelHeight(pObj->GetRelHeight());

    for (s_uint i = 1; i <= pObj->GetNumPoint(); i++)
    {
        s_ptr<Anchor> pAnchor = pObj->GetPoint(i);
        if (pAnchor)
        {
            Anchor mAnchor(this, pAnchor->GetPoint(), NULL, pAnchor->GetParentPoint());
            mAnchor.SetAbsOffset(pAnchor->GetAbsOffsetX(), pAnchor->GetAbsOffsetY());
            mAnchor.SetParentRawName(pAnchor->GetParentRawName());
            this->SetPoint(mAnchor);
        }
    }
}

const s_str& UIObject::GetName() const
{
    return sName_;
}

void UIObject::SetName( const s_str& sName )
{
    if (sName_.IsEmpty())
    {
        sName_ = sName;
        if (pParent_ && !bVirtual_)
        {
            sName_.Replace("$parent", pParent_->GetName());
        }
    }
    else
    {
        Warning(lType_.back(),
            "SetName() can only be called once."
        );
    }
}

const s_str& UIObject::GetObjectType() const
{
    return lType_.back();
}

const vector<s_str>& UIObject::GetObjectTypeList() const
{
    return lType_;
}

const s_float& UIObject::GetAlpha() const
{
    return fAlpha_;
}

void UIObject::SetAlpha( const s_float& fAlpha )
{
    fAlpha_ = fAlpha;
}

void UIObject::Show()
{
    bIsShown_ = true;
}

void UIObject::Hide()
{
    bIsShown_ = false;
}

void UIObject::SetShown( const s_bool& bIsShown )
{
    bIsShown_ = bIsShown;
}

const s_bool& UIObject::IsShown() const
{
    return bIsShown_;
}

s_bool UIObject::IsVisible() const
{
    if (pParent_ != NULL)
    {
        if (!IsShown())
            return false;
        else
            return pParent_->IsVisible();
    }
    else
        return IsShown();
}

void UIObject::SetAbsWidth( const s_uint& uiAbsWidth )
{
    bIsWidthAbs_ = true;
    uiAbsWidth_ = uiAbsWidth;
    FireUpdateDimensions();
}

void UIObject::SetAbsHeight( const s_uint& uiAbsHeight )
{
    bIsHeightAbs_ = true;
    uiAbsHeight_ = uiAbsHeight;
    FireUpdateDimensions();
}

const s_bool& UIObject::IsWidthAbsolute() const
{
    return bIsWidthAbs_;
}

const s_bool& UIObject::IsHeightAbsolute() const
{
    return bIsHeightAbs_;
}

void UIObject::SetRelWidth( const s_float& fRelWidth )
{
    bIsWidthAbs_ = false;
    fRelWidth_ = fRelWidth;
    FireUpdateDimensions();
}

void UIObject::SetRelHeight( const s_float& fRelHeight )
{
    bIsHeightAbs_ = false;
    fRelHeight_ = fRelHeight;
    FireUpdateDimensions();
}

const s_uint& UIObject::GetAbsWidth() const
{
    return uiAbsWidth_;
}

const s_uint& UIObject::GetAbsHeight() const
{
    return uiAbsHeight_;
}

const s_float& UIObject::GetRelWidth() const
{
    return fRelWidth_;
}

const s_float& UIObject::GetRelHeight() const
{
    return fRelHeight_;
}

void UIObject::SetParent( s_ptr<UIObject> pParent )
{
    if (pParent != this)
    {
        pParent_ = pParent;
        FireUpdateDimensions();
    }
    else
    {
        Error(lType_.back(),
            "Can't call SetParent(this)."
        );
    }
}

s_ptr<UIObject> UIObject::GetParent()
{
    return pParent_;
}

const s_int& UIObject::GetBottom() const
{
    return lBorderList_[BORDER_BOTTOM];
}

Point<s_int> UIObject::GetCenter() const
{
    return Point<s_int>(
        (lBorderList_[BORDER_LEFT] + s_int(uiAbsWidth_/2u)).Get(),
        (lBorderList_[BORDER_TOP] + s_int(uiAbsHeight_/2u)).Get()
    );
}

const s_int& UIObject::GetLeft() const
{
    return lBorderList_[BORDER_LEFT];
}

const s_int& UIObject::GetRight() const
{
    return lBorderList_[BORDER_RIGHT];
}

const s_int& UIObject::GetTop() const
{
    return lBorderList_[BORDER_TOP];
}

void UIObject::ClearAllPoints()
{
    lAnchorList_.clear();

    lDefinedBorderList_[BORDER_LEFT]   =
    lDefinedBorderList_[BORDER_TOP]    =
    lDefinedBorderList_[BORDER_RIGHT]  =
    lDefinedBorderList_[BORDER_BOTTOM] = false;
}

void UIObject::SetAllPoints( s_ptr<UIObject> pObj )
{
    if (pObj != this)
    {
        ClearAllPoints();

        Anchor mAnchor = Anchor(this, ANCHOR_TOPLEFT, pObj, ANCHOR_TOPLEFT);
        lAnchorList_[ANCHOR_TOPLEFT] = mAnchor;

        mAnchor = Anchor(this, ANCHOR_BOTTOMRIGHT, pObj, ANCHOR_BOTTOMRIGHT);
        lAnchorList_[ANCHOR_BOTTOMRIGHT] = mAnchor;

        lDefinedBorderList_[BORDER_LEFT]   =
        lDefinedBorderList_[BORDER_TOP]    =
        lDefinedBorderList_[BORDER_RIGHT]  =
        lDefinedBorderList_[BORDER_BOTTOM] = true;

        FireUpdateBorders();
    }
    else
    {
        Error(lType_.back(),
            "Can't call SetAllPoints(this)."
        );
    }
}

void UIObject::SetAbsPoint( AnchorPoint mPoint, s_ptr<UIObject> pObj, AnchorPoint mRelativePoint, const s_int& iX, const s_int& iY )
{
    map<AnchorPoint, Anchor>::iterator iterAnchor = lAnchorList_.find(mPoint);
    if (iterAnchor == lAnchorList_.end())
    {
        Anchor mAnchor = Anchor(this, mPoint, pObj, mRelativePoint);
        mAnchor.SetAbsOffset(iX, iY);
        lAnchorList_[mPoint] = mAnchor;
    }
    else
    {
        s_ptr<Anchor> pAnchor = &iterAnchor->second;
        pAnchor->SetParent(pObj);
        pAnchor->SetParentPoint(mRelativePoint);
        pAnchor->SetAbsOffset(iX, iY);
    }

    switch (mPoint)
    {
        case ANCHOR_TOPLEFT :
            lDefinedBorderList_[BORDER_TOP] = true;
            lDefinedBorderList_[BORDER_LEFT] = true;
            break;
        case ANCHOR_TOP :
            lDefinedBorderList_[BORDER_TOP] = true;
            break;
        case ANCHOR_TOPRIGHT :
            lDefinedBorderList_[BORDER_TOP] = true;
            lDefinedBorderList_[BORDER_RIGHT] = true;
            break;
        case ANCHOR_RIGHT :
            lDefinedBorderList_[BORDER_RIGHT] = true;
            break;
        case ANCHOR_BOTTOMRIGHT :
            lDefinedBorderList_[BORDER_BOTTOM] = true;
            lDefinedBorderList_[BORDER_RIGHT] = true;
            break;
        case ANCHOR_BOTTOM :
            lDefinedBorderList_[BORDER_BOTTOM] = true;
            break;
        case ANCHOR_BOTTOMLEFT :
            lDefinedBorderList_[BORDER_BOTTOM] = true;
            lDefinedBorderList_[BORDER_LEFT] = true;
            break;
        case ANCHOR_LEFT :
            lDefinedBorderList_[BORDER_LEFT] = true;
            break;
        default : break;
    }

    FireUpdateBorders();
}

void UIObject::SetRelPoint( AnchorPoint mPoint, s_ptr<UIObject> pObj, AnchorPoint mRelativePoint, const s_float& fX, const s_float& fY )
{
    map<AnchorPoint, Anchor>::iterator iterAnchor = lAnchorList_.find(mPoint);
    if (iterAnchor == lAnchorList_.end())
    {
        Anchor mAnchor = Anchor(this, mPoint, pObj, mRelativePoint);
        mAnchor.SetRelOffset(fX, fY);
        lAnchorList_[mPoint] = mAnchor;
    }
    else
    {
        s_ptr<Anchor> pAnchor = &iterAnchor->second;
        pAnchor->SetParent(pObj);
        pAnchor->SetParentPoint(mRelativePoint);
        pAnchor->SetRelOffset(fX, fY);
    }

    switch (mPoint)
    {
        case ANCHOR_TOPLEFT :
            lDefinedBorderList_[BORDER_TOP] = true;
            lDefinedBorderList_[BORDER_LEFT] = true;
            break;
        case ANCHOR_TOP :
            lDefinedBorderList_[BORDER_TOP] = true;
            break;
        case ANCHOR_TOPRIGHT :
            lDefinedBorderList_[BORDER_TOP] = true;
            lDefinedBorderList_[BORDER_RIGHT] = true;
            break;
        case ANCHOR_RIGHT :
            lDefinedBorderList_[BORDER_RIGHT] = true;
            break;
        case ANCHOR_BOTTOMRIGHT :
            lDefinedBorderList_[BORDER_BOTTOM] = true;
            lDefinedBorderList_[BORDER_RIGHT] = true;
            break;
        case ANCHOR_BOTTOM :
            lDefinedBorderList_[BORDER_BOTTOM] = true;
            break;
        case ANCHOR_BOTTOMLEFT :
            lDefinedBorderList_[BORDER_BOTTOM] = true;
            lDefinedBorderList_[BORDER_LEFT] = true;
            break;
        case ANCHOR_LEFT :
            lDefinedBorderList_[BORDER_LEFT] = true;
            break;
        default : break;
    }

    FireUpdateBorders();
}

void UIObject::SetPoint( const Anchor& mAnchor )
{
    lAnchorList_[mAnchor.GetPoint()] = mAnchor;
    switch (mAnchor.GetPoint())
    {
        case ANCHOR_TOPLEFT :
            lDefinedBorderList_[BORDER_TOP] = true;
            lDefinedBorderList_[BORDER_LEFT] = true;
            break;
        case ANCHOR_TOP :
            lDefinedBorderList_[BORDER_TOP] = true;
            break;
        case ANCHOR_TOPRIGHT :
            lDefinedBorderList_[BORDER_TOP] = true;
            lDefinedBorderList_[BORDER_RIGHT] = true;
            break;
        case ANCHOR_RIGHT :
            lDefinedBorderList_[BORDER_RIGHT] = true;
            break;
        case ANCHOR_BOTTOMRIGHT :
            lDefinedBorderList_[BORDER_BOTTOM] = true;
            lDefinedBorderList_[BORDER_RIGHT] = true;
            break;
        case ANCHOR_BOTTOM :
            lDefinedBorderList_[BORDER_BOTTOM] = true;
            break;
        case ANCHOR_BOTTOMLEFT :
            lDefinedBorderList_[BORDER_BOTTOM] = true;
            lDefinedBorderList_[BORDER_LEFT] = true;
            break;
        case ANCHOR_LEFT :
            lDefinedBorderList_[BORDER_LEFT] = true;
            break;
        default : break;
    }

    FireUpdateBorders();
}

s_uint UIObject::GetNumPoint() const
{
    return lAnchorList_.size();
}

s_ptr<Anchor> UIObject::GetPoint( const s_uint& uiPoint )
{
    s_ptr<Anchor> pAnchor;
    map<AnchorPoint, Anchor>::iterator iterAnchor = lAnchorList_.begin();
    for (s_uint i = 0; i < uiPoint; i++)
    {
        pAnchor = &(iterAnchor->second);
        iterAnchor++;
    }

    return pAnchor;
}

const s_bool& UIObject::IsVirtual() const
{
    return bVirtual_;
}

void UIObject::SetVirtual()
{
    bVirtual_ = true;
}

const s_uint& UIObject::GetID() const
{
    return uiID_;
}

void UIObject::SetID( const s_uint& uiID )
{
    if (!uiID_.IsValid())
        uiID_ = uiID;
    else
    {
        Warning(lType_.back(),
            "SetID() can't be called more than once."
        );
    }
}

void UIObject::UpdateDimensions_()
{
    if (pParent_ != NULL)
    {
        if (bIsHeightAbs_)
            fRelHeight_ = s_float(uiAbsHeight_)/s_float(pParent_->GetAbsHeight());
        else
            uiAbsHeight_ = s_uint(fRelHeight_*s_float(pParent_->GetAbsHeight()));

        if (bIsWidthAbs_)
            fRelWidth_ = s_float(uiAbsWidth_)/s_float(pParent_->GetAbsWidth());
        else
            uiAbsWidth_ = s_uint(fRelWidth_*s_float(pParent_->GetAbsWidth()));
    }
    else
    {
        if (bIsHeightAbs_)
            fRelHeight_ = s_float(uiAbsHeight_)/s_float(Engine::GetSingleton()->GetScreenHeight());
        else
            uiAbsHeight_ = s_uint(fRelHeight_*s_float(Engine::GetSingleton()->GetScreenHeight()));

        if (bIsWidthAbs_)
            fRelWidth_ = s_float(uiAbsWidth_)/s_float(Engine::GetSingleton()->GetScreenWidth());
        else
            uiAbsWidth_ = s_uint(fRelWidth_*s_float(Engine::GetSingleton()->GetScreenWidth()));
    }
}

void UIObject::MakeBorders_( s_int& iMin, s_int& iMax, const s_int& iCenter, const s_int& iSize )
{
    if (!iMin.IsValid() && !iMax.IsValid())
    {
        if (iSize.IsValid())
        {
            if (iCenter.IsValid())
            {
                iMin = iCenter - iSize/2;
                iMax = iCenter + iSize/2;
            }
            else
                bReady_ = false;
        }
        else
            bReady_ = false;
    }
    else if (!iMax.IsValid())
    {
        if (iSize.IsValid())
        {
            iMax = iMin + iSize;
        }
        else
        {
            if (iCenter.IsValid())
            {
                iMax = iMin + 2*(iCenter-iMin);
            }
            else
                bReady_ = false;
        }
    }
    else if (!iMin.IsValid())
    {
        if (iSize.IsValid())
        {
            iMin = iMax - iSize;
        }
        else
        {
            if (iCenter.IsValid())
            {
                iMin = iMax - 2*(iMax-iCenter);
            }
            else
                bReady_ = false;
        }
    }

    if (iMin >= iMax)
        bReady_ = false;
}

void UIObject::UpdateBorders_()
{
    bReady_ = true;

    if (bUpdateDimensions_)
    {
        this->UpdateDimensions_();
        bUpdateDimensions_ = false;
    }

    if (!lAnchorList_.empty())
    {
        s_int iLeft   = s_int(s_int::INFPLUS);
        s_int iRight  = s_int(s_int::INFMINUS);
        s_int iTop    = s_int(s_int::INFPLUS);
        s_int iBottom = s_int(s_int::INFMINUS);

        s_int iXCenter;
        s_int iYCenter;

        map<AnchorPoint, Anchor>::iterator iterAnchor;
        foreach (iterAnchor, lAnchorList_)
        {
            switch (iterAnchor->second.GetPoint())
            {
                case ANCHOR_TOPLEFT :
                    iTop = s_int::Min(iTop, iterAnchor->second.GetAbsY());
                    iLeft = s_int::Min(iLeft, iterAnchor->second.GetAbsX());
                    break;
                case ANCHOR_TOP :
                    iTop = s_int::Min(iTop, iterAnchor->second.GetAbsY());
                    iXCenter = iterAnchor->second.GetAbsX();
                    break;
                case ANCHOR_TOPRIGHT :
                    iTop = s_int::Min(iTop, iterAnchor->second.GetAbsY());
                    iRight = s_int::Max(iRight, iterAnchor->second.GetAbsX());
                    break;
                case ANCHOR_RIGHT :
                    iRight = s_int::Max(iRight, iterAnchor->second.GetAbsX());
                    iYCenter = iterAnchor->second.GetAbsY();
                    break;
                case ANCHOR_BOTTOMRIGHT :
                    iBottom = s_int::Max(iBottom, iterAnchor->second.GetAbsY());
                    iRight = s_int::Max(iRight, iterAnchor->second.GetAbsX());
                    break;
                case ANCHOR_BOTTOM :
                    iBottom = s_int::Max(iBottom, iterAnchor->second.GetAbsY());
                    iXCenter = iterAnchor->second.GetAbsX();
                    break;
                case ANCHOR_BOTTOMLEFT :
                    iBottom = s_int::Max(iBottom, iterAnchor->second.GetAbsY());
                    iLeft = s_int::Min(iLeft, iterAnchor->second.GetAbsX());
                    break;
                case ANCHOR_LEFT :
                    iLeft = s_int::Min(iLeft, iterAnchor->second.GetAbsX());
                    iYCenter = iterAnchor->second.GetAbsY();
                    break;
                case ANCHOR_CENTER :
                    iXCenter = iterAnchor->second.GetAbsX();
                    iYCenter = iterAnchor->second.GetAbsY();
                    break;
            }
        }

        MakeBorders_(iTop, iBottom, iYCenter, s_int(uiAbsHeight_));
        MakeBorders_(iLeft, iRight, iXCenter, s_int(uiAbsWidth_));

        if (bReady_)
        {
            lBorderList_[BORDER_LEFT] = iLeft;
            lBorderList_[BORDER_RIGHT] = iRight;
            lBorderList_[BORDER_TOP] = iTop;
            lBorderList_[BORDER_BOTTOM] = iBottom;

            bIsWidthAbs_ = true;
            uiAbsWidth_ = s_uint(iRight - iLeft);

            bIsHeightAbs_ = true;
            uiAbsHeight_ = s_uint(iBottom - iTop);

            this->UpdateDimensions_();
        }
        else
        {
            lBorderList_.Set(s_int::NaN);
        }

        bUpdateBorders_ = false;
    }
    else
        bReady_ = false;
}

void UIObject::FireUpdateBorders()
{
    bUpdateBorders_ = true;
}

void UIObject::FireUpdateDimensions()
{
    FireUpdateBorders();
    bUpdateDimensions_ = true;
}

void UIObject::Update()
{
    if (bUpdateBorders_)
        UpdateBorders_();
}

void UIObject::UpdateMaterial( const s_bool& bForceUpdate )
{
}
