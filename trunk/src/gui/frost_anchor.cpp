/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Anchor source              */
/*                                        */

#include "frost_gui.h"

#include "gui/frost_gui_uiobject.h"
#include "gui/frost_gui_frame.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str Anchor::CLASS_NAME = "GUI::Anchor";

Anchor::Anchor()
{
    mParentPoint_ = mPoint_ = ANCHOR_TOPLEFT;
    mType_ = ANCHOR_ABS;
}

Anchor::Anchor( s_ptr<UIObject> pObj, AnchorPoint mPoint, s_ptr<UIObject> pParent, AnchorPoint mParentPoint, const s_int& iX, const s_int& iY )
{
    pObj_ = pObj;
    mPoint_ = mPoint;
    pParent_ = pParent;
    mParentPoint_ = mParentPoint;
    iAbsOffX_ = iX;
    iAbsOffY_ = iY;
    mType_ = ANCHOR_ABS;
}

const s_int& Anchor::GetAbsX( s_bool bRelativeToParent )
{
    if (pObj_ != NULL)
    {
        s_int iParentX ;
        if (pParent_ != NULL)
        {
            iParentX = pParent_->GetLeft();
        }
        else if (pObj_->GetParent() != NULL)
        {
            pParent_ = pObj_->GetParent();
            iParentX = pParent_->GetLeft();
        }

        s_int iSelfOffset;
        if ((mPoint_ == ANCHOR_TOPLEFT) || (mPoint_ == ANCHOR_LEFT) || (mPoint_ == ANCHOR_BOTTOMLEFT))
        {
            //iSelfOffset = 0;
        }
        else if ((mPoint_ == ANCHOR_TOP) || (mPoint_ == ANCHOR_CENTER) || (mPoint_ == ANCHOR_BOTTOM))
        {
            iSelfOffset = -pObj_->GetAbsWidth()/2;
        }
        else if ((mPoint_ == ANCHOR_TOPRIGHT) || (mPoint_ == ANCHOR_RIGHT) || (mPoint_ == ANCHOR_BOTTOMRIGHT))
        {
            iSelfOffset = -pObj_->GetAbsWidth();
        }

        s_int iParentOffset;
        if ((mParentPoint_ == ANCHOR_TOPLEFT) || (mParentPoint_ == ANCHOR_LEFT) || (mParentPoint_ == ANCHOR_BOTTOMLEFT))
        {
            //iParentOffset = 0;
        }
        else if ((mParentPoint_ == ANCHOR_TOP) || (mParentPoint_ == ANCHOR_CENTER) || (mParentPoint_ == ANCHOR_BOTTOM))
        {
            if (pParent_ != NULL)
                iParentOffset = s_int(pParent_->GetAbsWidth())/2;
            else
                iParentOffset = s_int(Engine::GetSingleton()->GetScreenWidth())/2;
        }
        else if ((mParentPoint_ == ANCHOR_TOPRIGHT) || (mParentPoint_ == ANCHOR_RIGHT) || (mParentPoint_ == ANCHOR_BOTTOMRIGHT))
        {
            if (pParent_ != NULL)
                iParentOffset = s_int(pParent_->GetAbsWidth());
            else
                iParentOffset = s_int(Engine::GetSingleton()->GetScreenWidth());
        }

        s_int iX = iSelfOffset + iParentOffset;

        if (!bRelativeToParent)
            iX += iParentX;

        iAbsX_ = iAbsOffX_+iX;
    }

    return iAbsX_;
}

const s_int& Anchor::GetAbsY( s_bool bRelativeToParent )
{
    if (pObj_ != NULL)
    {
        s_int iParentY;
        if (pParent_ != NULL)
        {
            iParentY = pParent_->GetLeft();
        }
        else if (pObj_->GetParent() != NULL)
        {
            pParent_ = pObj_->GetParent();
            iParentY = pParent_->GetLeft();
        }

        s_int iSelfOffset;
        if ((mPoint_ == ANCHOR_TOPLEFT) || (mPoint_ == ANCHOR_TOP) || (mPoint_ == ANCHOR_TOPRIGHT))
        {
            //iSelfOffset = 0;
        }
        else if ((mPoint_ == ANCHOR_LEFT) || (mPoint_ == ANCHOR_CENTER) || (mPoint_ == ANCHOR_RIGHT))
        {
            iSelfOffset = -s_int(pObj_->GetAbsHeight())/2;
        }
        else if ((mPoint_ == ANCHOR_BOTTOMLEFT) || (mPoint_ == ANCHOR_BOTTOM) || (mPoint_ == ANCHOR_BOTTOMRIGHT))
        {
            iSelfOffset = -s_int(pObj_->GetAbsHeight());
        }

        s_int iParentOffset;
        if ((mParentPoint_ == ANCHOR_TOPLEFT) || (mParentPoint_ == ANCHOR_TOP) || (mParentPoint_ == ANCHOR_TOPRIGHT))
        {
            //iParentOffset = 0;
        }
        else if ((mParentPoint_ == ANCHOR_LEFT) || (mParentPoint_ == ANCHOR_CENTER) || (mParentPoint_ == ANCHOR_RIGHT))
        {
            if (pParent_ != NULL)
                iParentOffset = s_int(pParent_->GetAbsHeight())/2;
            else
                iParentOffset = s_int(Engine::GetSingleton()->GetScreenHeight())/2;
        }
        else if ((mParentPoint_ == ANCHOR_BOTTOMLEFT) || (mParentPoint_ == ANCHOR_BOTTOM) || (mParentPoint_ == ANCHOR_BOTTOMRIGHT))
        {
            if (pParent_ != NULL)
                iParentOffset = s_int(pParent_->GetAbsHeight());
            else
                iParentOffset = s_int(Engine::GetSingleton()->GetScreenHeight());
        }

        s_int iY = iSelfOffset + iParentOffset;

        if (!bRelativeToParent)
            iY += iParentY;

        iAbsY_ = iAbsOffY_+iY;
    }

    return iAbsY_;
}

s_ptr<UIObject> Anchor::GetObject() const
{
    return pObj_;
}

s_ptr<UIObject> Anchor::GetParent() const
{
    return pParent_;
}

AnchorPoint Anchor::GetPoint() const
{
    return mPoint_;
}

AnchorPoint Anchor::GetParentPoint() const
{
    return mParentPoint_;
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

void Anchor::SetParent( s_ptr<UIObject> pParent )
{
    pParent_ = pParent;
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

AnchorPoint Anchor::GetAnchorPoint( s_str sPoint )
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
