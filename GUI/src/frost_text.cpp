/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Text source              */
/*                                        */
/*                                        */

#include "frost_text.h"

#include "frost_fontmanager.h"
#include "frost_font.h"
#include "frost_sprite.h"
#include "frost_material.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str Text::CLASS_NAME = "GUI::Text";

Text::Text( s_ptr<GUIManager> pManager, const s_str& sFileName, const s_float& fSize )
{
    pManager_ = pManager;
    fBoxW_ = fBoxH_ = s_float::INFPLUS;
    mAlign_ = ALIGN_LEFT;
    mVertAlign_ = ALIGN_MIDDLE;
    fLineSpacing_ = 1.5f;
    fTracking_ = 0.0f;
    bWordWrap_ = true;
    sFileName_ = sFileName;
    fSize_ = fSize;
    mColor_ = Color(255, 255, 255);
    fX_ = fY_ = s_float::INFPLUS;

    pFont_ = FontManager::GetSingleton()->GetFont(sFileName_, s_uint(fSize_));
    if (pFont_)
    {
        bReady_ = true;
        fSpaceWidth_ = pFont_->GetCharacterWidth(33);
    }
    else
    {
        Error(CLASS_NAME,
            "Error initializing \""+sFileName+"\" (size : "+fSize+")."
        );
    }
}

Text::~Text()
{
}

const s_str& Text::GetFontName() const
{
    return sFileName_;
}

const s_float& Text::GetFontSize() const
{
    return fSize_;
}

s_float Text::GetLineHeight() const
{
    return fSize_;
}

void Text::SetText( const s_str& sText )
{
    if (sText_ != sText)
    {
        sText_ = sText;
        sUnicodeText_ = UTF8ToUnicode(sText_);
        bUpdateCache_ = true;
    }
}

const s_str& Text::GetText() const
{
    return sText_;
}

const s_ustr& Text::GetUnicodeText() const
{
    return sUnicodeText_;
}

void Text::SetColor( const Color& mColor, const s_bool& bForceColor )
{
    if (mColor_ != mColor || bForceColor_ != bForceColor)
    {
        mColor_ = mColor;
        bForceColor_ = bForceColor;
        bUpdateQuads_ = true;
    }
}

const Color& Text::GetColor() const
{
    return mColor_;
}

void Text::SetDimensions( const s_float& fW, const s_float& fH )
{
    if (((fBoxW_ != fW) && (!fBoxW_.IsNaN() && !fW.IsNaN())) ||
        ((fBoxH_ != fH) && (!fBoxH_.IsNaN() && !fH.IsNaN())))
    {
        fBoxW_ = fW; fBoxH_ = fH;
        bUpdateCache_ = true;
    }
}

void Text::SetBoxWidth( const s_float& fBoxW )
{
    if ( (fBoxW_ != fBoxW) && (!fBoxW_.IsNaN() && !fBoxW.IsNaN()) )
    {
        fBoxW_ = fBoxW;
        bUpdateCache_ = true;
    }
}

void Text::SetBoxHeight( const s_float& fBoxH )
{
    if ( (fBoxH_ != fBoxH) && (!fBoxH_.IsNaN() && !fBoxH.IsNaN()) )
    {
        fBoxH_ = fBoxH;
        bUpdateCache_ = true;
    }
}

const s_float& Text::GetWidth()
{
    Update();

    return fW_;
}

const s_float& Text::GetHeight()
{
    Update();

    return fH_;
}

const s_float& Text::GetBoxWidth() const
{
    return fBoxW_;
}

const s_float& Text::GetBoxHeight() const
{
    return fBoxH_;
}

