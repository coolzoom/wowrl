/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_fontstring.h"

#include "gui/frost_gui_layeredregion.h"
#include "gui/frost_guimanager.h"
#include "material/frost_material.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

#define OUTLINE_QUALITY   10.0f
#define OUTLINE_THICKNESS 2.0f

const s_str FontString::CLASS_NAME = "GUI::FontString";

FontString::FontString() : LayeredRegion()
{
    mObjectType_ = OJBECT_TYPE_FONTSTRING;
    lType_.PushBack("FontString");

    mTextColor_   = Color::WHITE;
    mShadowColor_ = Color::BLACK;
}

FontString::~FontString()
{
}

void FontString::Render()
{
    if (pText_ && IsVisible())
    {
        if (bHasShadow_)
        {
            pText_->SetColor(mShadowColor_, true);
            pText_->Render(s_float(lBorderList_[BORDER_LEFT] + iShadowXOffset_), s_float(lBorderList_[BORDER_TOP] + iShadowYOffset_));
        }

        if (bIsOutlined_)
        {
            pText_->SetColor(Color::BLACK, true);
            for (int i = 0; i < OUTLINE_QUALITY; i++)
            {
                pText_->Render(
                    s_float(lBorderList_[BORDER_LEFT]) + OUTLINE_THICKNESS*cos(2*s_float::PI*i/OUTLINE_QUALITY),
                    s_float(lBorderList_[BORDER_TOP])  + OUTLINE_THICKNESS*sin(2*s_float::PI*i/OUTLINE_QUALITY)
                );
            }
        }

        pText_->SetColor(mTextColor_);
        pText_->Render(s_float(lBorderList_[BORDER_LEFT]), s_float(lBorderList_[BORDER_TOP]));
    }
}

void FontString::Update()
{
    if (pText_)
        pText_->Update();
    UIObject::Update();
}

s_str FontString::Serialize(const s_str& sTab) const
{
    s_str sStr = UIObject::Serialize(sTab);

    return sStr;
}

void FontString::CreateGlue()
{
    s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
    pLua->PushString(sName_);
    lGlueList_.PushBack(
        pLua->Push<LuaFontString>(new LuaFontString(pLua->GetState()))
    );
    pLua->SetGlobal(sName_);
}

const s_str& FontString::GetFontName() const
{
    if (pText_)
        return pText_->GetFontName();
    else
        return s_str::EMPTY;
}

const s_float& FontString::GetFontHeight() const
{
    if (pText_)
        return pText_->GetFontSize();
    else
        return s_float::NaN;
}

void FontString::SetOutlined( const s_bool& bIsOutlined )
{
    bIsOutlined_ = bIsOutlined;
}

const s_bool& FontString::IsOutlined() const
{
    return bIsOutlined_;
}

Text::Alignment FontString::GetJustifyH() const
{
    if (pText_)
        return pText_->GetAlignment();
    else
        return Text::ALIGN_LEFT;
}

const Color& FontString::GetShadowColor() const
{
    return mShadowColor_;
}

s_array<s_int,2> FontString::GetShadowOffset() const
{
    return (iShadowXOffset_, iShadowYOffset_);
}

const s_int& FontString::GetShadowXOffset() const
{
    return iShadowXOffset_;
}

const s_int& FontString::GetShadowYOffset() const
{
    return iShadowYOffset_;
}

const s_float& FontString::GetSpacing() const
{
    if (pText_)
        return pText_->GetTracking();
    else
        return s_float::NaN;
}

const Color& FontString::GetTextColor() const
{
    return mTextColor_;
}

void FontString::SetFont( const s_str& sFontName, const s_uint& uiHeight )
{
    pText_ = s_refptr<Text>(new Text(sFontName, s_float(uiHeight)));
    pText_->SetRemoveStartingSpaces(true);
}

void FontString::SetJustifyH( Text::Alignment mJustifyH )
{
    if (pText_)
        pText_->SetAlignment(mJustifyH);
}

void FontString::SetShadowColor( const Color& mShadowColor )
{
    mShadowColor_ = mShadowColor;
}

void FontString::SetShadowOffsets( const s_int& iShadowXOffset, const s_int& iShadowYOffset )
{
    iShadowXOffset_ = iShadowXOffset;
    iShadowYOffset_ = iShadowYOffset;
}

void FontString::SetShadowOffsets( const s_array<s_int,2>& lShadowOffsets )
{
    iShadowXOffset_ = lShadowOffsets[0];
    iShadowYOffset_ = lShadowOffsets[1];
}

void FontString::SetSpacing( const s_float& fSpacing )
{
    if (pText_)
        pText_->SetTracking(fSpacing);
    else
    {
        Error(lType_.Back(),
            "Trying to call SetSpacing on an uninitialized Texture : "+sName_+"."
        );
    }
}

void FontString::SetTextColor( const Color& mTextColor )
{
    mTextColor_ = mTextColor;
}

const s_bool& FontString::CanNonSpaceWrap() const
{
    return bCanNonSpaceWrap_;
}

s_float FontString::GetStringHeight() const
{
    if (pText_)
        return pText_->GetTextHeight();
    else
        return s_float::NaN;
}

s_float FontString::GetStringWidth() const
{
    if (pText_)
        return pText_->GetTextWidth();
    else
        return s_float::NaN;
}

const s_str& FontString::GetText() const
{
    if (pText_)
        return pText_->GetText();
    else
        return s_str::EMPTY;
}

void FontString::SetNonSpaceWrap( const s_bool& bCanNonSpaceWrap )
{
    bCanNonSpaceWrap_ = bCanNonSpaceWrap;
}

const s_bool& FontString::HasShadow() const
{
    return bHasShadow_;
}

void FontString::SetShadow( const s_bool& bHasShadow )
{
    bHasShadow_ = bHasShadow;
}

void FontString::SetText( const s_str& sText )
{
    if (pText_)
    {
        pText_->SetText(sText);
        FireUpdateBorders();
        uiAbsWidth_ = uiAbsHeight_ = s_uint::NaN;
    }
    else
    {
        Error(lType_.Back(),
            "Trying to call SetText on an uninitialized FontString : "+sName_+"."
        );
    }
}

void FontString::UpdateBorders_()
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

        if (uiAbsWidth_.IsValid())
            pText_->SetBoxWidth(s_float(uiAbsWidth_));
        else
        {
            if (lDefinedBorderList_[BORDER_LEFT] && lDefinedBorderList_[BORDER_RIGHT])
            {
                uiAbsWidth_ = s_uint(iRight - iLeft);
                pText_->SetBoxWidth(s_float(uiAbsWidth_));
            }
            else
            {
                pText_->SetBoxWidth(s_float::NaN);
                uiAbsWidth_ = s_uint(pText_->GetWidth());
            }
        }

        if (uiAbsHeight_.IsValid())
            pText_->SetBoxHeight(s_float(uiAbsHeight_));
        else
        {
            if (lDefinedBorderList_[BORDER_TOP] && lDefinedBorderList_[BORDER_BOTTOM])
            {
                uiAbsHeight_ = s_uint(iBottom - iTop);
                pText_->SetBoxHeight(s_float(uiAbsHeight_));
            }
            else
            {
                pText_->SetBoxHeight(s_float::NaN);
                uiAbsHeight_ = s_uint(pText_->GetHeight());
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
