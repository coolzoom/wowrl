/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Sprite source             */
/*                                        */
/*  ## : Draws simple 2D objects on the   */
/*  screen. This class is inspired from   */
/*  HGE's sprite class. See :             */
/*  http://hge.relishgames.com/           */
/*                                        */


#include "gui/frost_sprite.h"
#include "gui/frost_spritemanager.h"
#include "material/frost_materialmanager.h"

#include <OgreMaterial.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgreTextureUnitState.h>

using namespace std;

namespace Frost
{
    const s_str Sprite::CLASS_NAME = "Sprite";

    Sprite::Sprite( s_refptr<Material> pMat )
    {
        mColor_ = Color(255, 255, 255, 255);

        pQuad_ = s_refptr<Quad>(new Quad());

        for (uint i = 0; i < 4; i++)
            pQuad_->lVertexArray[i].mColor = mColor_;

        if (pMat != NULL)
        {
            pQuad_->pMat = pMat;

            if (!pMat->IsPlain())
            {
                uiTexWidth_ = s_uint(pMat->GetWidth());
                uiTexHeight_ = s_uint(pMat->GetHeight());

                fX1_ = 0.0f;
                fY1_ = 0.0f;
                fX3_ = fWidth_ = s_float(uiTexWidth_);
                fY3_ = fHeight_ = s_float(uiTexHeight_);

                UpdateUVs_();

                bUsingMaterial_ = true;
            }
            else
            {
                Error(CLASS_NAME, "Can't create a Sprite if no texture unit has been assigned to the material.");
            }
        }
        else
        {
            pQuad_->pMat = MaterialManager::GetSingleton()->GetDefault2D();
            Error(CLASS_NAME, "Can't create a Sprite with no material and dimension.");
        }
    }

    Sprite::Sprite( s_refptr<Material> pMat, const s_float& fWidth, const s_float& fHeight )
    {
        mColor_ = Color(255, 255, 255, 255);

        pQuad_ = s_refptr<Quad>(new Quad());

        for (uint i = 0; i < 4; i++)
            pQuad_->lVertexArray[i].mColor = mColor_;

        fX1_ = 0.0f;
        fY1_ = 0.0f;
        fX3_ = fWidth_ = fWidth;
        fY3_ = fHeight_ = fHeight;

        if (pMat != NULL)
        {
            pQuad_->pMat = pMat;

            if (!pMat->IsPlain())
            {
                uiTexWidth_ = s_uint(pMat->GetWidth());
                uiTexHeight_ = s_uint(pMat->GetHeight());

                bUsingMaterial_ = true;
            }
        }
        else
            pQuad_->pMat = MaterialManager::GetSingleton()->GetDefault2D();

        if (!bUsingMaterial_)
        {
            pQuad_->lVertexArray[0].SetUV(0, 0);
            pQuad_->lVertexArray[1].SetUV(1, 0);
            pQuad_->lVertexArray[2].SetUV(1, 1);
            pQuad_->lVertexArray[3].SetUV(0, 1);
        }
        else
            UpdateUVs_();
    }

    Sprite::Sprite( s_refptr<Material> pMat, const s_float& fU, const s_float& fV, const s_float& fWidth, const s_float& fHeight )
    {
        mColor_ = Color(255, 255, 255, 255);

        pQuad_ = s_refptr<Quad>(new Quad());

        for (uint i = 0; i < 4; i++)
            pQuad_->lVertexArray[i].mColor = mColor_;

        fX1_ = fU;
        fY1_ = fV;
        fX3_ = fWidth_ = fWidth;
        fY3_ = fHeight_ = fHeight;

        if (pMat != NULL)
        {
            pQuad_->pMat = pMat;

            if (!pMat->IsPlain())
            {
                uiTexWidth_ = s_uint(pMat->GetWidth());
                uiTexHeight_ = s_uint(pMat->GetHeight());

                bUsingMaterial_ = true;
            }
        }
        else
            pQuad_->pMat = MaterialManager::GetSingleton()->GetDefault2D();

        if (!bUsingMaterial_)
        {
            pQuad_->lVertexArray[0].SetUV(0, 0);
            pQuad_->lVertexArray[1].SetUV(1, 0);
            pQuad_->lVertexArray[2].SetUV(1, 1);
            pQuad_->lVertexArray[3].SetUV(0, 1);
        }
        else
            UpdateUVs_();
    }

