/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Text source              */
/*                                        */
/*                                        */

#include "gui/frost_text.h"

#include "gui/frost_fontmanager.h"
#include "gui/frost_spritemanager.h"
#include "material/frost_materialmanager.h"
#include "material/frost_material.h"
#include "gui/frost_sprite.h"

#include <OgreFont.h>
#include <OgreFontManager.h>
#include <OgreRenderTarget.h>
#include <OgrePass.h>

#undef VOID

using namespace std;

namespace Frost
{
    const s_str Text::CLASS_NAME = "Text";

    Text::Text( const s_str& sFileName, const s_float& fSize )
    {
        fBoxW_ = fBoxH_ = s_float::INFPLUS;
        mAlign_ = ALIGN_LEFT;
        mVertAlign_ = ALIGN_MIDDLE;
        fLineSpacing_ = 1.5f;
        fTracking_ = 0.0f;
        sFileName_ = sFileName;
        fSize_ = fSize;
        mColor_ = Color(255, 255, 255);
        fX_ = fY_ = s_float::INFPLUS;
        pOgreFont_ = FontManager::GetSingleton()->GetFont(sFileName_, s_uint(fSize_));
        if (pOgreFont_)
        {
            bReady_ = true;

            pFontMat_ = MaterialManager::GetSingleton()->CreateMaterial2D(pOgreFont_->getMaterial().get());

            fSpaceWidth_ = GetCharacterWidth((uint)'0')*0.5f;
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
            bUpdateCache_ = true;
        }
    }

    const s_str& Text::GetText() const
    {
        return sText_;
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
            s_str::const_iterator iterChar, iterNext;
            foreach (iterChar, sText_)
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
                    fWidth += GetCharacterWidth((uint)*iterChar) + fTracking_;
                    if (iterNext != sText_.End())
                    {
                        if (*iterNext != ' ' && *iterNext != '\n')
                            fWidth += GetCharacterKerning((uint)*iterChar, (uint)*iterNext);
                    }
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
        s_float fWidth;
        s_float fMaxWidth = s_float::INFMINUS;
        if (bReady_)
        {
            s_str::const_iterator iterChar, iterNext;
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
                    fWidth += GetCharacterWidth((uint)*iterChar) + fTracking_;
                    if (iterNext != sString.End())
                    {
                        if (*iterNext != ' ' && *iterNext != '\n')
                            fWidth += GetCharacterKerning((uint)*iterChar, (uint)*iterNext);
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
            const Ogre::Font::UVRect& mUVRect = pOgreFont_->getGlyphTexCoords(uiChar.Get());
            return (mUVRect.right - mUVRect.left)*pFontMat_->GetWidth();
        }
        else
            return 0.0f;
    }

    s_float Text::GetCharacterKerning( const s_uint& uiChar1, const s_uint& uiChar2 ) const
    {
        return pFontMat_->GetWidth()*pOgreFont_->getGlyphInfo(uiChar1.Get()).kerningTable.find(uiChar2.Get())->second.x;
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

                Quad mQuad;
                mQuad.pMat = pFontMat_;

                s_ctnr<Letter>::iterator iterLetter;
                foreach (iterLetter, lLetterCache_)
                {
                    Quad mQuad;
                    mQuad.pMat = pFontMat_;

                    mQuad.lVertexArray[0].Set(iterLetter->fX1+fX, iterLetter->fY1+fY);
                    mQuad.lVertexArray[1].Set(iterLetter->fX2+fX, iterLetter->fY1+fY);
                    mQuad.lVertexArray[2].Set(iterLetter->fX2+fX, iterLetter->fY2+fY);
                    mQuad.lVertexArray[3].Set(iterLetter->fX1+fX, iterLetter->fY2+fY);

                    mQuad.lVertexArray[0].SetUV(iterLetter->fU1, iterLetter->fV1);
                    mQuad.lVertexArray[1].SetUV(iterLetter->fU2, iterLetter->fV1);
                    mQuad.lVertexArray[2].SetUV(iterLetter->fU2, iterLetter->fV2);
                    mQuad.lVertexArray[3].SetUV(iterLetter->fU1, iterLetter->fV2);

                    if (!iterLetter->mColor.IsNaN() && !bForceColor_)
                    {
                        mQuad.lVertexArray[0].mColor = mQuad.lVertexArray[1].mColor =
                        mQuad.lVertexArray[2].mColor = mQuad.lVertexArray[3].mColor = iterLetter->mColor;
                    }
                    else
                    {
                        mQuad.lVertexArray[0].mColor = mQuad.lVertexArray[1].mColor =
                        mQuad.lVertexArray[2].mColor = mQuad.lVertexArray[3].mColor = mColor_;
                    }

                    lQuadList_.PushBack(mQuad);
                }

                bUpdateQuads_ = false;
            }

            s_ctnr<Quad>::iterator iterQuad;
            foreach (iterQuad, lQuadList_)
            {
                SpriteManager::GetSingleton()->RenderQuad(*iterQuad);
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

    s_ptr<Ogre::Font> Text::GetOgreFont()
    {
        return pOgreFont_;
    }

    s_refptr<Material> Text::GetMaterial()
    {
        return pFontMat_;
    }

    void GetFormat( s_str::iterator& iterChar, Text::Format& mFormat )
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
            s_uchar ucA = s_uchar(sColorPart.HexToUInt());
            sColorPart.Clear();
            sColorPart += *iterChar; iterChar++;
            sColorPart += *iterChar; iterChar++;
            s_uchar ucR = s_uchar(sColorPart.HexToUInt());
            sColorPart.Clear();
            sColorPart += *iterChar; iterChar++;
            sColorPart += *iterChar; iterChar++;
            s_uchar ucG = s_uchar(sColorPart.HexToUInt());
            sColorPart.Clear();
            sColorPart += *iterChar; iterChar++;
            sColorPart += *iterChar;
            s_uchar ucB = s_uchar(sColorPart.HexToUInt());

            mFormat.mColorAction = Text::COLOR_ACTION_SET;
            mFormat.mColor = Color(ucA, ucR, ucG, ucB);
        }
    }

