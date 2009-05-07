/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               XXXX source              */
/*                                        */
/*  ## : ...                              */
/*                                        */

#include "frost_text.h"

#include <OgreFont.h>
#include <OgreFontManager.h>
#include <OgreRenderTarget.h>
#include <OgrePass.h>

#include "frost_fontmanager.h"
#include "frost_spritemanager.h"
#include "frost_materialmanager.h"
#include "frost_material.h"
#include "frost_sprite.h"

using namespace std;

namespace Frost
{
    const s_str Text::CLASS_NAME = "Text";

    Text::Text( const s_str& sFileName, const s_float& fSize )
    {
        fW_ = fH_ = s_float::INFPLUS;
        fLineSpacing_ = 1.0f;
        fTracking_ = 0.0f;
        sFileName_ = sFileName;
        fSize_ = fSize;
        mColor_ = Color(255, 255, 255);
        pOgreFont_ = FontManager::GetSingleton()->GetFont(sFileName_, s_uint(fSize_));
        if (pOgreFont_)
        {
            bReady_ = true;

            pFontMat_ = MaterialManager::GetSingleton()->CreateMaterial(pOgreFont_->getMaterial().get());
            s_ptr<Ogre::Pass> pPass = pFontMat_->GetDefaultPass();
            pPass->setDiffuse(Ogre::ColourValue(1.0f,1.0f,1.0f));
            pPass->setCullingMode(Ogre::CULL_NONE);
            pPass->setTextureFiltering(Ogre::TFO_NONE);
            pPass->setSeparateSceneBlending(
                Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA,
                Ogre::SBF_ONE, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA
            );

            pFontMat_->GetOgreMaterial()->setLightingEnabled(false);
            pFontMat_->GetOgreMaterial()->setDepthCheckEnabled(false);
            pFontMat_->GetOgreMaterial()->setCullingMode(Ogre::CULL_NONE);

            fSpaceWidth_ = GetCharacterWidth((uint)'0');
        }
    }

    Text::~Text()
    {
        pRTMat_.SetNull();
        SpriteManager::GetSingleton()->DeleteRenderTarget(pCache_);
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
        if ( (mColor_ != mColor) || bForceColor_ != bForceColor )
        {
            mColor_ = mColor;
            bForceColor_ = bForceColor;
            bUpdateCache_ = true;
        }
    }

    const Color& Text::GetColor() const
    {
        return mColor_;
    }

    void Text::SetDimensions( const s_float& fW, const s_float& fH )
    {
        if ( (fW_ != fW) || (fH_ != fH) )
        {
            fW_ = fW; fH_ = fH;
            bUpdateCache_ = true;
        }
    }

    void Text::SetWidth( const s_float& fW )
    {
        if (fW_ != fW)
        {
            fW_ = fW;
            bUpdateCache_ = true;
        }
    }

    void Text::SetHeight( const s_float& fH )
    {
        if (fH_ != fH)
        {
            fH_ = fH;
            bUpdateCache_ = true;
        }
    }

    const s_float& Text::GetWidth() const
    {
        return fW_;
    }

    const s_float& Text::GetHeight() const
    {
        return fH_;
    }

    s_float Text::GetTextWidth() const
    {
        s_float fWidth;
        if (bReady_)
        {
            s_str::const_iterator iterChar;
            foreach (iterChar, sText_)
            {
                if (*iterChar == ' ')
                    fWidth += fSpaceWidth_;
                else
                    fWidth += GetCharacterWidth((uint)*iterChar) + fTracking_;
            }
        }

        return fWidth;
    }

