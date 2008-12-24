/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_uiobject.h"

#include "gui/frost_gui_frame.h"
#include "frost_guimanager.h"
#include "frost_material.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str UIObject::CLASS_NAME = "GUI::UIObject";

UIObject::UIObject()
{
    mType_ = OJBECT_TYPE_UIOBJECT;
    lType_.push_back("UIObject");

    fAlpha_ = 1.0f;
    uiAbsWidth_ = 1u;
    uiAbsHeight_ = 1u;
    fRelWidth_ = 1.0f;
    fRelHeight_ = 1.0f;
    bIsShown_ = true;
    uiID_ = s_uint::NaN;
}

UIObject::~UIObject()
{
}

s_ptr<LuaUIObject> UIObject::GetGlue()
{
    return pGlue_;
}

s_refptr<Material> UIObject::GetMaterial()
{
    return pMaterial_;
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
        if (pParent_)
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

vector<s_str> UIObject::GetObjectType() const
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
    uiAbsWidth_ = uiAbsWidth;
    if (pParent_ != NULL)
        fRelWidth_ = s_float(uiAbsWidth_)/s_float(pParent_->GetAbsWidth());
    else
        fRelWidth_ = s_float(uiAbsWidth_)/s_float(Engine::GetSingleton()->GetScreenWidth());
}

void UIObject::SetAbsHeight( const s_uint& uiAbsHeight )
{
    uiAbsHeight_ = uiAbsHeight;
    if (pParent_ != NULL)
        fRelHeight_ = s_float(uiAbsHeight_)/s_float(pParent_->GetAbsHeight());
    else
        fRelHeight_ = s_float(uiAbsHeight_)/s_float(Engine::GetSingleton()->GetScreenHeight());
}

void UIObject::SetRelWidth( const s_float& fRelWidth )
{
    fRelWidth_ = fRelWidth;
    if (pParent_ != NULL)
        uiAbsWidth_ = s_uint(fRelWidth_*s_float(pParent_->GetAbsWidth()));
    else
        uiAbsWidth_ = s_uint(fRelWidth_*s_float(Engine::GetSingleton()->GetScreenHeight()));
}

void UIObject::SetRelHeight( const s_float& fRelHeight )
{
    fRelHeight_ = fRelHeight;
    if (pParent_ != NULL)
        uiAbsHeight_ = s_uint(fRelHeight_*s_float(pParent_->GetAbsHeight()));
    else
        uiAbsHeight_ = s_uint(fRelHeight_*s_float(Engine::GetSingleton()->GetScreenHeight()));
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
    }
    else
    {
        Error(lType_.back(),
            "Can't call SetParent(this)"
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
    return lBorderList_[BORDER_RIGHT];
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
    lAnchorStack_.clear();
}

void UIObject::SetAllPoints( s_ptr<UIObject> pObj )
{
    if (pObj != this)
    {
        ClearAllPoints();
        Anchor mAnchor = Anchor(this, ANCHOR_TOPLEFT, pObj, ANCHOR_TOPLEFT);
        lAnchorList_[ANCHOR_TOPLEFT] = mAnchor;
        lAnchorStack_.push_back(&lAnchorList_[ANCHOR_TOPLEFT]);
        mAnchor = Anchor(this, ANCHOR_BOTTOMRIGHT, pObj, ANCHOR_BOTTOMRIGHT);
        lAnchorList_[ANCHOR_BOTTOMRIGHT] = mAnchor;
        lAnchorStack_.push_back(&lAnchorList_[ANCHOR_BOTTOMRIGHT]);

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
        lAnchorStack_.push_back(&lAnchorList_[mPoint]);
    }
    else
    {
        s_ptr<Anchor> pAnchor = &iterAnchor->second;
        pAnchor->SetParent(pObj);
        pAnchor->SetParentPoint(mRelativePoint);
        pAnchor->SetAbsOffset(iX, iY);
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
        lAnchorStack_.push_back(&lAnchorList_[mPoint]);
    }
    else
    {
        s_ptr<Anchor> pAnchor = &iterAnchor->second;
        pAnchor->SetParent(pObj);
        pAnchor->SetParentPoint(mRelativePoint);
        pAnchor->SetRelOffset(fX, fY);
    }

    FireUpdateBorders();
}

s_uint UIObject::GetNumPoint() const
{
    return lAnchorStack_.size();
}

s_ptr<Anchor> UIObject::GetPoint( const s_uint& uiPoint )
{
    if (uiPoint <= lAnchorStack_.size())
        return lAnchorStack_[uiPoint.Get()-1];
    else
        return NULL;
}


const s_uint& UIObject::GetID() const
{
    return uiID_;
}

void UIObject::SetID( const s_uint& uiID )
{
    if (!uiID_)
        uiID_ = uiID;
    else
    {
        Warning(lType_.back(),
            "SetID() can't be called more than once."
        );
    }
}

void UIObject::UpdateBorders_()
{
    s_int iTop    = s_int(s_int::INTEGER_INF_PLUS);
    s_int iBottom = s_int(s_int::INTEGER_INF_MINUS);
    s_int iLeft   = s_int(s_int::INTEGER_INF_PLUS);
    s_int iRight  = s_int(s_int::INTEGER_INF_MINUS);

    map<AnchorPoint, Anchor>::iterator iterAnchor;
    foreach (iterAnchor, lAnchorList_)
    {
        s_int iNewTop = iterAnchor->second.GetAbsY();
        if (iNewTop < iTop) iTop = iNewTop;

        s_int iNewBottom = iterAnchor->second.GetAbsY() + s_int(uiAbsHeight_);
        if (iNewBottom < iBottom) iBottom = iNewBottom;

        s_int iNewLeft = iterAnchor->second.GetAbsX();
        if (iNewLeft < iLeft) iLeft = iNewLeft;

        s_int iNewRight = iterAnchor->second.GetAbsX() + s_int(uiAbsWidth_);
        if (iNewRight > iRight) iRight = iNewRight;
    }

    lBorderList_[BORDER_TOP] = iTop;
    lBorderList_[BORDER_RIGHT] = iRight;
    lBorderList_[BORDER_BOTTOM] = iBottom;
    lBorderList_[BORDER_LEFT] = iLeft;

    SetAbsWidth(s_uint(iBottom - iTop));
    SetAbsHeight(s_uint(iRight - iLeft));

    bUpdateBorders_ = false;
}

void UIObject::FireUpdateBorders()
{
    bUpdateBorders_ = true;
}

void UIObject::Update()
{
    if (bUpdateBorders_)
        UpdateBorders_();
}

void UIObject::UpdateMaterial( const s_bool& bForceUpdate )
{
}
