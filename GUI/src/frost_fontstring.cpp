/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_fontstring.h"

#include "frost_layeredregion.h"
#include "frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

#define OUTLINE_QUALITY   10
#define OUTLINE_THICKNESS 2.0f

const s_str FontString::CLASS_NAME = "GUI::FontString";

FontString::FontString(s_ptr<GUIManager> pManager) : LayeredRegion(pManager)
{
    lType_.PushBack("FontString");

    mTextColor_   = Color::WHITE;
    mShadowColor_ = Color::BLACK;

    mJustifyH_ = Text::ALIGN_CENTER;
    mJustifyV_ = Text::ALIGN_MIDDLE;

    bCanWordWrap_ = true;
    bAddEllipsis_ = true;
    bFormattingEnabled_ = true;
}

FontString::~FontString()
{
}

void FontString::Render()
{
    if (pText_ && IsVisible())
    {
        s_float fX, fY;

        if (pText_->GetBoxWidth().IsValid())
        {
            fX = s_float(lBorderList_[BORDER_LEFT]);
        }
        else
        {
            switch (mJustifyH_)
            {
                case Text::ALIGN_LEFT   : fX = s_float(lBorderList_[BORDER_LEFT]); break;
                case Text::ALIGN_CENTER : fX = s_float((lBorderList_[BORDER_LEFT] + lBorderList_[BORDER_RIGHT])/2); break;
                case Text::ALIGN_RIGHT  : fX = s_float(lBorderList_[BORDER_RIGHT]); break;
            }
        }

        if (pText_->GetBoxHeight().IsValid())
        {
            fY = s_float(lBorderList_[BORDER_TOP]);
        }
        else
        {
            switch (mJustifyV_)
            {
                case Text::ALIGN_TOP    : fY = s_float(lBorderList_[BORDER_TOP]); break;
                case Text::ALIGN_MIDDLE : fY = s_float((lBorderList_[BORDER_TOP] + lBorderList_[BORDER_BOTTOM])/2); break;
                case Text::ALIGN_BOTTOM : fY = s_float(lBorderList_[BORDER_BOTTOM]); break;
            }
        }

        fX += s_float(iXOffset_);
        fY += s_float(iYOffset_);

        if (bHasShadow_)
        {
            pText_->SetColor(mShadowColor_, true);
            pText_->Render(fX + s_float(iShadowXOffset_), fY + s_float(iShadowYOffset_));
        }

        if (bIsOutlined_)
        {
            pText_->SetColor(Color::BLACK, true);
            for (int i = 0; i < OUTLINE_QUALITY; ++i)
            {
                pText_->Render(
                    fX + OUTLINE_THICKNESS*cos(s_float(i)/OUTLINE_QUALITY),
                    fY + OUTLINE_THICKNESS*sin(s_float(i)/OUTLINE_QUALITY)
                );
            }
        }

        pText_->SetColor(mTextColor_);
        pText_->Render(fX, fY);
    }
}

void FontString::Update()
{
    if (pText_)
    {
        pText_->Update();
    }

    UIObject::Update();
}

s_str FontString::Serialize(const s_str& sTab) const
{
    s_str sStr = LayeredRegion::Serialize(sTab);

    sStr << sTab << "  # Font name   : " << sFontName_ << "\n";
    sStr << sTab << "  # Font height : " << uiHeight_ << "\n";
    sStr << sTab << "  # Text ready  : " << pText_.IsValid() << "\n";
    sStr << sTab << "  # Text        : \"" << sText_ << "\"\n";
    sStr << sTab << "  # Outlined    : " << bIsOutlined_ << "\n";
    sStr << sTab << "  # Text color  : " +  mTextColor_ << "\n";
    sStr << sTab << "  # Spacing     : " << fSpacing_ << "\n";
    sStr << sTab << "  # Justify     :\n";
    sStr << sTab << "  #-###\n";
    sStr << sTab << "  |   # horizontal : ";
    switch (mJustifyH_)
    {
        case Text::ALIGN_LEFT : sStr << "LEFT\n"; break;
        case Text::ALIGN_CENTER : sStr << "CENTER\n"; break;
        case Text::ALIGN_RIGHT : sStr << "RIGHT\n"; break;
        default : sStr << "<error>\n"; break;
    }
    sStr << sTab << "  |   # vertical   : ";
    switch (mJustifyH_)
    {
        case Text::ALIGN_TOP : sStr << "TOP\n"; break;
        case Text::ALIGN_MIDDLE : sStr << "MIDDLE\n"; break;
        case Text::ALIGN_BOTTOM : sStr << "BOTTOM\n"; break;
        default : sStr << "<error>\n"; break;
    }
    sStr << sTab << "  #-###\n";
    sStr << sTab << "  # NonSpaceW.  : " << bCanNonSpaceWrap_ << "\n";
    if (bHasShadow_)
    {
    sStr << sTab << "  # Shadow off. : " + (iShadowXOffset_, iShadowYOffset_) << "\n";
    sStr << sTab << "  # Shadow col. : " +  mShadowColor_ << "\n";
    }

    return sStr;
}