s_float Text::GetTextWidth() const
{
    s_float fWidth;
    s_float fMaxWidth = s_float::INFMINUS;

    if (bReady_)
    {
        s_ustr::const_iterator iterChar, iterNext;
        foreach (iterChar, sUnicodeText_)
        {
            iterNext = iterChar + 1;
            if (*iterChar == '\n')
            {
                if (fWidth > fMaxWidth)
                    fMaxWidth = fWidth;

                fWidth = 0.0f;
            }
            else
            {
                fWidth += GetCharacterWidth((uint)(uchar)*iterChar) + fTracking_;
                if (iterNext != sUnicodeText_.End() && *iterChar != ' ' && *iterNext != ' ' && *iterNext != '\n')
                    fWidth += GetCharacterKerning((uint)(uchar)*iterChar, (uint)(uchar)*iterNext);
            }
        }
    }

    return fWidth;
}

s_float Text::GetTextHeight() const
{
    s_float fHeight;

    if (bReady_)
    {
        fHeight = GetLineHeight()*(s_float(sText_.CountOccurences("\n"))*GetLineHeight()*fLineSpacing_ + 1.0f);
    }

    return fHeight;
}

s_float Text::GetStringWidth( const s_str& sString ) const
{
    return GetStringWidth(UTF8ToUnicode(sString));
}

s_float Text::GetStringWidth( const s_ustr& sString ) const
{
    s_float fWidth;
    s_float fMaxWidth = s_float::INFMINUS;
    if (bReady_)
    {
        s_ustr::const_iterator iterChar, iterNext;
        foreach (iterChar, sString)
        {
            iterNext = iterChar + 1;
            if (*iterChar == ' ')
                fWidth += fSpaceWidth_;
            else if (*iterChar == '\n')
            {
                if (fWidth > fMaxWidth)
                    fMaxWidth = fWidth;
                fWidth = 0.0f;
            }
            else
            {
                fWidth += GetCharacterWidth((uint)(uchar)*iterChar) + fTracking_;
                if (iterNext != sString.End())
                {
                    if (*iterNext != ' ' && *iterNext != '\n')
                        fWidth += GetCharacterKerning((uint)(uchar)*iterChar, (uint)(uchar)*iterNext);
                }
            }
        }
    }

    return fWidth;
}

s_float Text::GetCharacterWidth( const s_uint& uiChar ) const
{
    if (bReady_)
    {
        if (uiChar == 32) // Space
            return fSpaceWidth_;
        else if (uiChar == 9) // Tab
            return 4.0f*fSpaceWidth_;
        else
        {
            return pFont_->GetCharacterWidth(uiChar);
        }
    }
    else
        return 0.0f;
}

s_float Text::GetCharacterKerning( const s_uint& uiChar1, const s_uint& uiChar2 ) const
{
    return pFont_->GetCharacterKerning(uiChar1, uiChar2);
}

void Text::SetAlignment( const Text::Alignment& mAlign )
{
    if (mAlign_ != mAlign)
    {
        mAlign_ = mAlign;
        bUpdateCache_ = true;
    }
}

void Text::SetVerticalAlignment( const Text::VerticalAlignment& mVertAlign )
{
    if (mVertAlign_ != mVertAlign)
    {
        mVertAlign_ = mVertAlign;
        bUpdateCache_ = true;
    }
}

const Text::Alignment& Text::GetAlignment() const
{
    return mAlign_;
}

const Text::VerticalAlignment& Text::GetVerticalAlignment() const
{
    return mVertAlign_;
}

void Text::SetTracking( const s_float& fTracking )
{
    if (fTracking_ != fTracking)
    {
        fTracking_ = fTracking;
        bUpdateCache_ = true;
    }
}

const s_float& Text::GetTracking() const
{
    return fTracking_;
}

void Text::SetLineSpacing( const s_float& fLineSpacing )
{
    if (fLineSpacing_ != fLineSpacing)
    {
        fLineSpacing_ = fLineSpacing;
        bUpdateCache_ = true;
    }
}

const s_float& Text::GetLineSpacing() const
{
    return fLineSpacing_;
}

void Text::SetRemoveStartingSpaces( const s_bool& bRemoveStartingSpaces )
{
    if (bRemoveStartingSpaces_ != bRemoveStartingSpaces)
    {
        bRemoveStartingSpaces_ = bRemoveStartingSpaces;
        bUpdateCache_ = true;
    }
}