    Sprite::Sprite( const s_float& fWidth, const s_float& fHeight, const Color& mColor )
    {
        mColor_ = mColor;

        pQuad_ = s_refptr<Quad>(new Quad());

        for (uint i = 0; i < 4; i++)
            pQuad_->lVertexArray[i].mColor = mColor_;

        fX1_ = 0.0f;
        fY1_ = 0.0f;
        fX3_ = fWidth_ = fWidth;
        fY3_ = fHeight_ = fHeight;

        pQuad_->pMat = MaterialManager::GetSingleton()->GetDefault2D();
        bUsingMaterial_ = false;

        pQuad_->lVertexArray[0].SetUV(0, 0);
        pQuad_->lVertexArray[1].SetUV(1, 0);
        pQuad_->lVertexArray[2].SetUV(1, 1);
        pQuad_->lVertexArray[3].SetUV(0, 1);
    }

    void Sprite::Render( const s_float& fX, const s_float& fY )
    {
        s_float x1, y1, x2, y2;
        x1 = fX - fHotSpotX_;
        x2 = fX + fWidth_ - fHotSpotX_;
        if (SpriteManager::GetSingleton()->GetYAxisType() == AXIS_DOWN)
        {
            y1 = fY - fHotSpotY_;
            y2 = fY + fHeight_ - fHotSpotY_;
        }
        else
        {
            y1 = fY + fHeight_ - fHotSpotY_;
            y2 = fY - fHotSpotY_;
        }

        pQuad_->lVertexArray[0].Set(x1, y1);
        pQuad_->lVertexArray[1].Set(x2, y1);
        pQuad_->lVertexArray[2].Set(x2, y2);
        pQuad_->lVertexArray[3].Set(x1, y2);

        SpriteManager::GetSingleton()->RenderQuad(*pQuad_);
    }

    void Sprite::RenderEx( const s_float& fX, const s_float& fY, const s_float& fRot, const s_float& fHScale, const s_float& fVScale )
    {
        s_float x1, y1, x2, y2;
        x1 = -fHotSpotX_*fHScale;
        y1 = (fHeight_-fHotSpotY_)*fVScale;
        x2 = (fWidth_-fHotSpotX_)*fHScale;
        y2 = -fHotSpotY_*fVScale;

        x1 = -fHotSpotX_*fHScale;
        x2 = (fWidth_-fHotSpotX_)*fHScale;
        if (SpriteManager::GetSingleton()->GetYAxisType() == AXIS_DOWN)
        {
            y1 = -fHotSpotY_*fHScale;
            y2 = (fHeight_-fHotSpotY_)*fHScale;
        }
        else
        {
            y1 = (fHeight_-fHotSpotY_)*fHScale;
            y2 = -fHotSpotY_*fHScale;
        }

        if (!fRot.IsNull())
        {
            s_float cost = cos(fRot);
            s_float sint = sin(fRot);

            pQuad_->lVertexArray[0].Set(x1*cost - y1*sint + fX, x1*sint + y1*cost + fY);
            pQuad_->lVertexArray[1].Set(x2*cost - y1*sint + fX, x2*sint + y1*cost + fY);
            pQuad_->lVertexArray[2].Set(x2*cost - y2*sint + fX, x2*sint + y2*cost + fY);
            pQuad_->lVertexArray[3].Set(x1*cost - y2*sint + fX, x1*sint + y2*cost + fY);
        }
        else
        {
            pQuad_->lVertexArray[0].Set(x1 + fX, y1 + fY);
            pQuad_->lVertexArray[1].Set(x2 + fX, y1 + fY);
            pQuad_->lVertexArray[2].Set(x2 + fX, y2 + fY);
            pQuad_->lVertexArray[3].Set(x1 + fX, y2 + fY);
        }

        SpriteManager::GetSingleton()->RenderQuad(*pQuad_);
    }

    void Sprite::Render2V( const s_float& fX1, const s_float& fY1, const s_float& fX3, const s_float& fY3 )
    {
        pQuad_->lVertexArray[0].Set(fX1, fY1);
        pQuad_->lVertexArray[1].Set(fX3, fY1);
        pQuad_->lVertexArray[2].Set(fX3, fY3);
        pQuad_->lVertexArray[3].Set(fX1, fY3);

        SpriteManager::GetSingleton()->RenderQuad(*pQuad_);
    }

    void Sprite::Render4V( const s_float& fX1, const s_float& fY1, const s_float& fX2, const s_float& fY2, const s_float& fX3, const s_float& fY3, const s_float& fX4, const s_float& fY4 )
    {
        pQuad_->lVertexArray[0].Set(fX1, fY1);
        pQuad_->lVertexArray[1].Set(fX2, fY2);
        pQuad_->lVertexArray[2].Set(fX3, fY3);
        pQuad_->lVertexArray[3].Set(fX4, fY4);

        SpriteManager::GetSingleton()->RenderQuad(*pQuad_);
    }