void FontString::CreateGlue()
{
    s_ptr<Lua::State> pLua = pManager_->GetLua();
    pLua->PushString(sName_);
    lGlueList_.PushBack(pLua->PushNew<LuaFontString>());
    pLua->SetGlobal(sLuaName_);
}

void FontString::CopyFrom( s_ptr<UIObject> pObj )
{
    UIObject::CopyFrom(pObj);

    s_ptr<FontString> pFontString = s_ptr<FontString>::DynamicCast(pObj);

    if (pFontString)
    {
        s_str sFontName = pFontString->GetFontName();
        s_uint uiHeight = pFontString->GetFontHeight();
        if (!sFontName.IsEmpty() && !uiHeight.IsNull())
        {
            this->SetFont(sFontName, uiHeight);
        }

        this->SetJustifyH(pFontString->GetJustifyH());
        this->SetJustifyV(pFontString->GetJustifyV());
        this->SetSpacing(pFontString->GetSpacing());
        this->SetText(pFontString->GetText());
        this->SetOutlined(pFontString->IsOutlined());
        if (pFontString->HasShadow())
        {
            this->SetShadow(true);
            this->SetShadowColor(pFontString->GetShadowColor());
            this->SetShadowOffsets(pFontString->GetShadowOffsets());
        }
        this->SetTextColor(pFontString->GetTextColor());
        this->SetNonSpaceWrap(pFontString->CanNonSpaceWrap());
    }
}

const s_str& FontString::GetFontName() const
{
    return sFontName_;
}

const s_uint& FontString::GetFontHeight() const
{
    return uiHeight_;
}

void FontString::SetOutlined( const s_bool& bIsOutlined )
{
    if (bIsOutlined_ != bIsOutlined)
    {
        bIsOutlined_ = bIsOutlined;
        NotifyRendererNeedRedraw();
    }
}

const s_bool& FontString::IsOutlined() const
{
    return bIsOutlined_;
}

Text::Alignment FontString::GetJustifyH() const
{
    return mJustifyH_;
}

Text::VerticalAlignment FontString::GetJustifyV() const
{
    return mJustifyV_;
}

const Color& FontString::GetShadowColor() const
{
    return mShadowColor_;
}

s_array<s_int,2> FontString::GetShadowOffsets() const
{
    return (iShadowXOffset_, iShadowYOffset_);
}

s_array<s_int,2> FontString::GetOffsets() const
{
    return (iXOffset_, iYOffset_);
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
    return fSpacing_;
}

const Color& FontString::GetTextColor() const
{
    return mTextColor_;
}

void FontString::SetFont( const s_str& sFontName, const s_uint& uiHeight )
{
    sFontName_ = sFontName;
    uiHeight_ = uiHeight;

    pText_ = s_refptr<Text>(new Text(pManager_, sFontName, s_float(uiHeight)));
    pText_->SetRemoveStartingSpaces(true);
    pText_->SetText(sText_);
    pText_->SetAlignment(mJustifyH_);
    pText_->SetVerticalAlignment(mJustifyV_);
    pText_->SetTracking(fSpacing_);
    pText_->EnableWordWrap(bCanWordWrap_, bAddEllipsis_);
    pText_->EnableFormatting(bFormattingEnabled_);

    FireUpdateBorders();
    NotifyRendererNeedRedraw();
}

void FontString::SetJustifyH( Text::Alignment mJustifyH )
{
    if (mJustifyH_ != mJustifyH)
    {
        mJustifyH_ = mJustifyH;
        if (pText_)
        {
            pText_->SetAlignment(mJustifyH_);
            NotifyRendererNeedRedraw();
        }
    }
}

void FontString::SetJustifyV( Text::VerticalAlignment mJustifyV )
{
    if (mJustifyV_ != mJustifyV)
    {
        mJustifyV_ = mJustifyV;
        if (pText_)
        {
            pText_->SetVerticalAlignment(mJustifyV_);
            NotifyRendererNeedRedraw();
        }
    }
}

void FontString::SetShadowColor( const Color& mShadowColor )
{
    if (mShadowColor_ != mShadowColor)
    {
        mShadowColor_ = mShadowColor;
        if (bHasShadow_)
        {
            NotifyRendererNeedRedraw();
        }
    }
}

void FontString::SetShadowOffsets( const s_int& iShadowXOffset, const s_int& iShadowYOffset )
{
    if (iShadowXOffset_ != iShadowXOffset || iShadowYOffset_ != iShadowYOffset)
    {
        iShadowXOffset_ = iShadowXOffset;
        iShadowYOffset_ = iShadowYOffset;
        if (bHasShadow_)
        {
            NotifyRendererNeedRedraw();
        }
    }
}

void FontString::SetShadowOffsets( const s_array<s_int,2>& lShadowOffsets )
{
    if (iShadowXOffset_ != lShadowOffsets[0] || iShadowYOffset_ != lShadowOffsets[1])
    {
        iShadowXOffset_ = lShadowOffsets[0];
        iShadowYOffset_ = lShadowOffsets[1];
        if (bHasShadow_)
        {
            NotifyRendererNeedRedraw();
        }
    }
}

