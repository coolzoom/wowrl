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
        sText_ = sText;
        bUpdateCache_ = true;
    }

    const s_str& Text::GetText() const
    {
        return sText_;
    }

    void Text::SetColor( const Color& mColor, const s_bool& bForceColor )
    {
        mColor_ = mColor;
        bForceColor_ = bForceColor;

        bUpdateCache_ = true;
    }

    const Color& Text::GetColor() const
    {
        return mColor_;
    }

    void Text::SetDimensions( const s_float& fW, const s_float& fH )
    {
        fW_ = fW; fH_ = fH;

        bUpdateCache_ = true;
    }

    void Text::SetWidth( const s_float& fW )
    {
        fW_ = fW;

        bUpdateCache_ = true;
    }

    void Text::SetHeight( const s_float& fH )
    {
        fH_ = fH;

        bUpdateCache_ = true;
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
        fTracking_ = fTracking;

        bUpdateCache_ = true;
    }

    const s_float& Text::GetTracking() const
    {
        return fTracking_;
    }

    void Text::SetLineSpacing( const s_float& fLineSpacing )
    {
        fLineSpacing_ = fLineSpacing;

        bUpdateCache_ = true;
    }

    const s_float& Text::GetLineSpacing() const
    {
        return fLineSpacing_;
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

    void Text::UpdateLines_()
    {
        lLineList_.clear();

        s_uint uiMaxLineNbr;
        if (fH_.IsValid())
            uiMaxLineNbr = s_uint(s_float::Round(fH_/(fSize_*fLineSpacing_), s_float::ROUND_FLOOR));
        else
            uiMaxLineNbr = s_uint::INF;

        vector<s_str> lManualLineList = sText_.CutEach("\n");
        vector<s_str>::iterator iterManual;
        foreach (iterManual, lManualLineList)
        {
            vector<Line> lLines;

            Line mLine;
            s_str::iterator iterChar1;
            foreach (iterChar1, *iterManual)
            {
                if (*iterChar1 == '|')
                {
                    iterChar1++;
                    if (iterChar1 != iterManual->end())
                    {
                        if (*iterChar1 == 'r')
                        {
                            continue;
                        }
                        else if (*iterChar1 == 'c')
                        {
                            iterChar1 = iterChar1+s_uint(8u);
                            continue;
                        }
                        else if (*iterChar1 == '|')
                        {
                        }
                    }
                    else
                        break;
                }

                mLine.fWidth += GetCharacterWidth(*iterChar1);
                mLine.sCaption += *iterChar1;
                if (mLine.fWidth > fW_)
                {
                    if (mLine.sCaption.FindPos(" ").IsValid())
                    {
                        // There are several words on this line, we'll
                        // be able to put the last one on the next line
                        s_str::iterator iterChar2 = mLine.sCaption.end();
                        s_str sErasedString;
                        s_uint uiCharToErase;
                        s_float fErasedWidth;
                        while ( (mLine.fWidth > fW_) && (iterChar2 != mLine.sCaption.begin()) )
                        {
                            iterChar2--;
                            if (*iterChar2 == ' ')
                            {
                                mLine.fWidth -= fErasedWidth + GetCharacterWidth(*iterChar2);
                                fErasedWidth = 0.0f;
                                uiCharToErase++;
                            }
                            else
                            {
                                fErasedWidth += GetCharacterWidth(*iterChar2);
                                sErasedString.PushFront(*iterChar2);
                                uiCharToErase++;
                            }
                        }
                        mLine.sCaption.EraseFromEnd(uiCharToErase);

                        lLines.push_back(mLine);
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

                        iterChar1 = iterManual->begin() + iterManual->FindPos(" ", iterChar1 - iterManual->begin());
                        while (iterChar1 != iterManual->end())
                        {
                            if ((*iterChar1) == ' ')
                                iterChar1++;
                            else
                                break;
                        }
                        if (iterChar1 != iterManual->end())
                        {
                            iterChar1--;
                            lLines.push_back(mLine);
                            mLine.fWidth = 0.0f;
                            mLine.sCaption = "";
                        }
                    }
                }
            }
            lLines.push_back(mLine);

            vector<Line>::iterator iterLine;
            foreach (iterLine, lLines)
            {
                lLineList_.push_back(*iterLine);
                if (s_uint(lLineList_.size()) == uiMaxLineNbr)
                {
                    foreach (iterLine, lLineList_)
                    {
                        Log(iterLine->sCaption);
                    }
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

        // TODO : afficher la couleur (ça marche pas)
        // TODO : uniformiser l'espace entre les lettres ? (l'espace marche pas)

        vector<Line>::iterator iterLine;
        foreach (iterLine, lLineList_)
        {
            s_str::iterator iterChar;
            foreach (iterChar, iterLine->sCaption)
            {
                if (*iterChar == '|')
                {
                    iterChar++;
                    if (iterChar != iterLine->sCaption.end())
                    {
                        if ( (*iterChar == 'c' ) && !bForceColor_ )
                        {
                            // New color
                            iterChar = iterChar + s_uint(8u);
                            if (iterChar != iterLine->sCaption.end())
                            {
                                s_uint uiIterPos = iterChar - iterLine->sCaption.begin() - 7;
                                s_str sColorPart = iterLine->sCaption.Extract(uiIterPos, 2);
                                s_uint uiA = sColorPart.HexToUInt();
                                sColorPart = iterLine->sCaption.Extract(uiIterPos+2, 2);
                                s_uint uiR = sColorPart.HexToUInt();
                                sColorPart = iterLine->sCaption.Extract(uiIterPos+4, 2);
                                s_uint uiG = sColorPart.HexToUInt();
                                sColorPart = iterLine->sCaption.Extract(uiIterPos+6, 2);
                                s_uint uiB = sColorPart.HexToUInt();

                                mQuad.lVertexArray[0].mColor = mQuad.lVertexArray[1].mColor =
                                mQuad.lVertexArray[2].mColor = mQuad.lVertexArray[3].mColor =
                                    Color(uiA, uiR, uiG, uiB);
                            }
                            continue;
                        }
                        else if ( (*iterChar == 'r') && !bForceColor_ )
                        {
                            // Default color
                            mQuad.lVertexArray[0].mColor = mQuad.lVertexArray[1].mColor =
                            mQuad.lVertexArray[2].mColor = mQuad.lVertexArray[3].mColor =
                                mColor_;

                            continue;
                        }
                        else if (*iterChar == '|')
                        {
                        }
                    }
                    else
                        break;
                }

                const Ogre::Font::UVRect& mUVRect = pOgreFont_->getGlyphTexCoords((uint)*iterChar);
                s_float fCharWidth = GetCharacterWidth((uint)*iterChar);
                s_float fCharHeight = (mUVRect.bottom - mUVRect.top)*pFontMat_->GetHeight();
                s_float fYAdjust = -fCharHeight/2+fSize_/2;
                mQuad.lVertexArray[0].Set(fX, fY+fYAdjust);
                mQuad.lVertexArray[0].SetUV(mUVRect.left, mUVRect.top);
                mQuad.lVertexArray[1].Set(fX+fCharWidth, fY+fYAdjust);
                mQuad.lVertexArray[1].SetUV(mUVRect.right, mUVRect.top);
                mQuad.lVertexArray[2].Set(fX+fCharWidth, fY+fYAdjust+fCharHeight);
                mQuad.lVertexArray[2].SetUV(mUVRect.right, mUVRect.bottom);
                mQuad.lVertexArray[3].Set(fX, fY+fYAdjust+fCharHeight);
                mQuad.lVertexArray[3].SetUV(mUVRect.left, mUVRect.bottom);

                pSpriteMgr->RenderQuad(mQuad);

                fX += fCharWidth + fTracking_;
            }

			fY += fSize_*fLineSpacing_;
			fX = 0;
        }

        pSpriteMgr->End();
    }
}
