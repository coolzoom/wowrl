/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Text source              */
/*                                        */
/*                                        */

#include "frost_text.h"

#include "frost_fontmanager.h"
#include "frost_spritemanager.h"
#include "frost_materialmanager.h"
#include "frost_material.h"
#include "frost_sprite.h"

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
        fLineSpacing_ = 1.0f;
        fTracking_ = 0.0f;
        sFileName_ = sFileName;
        fSize_ = fSize;
        mColor_ = Color(255, 255, 255);
        pOgreFont_ = FontManager::GetSingleton()->GetFont(sFileName_, s_uint(fSize_));
        if (pOgreFont_)
        {
            bReady_ = true;

            pFontMat_ = MaterialManager::GetSingleton()->CreateMaterial2D(pOgreFont_->getMaterial().get());

            fSpaceWidth_ = GetCharacterWidth((uint)'0');
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
        const Ogre::Font::UVRect& mUVRect = pOgreFont_->getGlyphTexCoords((uint)'0');
        return (mUVRect.bottom - mUVRect.top)*pFontMat_->GetWidth();
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
        mColor_ = mColor;
        bForceColor_ = bForceColor;
    }

    const Color& Text::GetColor() const
    {
        return mColor_;
    }

    void Text::SetDimensions( const s_float& fW, const s_float& fH )
    {
        if ( (fBoxW_ != fW) || (fBoxH_ != fH) )
        {
            fBoxW_ = fW; fBoxH_ = fH;
            bUpdateCache_ = true;
        }
    }

    void Text::SetBoxWidth( const s_float& fBoxW )
    {
        if (fBoxW_ != fBoxW)
        {
            fBoxW_ = fBoxW;
            bUpdateCache_ = true;
        }
    }

    void Text::SetBoxHeight( const s_float& fBoxH )
    {
        if (fBoxH_ != fBoxH)
        {
            fBoxH_ = fBoxH;
            bUpdateCache_ = true;
        }
    }

    const s_float& Text::GetWidth()
    {
        if (bUpdateCache_)
        {
            UpdateLines_();
            UpdateCache_();
            bUpdateCache_ = false;
        }

        return fW_;
    }

    const s_float& Text::GetHeight()
    {
        if (bUpdateCache_)
        {
            UpdateLines_();
            UpdateCache_();
            bUpdateCache_ = false;
        }

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
                    if (iterNext != sText_.end())
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
            fHeight = s_float(sText_.CountOccurences("\n")+1)*GetLineHeight();
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
                    if (iterNext != sString.end())
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
        if (bReady_)
        {
            const Ogre::Font::GlyphInfo& mInfo = pOgreFont_->getGlyphInfo(uiChar1.Get());
            map<Ogre::Font::CodePoint, Ogre::Vector2>::const_iterator iter = mInfo.kerningTable.find(uiChar2.Get());
            if (iter != mInfo.kerningTable.end())
            {
                return iter->second.x*pFontMat_->GetWidth();
            }
            else
            {
                if (uiChar2 != 0)
                {
                    Error(CLASS_NAME,
                        "Couldn't find kerning information for character "+uiChar2+"."
                    );
                }

                return 0.0f;
            }
        }
        else
        {
            return 0.0f;
        }
    }

    void Text::SetAlignment( const Text::Alignment& mAlign )
    {
        mAlign_ = mAlign;
    }

    const Text::Alignment& Text::GetAlignment() const
    {
        return mAlign_;
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
            if (bUpdateCache_)
            {
                UpdateLines_();
                UpdateCache_();
                bUpdateCache_ = false;
            }

            Quad mQuad;
            mQuad.pMat = pFontMat_;

            vector<Letter>::iterator iterLetter;
            foreach (iterLetter, lLetterCache_)
            {
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

                SpriteManager::GetSingleton()->RenderQuad(mQuad);
            }
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
            s_uint uiA = sColorPart.HexToUInt();
            sColorPart.Clear();
            sColorPart += *iterChar; iterChar++;
            sColorPart += *iterChar; iterChar++;
            s_uint uiR = sColorPart.HexToUInt();
            sColorPart.Clear();
            sColorPart += *iterChar; iterChar++;
            sColorPart += *iterChar; iterChar++;
            s_uint uiG = sColorPart.HexToUInt();
            sColorPart.Clear();
            sColorPart += *iterChar; iterChar++;
            sColorPart += *iterChar;
            s_uint uiB = sColorPart.HexToUInt();

            mFormat.mColorAction = Text::COLOR_ACTION_SET;
            mFormat.mColor = Color(uiA, uiR, uiG, uiB);
        }
    }

    void Text::UpdateLines_()
    {
        // Update the line list, read format tags, do word wrapping, ...
        lLineList_.clear();

        s_uint uiMaxLineNbr, uiCounter;
        if (fBoxH_.IsValid())
            uiMaxLineNbr = s_uint(s_float::Round(fBoxH_/(GetLineHeight()*fLineSpacing_), s_float::ROUND_FLOOR));
        else
            uiMaxLineNbr = s_uint::INF;

        vector<s_str> lManualLineList = sText_.CutEach("\n");
        vector<s_str>::iterator iterManual;
        foreach (iterManual, lManualLineList)
        {
            // Make a temporary line array
            vector<Line> lLines;
            Line mLine;

            s_str::iterator iterChar1;
            foreach (iterChar1, *iterManual)
            {
                // Read format tags
                if (*iterChar1 == '|')
                {
                    iterChar1++;
                    if (iterChar1 != iterManual->end())
                    {
                        if (*iterChar1 == '|')
                        {
                        }
                        else
                        {
                            GetFormat(iterChar1, lFormatList_[uiCounter+mLine.sCaption.Length()]);
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
                    if (iterNext != iterManual->end())
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
                        s_str::iterator iterChar2 = mLine.sCaption.end();
                        s_str sErasedString;
                        s_uint uiCharToErase;
                        s_float fErasedWidth;
                        s_bool bLastWasWord;
                        while ( (mLine.fWidth > fBoxW_) && (iterChar2 != mLine.sCaption.begin()) )
                        {
                            iterChar2--;
                            if (*iterChar2 == ' ')
                            {
                                if ( bLastWasWord && (mLine.fWidth-fErasedWidth < fBoxW_) && !bRemoveStartingSpaces_ )
                                {
                                    break;
                                }
                                else
                                {
                                    mLine.fWidth -= fErasedWidth + fSpaceWidth_;
                                    sErasedString.PushFront(*iterChar2);
                                    fErasedWidth = 0.0f;
                                    uiCharToErase++;
                                }
                            }
                            else
                            {
                                fErasedWidth += GetCharacterWidth(*iterChar2);
                                sErasedString.PushFront(*iterChar2);
                                uiCharToErase++;
                                bLastWasWord = true;
                            }
                        }

                        if (bRemoveStartingSpaces_)
                        {
                            while (*iterChar2 == ' ')
                            {
                                uiCharToErase--;
                                sErasedString.EraseFromStart(1);
                                iterChar2++;
                            }
                        }

                        mLine.sCaption.EraseFromEnd(uiCharToErase);

                        lLines.push_back(mLine);
                        uiCounter += mLine.sCaption.Length();
                        mLine.fWidth = GetStringWidth(sErasedString);
                        mLine.sCaption = sErasedString;
                    }
                    else
                    {
                        // There is only one word on this line, so this
                        // word is just too long for the text box : our
                        // only option is to truncate it.
                        s_float fWordWidth = 3*(GetCharacterWidth((uint)'.') + fTracking_);
                        s_str::iterator iterChar2 = mLine.sCaption.end();
                        s_str sErasedWord;
                        s_uint uiCharToErase;
                        while ( (mLine.fWidth + fWordWidth > fBoxW_) && (iterChar2 != mLine.sCaption.begin()) )
                        {
                            iterChar2--;
                            mLine.fWidth -= GetCharacterWidth(*iterChar2);
                            uiCharToErase++;
                        }
                        mLine.sCaption.EraseFromEnd(uiCharToErase);
                        mLine.sCaption << "...";

                        s_str::iterator iterTemp = iterChar1;
                        iterChar1 = iterManual->begin() + s_int(iterManual->FindPos(" ", s_uint(iterChar1 - iterManual->begin())));

                        if (iterChar1 != iterManual->end())
                        {
                            // Read cutted format tags
                            while (iterTemp != iterChar1)
                            {
                                if ((*iterTemp) == '|')
                                {
                                    iterTemp++;
                                    if (iterTemp != iterChar1)
                                    {
                                        if ((*iterTemp) == '|')
                                        {
                                        }
                                        else
                                        {
                                            GetFormat(iterTemp, lFormatList_[uiCounter+mLine.sCaption.Length()]);
                                        }
                                    }
                                }
                                iterTemp++;
                            }

                            // Look for the next word
                            while (iterChar1 != iterManual->end())
                            {
                                if ((*iterChar1) == ' ')
                                    iterChar1++;
                                else
                                    break;
                            }

                            // Add the line
                            if (iterChar1 != iterManual->end())
                            {
                                iterChar1--;
                                lLines.push_back(mLine);
                                uiCounter += mLine.sCaption.Length();
                                mLine.fWidth = 0.0f;
                                mLine.sCaption = "";
                            }
                        }
                    }
                }
            }
            lLines.push_back(mLine);
            uiCounter += mLine.sCaption.Length();

            // Add the maximum number of line to this Text
            vector<Line>::iterator iterLine;
            foreach (iterLine, lLines)
            {
                lLineList_.push_back(*iterLine);
                if (s_uint((uint)lLineList_.size()) == uiMaxLineNbr)
                {
                    return;
                }
            }
        }
    }

    void Text::UpdateCache_()
    {
        if (fBoxW_.IsValid())
        {
            fW_ = fBoxW_;
        }
        else
        {
            fW_ = 0;
            vector<Line>::iterator iterLine;
            foreach (iterLine, lLineList_)
            {
                fW_ = s_float::Max(fW_, iterLine->fWidth);
            }
        }

        s_float fX, fY = 0;
        s_uint uiCounter;

        lLetterCache_.clear();

        Letter mLetter;

        Color mColor = Color(s_uint::NaN);

        vector<Line>::iterator iterLine;
        foreach (iterLine, lLineList_)
        {
            switch (mAlign_)
            {
                case ALIGN_LEFT :
                    fX = 0;
                    break;
                case ALIGN_CENTER :
                    fX = (fW_ - iterLine->fWidth)/2;
                    break;
                case ALIGN_RIGHT :
                    fX = (fW_ - iterLine->fWidth);
                    break;
            }

            s_str::iterator iterChar;
            foreach (iterChar, iterLine->sCaption)
            {
                // Format our text
                if (MAPFIND(uiCounter, lFormatList_))
                {
                    s_ptr<Format> f = &lFormatList_[uiCounter];
                    switch (f->mColorAction)
                    {
                        case COLOR_ACTION_SET :
                            mColor = f->mColor;
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
                    s_float fYOffset = -fCharHeight/2+fSize_/2;

                    mLetter.fX1 = fX;            mLetter.fY1 = fY+fYOffset;
                    mLetter.fX2 = fX+fCharWidth; mLetter.fY2 = fY+fYOffset+fCharHeight;

                    mLetter.fU1 = mUVRect.left;  mLetter.fV1 = mUVRect.top;
                    mLetter.fU2 = mUVRect.right; mLetter.fV2 = mUVRect.bottom;

                    mLetter.mColor = mColor;

                    lLetterCache_.push_back(mLetter);
                }

                fX += fCharWidth + fTracking_;
                uiCounter++;
            }

			fY += GetLineHeight()*fLineSpacing_;
        }

        fH_ = fY;
    }
}
