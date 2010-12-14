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

#include "frost_sprite.h"
#include "frost_rendertarget.h"
#include "frost_material.h"
#include "frost_guimanager.h"
#include "impl/sfml/sfml_sprite.h"
#include "impl/sfml/sfml_texturemanager.h"
#include "impl/sfml/sfml_rendertarget.h"
#include "impl/sfml/sfml_material.h"
#include "impl/sfml/sfml_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str SpriteImpl::CLASS_NAME = "Sprite";

void Frost::RenderQuad(const sf::Quad& mQuad);

SpriteImpl::~SpriteImpl()
{
}

SpriteImpl::SpriteImpl( s_ptr<GUIManager> pManager ) : pManager_(pManager)
{
}

SpriteImpl::SpriteImpl( s_ptr<GUIManager> pManager, const s_str& sTextureFile ) :
    pManager_(pManager), sTextureFile_(sTextureFile)
{
    mQuad_.SetImage(*TextureManager::GetSingleton()->LoadTexture(sTextureFile_));
    fTexWidth_ = fWidth_ = mQuad_.GetImage()->GetWidth();
    fTexHeight_ = fHeight_ = mQuad_.GetImage()->GetHeight();
}

SpriteImpl::SpriteImpl( s_ptr<GUIManager> pManager, const s_str& sTextureFile, const s_float& fWidth, const s_float& fHeight ) :
    pManager_(pManager), sTextureFile_(sTextureFile)
{
    mQuad_.SetImage(*TextureManager::GetSingleton()->LoadTexture(sTextureFile_));
    fWidth_ = fWidth;
    fHeight_ = fHeight;
}

SpriteImpl::SpriteImpl( s_ptr<GUIManager> pManager, const Material& mMat ) : pManager_(pManager)
{
    mQuad_.SetImage(*mMat.GetImpl()->GetImage());
    fTexWidth_ = fWidth_ = mMat.GetWidth();
    fTexHeight_ = fHeight_ = mMat.GetHeight();
}

SpriteImpl::SpriteImpl( s_ptr<GUIManager> pManager, const Material& mMat, const s_float& fWidth, const s_float& fHeight ) : pManager_(pManager)
{
    mQuad_.SetImage(*mMat.GetImpl()->GetImage());
    fTexWidth_ = mMat.GetWidth();
    fTexHeight_ = mMat.GetHeight();
    fWidth_ = fWidth;
    fHeight_ = fHeight;
}

SpriteImpl::SpriteImpl( s_ptr<GUIManager> pManager, const Material& mMat, const s_float& fU, const s_float& fV, const s_float& fWidth, const s_float& fHeight ) : pManager_(pManager)
{
    mQuad_.SetImage(*mMat.GetImpl()->GetImage());
    fTexWidth_ = mMat.GetWidth();
    fTexHeight_ = mMat.GetHeight();
    fWidth_ = fWidth;
    fHeight_ = fHeight;

    mQuad_.SetVertexUVs(0, sf::Vector2f((fU/fTexWidth_).Get(),          (fV/fTexHeight_).Get()));
    mQuad_.SetVertexUVs(1, sf::Vector2f(((fU+fWidth)/fTexWidth_).Get(), (fV/fTexHeight_).Get()));
    mQuad_.SetVertexUVs(3, sf::Vector2f(((fU+fWidth)/fTexWidth_).Get(), ((fV+fHeight)/fTexHeight_).Get()));
    mQuad_.SetVertexUVs(2, sf::Vector2f((fU/fTexWidth_).Get(),          ((fV+fHeight)/fTexHeight_).Get()));
}

SpriteImpl::SpriteImpl( s_ptr<GUIManager> pManager, const Color& mColor, const s_float& fWidth, const s_float& fHeight ) : pManager_(pManager)
{
    mQuad_.SetColor(sf::Color(
        255.0f*mColor.GetR().Get(),
        255.0f*mColor.GetG().Get(),
        255.0f*mColor.GetB().Get(),
        255.0f*mColor.GetA().Get()
    ));
    fTexWidth_ = 1.0f;
    fTexHeight_ = 1.0f;
    fWidth_ = fWidth;
    fHeight_ = fHeight;
}

SpriteImpl::SpriteImpl( s_ptr<GUIManager> pManager, s_ptr<RenderTarget> pRenderTarget ) : pManager_(pManager)
{
    mQuad_.SetImage(pRenderTarget->GetImpl()->GetImage());
    mQuad_.SetBlendMode(sf::Blend::None);
    mQuad_.FlipY(true);
    fTexWidth_ = fWidth_ = s_float(pRenderTarget->GetWidth());
    fTexHeight_ = fHeight_ = s_float(pRenderTarget->GetHeight());
}