    void Text::UpdateLines_()
    {
        // Update the line list, read format tags, do word wrapping, ...
        lLineList_.Clear();
        lFormatList_.Clear();

        s_uint uiMaxLineNbr, uiCounter;
        if (fBoxH_.IsValid())
            uiMaxLineNbr = s_uint(s_float::RoundDown(fBoxH_/(GetLineHeight()*fLineSpacing_)));
        else
            uiMaxLineNbr = s_uint::INF;

        if (uiMaxLineNbr >= 1)
        {
            s_ctnr<s_str> lManualLineList = sText_.CutEach("\n");
            s_ctnr<s_str>::iterator iterManual;
            foreach (iterManual, lManualLineList)
            {
                // Make a temporary line array
                s_ctnr<Line> lLines;
                Line mLine;
                s_map<s_uint, Format> lTempFormatList;

                s_str::iterator iterChar1;
                foreach (iterChar1, *iterManual)
                {
                    // Read format tags
                    if (*iterChar1 == '|')
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
                        mLine.fWidth += GetCharacterWidth(*iterChar1);
                        s_str::iterator iterNext = iterChar1 + 1;
                        if (iterNext != iterManual->End())
                        {
                            if (*iterNext != ' ')
                                mLine.fWidth += GetCharacterKerning((uint)*iterChar1, (uint)*iterNext);
                        }
                    }
                    mLine.sCaption += *iterChar1;

                    if (mLine.fWidth > fBoxW_)
                    {
                        // Whoops, the line is too long...
                        if (mLine.sCaption.FindPos(" ").IsValid())
                        {
                            // There are several words on this line, we'll
                            // be able to put the last one on the next line
                            s_str::iterator iterChar2 = mLine.sCaption.End();
                            s_str sErasedString;
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
                                    fErasedWidth += GetCharacterWidth(*iterChar2);
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
                            Log(mLine.sCaption);
                            s_map<s_uint, Format>::iterator iterFormat;
                            foreach (iterFormat, lTempFormatList)
                            {
                                Log("f1 : p["+iterFormat->first+"], c : "+iterFormat->second.mColor);
                                lFormatList_[iterFormat->first] = iterFormat->second;
                            }
                            lTempFormatList.Clear();
                            uiCounter += mLine.sCaption.GetLength();
                            mLine.fWidth = GetStringWidth(sErasedString);
                            mLine.sCaption = sErasedString;
                        }
                        else
                        {
                            // There is only one word on this line, so this
                            // word is just too long for the text box : our
                            // only option is to truncate it.
                            s_float fWordWidth = 3*(GetCharacterWidth((uint)'.') + fTracking_);
                            s_str::iterator iterChar2 = mLine.sCaption.End();
                            s_str sErasedWord;
                            s_uint uiCharToErase;
                            while ( (mLine.fWidth + fWordWidth > fBoxW_) && (iterChar2 != mLine.sCaption.Begin()) )
                            {
                                --iterChar2;
                                mLine.fWidth -= GetCharacterWidth(*iterChar2);
                                ++uiCharToErase;
                            }
                            mLine.sCaption.EraseFromEnd(uiCharToErase);
                            mLine.sCaption << "...";

                            s_str::iterator iterTemp = iterChar1;
                            iterChar1 = iterManual->Get(" ", s_uint(s_ptrdiff(iterChar1 - iterManual->begin())));

                            if (iterChar1 != iterManual->End())
                            {
                                // Read cutted format tags
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
                                    Log(mLine.sCaption);
                                    s_map<s_uint, Format>::iterator iterFormat;
                                    foreach (iterFormat, lTempFormatList)
                                    {
                                        Log("f2 : p["+iterFormat->first+"], c : "+iterFormat->second.mColor);
                                        if (s_int(uiCounter) - s_int(iterFormat->first) > 0)
                                        {
                                            Log("(ok)");
                                            lFormatList_[iterFormat->first] = iterFormat->second;
                                        }
                                        else
                                        {
                                            Log("(cut)");
                                            lFormatList_[uiCounter] = iterFormat->second;
                                        }
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
                lLines.PushBack(mLine);
                Log(mLine.sCaption);
                s_map<s_uint, Format>::iterator iterFormat;
                foreach (iterFormat, lTempFormatList)
                {
                    Log("f3 : p["+iterFormat->first+"], c : "+iterFormat->second.mColor);
                    lFormatList_[iterFormat->first] = iterFormat->second;
                }
                lTempFormatList.Clear();
                uiCounter += mLine.sCaption.GetLength();

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
                        fX0 = fBoxW_*0.5f;
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

            s_float fYOffset;
            if (fBoxH_.IsValid())
            {
                switch (mVertAlign_)
                {
                    case ALIGN_TOP :
                        fY = 0.0f;
                        break;
                    case ALIGN_MIDDLE :
                        fY = (fBoxH_ - fH_)*0.5f;
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
                        fY = -fH_*0.5f;
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
                        fX = fX0 - iterLine->fWidth*0.5f;
                        break;
                    case ALIGN_RIGHT :
                        fX = fX0 - iterLine->fWidth;
                        break;
                }

                s_str::iterator iterChar, iterNext;
                foreach (iterChar, iterLine->sCaption)
                {
                    // Format our text
                    if (lFormatList_.Find(uiCounter))
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
                    if (*iterChar == ' ')
                    {
                        fCharWidth = fSpaceWidth_;
                    }
                    else
                    {
                        const Ogre::Font::UVRect& mUVRect = pOgreFont_->getGlyphTexCoords((uint)*iterChar);
                        fCharWidth = GetCharacterWidth((uint)*iterChar);
                        fCharHeight = (mUVRect.bottom - mUVRect.top)*pFontMat_->GetHeight();
                        s_float fYOffset = fSize_/2 - fCharHeight/2;

                        mLetter.fX1 = fX;            mLetter.fY1 = fY+fYOffset;
                        mLetter.fX2 = fX+fCharWidth; mLetter.fY2 = fY+fYOffset+fCharHeight;

                        mLetter.fU1 = mUVRect.left;  mLetter.fV1 = mUVRect.top;
                        mLetter.fU2 = mUVRect.right; mLetter.fV2 = mUVRect.bottom;

                        mLetter.mColor = mColor;

                        lLetterCache_.PushBack(mLetter);
                    }

                    iterNext = iterChar + 1;
                    s_float fKerning = 0.0f;
                    if (iterNext != iterLine->sCaption.End())
                    {
                        if (*iterNext != ' ' && *iterChar != ' ')
                        {
                            fKerning = GetCharacterKerning((uint)*iterChar, (uint)*iterNext);
                        }
                    }

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
}