    void Sprite::SetColor( const Color& mColor, const s_uint& uiIndex )
    {
        if (!uiIndex.IsValid())
        {
            mColor_ = mColor;
            for (uint i = 0; i < 4; i++)
                pQuad_->lVertexArray[i].mColor = mColor_;
        }
        else if (uiIndex < 4)
        {
            pQuad_->lVertexArray[uiIndex.Get()].mColor = mColor;
        }
        else
            Warning(CLASS_NAME, "Second parameter of SetColor mustn't be greater than 3.");
    }

    void Sprite::SetHotSpot( const Point<s_float>& mHotSpot )
    {
        fHotSpotX_ = mHotSpot.X();
        fHotSpotY_ = mHotSpot.Y();
    }

    void Sprite::SetHotSpot( const s_float& fX, const s_float& fY )
    {
        fHotSpotX_ = fX;
        fHotSpotY_ = fY;
    }

    void Sprite::SetTextureRect( const s_array<s_float,4>& lTextureRect, const s_bool& bNormalized )
    {
        bUsing8Coords_ = false;

        if (bNormalized)
        {
            if (bUsingMaterial_)
            {
                fX1_ = lTextureRect[0]*s_float(uiTexWidth_);
                fY1_ = lTextureRect[1]*s_float(uiTexHeight_);
                fX3_ = lTextureRect[2]*s_float(uiTexWidth_);
                fY3_ = lTextureRect[3]*s_float(uiTexHeight_);
            }
        }
        else
        {
            fX1_ = lTextureRect[0];
            fY1_ = lTextureRect[1];
            fX3_ = lTextureRect[2];
            fY3_ = lTextureRect[3];
        }

        if (bUsingMaterial_)
            UpdateUVs_();
    }

    void Sprite::SetTextureRect( const s_float& fX1, const s_float& fY1, const s_float& fX3, const s_float& fY3, const s_bool& bNormalized )
    {
        bUsing8Coords_ = false;

        if (bNormalized)
        {
            if (bUsingMaterial_)
            {
                fX1_ = fX1*s_float(uiTexWidth_);
                fY1_ = fY1*s_float(uiTexHeight_);
                fX3_ = fX3*s_float(uiTexWidth_);
                fY3_ = fY3*s_float(uiTexHeight_);
            }
        }
        else
        {
            fX1_ = fX1;
            fY1_ = fY1;
            fX3_ = fX3;
            fY3_ = fY3;
        }

        if (bUsingMaterial_)
            UpdateUVs_();
    }

    void Sprite::SetTextureCoords( const s_array<s_float,8>& lTextureRect, const s_bool& bNormalized )
    {
        bUsing8Coords_ = true;

        if (bNormalized)
        {
            if (bUsingMaterial_)
            {
                fX1_ = lTextureRect[0]*s_float(uiTexWidth_);
                fY1_ = lTextureRect[1]*s_float(uiTexHeight_);
                fX2_ = lTextureRect[2]*s_float(uiTexWidth_);
                fY2_ = lTextureRect[3]*s_float(uiTexHeight_);
                fX3_ = lTextureRect[4]*s_float(uiTexWidth_);
                fY3_ = lTextureRect[5]*s_float(uiTexHeight_);
                fX4_ = lTextureRect[6]*s_float(uiTexWidth_);
                fY4_ = lTextureRect[7]*s_float(uiTexHeight_);
            }
        }
        else
        {
            fX1_ = lTextureRect[0];
            fY1_ = lTextureRect[1];
            fX2_ = lTextureRect[2];
            fY2_ = lTextureRect[3];
            fX3_ = lTextureRect[4];
            fY3_ = lTextureRect[5];
            fX4_ = lTextureRect[6];
            fY4_ = lTextureRect[7];
        }

        if (bUsingMaterial_)
            UpdateUVs_();
    }

    void Sprite::SetTextureCoords( const s_float& fX1, const s_float& fY1, const s_float& fX2, const s_float& fY2, const s_float& fX3, const s_float& fY3, const s_float& fX4, const s_float& fY4, const s_bool& bNormalized )
    {
        bUsing8Coords_ = true;

        if (bNormalized)
        {
            if (bUsingMaterial_)
            {
                fX1_ = fX1*s_float(uiTexWidth_);
                fY1_ = fY1*s_float(uiTexHeight_);
                fX2_ = fX2*s_float(uiTexWidth_);
                fY2_ = fY2*s_float(uiTexHeight_);
                fX3_ = fX3*s_float(uiTexWidth_);
                fY3_ = fY3*s_float(uiTexHeight_);
                fX4_ = fX4*s_float(uiTexWidth_);
                fY4_ = fY4*s_float(uiTexHeight_);
            }
        }
        else
        {
            fX1_ = fX1;
            fY1_ = fY1;
            fX2_ = fX2;
            fY2_ = fY2;
            fX3_ = fX3;
            fY3_ = fY3;
            fX4_ = fX4;
            fY4_ = fY4;
        }

        if (bUsingMaterial_)
            UpdateUVs_();
    }