SpriteImpl::SpriteImpl( s_ptr<GUIManager> pManager, s_ptr<RenderTarget> pRenderTarget, const s_float& fWidth, const s_float& fHeight ) : pManager_(pManager)
{
    mQuad_.SetImage(pRenderTarget->GetImpl()->GetImage());
    mQuad_.SetBlendMode(sf::Blend::None);
    mQuad_.FlipY(true);
    fTexWidth_ = s_float(pRenderTarget->GetWidth());
    fTexHeight_ = s_float(pRenderTarget->GetHeight());
    fWidth_ = fWidth;
    fHeight_ = fHeight;
}

void SpriteImpl::Render( const s_float& fX, const s_float& fY ) const
{
    mQuad_.SetVertexPosition(0, sf::Vector2f((fX-fHotSpotX_).Get(),         (fY-fHotSpotY_).Get()));
    mQuad_.SetVertexPosition(1, sf::Vector2f((fX-fHotSpotX_+fWidth_).Get(), (fY-fHotSpotY_).Get()));
    mQuad_.SetVertexPosition(3, sf::Vector2f((fX-fHotSpotX_+fWidth_).Get(), (fY-fHotSpotY_+fHeight_).Get()));
    mQuad_.SetVertexPosition(2, sf::Vector2f((fX-fHotSpotX_).Get(),         (fY-fHotSpotY_+fHeight_).Get()));

    if (pManager_)
        pManager_->GetImpl()->GetCurrentRenderTarget()->Draw(mQuad_);
    else
        RenderQuad(mQuad_);
}

void SpriteImpl::RenderEx( const s_float& fX, const s_float& fY, const s_float& fRot, const s_float& fHScale, const s_float& fVScale ) const
{
    s_float x1, y1, x2, y2;
    x1 = -fHotSpotX_*fHScale;
    x2 = (fWidth_-fHotSpotX_)*fHScale;
    y1 = -fHotSpotY_*fVScale;
    y2 = (fHeight_-fHotSpotY_)*fVScale;

    if (!fRot.IsNull())
    {
        s_float cost = cos(fRot);
        s_float sint = sin(fRot);

        mQuad_.SetVertexPosition(0, sf::Vector2f((x1*cost - y1*sint + fX).Get(), (x1*sint + y1*cost + fY).Get()));
        mQuad_.SetVertexPosition(1, sf::Vector2f((x2*cost - y1*sint + fX).Get(), (x2*sint + y1*cost + fY).Get()));
        mQuad_.SetVertexPosition(3, sf::Vector2f((x2*cost - y2*sint + fX).Get(), (x2*sint + y2*cost + fY).Get()));
        mQuad_.SetVertexPosition(2, sf::Vector2f((x1*cost - y2*sint + fX).Get(), (x1*sint + y2*cost + fY).Get()));
    }
    else
    {
        mQuad_.SetVertexPosition(0, sf::Vector2f((x1 + fX).Get(), (y1 + fY).Get()));
        mQuad_.SetVertexPosition(1, sf::Vector2f((x2 + fX).Get(), (y1 + fY).Get()));
        mQuad_.SetVertexPosition(3, sf::Vector2f((x2 + fX).Get(), (y2 + fY).Get()));
        mQuad_.SetVertexPosition(2, sf::Vector2f((x1 + fX).Get(), (y2 + fY).Get()));
    }

    if (pManager_)
        pManager_->GetImpl()->GetCurrentRenderTarget()->Draw(mQuad_);
    else
        RenderQuad(mQuad_);
}

void SpriteImpl::Render2V( const s_float& fX1, const s_float& fY1, const s_float& fX3, const s_float& fY3 )
{
    mQuad_.SetVertexPosition(0, sf::Vector2f(fX1.Get(), fY1.Get()));
    mQuad_.SetVertexPosition(1, sf::Vector2f(fX3.Get(), fY1.Get()));
    mQuad_.SetVertexPosition(3, sf::Vector2f(fX3.Get(), fY3.Get()));
    mQuad_.SetVertexPosition(2, sf::Vector2f(fX1.Get(), fY3.Get()));

    if (pManager_)
        pManager_->GetImpl()->GetCurrentRenderTarget()->Draw(mQuad_);
    else
        RenderQuad(mQuad_);
}