const s_bool& Text::GetRemoveStartingSpaces() const
{
    return bRemoveStartingSpaces_;
}

void Text::EnableWordWrap( const s_bool& bWrap, const s_bool& bAddEllipsis )
{
    if (bWordWrap_ != bWrap || bAddEllipsis_ != bAddEllipsis)
    {
        bWordWrap_ = bWrap;
        bAddEllipsis_ = bAddEllipsis;
        bUpdateCache_ = true;
    }
}

const s_bool& Text::IsWordWrapEnabled() const
{
    return bWordWrap_;
}

void Text::EnableFormatting( const s_bool& bFormatting )
{
    if (bFormattingEnabled_ != bFormatting)
    {
        bFormattingEnabled_ = bFormatting;
        bUpdateCache_ = true;
    }
}

void Text::Render( const s_float& fX, const s_float& fY )
{
    if (bReady_)
    {
        Update();

        if (fX != fX_ || fY != fY_)
            bUpdateQuads_ = true;

        if (bUpdateQuads_)
        {
            fX_ = fX;
            fY_ = fY;

            lQuadList_.Clear();

            Sprite mSprite(pManager_, pFont_->GetTexture());

            s_array<Vertex,4> lVertexList;

            if (!bFormattingEnabled_)
            {
                for (int i = 0; i < 4; ++i)
                    lVertexList[i].mColor = mColor_;
            }

            s_ctnr<Letter>::iterator iterLetter;
            foreach (iterLetter, lLetterCache_)
            {
                if (iterLetter->bNoRender)
                    continue;

                s_float fX1 = iterLetter->fX1+fX;
                s_float fY1 = iterLetter->fY1+fY;
                s_float fX2 = iterLetter->fX2+fX;
                s_float fY2 = iterLetter->fY2+fY;

                lVertexList[0].mPosition = Vector2D(fX1, fY1);
                lVertexList[1].mPosition = Vector2D(fX2, fY1);
                lVertexList[2].mPosition = Vector2D(fX2, fY2);
                lVertexList[3].mPosition = Vector2D(fX1, fY2);

                lVertexList[0].mUVs = Vector2D(iterLetter->fU1, iterLetter->fV1);
                lVertexList[1].mUVs = Vector2D(iterLetter->fU2, iterLetter->fV1);
                lVertexList[2].mUVs = Vector2D(iterLetter->fU2, iterLetter->fV2);
                lVertexList[3].mUVs = Vector2D(iterLetter->fU1, iterLetter->fV2);

                if (bFormattingEnabled_)
                {
                    if (!iterLetter->mColor.IsNaN() && !bForceColor_)
                    {
                        for (int i = 0; i < 4; ++i)
                            lVertexList[i].mColor = iterLetter->mColor;
                    }
                    else
                    {
                        for (int i = 0; i < 4; ++i)
                            lVertexList[i].mColor = mColor_;
                    }
                }

                mSprite.SetQuad(lVertexList);

                lQuadList_.PushBack(mSprite);
            }

            bUpdateQuads_ = false;
        }

        s_ctnr<Sprite>::iterator iterQuad;
        foreach (iterQuad, lQuadList_)
        {
            iterQuad->RenderStatic();
        }
    }
}

void Text::Update()
{
    if (bReady_ && bUpdateCache_)
    {
        UpdateLines_();
        UpdateCache_();
        bUpdateCache_ = false;
        bUpdateQuads_ = true;
    }
}

void GetFormat( s_ustr::iterator& iterChar, Text::Format& mFormat )
{
    if (*iterChar == 'r')
    {
        mFormat.mColorAction = Text::COLOR_ACTION_RESET;
    }
    else if (*iterChar == 'c')
    {
        s_str sColorPart;
        iterChar++;
        sColorPart += *iterChar; iterChar++;
        sColorPart += *iterChar; iterChar++;
        s_float fA = s_float(sColorPart.HexToUInt())/255.0f;
        sColorPart.Clear();
        sColorPart += *iterChar; iterChar++;
        sColorPart += *iterChar; iterChar++;
        s_float fR = s_float(sColorPart.HexToUInt())/255.0f;
        sColorPart.Clear();
        sColorPart += *iterChar; iterChar++;
        sColorPart += *iterChar; iterChar++;
        s_float fG = s_float(sColorPart.HexToUInt())/255.0f;
        sColorPart.Clear();
        sColorPart += *iterChar; iterChar++;
        sColorPart += *iterChar;
        s_float fB = s_float(sColorPart.HexToUInt())/255.0f;

        mFormat.mColorAction = Text::COLOR_ACTION_SET;
        mFormat.mColor = Color(fA, fR, fG, fB);
    }
}

