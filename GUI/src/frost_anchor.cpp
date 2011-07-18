/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Anchor source              */
/*                                        */

#include "frost_anchor.h"
#include "frost_uiobject.h"
#include "frost_frame.h"
#include "frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str Anchor::CLASS_NAME = "GUI::Anchor";

Anchor::Anchor()
{
    mParentPoint_ = mPoint_ = ANCHOR_TOPLEFT;
    mType_ = ANCHOR_ABS;
}

Anchor::Anchor( s_ptr<UIObject> pObj, AnchorPoint mPoint, const s_str& sParent, AnchorPoint mParentPoint )
{
    pObj_ = pObj;
    mPoint_ = mPoint;
    sParent_ = sParent;
    mParentPoint_ = mParentPoint;
    mType_ = ANCHOR_ABS;
}

void Anchor::UpdateParent() const
{
    if (!bParentUpdated_)
    {
        if (!sParent_.IsEmpty())
        {
            s_ptr<const UIObject> pObjParent = pObj_->GetParent();

            if (pObjParent)
            {
                sParent_.Replace("$parent", pObjParent->GetLuaName());
            }
            else if (sParent_.Find("$parent"))
            {
                pParent_ = nullptr;
                bParentUpdated_ = true;
                Warning(pObj_->GetObjectType(),
                    "UIObject \""+pObj_->GetName()+"\" has no parent but uses $parent in its anchors."
                );
                return;
            }

            pParent_ = pObj_->GetManager()->GetUIObjectByName(sParent_);
            if (!pParent_)
            {
                Warning(pObj_->GetObjectType(),
                    "UIObject \""+pObj_->GetName()+"\" tries to anchor to \""+sParent_+"\" but this "
                    "widget doesn't exist."
                );
            }
        }
        else
            pParent_ = nullptr;

        bParentUpdated_ = true;
    }
}

const s_int& Anchor::GetAbsX() const
{
    if (pObj_)
    {
        UpdateParent();

        s_int iParentX;
        if (pParent_)
        {
            iParentX = pParent_->GetLeft();
            iParentWidth_ = s_int(pParent_->GetAppearentWidth());
        }
        else
        {
            iParentX = 0;
            iParentWidth_ = s_int(pObj_->GetManager()->GetScreenWidth());
        }

        s_int iOffset;
        if (mType_ == ANCHOR_ABS)
            iOffset = iAbsOffX_;
        else
            iOffset = s_int(fRelOffX_*s_float(iParentWidth_));

        s_int iParentOffset;
        if ((mParentPoint_ == ANCHOR_TOPLEFT) || (mParentPoint_ == ANCHOR_LEFT) || (mParentPoint_ == ANCHOR_BOTTOMLEFT))
        {
            iParentOffset = 0;
        }
        else if ((mParentPoint_ == ANCHOR_TOP) || (mParentPoint_ == ANCHOR_CENTER) || (mParentPoint_ == ANCHOR_BOTTOM))
        {
            iParentOffset = iParentWidth_/2;
        }
        else if ((mParentPoint_ == ANCHOR_TOPRIGHT) || (mParentPoint_ == ANCHOR_RIGHT) || (mParentPoint_ == ANCHOR_BOTTOMRIGHT))
        {
            iParentOffset = iParentWidth_;
        }

        iAbsX_ = iOffset + iParentOffset + iParentX;
    }

    return iAbsX_;
}

const s_int& Anchor::GetAbsY() const
{
    if (pObj_)
    {
        UpdateParent();

        s_int iParentY;
        if (pParent_)
        {
            iParentY = pParent_->GetTop();
            iParentHeight_ = s_int(pParent_->GetAppearentHeight());
        }
        else
        {
            iParentY = 0;
            iParentHeight_ = s_int(pObj_->GetManager()->GetScreenHeight());
        }

        s_int iOffset;
        if (mType_ == ANCHOR_ABS)
            iOffset = iAbsOffY_;
        else
            iOffset = s_int(fRelOffY_*s_float(iParentHeight_));

        s_int iParentOffset;
        if ((mParentPoint_ == ANCHOR_TOPLEFT) || (mParentPoint_ == ANCHOR_TOP) || (mParentPoint_ == ANCHOR_TOPRIGHT))
        {
            iParentOffset = 0;
        }
        else if ((mParentPoint_ == ANCHOR_LEFT) || (mParentPoint_ == ANCHOR_CENTER) || (mParentPoint_ == ANCHOR_RIGHT))
        {
            iParentOffset = iParentHeight_/2;
        }
        else if ((mParentPoint_ == ANCHOR_BOTTOMLEFT) || (mParentPoint_ == ANCHOR_BOTTOM) || (mParentPoint_ == ANCHOR_BOTTOMRIGHT))
        {
            iParentOffset = iParentHeight_;
        }

        iAbsY_ = iOffset + iParentOffset + iParentY;
    }

    return iAbsY_;
}

s_ptr<const UIObject> Anchor::GetObject() const
{
    return pObj_;
}