void SpriteImpl::Render4V( const s_float& fX1, const s_float& fY1,
                       const s_float& fX2, const s_float& fY2,
                       const s_float& fX3, const s_float& fY3,
                       const s_float& fX4, const s_float& fY4 )
{
    mQuad_.SetVertexPosition(0, sf::Vector2f(fX1.Get(), fY1.Get()));
    mQuad_.SetVertexPosition(1, sf::Vector2f(fX2.Get(), fY2.Get()));
    mQuad_.SetVertexPosition(3, sf::Vector2f(fX3.Get(), fY3.Get()));
    mQuad_.SetVertexPosition(2, sf::Vector2f(fX4.Get(), fY4.Get()));

    if (pManager_)
        pManager_->GetImpl()->GetCurrentRenderTarget()->Draw(mQuad_);
    else
        RenderQuad(mQuad_);
}

void SpriteImpl::RenderStatic() const
{
    if (pManager_)
        pManager_->GetImpl()->GetCurrentRenderTarget()->Draw(mQuad_);
    else
        RenderQuad(mQuad_);
}

void SpriteImpl::SetQuad( const s_array<Vertex,4>& lVertexArray )
{
    for (uint i = 0; i < 4; ++i)
    {
        uint uiIndex = i;
        if (i == 3)
            uiIndex = 2;
        else if (i == 2)
            uiIndex = 3;

        const Vertex& mV = lVertexArray[i];
        mQuad_.SetVertexPosition(uiIndex, sf::Vector2f(mV.mPosition.X().Get(), mV.mPosition.Y().Get()));
        mQuad_.SetVertexUVs(uiIndex,      sf::Vector2f(mV.mUVs.X().Get(), mV.mUVs.Y().Get()));
        mQuad_.SetVertexColor(uiIndex,    sf::Color(
            255.0f*mV.mColor.GetR().Get(),
            255.0f*mV.mColor.GetG().Get(),
            255.0f*mV.mColor.GetB().Get(),
            255.0f*mV.mColor.GetA().Get()
        ));
    }
}

void SpriteImpl::SetColor( const Color& mColor, const s_uint& uiIndex )
{
    if (uiIndex.IsValid())
    {
        s_uint uiCorrectedIndex = uiIndex;
        if (uiIndex == 3)
            uiCorrectedIndex = 2;
        else if (uiIndex == 2)
            uiCorrectedIndex = 3;

        mQuad_.SetVertexColor(uiCorrectedIndex.Get(), sf::Color(
            255.0f*mColor.GetR().Get(),
            255.0f*mColor.GetG().Get(),
            255.0f*mColor.GetB().Get(),
            255.0f*mColor.GetA().Get()
        ));
    }
    else
    {
        mQuad_.SetColor(sf::Color(
            255.0f*mColor.GetR().Get(),
            255.0f*mColor.GetG().Get(),
            255.0f*mColor.GetB().Get(),
            255.0f*mColor.GetA().Get()
        ));
    }
}

void SpriteImpl::SetDesaturated( const s_bool& bDesaturated )
{

}

void SpriteImpl::SetBlendMode( BlendMode mBlendMode )
{
    switch (mBlendMode)
    {
        case BLEND_NORMAL : mQuad_.SetBlendMode(sf::Blend::Alpha); break;
        case BLEND_ADD : mQuad_.SetBlendMode(sf::Blend::Add); break;
        case BLEND_MUL : mQuad_.SetBlendMode(sf::Blend::Multiply); break;
        default : break;
    }
}

void SpriteImpl::SetHotSpot( const Point<s_float>& mHotSpot )
{
    fHotSpotX_ = mHotSpot.X();
    fHotSpotY_ = mHotSpot.Y();
}

void SpriteImpl::SetHotSpot( const s_float& fX, const s_float& fY )
{
    fHotSpotX_ = fX;
    fHotSpotY_ = fY;
}