void Text::UpdateLines_()
{
    // Update the line list, read format tags, do word wrapping, ...
    lLineList_.Clear();
    lFormatList_.Clear();

    s_uint uiMaxLineNbr, uiCounter;
    if (fBoxH_.IsValid())
    {
        if (fBoxH_ < GetLineHeight())
        {
            uiMaxLineNbr = 0;
            return;
        }
        else
        {
            s_float fRemaining = fBoxH_ - GetLineHeight();
            uiMaxLineNbr = 1 + s_uint(s_float::RoundDown(fRemaining/(GetLineHeight()*fLineSpacing_)));
        }
    }
    else
        uiMaxLineNbr = s_uint::INF;

    if (uiMaxLineNbr >= 1)
    {
        s_ctnr<s_ustr> lManualLineList = sUnicodeText_.CutEach("\n");
        s_ctnr<s_ustr>::iterator iterManual;
        foreach (iterManual, lManualLineList)
        {
            // Make a temporary line array
            s_ctnr<Line> lLines;
            Line mLine;
            s_map<s_uint, Format> lTempFormatList;

            s_ustr::iterator iterChar1;
            foreach (iterChar1, *iterManual)
            {
                // Read format tags
                if (*iterChar1 == '|' && bFormattingEnabled_)
                {
                    ++iterChar1;
                    if (iterChar1 != iterManual->End())
                    {
                        if (*iterChar1 == '|')
                        {
                        }
                        else
                        {
                            GetFormat(iterChar1, lTempFormatList[uiCounter+mLine.sCaption.GetLength()]);
                            continue;
                        }
                    }
                    else
                        break;
                }

                if (*iterChar1 == ' ')
                    mLine.fWidth += fSpaceWidth_;
                else
                {
                    mLine.fWidth += GetCharacterWidth((uint)(uchar)*iterChar1) + fTracking_;
                    s_ustr::iterator iterNext = iterChar1 + 1;
                    if (iterNext != iterManual->End())
                    {
                        if (*iterNext != ' ')
                            mLine.fWidth += GetCharacterKerning((uint)(uchar)*iterChar1, (uint)(uchar)*iterNext);
                    }
                }
                mLine.sCaption += *iterChar1;

                if (mLine.fWidth > fBoxW_)
                {
                    // Whoops, the line is too long...
                    if (mLine.sCaption.FindPos(" ").IsValid() && bWordWrap_)
                    {
                        // There are several words on this line, we'll
                        // be able to put the last one on the next line
                        s_ustr::iterator iterChar2 = mLine.sCaption.End();
                        s_ustr sErasedString;
                        s_uint uiCharToErase;
                        s_float fErasedWidth;
                        s_bool bLastWasWord;
                        while ( (mLine.fWidth > fBoxW_) && (iterChar2 != mLine.sCaption.Begin()) )
                        {
                            --iterChar2;
                            if (*iterChar2 == ' ')
                            {
                                if ( bLastWasWord && (mLine.fWidth-fErasedWidth <= fBoxW_) && !bRemoveStartingSpaces_ )
                                {
                                    break;
                                }
                                else
                                {
                                    mLine.fWidth -= fErasedWidth + fSpaceWidth_;
                                    sErasedString.PushFront(*iterChar2);
                                    fErasedWidth = 0.0f;
                                    ++uiCharToErase;
                                }
                            }
                            else
                            {
                                fErasedWidth += GetCharacterWidth((uint)(uchar)*iterChar2) + fTracking_;
                                sErasedString.PushFront(*iterChar2);
                                ++uiCharToErase;
                                bLastWasWord = true;
                            }
                        }

                        if (bRemoveStartingSpaces_)
                        {
                            while (*iterChar2 == ' ')
                            {
                                --uiCharToErase;
                                sErasedString.EraseFromStart(1);
                                ++iterChar2;
                            }
                        }

                        mLine.sCaption.EraseFromEnd(uiCharToErase);

                        lLines.PushBack(mLine);
                        s_map<s_uint, Format>::iterator iterFormat;
                        foreach (iterFormat, lTempFormatList)
                        {
                            lFormatList_[iterFormat->first] = iterFormat->second;
                        }
                        lTempFormatList.Clear();
                        uiCounter += mLine.sCaption.GetLength();
                        mLine.fWidth = GetStringWidth(sErasedString);
                        mLine.sCaption = sErasedString;
                    }
                    else
                    {
                        // There is only one word on this line, or word
                        // wrap is disabled. Anyway, this line is just
                        // too long for the text box : our only option
                        // is to truncate it.
                        if (bAddEllipsis_)
                        {
                            s_float fWordWidth = 3*(GetCharacterWidth((uint)'.') + fTracking_);
                            s_ustr::iterator iterChar2 = mLine.sCaption.End();
                            s_uint uiCharToErase;
                            while ( (mLine.fWidth + fWordWidth > fBoxW_) && (iterChar2 != mLine.sCaption.Begin()) )
                            {
                                --iterChar2;
                                mLine.fWidth -= GetCharacterWidth((uint)(uchar)*iterChar2) + fTracking_;
                                ++uiCharToErase;
                            }
                            mLine.sCaption.EraseFromEnd(uiCharToErase);
                            mLine.sCaption << "...";
                        }
                        else
                        {
                            s_ustr::iterator iterChar2 = mLine.sCaption.End();
                            s_uint uiCharToErase;
                            while ( (mLine.fWidth  > fBoxW_) && (iterChar2 != mLine.sCaption.Begin()) )
                            {
                                --iterChar2;
                                mLine.fWidth -= GetCharacterWidth((uint)(uchar)*iterChar2) + fTracking_;
                                ++uiCharToErase;
                            }
                            mLine.sCaption.EraseFromEnd(uiCharToErase);
                        }

                        if (!bWordWrap_)
                        {
                            // Word wrap is disabled, so we can only display one line
                            // anyway.
                            lLineList_.PushBack(mLine);
                            s_map<s_uint, Format>::iterator iterFormat;
                            foreach (iterFormat, lTempFormatList)
                            {
                                lFormatList_[iterFormat->first] = iterFormat->second;
                            }
                            return;
                        }

                        s_ustr::iterator iterTemp = iterChar1;
                        iterChar1 = iterManual->Get(" ", s_uint(s_ptrdiff(iterChar1 - iterManual->begin())));

                        if (iterChar1 != iterManual->End())
                        {
                            // Read cutted format tags
                            if (bFormattingEnabled_)
                            {
                                while (iterTemp != iterChar1)
                                {
                                    if ((*iterTemp) == '|')
                                    {
                                        ++iterTemp;
                                        if (iterTemp != iterChar1)
                                        {
                                            if ((*iterTemp) == '|')
                                            {
                                            }
                                            else
                                            {
                                                GetFormat(iterTemp, lTempFormatList[uiCounter+mLine.sCaption.GetLength()]);
                                            }
                                        }
                                    }
                                    ++iterTemp;
                                }
                            }

                            // Look for the next word
                            while (iterChar1 != iterManual->End())
                            {
                                if ((*iterChar1) == ' ')
                                    ++iterChar1;
                                else
                                    break;
                            }

                            // Add the line
                            if (iterChar1 != iterManual->End())
                            {
                                --iterChar1;
                                lLines.PushBack(mLine);
                                uiCounter += mLine.sCaption.GetLength();
                                s_map<s_uint, Format>::iterator iterFormat;
                                foreach (iterFormat, lTempFormatList)
                                {
                                    lFormatList_[iterFormat->first] = iterFormat->second;
                                }
                                lTempFormatList.Clear();
                                mLine.fWidth = 0.0f;
                                mLine.sCaption = "";
                            }
                            else
                                break;
                        }
                        else
                            break;
                    }
                }
            }

            if (iterManual != lManualLineList.End() - 1)
                mLine.sCaption += "\n";

            lLines.PushBack(mLine);
            s_map<s_uint, Format>::iterator iterFormat;
            foreach (iterFormat, lTempFormatList)
            {
                lFormatList_[iterFormat->first] = iterFormat->second;
            }
            lTempFormatList.Clear();
            uiCounter += mLine.sCaption.GetLength() - 1;

            // Add the maximum number of line to this Text
            s_ctnr<Line>::iterator iterLine;
            foreach (iterLine, lLines)
            {
                lLineList_.PushBack(*iterLine);
                if (lLineList_.GetSize() == uiMaxLineNbr)
                {
                    return;
                }
            }
        }
    }
}