s_ptr<const UIObject> Anchor::GetParent() const
{
    UpdateParent();
    return pParent_;
}

const s_str& Anchor::GetParentRawName() const
{
    return sParent_;
}

AnchorPoint Anchor::GetPoint() const
{
    return mPoint_;
}

AnchorPoint Anchor::GetParentPoint() const
{
    return mParentPoint_;
}

AnchorType Anchor::GetType() const
{
    return mType_;
}

const s_int& Anchor::GetAbsOffsetX() const
{
    return iAbsOffX_;
}

const s_int& Anchor::GetAbsOffsetY() const
{
    return iAbsOffY_;
}

const s_float& Anchor::GetRelOffsetX() const
{
    return fRelOffX_;
}

const s_float& Anchor::GetRelOffsetY() const
{
    return fRelOffY_;
}

void Anchor::SetObject( s_ptr<UIObject> pObj )
{
    pObj_ = pObj;
}

void Anchor::SetParentRawName( const s_str& sName )
{
    sParent_ = sName;
    bParentUpdated_ = false;
}

void Anchor::SetPoint( AnchorPoint mPoint )
{
    mPoint_ = mPoint;
}

void Anchor::SetParentPoint( AnchorPoint mParentPoint )
{
    mParentPoint_ = mParentPoint;
}

void Anchor::SetAbsOffset( const s_int& iX, const s_int& iY )
{
    iAbsOffX_ = iX;
    iAbsOffY_ = iY;
    mType_ = ANCHOR_ABS;
}

void Anchor::SetRelOffset( const s_float& fX, const s_float& fY )
{
    fRelOffX_ = fX;
    fRelOffY_ = fY;
    mType_ = ANCHOR_REL;
}

s_str Anchor::Serialize( const s_str& sTab ) const
{
    s_str sStr;

    sStr << sTab << "  |   # Point      : " << GetStringPoint(mPoint_) << "\n";
    if (pParent_)
    sStr << sTab << "  |   # Parent     : " << pParent_->GetName();
    else
    sStr << sTab << "  |   # Parent     : none";
    if (!sParent_.IsEmpty())
    sStr << " (raw name : " << sParent_ << ")\n";
    else
    sStr << "\n";
    sStr << sTab << "  |   # Rel. point : " << GetStringPoint(mParentPoint_) << "\n";
    if (mType_ == ANCHOR_ABS)
    {
    sStr << sTab << "  |   # Offset X   : " << iAbsOffX_ << "\n";
    sStr << sTab << "  |   # Offset Y   : " << iAbsOffY_ << "\n";
    }
    else
    {
    sStr << sTab << "  |   # Offset X   : " << fRelOffX_ << " (" << s_int(fRelOffX_*s_float(iParentWidth_)) << ")\n";
    sStr << sTab << "  |   # Offset Y   : " << fRelOffY_ << " (" << s_int(fRelOffY_*s_float(iParentHeight_)) << ")\n";
    }

    return sStr;
}

s_str Anchor::GetStringPoint( AnchorPoint mPoint )
{
    s_str sPoint;
    switch (mPoint)
    {
        case ANCHOR_TOPLEFT :     sPoint = "TOPLEFT";     break;
        case ANCHOR_TOP :         sPoint = "TOP";         break;
        case ANCHOR_TOPRIGHT :    sPoint = "TOPRIGHT";    break;
        case ANCHOR_RIGHT :       sPoint = "RIGHT";       break;
        case ANCHOR_BOTTOMRIGHT : sPoint = "BOTTOMRIGHT"; break;
        case ANCHOR_BOTTOM :      sPoint = "BOTTOM";      break;
        case ANCHOR_BOTTOMLEFT :  sPoint = "BOTTOMLEFT";  break;
        case ANCHOR_LEFT :        sPoint = "LEFT";        break;
        case ANCHOR_CENTER :      sPoint = "CENTER";      break;
    };
    return sPoint;
}

AnchorPoint Anchor::GetAnchorPoint( const s_str& sPoint )
{
    AnchorPoint mPoint = ANCHOR_TOPLEFT;
    if (sPoint == "TOPLEFT")          mPoint = ANCHOR_TOPLEFT;
    else if (sPoint == "TOP")         mPoint = ANCHOR_TOP;
    else if (sPoint == "TOPRIGHT")    mPoint = ANCHOR_TOPRIGHT;
    else if (sPoint == "RIGHT")       mPoint = ANCHOR_RIGHT;
    else if (sPoint == "BOTTOMRIGHT") mPoint = ANCHOR_BOTTOMRIGHT;
    else if (sPoint == "BOTTOM")      mPoint = ANCHOR_BOTTOM;
    else if (sPoint == "BOTTOMLEFT")  mPoint = ANCHOR_BOTTOMLEFT;
    else if (sPoint == "LEFT")        mPoint = ANCHOR_LEFT;
    else if (sPoint == "CENTER")      mPoint = ANCHOR_CENTER;
    return mPoint;
}