void SpriteImpl::SetTextureRect( const s_array<s_float,4>& lTextureRect, const s_bool& bNormalized )
{
    if (bNormalized)
    {
        mQuad_.SetVertexUVs(0, sf::Vector2f(lTextureRect[0].Get(), lTextureRect[1].Get()));
        mQuad_.SetVertexUVs(1, sf::Vector2f(lTextureRect[2].Get(), lTextureRect[1].Get()));
        mQuad_.SetVertexUVs(3, sf::Vector2f(lTextureRect[2].Get(), lTextureRect[3].Get()));
        mQuad_.SetVertexUVs(2, sf::Vector2f(lTextureRect[0].Get(), lTextureRect[3].Get()));
    }
    else
    {
        s_ptr<const sf::Image> pImage = mQuad_.GetImage();
        s_float fWidth, fHeight;
        if (pImage)
        {
            fWidth = pImage->GetWidth();
            fHeight = pImage->GetHeight();
        }
        else
        {
            fWidth = 1.0f;
            fHeight = 1.0f;
        }

        mQuad_.SetVertexUVs(0, sf::Vector2f((lTextureRect[0]/fWidth).Get(), (lTextureRect[1]/fHeight).Get()));
        mQuad_.SetVertexUVs(1, sf::Vector2f((lTextureRect[2]/fWidth).Get(), (lTextureRect[1]/fHeight).Get()));
        mQuad_.SetVertexUVs(3, sf::Vector2f((lTextureRect[2]/fWidth).Get(), (lTextureRect[3]/fHeight).Get()));
        mQuad_.SetVertexUVs(2, sf::Vector2f((lTextureRect[0]/fWidth).Get(), (lTextureRect[3]/fHeight).Get()));
    }
}

void SpriteImpl::SetTextureRect( const s_float& fX1, const s_float& fY1, const s_float& fX3, const s_float& fY3, const s_bool& bNormalized )
{
    if (bNormalized)
    {
        mQuad_.SetVertexUVs(0, sf::Vector2f(fX1.Get(), fY1.Get()));
        mQuad_.SetVertexUVs(1, sf::Vector2f(fX3.Get(), fY1.Get()));
        mQuad_.SetVertexUVs(3, sf::Vector2f(fX3.Get(), fY3.Get()));
        mQuad_.SetVertexUVs(2, sf::Vector2f(fX1.Get(), fY3.Get()));
    }
    else
    {
        s_ptr<const sf::Image> pImage = mQuad_.GetImage();
        s_float fWidth, fHeight;
        if (pImage)
        {
            fWidth = pImage->GetWidth();
            fHeight = pImage->GetHeight();
        }
        else
        {
            fWidth = 1.0f;
            fHeight = 1.0f;
        }

        mQuad_.SetVertexUVs(0, sf::Vector2f((fX1/fWidth).Get(), (fY1/fHeight).Get()));
        mQuad_.SetVertexUVs(1, sf::Vector2f((fX3/fWidth).Get(), (fY1/fHeight).Get()));
        mQuad_.SetVertexUVs(3, sf::Vector2f((fX3/fWidth).Get(), (fY3/fHeight).Get()));
        mQuad_.SetVertexUVs(2, sf::Vector2f((fX1/fWidth).Get(), (fY3/fHeight).Get()));
    }
}

void SpriteImpl::SetTextureCoords( const s_array<s_float,8>& lTextureRect, const s_bool& bNormalized )
{
    if (bNormalized)
    {
        mQuad_.SetVertexUVs(0, sf::Vector2f(lTextureRect[0].Get(), lTextureRect[1].Get()));
        mQuad_.SetVertexUVs(1, sf::Vector2f(lTextureRect[2].Get(), lTextureRect[3].Get()));
        mQuad_.SetVertexUVs(3, sf::Vector2f(lTextureRect[4].Get(), lTextureRect[5].Get()));
        mQuad_.SetVertexUVs(2, sf::Vector2f(lTextureRect[6].Get(), lTextureRect[7].Get()));
    }
    else
    {
        s_ptr<const sf::Image> pImage = mQuad_.GetImage();
        s_float fWidth, fHeight;
        if (pImage)
        {
            fWidth = pImage->GetWidth();
            fHeight = pImage->GetHeight();
        }
        else
        {
            fWidth = 1.0f;
            fHeight = 1.0f;
        }

        mQuad_.SetVertexUVs(0, sf::Vector2f((lTextureRect[0]/fWidth).Get(), (lTextureRect[1]/fHeight).Get()));
        mQuad_.SetVertexUVs(1, sf::Vector2f((lTextureRect[2]/fWidth).Get(), (lTextureRect[3]/fHeight).Get()));
        mQuad_.SetVertexUVs(3, sf::Vector2f((lTextureRect[4]/fWidth).Get(), (lTextureRect[5]/fHeight).Get()));
        mQuad_.SetVertexUVs(2, sf::Vector2f((lTextureRect[6]/fWidth).Get(), (lTextureRect[7]/fHeight).Get()));
    }
}