    void Sprite::SetWidth( const s_float& fWidth, const s_bool& bAdjustUVs )
    {
        if (fWidth <= 0.0f)
            fWidth_ = 1.0f;
        else
            fWidth_ = fWidth;

        if (bAdjustUVs)
        {
            fX3_ = fX1_ + fWidth_;
            UpdateUVs_();
        }
    }

    void Sprite::SetHeight( const s_float& fHeight, const s_bool& bAdjustUVs )
    {
        if (fHeight <= 0.0f)
            fHeight_ = 1.0f;
        else
            fHeight_ = fHeight;

        if (bAdjustUVs)
        {
            fY1_ = fY3_ - fHeight_;
            UpdateUVs_();
        }
    }

    void Sprite::SetDimensions(const s_float& fWidth, const s_float& fHeight, const s_bool& bAdjustUVs )
    {
        if (fWidth < 1.0f)
            fWidth_ = 1.0f;
        else
            fWidth_ = fWidth;

        if (fHeight < 1.0f)
            fHeight_ = 1.0f;
        else
            fHeight_ = fHeight;

        if (bAdjustUVs)
        {
            fX3_ = fX1_ + fWidth_;
            fY1_ = fY3_ - fHeight_;
            UpdateUVs_();
        }
    }

    const Color& Sprite::GetColor() const
    {
        return mColor_;
    }

    Point<s_float> Sprite::GetHotSpot() const
    {
        return Point<s_float>(fHotSpotX_, fHotSpotY_);
    }

    s_array<s_float,4> Sprite::GetTextureRect() const
    {
        return s_array<s_float,4>((fX1_, fY1_, fX3_, fY3_));
    }

    s_array<s_float,8> Sprite::GetTextureCoords() const
    {
        if (bUsing8Coords_)
            return s_array<s_float,8>((fX1_, fY1_, fX2_, fY2_, fX3_, fY3_, fX4_, fY4_));
        else
            return s_array<s_float,8>((fX1_, fY1_, fX3_, fY1_, fX3_, fY3_, fX1_, fY3_));
    }

    const s_float& Sprite::GetWidth() const
    {
        return fWidth_;
    }

    const s_float& Sprite::GetHeight() const
    {
        return fHeight_;
    }

    s_refptr<Material> Sprite::GetMaterial()
    {
        return pQuad_->pMat;
    }

    void Sprite::UpdateUVs_()
    {
        if (bUsing8Coords_)
        {
            s_float fTX1 = fX1_/s_float(uiTexWidth_);
            s_float fTY1 = fY1_/s_float(uiTexHeight_);
            s_float fTX2 = fX2_/s_float(uiTexWidth_);
            s_float fTY2 = fY2_/s_float(uiTexHeight_);
            s_float fTX3 = fX3_/s_float(uiTexWidth_);
            s_float fTY3 = fY3_/s_float(uiTexHeight_);
            s_float fTX4 = fX4_/s_float(uiTexWidth_);
            s_float fTY4 = fY4_/s_float(uiTexHeight_);

            pQuad_->lVertexArray[0].SetUV(fTX1, fTY1);
            pQuad_->lVertexArray[1].SetUV(fTX2, fTY2);
            pQuad_->lVertexArray[2].SetUV(fTX3, fTY3);
            pQuad_->lVertexArray[3].SetUV(fTX4, fTY4);
        }
        else
        {
            s_float fTX1 = fX1_/s_float(uiTexWidth_);
            s_float fTY1 = fY1_/s_float(uiTexHeight_);
            s_float fTX3 = fX3_/s_float(uiTexWidth_);
            s_float fTY3 = fY3_/s_float(uiTexHeight_);

            pQuad_->lVertexArray[0].SetUV(fTX1, fTY1);
            pQuad_->lVertexArray[1].SetUV(fTX3, fTY1);
            pQuad_->lVertexArray[2].SetUV(fTX3, fTY3);
            pQuad_->lVertexArray[3].SetUV(fTX1, fTY3);
        }
    }
}