void FontString::SetOffsets( const s_int& iXOffset, const s_int& iYOffset )
{
    if (iXOffset_ != iXOffset || iYOffset_ != iYOffset)
    {
        iXOffset_ = iXOffset;
        iYOffset_ = iYOffset;
        NotifyRendererNeedRedraw();
    }
}

void FontString::SetOffsets( const s_array<s_int,2>& lOffsets )
{
    if (iXOffset_ != lOffsets[0] || iYOffset_ != lOffsets[1])
    {
        iXOffset_ = lOffsets[0];
        iYOffset_ = lOffsets[1];
        NotifyRendererNeedRedraw();
    }
}

void FontString::SetSpacing( const s_float& fSpacing )
{
    if (fSpacing_ != fSpacing)
    {
        fSpacing_ = fSpacing;
        if (pText_)
        {
            pText_->SetTracking(fSpacing_);
            NotifyRendererNeedRedraw();
        }
    }
}

void FontString::SetTextColor( const Color& mTextColor )
{
    if (mTextColor_ != mTextColor)
    {
        mTextColor_ = mTextColor;
        NotifyRendererNeedRedraw();
    }
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
    return sText_;
}

const s_ustr& FontString::GetUnicodeText() const
{
    if (pText_)
        return pText_->GetUnicodeText();
    else
        return s_ustr::EMPTY;
}

void FontString::SetNonSpaceWrap( const s_bool& bCanNonSpaceWrap )
{
    if (bCanNonSpaceWrap_ != bCanNonSpaceWrap)
    {
        bCanNonSpaceWrap_ = bCanNonSpaceWrap;
        NotifyRendererNeedRedraw();
    }
}

const s_bool& FontString::HasShadow() const
{
    return bHasShadow_;
}

void FontString::SetShadow( const s_bool& bHasShadow )
{
    if (bHasShadow_ != bHasShadow)
    {
        bHasShadow_ = bHasShadow;
        NotifyRendererNeedRedraw();
    }
}

void FontString::SetWordWrap( const s_bool& bCanWordWrap, const s_bool& bAddEllipsis )
{
    bCanWordWrap_ = bCanWordWrap;
    bAddEllipsis_ = bAddEllipsis;
    if (pText_)
    {
        pText_->EnableWordWrap(bCanWordWrap_, bAddEllipsis_);
    }
}

const s_bool& FontString::CanWordWrap() const
{
    return bCanWordWrap_;
}

void FontString::EnableFormatting( const s_bool& bFormatting )
{
    bFormattingEnabled_ = bFormatting;
    if (pText_)
    {
        pText_->EnableFormatting(bFormattingEnabled_);
    }
}

const s_bool& FontString::IsFormattingEnabled() const
{
    return bFormattingEnabled_;
}

void FontString::SetText( const s_str& sText )
{
    if (sText_ != sText)
    {
        sText_ = sText;
        if (pText_)
        {
            pText_->SetText(sText_);
            FireUpdateBorders();
        }
    }
}

s_wptr<Text> FontString::GetTextObject()
{
    return pText_;
}

void FontString::UpdateBorders_() const
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

        if (uiAbsWidth_.IsValid() && !uiAbsWidth_.IsNull())
        {
            pText_->SetBoxWidth(s_float(uiAbsWidth_));
        }
        else
        {
            if (lDefinedBorderList_[BORDER_LEFT] && lDefinedBorderList_[BORDER_RIGHT])
            {
                pText_->SetBoxWidth(s_float(iRight - iLeft));
            }
            else
            {
                pText_->SetBoxWidth(s_float::NaN);
            }
        }

        if (uiAbsHeight_.IsValid() && !uiAbsHeight_.IsNull())
        {
            pText_->SetBoxHeight(s_float(uiAbsHeight_));
        }
        else
        {
            if (lDefinedBorderList_[BORDER_TOP] && lDefinedBorderList_[BORDER_BOTTOM])
            {
                pText_->SetBoxHeight(s_float(iBottom - iTop));
            }
            else
            {
                pText_->SetBoxHeight(s_float::NaN);
            }
        }

        if (uiAbsHeight_.IsValid())
            MakeBorders_(iTop, iBottom, iYCenter, s_int(uiAbsHeight_));
        else
            MakeBorders_(iTop, iBottom, iYCenter, s_int(pText_->GetHeight()));

        if (uiAbsWidth_.IsValid())
            MakeBorders_(iLeft, iRight, iXCenter, s_int(uiAbsWidth_));
        else
            MakeBorders_(iLeft, iRight, iXCenter, s_int(pText_->GetWidth()));

        if (bReady_)
        {
            lBorderList_[BORDER_LEFT] = iLeft;
            lBorderList_[BORDER_RIGHT] = iRight;
            lBorderList_[BORDER_TOP] = iTop;
            lBorderList_[BORDER_BOTTOM] = iBottom;

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
        NotifyRendererNeedRedraw();
}