void SpriteImpl::SetTextureCoords( const s_float& fX1, const s_float& fY1, const s_float& fX2, const s_float& fY2, const s_float& fX3, const s_float& fY3, const s_float& fX4, const s_float& fY4, const s_bool& bNormalized )
{
    if (bNormalized)
    {
        mQuad_.SetVertexUVs(0, sf::Vector2f(fX1.Get(), fY1.Get()));
        mQuad_.SetVertexUVs(1, sf::Vector2f(fX2.Get(), fY2.Get()));
        mQuad_.SetVertexUVs(3, sf::Vector2f(fX3.Get(), fY3.Get()));
        mQuad_.SetVertexUVs(2, sf::Vector2f(fX4.Get(), fY4.Get()));
    }
    else
    {
        s_ptr<const sf::Image> pImage = mQuad_.GetImage();
        s_float fWidth, fHeight;
        if (pImage)
        {
            fWidth = pImage->GetWidth();
            fHeight = pImage->GetHeight();
        }
        else
        {
            fWidth = 1.0f;
            fHeight = 1.0f;
        }

        mQuad_.SetVertexUVs(0, sf::Vector2f((fX1/fWidth).Get(), (fY1/fHeight).Get()));
        mQuad_.SetVertexUVs(1, sf::Vector2f((fX2/fWidth).Get(), (fY2/fHeight).Get()));
        mQuad_.SetVertexUVs(3, sf::Vector2f((fX3/fWidth).Get(), (fY3/fHeight).Get()));
        mQuad_.SetVertexUVs(2, sf::Vector2f((fX4/fWidth).Get(), (fY4/fHeight).Get()));
    }
}

void SpriteImpl::SetDimensions(const s_float& fWidth, const s_float& fHeight )
{
    fWidth_ = fWidth;
    fHeight_ = fHeight;
}

s_float SpriteImpl::GetWidth() const
{
    return fWidth_;
}

s_float SpriteImpl::GetHeight() const
{
    return fHeight_;
}

s_array<s_float,4> SpriteImpl::GetTextureRect() const
{
    return s_float(mQuad_.GetVertexUVs(0).x), s_float(mQuad_.GetVertexUVs(0).y),
           s_float(mQuad_.GetVertexUVs(3).x), s_float(mQuad_.GetVertexUVs(3).y);
}

s_array<s_float,8> SpriteImpl::GetTextureCoords( const s_bool& bNormalized ) const
{
    if (bNormalized)
    {
        return s_float(mQuad_.GetVertexUVs(0).x), s_float(mQuad_.GetVertexUVs(0).y),
               s_float(mQuad_.GetVertexUVs(1).x), s_float(mQuad_.GetVertexUVs(1).y),
               s_float(mQuad_.GetVertexUVs(3).x), s_float(mQuad_.GetVertexUVs(3).y),
               s_float(mQuad_.GetVertexUVs(2).x), s_float(mQuad_.GetVertexUVs(2).y);
    }
    else
    {
        s_ptr<const sf::Image> pImage = mQuad_.GetImage();
        s_float fWidth, fHeight;
        if (pImage)
        {
            fWidth = pImage->GetWidth();
            fHeight = pImage->GetHeight();
        }
        else
        {
            fWidth = 1.0f;
            fHeight = 1.0f;
        }

        return mQuad_.GetVertexUVs(0).x*fWidth, mQuad_.GetVertexUVs(0).y*fHeight,
               mQuad_.GetVertexUVs(1).x*fWidth, mQuad_.GetVertexUVs(1).y*fHeight,
               mQuad_.GetVertexUVs(3).x*fWidth, mQuad_.GetVertexUVs(3).y*fHeight,
               mQuad_.GetVertexUVs(2).x*fWidth, mQuad_.GetVertexUVs(2).y*fHeight;
    }
}

Color SpriteImpl::GetColor() const
{
    const sf::Color& mColor = mQuad_.GetColor();
    return Color(
        mColor.a/255.0f,
        mColor.r/255.0f,
        mColor.g/255.0f,
        mColor.b/255.0f
    );
}

BlendMode SpriteImpl::GetBlendMode() const
{
    switch (mQuad_.GetBlendMode())
    {
        case sf::Blend::Alpha    : return BLEND_NORMAL;
        case sf::Blend::Add      : return BLEND_ADD;
        case sf::Blend::Multiply : return BLEND_MUL;
        default : return BLEND_NORMAL;
    }
}

s_ptr<const sf::Image> SpriteImpl::GetImage() const
{
    return mQuad_.GetImage();
}

Material SpriteImpl::LoadTexture( const s_str& sFileName )
{
    return s_refptr<MaterialImpl>(new MaterialImpl(TextureManager::GetSingleton()->LoadTexture(sFileName)));
}