void Text::UpdateCache_()
{
    lLetterCache_.Clear();

    if (!lLineList_.IsEmpty())
    {
        if (fBoxW_.IsValid())
        {
            fW_ = fBoxW_;
        }
        else
        {
            fW_ = 0.0f;
            s_ctnr<Line>::iterator iterLine;
            foreach (iterLine, lLineList_)
            {
                fW_ = s_float::Max(fW_, iterLine->fWidth);
            }
        }

        fH_ = GetLineHeight()*(s_float(lLineList_.GetSize() - 1)*fLineSpacing_ + 1.0f);

        s_float fX, fY;
        s_float fX0;

        if (fBoxW_.IsValid())
        {
            switch (mAlign_)
            {
                case ALIGN_LEFT :
                    fX0 = 0.0f;
                    break;
                case ALIGN_CENTER :
                    fX0 = s_float::RoundDown(fBoxW_*0.5f);
                    break;
                case ALIGN_RIGHT :
                    fX0 = fBoxW_;
                    break;
            }
        }
        else
        {
            fX0 = 0.0f;
        }

        if (fBoxH_.IsValid())
        {
            switch (mVertAlign_)
            {
                case ALIGN_TOP :
                    fY = 0.0f;
                    break;
                case ALIGN_MIDDLE :
                    fY = s_float::RoundDown((fBoxH_ - fH_)*0.5f);
                    break;
                case ALIGN_BOTTOM :
                    fY = (fBoxH_ - fH_);
                    break;
            }
        }
        else
        {
            switch (mVertAlign_)
            {
                case ALIGN_TOP :
                    fY = 0.0f;
                    break;
                case ALIGN_MIDDLE :
                    fY = -s_float::RoundDown(fH_*0.5f);
                    break;
                case ALIGN_BOTTOM :
                    fY = -fH_;
                    break;
            }
        }

        s_uint uiCounter;

        Letter mLetter;

        Color mColor = Color(s_uint::NaN);

        s_ctnr<Line>::iterator iterLine;
        foreach (iterLine, lLineList_)
        {
            switch (mAlign_)
            {
                case ALIGN_LEFT :
                    fX = fX0;
                    break;
                case ALIGN_CENTER :
                    fX = fX0 - s_float::RoundDown(iterLine->fWidth*0.5f);
                    break;
                case ALIGN_RIGHT :
                    fX = fX0 - iterLine->fWidth;
                    break;
            }

            s_ustr::iterator iterChar, iterNext;
            foreach (iterChar, iterLine->sCaption)
            {
                // Format our text
                if (bFormattingEnabled_ && lFormatList_.Find(uiCounter))
                {
                    const Format& mFormat = lFormatList_[uiCounter];
                    switch (mFormat.mColorAction)
                    {
                        case COLOR_ACTION_SET :
                            mColor = mFormat.mColor;
                            break;
                        case COLOR_ACTION_RESET :
                            mColor = Color(s_uint::NaN);
                            break;
                        default : break;
                    }
                }

                s_float fCharWidth, fCharHeight;

                // Add the character to the cache
                if (*iterChar == '\n')
                {
                    s_array<s_float,4> lUVs = pFont_->GetCharacterUVs((uint)(uchar)'_');
                    fCharHeight = (lUVs[3] - lUVs[1])*pFont_->GetTextureHeight();
                    s_float fYOffset = s_float::RoundDown(fSize_/2.0f + fSize_/8.0f - fCharHeight/2.0f);

                    mLetter.fX1 = fX; mLetter.fY1 = fY+fYOffset;
                    mLetter.fX2 = fX; mLetter.fY2 = fY+fYOffset+fCharHeight;

                    mLetter.bNoRender = true;

                    lLetterCache_.PushBack(mLetter);

                    continue; // Don't increase the uiCounter
                }
                else if (*iterChar == ' ' || *iterChar == '\t')
                {
                    s_array<s_float,4> lUVs = pFont_->GetCharacterUVs(33);
                    fCharWidth = fSpaceWidth_;
                    if (*iterChar == '\t')
                        fCharWidth *= 4;
                    fCharHeight = (lUVs[3] - lUVs[1])*pFont_->GetTextureHeight();
                    s_float fYOffset = s_float::RoundDown(fSize_/2.0f + fSize_/8.0f - fCharHeight/2.0f);

                    mLetter.fX1 = fX;            mLetter.fY1 = fY+fYOffset;
                    mLetter.fX2 = fX+fCharWidth; mLetter.fY2 = fY+fYOffset+fCharHeight;

                    mLetter.bNoRender = true;

                    lLetterCache_.PushBack(mLetter);
                }
                else
                {
                    s_array<s_float,4> lUVs = pFont_->GetCharacterUVs(*iterChar);
                    fCharWidth = GetCharacterWidth((uint)(uchar)*iterChar);
                    fCharHeight = (lUVs[3] - lUVs[1])*pFont_->GetTextureHeight();
                    s_float fYOffset = s_float::RoundDown(fSize_/2.0f + fSize_/8.0f - fCharHeight/2.0f);

                    mLetter.fX1 = fX;            mLetter.fY1 = fY+fYOffset;
                    mLetter.fX2 = fX+fCharWidth; mLetter.fY2 = fY+fYOffset+fCharHeight;

                    mLetter.fU1 = lUVs[0]; mLetter.fV1 = lUVs[1];
                    mLetter.fU2 = lUVs[2]; mLetter.fV2 = lUVs[3];

                    mLetter.mColor = mColor;

                    mLetter.bNoRender = false;

                    lLetterCache_.PushBack(mLetter);
                }

                iterNext = iterChar + 1;
                s_float fKerning = 0.0f;
                /*if (iterNext != iterLine->sCaption.End())
                {
                    if (*iterNext != ' ' && *iterChar != ' ')
                    {
                        fKerning = GetCharacterKerning((uint)(uchar)*iterChar, (uint)(uchar)*iterNext);
                    }
                }*/

                fX += fCharWidth + fKerning + fTracking_;
                ++uiCounter;
            }

            fY += GetLineHeight()*fLineSpacing_;
        }
    }
    else
    {
        fW_ = 0.0f;
        fH_ = 0.0f;
    }
}

s_refptr<Sprite> Text::CreateSprite( const s_uint& uiChar ) const
{
    s_array<s_float,4> lUVs = pFont_->GetCharacterUVs(uiChar);

    s_float fWidth = GetCharacterWidth(uiChar);
    s_float fHeight = (lUVs[3] - lUVs[1])*pFont_->GetTextureHeight();

    s_refptr<Sprite> pSprite = s_refptr<Sprite>(new Sprite(pManager_, pFont_->GetTexture(), fWidth, fHeight));
    pSprite->SetTextureRect(lUVs, true);

    pSprite->SetColor(mColor_);

    return pSprite;
}

const s_ctnr<Text::Letter>& Text::GetLetterCache()
{
    Update();
    return lLetterCache_;
}