    s_float Text::GetStringWidth( const s_str& sString ) const
    {
        s_float fWidth;
        if (bReady_)
        {
            s_str::const_iterator iterChar;
            foreach (iterChar, sString)
            {
                if (*iterChar == ' ')
                    fWidth += fSpaceWidth_;
                else
                    fWidth += GetCharacterWidth((uint)*iterChar) + fTracking_;
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

    s_float Text::GetTextHeight() const
    {
        return fSize_;
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

    void Text::Update()
    {
        if (bUpdateCache_ && bReady_)
        {
            UpdateLines_();
            UpdateCache_();
            bUpdateCache_ = false;
        }
    }

    void Text::Render( const s_float& fX, const s_float& fY )
    {
        if (bReady_ && pCache_)
        {
            Quad mQuad;
            mQuad.pMat = pRTMat_;
            mQuad.lVertexArray[0].mColor = mQuad.lVertexArray[1].mColor =
            mQuad.lVertexArray[2].mColor = mQuad.lVertexArray[3].mColor =
                Color(255, 255, 255);

            s_float fUMax = s_float(pCache_->GetWidth())/s_float(pCache_->GetRealWidth());
            s_float fVMax = s_float(pCache_->GetHeight())/s_float(pCache_->GetRealHeight());

            mQuad.lVertexArray[0].Set(fX, fY);
            mQuad.lVertexArray[0].SetUV(0, 0);
            mQuad.lVertexArray[1].Set(fX+s_float(pCache_->GetWidth()), fY);
            mQuad.lVertexArray[1].SetUV(fUMax, 0);
            mQuad.lVertexArray[2].Set(fX+s_float(pCache_->GetWidth()), fY+s_float(pCache_->GetHeight()));
            mQuad.lVertexArray[2].SetUV(fUMax, fVMax);
            mQuad.lVertexArray[3].Set(fX, fY+s_float(pCache_->GetHeight()));
            mQuad.lVertexArray[3].SetUV(0, fVMax);

            SpriteManager::GetSingleton()->RenderQuad(mQuad);
        }
    }

    s_ptr<RenderTarget> Text::GetCache()
    {
        return pCache_;
    }

    s_ptr<Ogre::Font> Text::GetOgreFont()
    {
        return pOgreFont_;
    }

    void GetFormat( s_str::iterator& iterChar, Format& mFormat )
    {
        if (*iterChar == 'r')
        {
            mFormat.mColorAction = COLOR_ACTION_RESET;
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

            mFormat.mColorAction = COLOR_ACTION_SET;
            mFormat.mColor = Color(uiA, uiR, uiG, uiB);
        }
    }

    void Text::UpdateLines_()
    {
        // Update the line list, read format tags, do word wrapping, ...
        lLineList_.clear();

        s_uint uiMaxLineNbr, uiCounter;
        if (fH_.IsValid())
            uiMaxLineNbr = s_uint(s_float::Round(fH_/(fSize_*fLineSpacing_), s_float::ROUND_FLOOR));
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
                    mLine.fWidth += GetCharacterWidth(*iterChar1);
                mLine.sCaption += *iterChar1;

                if (mLine.fWidth > fW_)
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
                        while ( (mLine.fWidth > fW_) && (iterChar2 != mLine.sCaption.begin()) )
                        {
                            iterChar2--;
                            if (*iterChar2 == ' ')
                            {
                                if ( bLastWasWord && (mLine.fWidth-fErasedWidth < fW_) && !bRemoveStartingSpaces_ )
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
                        while ( (mLine.fWidth + fWordWidth > fW_) && (iterChar2 != mLine.sCaption.begin()) )
                        {
                            iterChar2--;
                            mLine.fWidth -= GetCharacterWidth(*iterChar2);
                            uiCharToErase++;
                        }
                        mLine.sCaption.EraseFromEnd(uiCharToErase);
                        mLine.sCaption << "...";

                        s_str::iterator iterTemp = iterChar1;
                        iterChar1 = iterManual->begin() + iterManual->FindPos(" ", iterChar1 - iterManual->begin());

                        if (iterChar1 != iterManual->end())
                        {
                            // Read cutted format tags
                            for (; iterTemp != iterChar1; iterTemp++)
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

            // Add the maximum number of line to this Text
            vector<Line>::iterator iterLine;
            foreach (iterLine, lLines)
            {
                lLineList_.push_back(*iterLine);
                if (s_uint(lLineList_.size()) == uiMaxLineNbr)
                {
                    return;
                }
            }
        }
    }

    void Text::UpdateCache_()
    {
        s_ptr<SpriteManager> pSpriteMgr = SpriteManager::GetSingleton();

        s_float fWidth, fHeight;
        if (fW_.IsValid())
        {
            // The text will be adjusted to fit in the imposed horizontal
            // space, so the render target doesn't need to be larger than this.
            fWidth = fW_;

            if (fH_.IsValid())
            {
                // Same thing for the vertical space.
                fHeight = fH_;
            }
            else
            {
                // The text will be able to grow vertically as it needs,
                // so we calculate the number of line it will take.
                fHeight = lLineList_.size()*fSize_;
            }
        }
        else
        {
            // We're free to take as much horizontal space as we need,
            // we need to draw everything, so let's take the maximum.
            vector<Line>::iterator iterLine;
            foreach (iterLine, lLineList_)
            {
                fWidth = s_float::Max(fWidth, iterLine->fWidth);
            }
            // The text will be drawn on a single line, so no need to
            // ask for more than a character's height.
            fHeight = lLineList_.size()*fSize_;
        }

        // Create/update the render target
        if (!pCache_)
        {
            pCache_ = pSpriteMgr->CreateRenderTarget(s_uint(fWidth), s_uint(fHeight));
            pRTMat_ = MaterialManager::GetSingleton()->CreateMaterial2DFromRT(pCache_);
        }
        else
        {
            if (pCache_->SetDimensions(s_uint(fWidth), s_uint(fHeight)))
                pRTMat_ = MaterialManager::GetSingleton()->CreateMaterial2DFromRT(pCache_);
        }

        pSpriteMgr->Begin(pCache_);
        pSpriteMgr->Clear(Color(0, 0, 0, 0));

        s_float fX = 0, fY = 0;
        Quad mQuad;
        mQuad.pMat = pFontMat_;
        mQuad.lVertexArray[0].mColor = mQuad.lVertexArray[1].mColor =
        mQuad.lVertexArray[2].mColor = mQuad.lVertexArray[3].mColor =
            mColor_;

        s_uint uiCounter;

        vector<Line>::iterator iterLine;
        foreach (iterLine, lLineList_)
        {
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
                            mQuad.lVertexArray[0].mColor = mQuad.lVertexArray[1].mColor =
                            mQuad.lVertexArray[2].mColor = mQuad.lVertexArray[3].mColor =
                                f->mColor;
                            break;
                        case COLOR_ACTION_RESET :
                            mQuad.lVertexArray[0].mColor = mQuad.lVertexArray[1].mColor =
                            mQuad.lVertexArray[2].mColor = mQuad.lVertexArray[3].mColor =
                                mColor_;
                            break;
                        default : break;
                    }
                }

                s_float fCharWidth, fCharHeight;

                // Render the character on the screen
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

                    mQuad.lVertexArray[0].Set(fX,            fY+fYOffset);
                    mQuad.lVertexArray[1].Set(fX+fCharWidth, fY+fYOffset);
                    mQuad.lVertexArray[2].Set(fX+fCharWidth, fY+fYOffset+fCharHeight);
                    mQuad.lVertexArray[3].Set(fX,            fY+fYOffset+fCharHeight);

                    mQuad.lVertexArray[0].SetUV(mUVRect.left,  mUVRect.top);
                    mQuad.lVertexArray[1].SetUV(mUVRect.right, mUVRect.top);
                    mQuad.lVertexArray[2].SetUV(mUVRect.right, mUVRect.bottom);
                    mQuad.lVertexArray[3].SetUV(mUVRect.left,  mUVRect.bottom);

                    pSpriteMgr->RenderQuad(mQuad);
                }

                fX += fCharWidth + fTracking_;
                uiCounter++;
            }

			fY += fSize_*fLineSpacing_;
			fX = 0;
        }

        pSpriteMgr->End();
    }
}
