/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_uiobject.h"

#include "frost_frame.h"
#include "frost_layeredregion.h"
#include "frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str UIObject::CLASS_NAME = "GUI::UIObject";

UIObject::UIObject(s_ptr<GUIManager> pManager) : pManager_(pManager)
{
    lType_.PushBack("UIObject");

    fAlpha_ = 1.0f;
    uiAbsWidth_ = s_uint::NaN;
    uiAbsHeight_ = s_uint::NaN;
    bIsWidthAbs_ = true;
    bIsHeightAbs_ = true;
    fRelWidth_ = s_float::NaN;
    fRelHeight_ = s_float::NaN;
    bIsShown_ = true;
    bIsVisible_ = true;
    bUpdateBorders_ = true;
    uiID_ = s_uint::NaN;
    bReady_ = true;
}

UIObject::~UIObject()
{
    s_ctnr< s_ptr<LuaGlue> >::iterator iter;
    foreach (iter, lGlueList_)
    {
        (*iter)->NotifyDeleted();
    }
}

s_ptr<const GUIManager> UIObject::GetManager() const
{
    return pManager_;
}

s_ptr<GUIManager> UIObject::GetManager()
{
    return pManager_;
}

s_str UIObject::Serialize( const s_str& sTab ) const
{
    s_str sStr;

    sStr << sTab << "  # Name        : " << sName_ << " ("+s_str(bReady_ ? "ready" : "not ready")+s_str(bSpecial_ ? ", special)\n" : ")\n");
    sStr << sTab << "  # Raw name    : " << sRawName_ << "\n";
    sStr << sTab << "  # Lua name    : " << sLuaName_ << "\n";
    sStr << sTab << "  # ID          : " << uiID_ << "\n";
    sStr << sTab << "  # Type        : " << lType_.Back() << "\n";
    if (bManuallyRendered_)
    sStr << sTab << "  # Man. Render : " << (pRenderer_ ? pRenderer_->GetName() : "none") << "\n";
    if (pParent_)
    sStr << sTab << "  # Parent      : " << pParent_->GetName() << "\n";
    else
    sStr << sTab << "  # Parent      : none\n";
    sStr << sTab << "  # Num anchors : " << lAnchorList_.GetSize() << "\n";
    if (!lAnchorList_.IsEmpty())
    {
        sStr << sTab << "  |-###\n";
        s_map<AnchorPoint, Anchor>::const_iterator iterAnchor;
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
    sStr << sTab << "  # Alpha       : " << fAlpha_ << "\n";
    sStr << sTab << "  # Shown       : " << bIsShown_ << "\n";
    sStr << sTab << "  # Abs width   : " << uiAbsWidth_ << "\n";
    sStr << sTab << "  # Abs height  : " << uiAbsHeight_ << "\n";
    sStr << sTab << "  # Rel width   : " << fRelWidth_ << "\n";
    sStr << sTab << "  # Rel height  : " << fRelHeight_ << "\n";

    return sStr;
}

void UIObject::CopyFrom( s_ptr<UIObject> pObj )
{
    if (pObj)
    {
        // Copy marked variables
        if (!pObj->lCopyList_.IsEmpty())
        {
            if (IsVirtual())
            {
                // The current object is virtual too,
                // add the variables to its copy list.
                lCopyList_.PushBack(pObj->lCopyList_);
            }

            s_ptr<Lua::State> pLua = pManager_->GetLua();
            pLua->GetGlobal(pObj->GetLuaName());
            if (pLua->GetType() != Lua::TYPE_NIL)
            {
                pLua->GetGlobal(sLuaName_);
                if (pLua->GetType() != Lua::TYPE_NIL)
                {
                    s_ctnr<s_str>::iterator iterVariable;
                    foreach (iterVariable, pObj->lCopyList_)
                    {
                        pLua->GetField(*iterVariable, -2);
                        pLua->SetField(*iterVariable);
                    }
                }
                pLua->Pop();
            }
            pLua->Pop();
        }

        bInherits_ = true;
        pInheritance_ = pObj;

        // Inherit properties
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

        const s_map<AnchorPoint, Anchor>& lAnchorList = pObj->GetPointList();
        s_map<AnchorPoint, Anchor>::const_iterator iterAnchor;
        foreach (iterAnchor, lAnchorList)
        {
            if (iterAnchor->second.GetType() == ANCHOR_ABS)
            {
                this->SetAbsPoint(
                    iterAnchor->second.GetPoint(),
                    iterAnchor->second.GetParentRawName(),
                    iterAnchor->second.GetParentPoint(),
                    iterAnchor->second.GetAbsOffsetX(),
                    iterAnchor->second.GetAbsOffsetY()
                );
            }
            else
            {
                this->SetRelPoint(
                    iterAnchor->second.GetPoint(),
                    iterAnchor->second.GetParentRawName(),
                    iterAnchor->second.GetParentPoint(),
                    iterAnchor->second.GetRelOffsetX(),
                    iterAnchor->second.GetRelOffsetY()
                );
            }
        }
    }
}

const s_str& UIObject::GetName() const
{
    return sName_;
}

const s_str& UIObject::GetLuaName() const
{
    return sLuaName_;
}

const s_str& UIObject::GetRawName() const
{
    return sRawName_;
}

void UIObject::SetName( const s_str& sName )
{
    if (sName_.IsEmpty())
    {
        sName_ = sLuaName_ = sRawName_ = sName;
        if (sName_.StartsWith("$parent"))
        {
            if (pParent_)
                sLuaName_.Replace("$parent", pParent_->GetLuaName());
            else
                sLuaName_.Replace("$parent", "");
        }

        if (!bVirtual_)
            sName_ = sLuaName_;
    }
    else
    {
        Warning(lType_.Back(),
            "SetName() can only be called once."
        );
    }
}

const s_str& UIObject::GetObjectType() const
{
    return lType_.Back();
}

const s_ctnr<s_str>& UIObject::GetObjectTypeList() const
{
    return lType_;
}

s_bool UIObject::IsObjectType(const s_str& sType) const
{
    return lType_.Find(sType);
}

const s_float& UIObject::GetAlpha() const
{
    return fAlpha_;
}

void UIObject::SetAlpha( const s_float& fAlpha )
{
    if (fAlpha_ != fAlpha)
    {
        fAlpha_ = fAlpha;
        NotifyRendererNeedRedraw();
    }
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
    return bIsVisible_;
}

void UIObject::SetAbsWidth( const s_uint& uiAbsWidth )
{
    if (uiAbsWidth_ != uiAbsWidth || !bIsWidthAbs_ || !uiAbsWidth_.IsValid())
    {
        pManager_->NotifyObjectMoved();
        bIsWidthAbs_ = true;
        uiAbsWidth_ = uiAbsWidth;
        FireUpdateDimensions();
        NotifyRendererNeedRedraw();
    }
}

void UIObject::SetAbsHeight( const s_uint& uiAbsHeight )
{
    if (uiAbsHeight_ != uiAbsHeight || !bIsWidthAbs_ || !uiAbsHeight_.IsValid())
    {
        pManager_->NotifyObjectMoved();
        bIsHeightAbs_ = true;
        uiAbsHeight_ = uiAbsHeight;
        FireUpdateDimensions();
        NotifyRendererNeedRedraw();
    }
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
    if (fRelWidth_ != fRelWidth || bIsWidthAbs_ || !fRelWidth_.IsValid())
    {
        pManager_->NotifyObjectMoved();
        bIsWidthAbs_ = false;
        fRelWidth_ = fRelWidth;
        FireUpdateDimensions();
        NotifyRendererNeedRedraw();
    }
}

void UIObject::SetRelHeight( const s_float& fRelHeight )
{
    if (fRelHeight_ != fRelHeight || bIsHeightAbs_ || !fRelHeight_.IsValid())
    {
        pManager_->NotifyObjectMoved();
        bIsHeightAbs_ = false;
        fRelHeight_ = fRelHeight;
        FireUpdateDimensions();
        NotifyRendererNeedRedraw();
    }
}

const s_uint& UIObject::GetAbsWidth() const
{
    return uiAbsWidth_;
}

s_uint UIObject::GetAppearentWidth() const
{
    UpdateBorders_();
    return s_uint(lBorderList_[BORDER_RIGHT] - lBorderList_[BORDER_LEFT]);
}

const s_uint& UIObject::GetAbsHeight() const
{
    return uiAbsHeight_;
}

s_uint UIObject::GetAppearentHeight() const
{
    UpdateBorders_();
    return s_uint(lBorderList_[BORDER_BOTTOM] - lBorderList_[BORDER_TOP]);
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
    if (pParent != this && pParent_ != pParent)
    {
        pParent_ = pParent;
        FireUpdateDimensions();
    }
    else
    {
        Error(lType_.Back(),
            "Can't call SetParent(this)."
        );
    }
}

s_ptr<const UIObject> UIObject::GetParent() const
{
    return pParent_;
}

s_ptr<UIObject> UIObject::GetParent()
{
    return pParent_;
}

s_ptr<UIObject> UIObject::GetBase()
{
    return pInheritance_;
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

const s_array<s_int,4> UIObject::GetBorders() const
{
    return lBorderList_;
}

void UIObject::ClearAllPoints()
{
    if (lAnchorList_.GetSize() != 0)
    {
        lAnchorList_.Clear();

        lDefinedBorderList_[BORDER_LEFT]   =
        lDefinedBorderList_[BORDER_TOP]    =
        lDefinedBorderList_[BORDER_RIGHT]  =
        lDefinedBorderList_[BORDER_BOTTOM] = false;

        bUpdateAnchors_ = true;
        FireUpdateBorders();
        NotifyRendererNeedRedraw();
        pManager_->NotifyObjectMoved();
    }
}

void UIObject::SetAllPoints( const s_str& sObjName )
{
    if (sObjName != sName_)
    {
        ClearAllPoints();
        Anchor mAnchor = Anchor(this, ANCHOR_TOPLEFT, sObjName, ANCHOR_TOPLEFT);
        lAnchorList_[ANCHOR_TOPLEFT] = mAnchor;

        mAnchor = Anchor(this, ANCHOR_BOTTOMRIGHT, sObjName, ANCHOR_BOTTOMRIGHT);
        lAnchorList_[ANCHOR_BOTTOMRIGHT] = mAnchor;

        lDefinedBorderList_[BORDER_LEFT]   =
        lDefinedBorderList_[BORDER_TOP]    =
        lDefinedBorderList_[BORDER_RIGHT]  =
        lDefinedBorderList_[BORDER_BOTTOM] = true;

        bUpdateAnchors_ = true;
        FireUpdateBorders();
        NotifyRendererNeedRedraw();
        pManager_->NotifyObjectMoved();
    }
    else
    {
        Error(lType_.Back(),
            "Can't call SetAllPoints(this)."
        );
    }
}

void UIObject::SetAllPoints( s_ptr<UIObject> pObj )
{
    if (pObj != this)
    {
        ClearAllPoints();
        Anchor mAnchor = Anchor(this, ANCHOR_TOPLEFT, pObj ? pObj->GetName() : "", ANCHOR_TOPLEFT);
        lAnchorList_[ANCHOR_TOPLEFT] = mAnchor;

        mAnchor = Anchor(this, ANCHOR_BOTTOMRIGHT, pObj ? pObj->GetName() : "", ANCHOR_BOTTOMRIGHT);
        lAnchorList_[ANCHOR_BOTTOMRIGHT] = mAnchor;

        lDefinedBorderList_[BORDER_LEFT]   =
        lDefinedBorderList_[BORDER_TOP]    =
        lDefinedBorderList_[BORDER_RIGHT]  =
        lDefinedBorderList_[BORDER_BOTTOM] = true;

        bUpdateAnchors_ = true;
        FireUpdateBorders();
        NotifyRendererNeedRedraw();
        pManager_->NotifyObjectMoved();
    }
    else
    {
        Error(lType_.Back(),
            "Can't call SetAllPoints(this)."
        );
    }
}

void UIObject::SetAbsPoint( AnchorPoint mPoint, const s_str& sParentName, AnchorPoint mRelativePoint, const s_int& iX, const s_int& iY )
{
    s_map<AnchorPoint, Anchor>::iterator iterAnchor = lAnchorList_.Get(mPoint);
    if (iterAnchor == lAnchorList_.End())
    {
        Anchor mAnchor = Anchor(this, mPoint, sParentName, mRelativePoint);
        mAnchor.SetAbsOffset(iX, iY);
        lAnchorList_[mPoint] = mAnchor;
    }
    else
    {
        s_ptr<Anchor> pAnchor = &iterAnchor->second;
        pAnchor->SetParentRawName(sParentName);
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

    bUpdateAnchors_ = true;
    FireUpdateBorders();
    NotifyRendererNeedRedraw();
    pManager_->NotifyObjectMoved();
}

void UIObject::SetRelPoint( AnchorPoint mPoint, const s_str& sParentName, AnchorPoint mRelativePoint, const s_float& fX, const s_float& fY )
{
    s_map<AnchorPoint, Anchor>::iterator iterAnchor = lAnchorList_.Get(mPoint);
    if (iterAnchor == lAnchorList_.End())
    {
        Anchor mAnchor = Anchor(this, mPoint, sParentName, mRelativePoint);
        mAnchor.SetRelOffset(fX, fY);
        lAnchorList_[mPoint] = mAnchor;
    }
    else
    {
        s_ptr<Anchor> pAnchor = &iterAnchor->second;
        pAnchor->SetParentRawName(sParentName);
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

    bUpdateAnchors_ = true;
    FireUpdateBorders();
    NotifyRendererNeedRedraw();
    pManager_->NotifyObjectMoved();
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

    bUpdateAnchors_ = true;
    FireUpdateBorders();
    NotifyRendererNeedRedraw();
    pManager_->NotifyObjectMoved();
}

s_bool UIObject::DependsOn( s_ptr<UIObject> pObj ) const
{
    if (pObj)
    {
        s_map<AnchorPoint, Anchor>::const_iterator iterAnchor;
        foreach (iterAnchor, lAnchorList_)
        {
            s_ptr<const UIObject> pParent = iterAnchor->second.GetParent();
            if (pParent == pObj)
                return true;

            if (pParent)
                return pParent->DependsOn(pObj);
        }
    }

    return false;
}

s_uint UIObject::GetNumPoint() const
{
    return lAnchorList_.GetSize();
}

s_ptr<Anchor> UIObject::ModifyPoint( AnchorPoint mPoint )
{
    pManager_->NotifyObjectMoved();

    FireUpdateBorders();

    if (lAnchorList_.Find(mPoint))
        return &lAnchorList_[mPoint];
    else
        return nullptr;
}

s_ptr<const Anchor> UIObject::GetPoint( AnchorPoint mPoint ) const
{
    s_map<AnchorPoint, Anchor>::const_iterator iterAnchor = lAnchorList_.Get(mPoint);
    if (iterAnchor != lAnchorList_.End())
        return &iterAnchor->second;
    else
        return nullptr;
}

const s_map<AnchorPoint, Anchor>& UIObject::GetPointList() const
{
    return lAnchorList_;
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
        Warning(lType_.Back(),
            "SetID() can't be called more than once."
        );
    }
}

void UIObject::NotifyAnchoredObject( s_ptr<UIObject> pObj, const s_bool& bAnchored ) const
{
    if (!pObj)
        return;

    if (bAnchored)
    {
        if (!lAnchoredObjectList_.Find(pObj->GetID()))
            lAnchoredObjectList_[pObj->GetID()] = pObj;
    }
    else
    {
        lAnchoredObjectList_.Erase(pObj->GetID());
    }
}

void UIObject::UpdateDimensions_() const
{
    if (pParent_)
    {
        if (bIsHeightAbs_)
            fRelHeight_ = s_float(uiAbsHeight_)/s_float(pParent_->GetAppearentHeight());
        else
            uiAbsHeight_ = s_uint(fRelHeight_*s_float(pParent_->GetAppearentHeight()));

        if (bIsWidthAbs_)
            fRelWidth_ = s_float(uiAbsWidth_)/s_float(pParent_->GetAppearentWidth());
        else
            uiAbsWidth_ = s_uint(fRelWidth_*s_float(pParent_->GetAppearentWidth()));
    }
    else
    {
        if (bIsHeightAbs_)
            fRelHeight_ = s_float(uiAbsHeight_)/s_float(pManager_->GetScreenHeight());
        else
            uiAbsHeight_ = s_uint(fRelHeight_*s_float(pManager_->GetScreenHeight()));

        if (bIsWidthAbs_)
            fRelWidth_ = s_float(uiAbsWidth_)/s_float(pManager_->GetScreenWidth());
        else
            uiAbsWidth_ = s_uint(fRelWidth_*s_float(pManager_->GetScreenWidth()));
    }
}

void UIObject::MakeBorders_( s_int& iMin, s_int& iMax, const s_int& iCenter, const s_int& iSize ) const
{
    if (!iMin.IsValid() && !iMax.IsValid())
    {
        if (iSize.IsValid())
        {
            if (iCenter.IsValid())
            {
                iMin = iCenter - iSize/2;
                iMax = iCenter + s_int(s_float(iSize)/2.0f);
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

void UIObject::ReadAnchors_( s_int& iLeft, s_int& iRight, s_int& iTop, s_int& iBottom, s_int& iXCenter, s_int& iYCenter ) const
{
    iLeft   = s_int::INFPLUS;
    iRight  = s_int::INFMINUS;
    iTop    = s_int::INFPLUS;
    iBottom = s_int::INFMINUS;

    s_map<AnchorPoint, Anchor>::const_iterator iterAnchor;
    foreach (iterAnchor, lAnchorList_)
    {
        // Make sure the anchored object has its borders updated
        s_ptr<const UIObject> pObj = iterAnchor->second.GetParent();
        if (pObj)
            pObj->UpdateBorders_();

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
}

void UIObject::UpdateBorders_() const
{
    if (!bUpdateBorders_)
        return;

    s_bool bOldReady = bReady_;
    bReady_ = true;

    if (bUpdateDimensions_)
    {
        UpdateDimensions_();
        bUpdateDimensions_ = false;
    }

    if (!lAnchorList_.IsEmpty())
    {
        s_int iLeft, iRight, iTop, iBottom;
        s_int iXCenter, iYCenter;

        ReadAnchors_(iLeft, iRight, iTop, iBottom, iXCenter, iYCenter);

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

    if (bReady_ || (!bReady_ && bOldReady))
    {
        NotifyRendererNeedRedraw();
    }
}

void UIObject::UpdateAnchors()
{
    if (bUpdateAnchors_)
    {
        s_ctnr<s_map<AnchorPoint, Anchor>::iterator> lEraseList;
        s_map<AnchorPoint, Anchor>::iterator iterAnchor;
        foreach (iterAnchor, lAnchorList_)
        {
            s_ptr<const UIObject> pObj = iterAnchor->second.GetParent();
            if (pObj && pObj->DependsOn(this))
            {
                Error(CLASS_NAME, "Cyclic anchor dependency ! \""+sName_+"\" and \""+pObj->GetName()+"\"\n"
                    "depends on eachothers (directly or indirectly). \""+Anchor::GetStringPoint(iterAnchor->first)+
                    "\" anchor removed."
                );
                lEraseList.PushBack(iterAnchor);
            }
        }

        s_ctnr<s_map<AnchorPoint, Anchor>::iterator>::iterator iterErase;
        foreach (iterErase, lEraseList)
        {
            lAnchorList_.Erase(*iterErase);
        }

        s_ctnr< s_ptr<const UIObject> > lAnchorParentList;
        foreach (iterAnchor, lAnchorList_)
        {
            s_ptr<const UIObject> pParent = iterAnchor->second.GetParent();
            if (pParent && !lAnchorParentList.Find(pParent))
                lAnchorParentList.PushBack(pParent);
        }

        s_ctnr< s_ptr<const UIObject> >::iterator iterOldParent;
        foreach (iterOldParent, lPreviousAnchorParentList_)
        {
            s_ptr<const UIObject> pParent = *iterOldParent;
            if (!lAnchorParentList.Find(pParent))
                pParent->NotifyAnchoredObject(this, false);
        }

        s_ctnr< s_ptr<const UIObject> >::iterator iterParent;
        foreach (iterParent, lAnchorParentList)
        {
            s_ptr<const UIObject> pParent = *iterParent;
            if (!lPreviousAnchorParentList_.Find(pParent))
                pParent->NotifyAnchoredObject(this, true);
        }

        lPreviousAnchorParentList_ = lAnchorParentList;
        bUpdateAnchors_ = false;
    }
}

void UIObject::FireUpdateBorders() const
{
    bUpdateBorders_ = true;

    s_map< s_uint, s_ptr<UIObject> >::const_iterator iterAnchored;
    foreach (iterAnchored, lAnchoredObjectList_)
    {
        iterAnchored->second->FireUpdateBorders();
    }
}

void UIObject::FireUpdateDimensions() const
{
    FireUpdateBorders();
    bUpdateDimensions_ = true;
}

void UIObject::Update()
{
    UpdateBorders_();

    if (bNewlyCreated_)
    {
        bNewlyCreated_ = false;
        NotifyRendererNeedRedraw();
    }
}

void UIObject::PushOnLua( s_ptr<Lua::State> pLua ) const
{
    pLua->PushGlobal(sLuaName_);
}

void UIObject::RemoveGlue()
{
    s_ptr<Lua::State> pLua = pManager_->GetLua();
    pLua->PushNil();
    pLua->SetGlobal(sLuaName_);
}

void UIObject::SetSpecial()
{
    bSpecial_ = true;
}

const s_bool& UIObject::IsSpecial() const
{
    return bSpecial_;
}

void UIObject::SetManuallyRendered( const s_bool& bManuallyRendered, s_ptr<UIObject> pRenderer )
{
    s_ptr<UIObject> pOldRenderer = pRenderer_;

    if (pOldRenderer && pOldRenderer != pRenderer)
        pOldRenderer->NotifyManuallyRenderedObject_(this, false);

    NotifyRendererNeedRedraw();

    bManuallyRendered_ = bManuallyRendered;
    if (bManuallyRendered_)
        pRenderer_ = pRenderer;
    else
        pRenderer_ = nullptr;

    if (pRenderer_ && pRenderer_ != pOldRenderer && bManuallyRendered_)
        pRenderer_->NotifyManuallyRenderedObject_(this, true);

    NotifyRendererNeedRedraw();
}

const s_bool& UIObject::IsManuallyRendered() const
{
    return bManuallyRendered_;
}

void UIObject::SetNewlyCreated()
{
    bNewlyCreated_ = true;
}

const s_bool& UIObject::IsNewlyCreated() const
{
    return bNewlyCreated_;
}

void UIObject::NotifyRendererNeedRedraw() const
{
}

void UIObject::FireRedraw() const
{
}

void UIObject::NotifyManuallyRenderedObject_(s_ptr<UIObject> pObject, const s_bool& bManuallyRendered)
{

}

void UIObject::MarkForCopy( const s_str& sVariable )
{
    if (!lCopyList_.Find(sVariable))
        lCopyList_.PushBack(sVariable);
    else
    {
        Warning(lType_.Back(),
            "\""+sName_+"."+sVariable+"\" has already been marked for copy. Ignoring."
        );
    }
}

s_ctnr< s_ptr<UIObject> > UIObject::ClearLinks()
{
    // Remove this widget from its parent's children
    if (pParent_)
    {
        s_ptr<Frame> pParentFrame = s_ptr<Frame>::DynamicCast(pParent_);
        s_ptr<Frame> pThisFrame = s_ptr<Frame>::DynamicCast(this);
        if (pThisFrame)
            pParentFrame->RemoveChild(pThisFrame);
        else
        {
            s_ptr<LayeredRegion> pThisRegion = s_ptr<LayeredRegion>::DynamicCast(this);
            if (pThisRegion)
                pParentFrame->RemoveRegion(pThisRegion);
        }
        pParent_ = nullptr;
    }

    NotifyRendererNeedRedraw();

    // Tell the renderer to no longer render this widget
    if (bManuallyRendered_ && pRenderer_)
        pRenderer_->NotifyManuallyRenderedObject_(this, false);

    // Tell this widget's anchor parents that it is no longer anchored to them
    s_map<AnchorPoint, Anchor>::const_iterator iterAnchor;
    foreach (iterAnchor, lAnchorList_)
    {
        if (iterAnchor->second.GetParent())
            iterAnchor->second.GetParent()->NotifyAnchoredObject(this, false);
    }

    lAnchorList_.Clear();

    // Replace anchors pointing to this widget by absolute anchors
    s_map< s_uint, s_ptr<UIObject> >::iterator iterAnchored;
    s_map< s_uint, s_ptr<UIObject> > lTempAnchoredObjectList = lAnchoredObjectList_;
    foreach (iterAnchored, lTempAnchoredObjectList)
    {
        s_ptr<UIObject> pObj = iterAnchored->second;
        s_ctnr<AnchorPoint> lAnchoredPointList;
        const s_map<AnchorPoint, Anchor>& lAnchorList = pObj->GetPointList();
        s_map<AnchorPoint, Anchor>::const_iterator iterAnchor;
        foreach (iterAnchor, lAnchorList)
        {
            if (iterAnchor->second.GetParent() == this)
                lAnchoredPointList.PushBack(iterAnchor->first);
        }

        s_ctnr<AnchorPoint>::iterator iterAnchorPoint;
        foreach (iterAnchorPoint, lAnchoredPointList)
        {
            s_ptr<const Anchor> pAnchor = pObj->GetPoint(*iterAnchorPoint);
            Anchor mNewAnchor = Anchor(pObj, *iterAnchorPoint, "", ANCHOR_TOPLEFT);

            s_int iX = pAnchor->GetAbsOffsetX();
            s_int iY = pAnchor->GetAbsOffsetY();

            switch (pAnchor->GetParentPoint())
            {
                case ANCHOR_TOPLEFT :
                    iX += lBorderList_[BORDER_LEFT];
                    iY += lBorderList_[BORDER_TOP];
                    break;

                case ANCHOR_TOP :
                    iY += lBorderList_[BORDER_TOP];
                    break;

                case ANCHOR_TOPRIGHT :
                    iX += lBorderList_[BORDER_RIGHT];
                    iY += lBorderList_[BORDER_TOP];
                    break;

                case ANCHOR_RIGHT :
                    iX += lBorderList_[BORDER_RIGHT];
                    break;

                case ANCHOR_BOTTOMRIGHT :
                    iX += lBorderList_[BORDER_RIGHT];
                    iY += lBorderList_[BORDER_BOTTOM];
                    break;

                case ANCHOR_BOTTOM :
                    iY += lBorderList_[BORDER_BOTTOM];
                    break;

                case ANCHOR_BOTTOMLEFT :
                    iX += lBorderList_[BORDER_LEFT];
                    iY += lBorderList_[BORDER_BOTTOM];
                    break;

                case ANCHOR_LEFT :
                    iX += lBorderList_[BORDER_LEFT];
                    break;

                case ANCHOR_CENTER :
                    iX += (lBorderList_[BORDER_LEFT] + lBorderList_[BORDER_RIGHT])/2;
                    iY += (lBorderList_[BORDER_TOP] + lBorderList_[BORDER_BOTTOM])/2;
            }

            mNewAnchor.SetAbsOffset(iX, iY);

            pObj->SetPoint(mNewAnchor);
        }

        pObj->UpdateAnchors();
    }

    s_ctnr< s_ptr<UIObject> > lList;
    lList.PushBack(this);

    return lList;
}

void UIObject::NotifyLoaded()
{
    bLoaded_ = true;
}

