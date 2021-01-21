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

#if FRONT_END_OGRE
    #include "impl/ogre/ogre_sprite.h"
#elif FRONT_END_SFML
    #include "impl/sfml/sfml_sprite.h"
#else
    #error "No implementation defined for Sprite !"
#endif

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str Sprite::CLASS_NAME = "GUI::Sprite";

Vertex::Vertex() : mColor(Color::WHITE)
{
}

Vertex::Vertex( const Vector2D& mPos, const Vector2D& mUV, const Color& mCol ) :
    mPosition(mPos), mUVs(mUV), mColor(mCol)
{
}

Sprite::Sprite( s_ptr<GUIManager> pManager )
{
    pImpl_ = s_refptr<SpriteImpl>(new SpriteImpl(pManager));
}

Sprite::Sprite( const Sprite& mSprite ) : pImpl_(new SpriteImpl(*mSprite.pImpl_.Get()))
{
}

Sprite::~Sprite()
{
}

Sprite::Sprite( s_ptr<GUIManager> pManager, const s_str& sTextureFile )
{
    pImpl_ = s_refptr<SpriteImpl>(new SpriteImpl(pManager, sTextureFile));
}

Sprite::Sprite( s_ptr<GUIManager> pManager, const s_str& sTextureFile, const s_float& fWidth, const s_float& fHeight )
{
    pImpl_ = s_refptr<SpriteImpl>(new SpriteImpl(pManager, sTextureFile, fWidth, fHeight));
}

Sprite::Sprite( s_ptr<GUIManager> pManager, const Material& mMat )
{
    pImpl_ = s_refptr<SpriteImpl>(new SpriteImpl(pManager, mMat));
}

Sprite::Sprite( s_ptr<GUIManager> pManager, const Material& mMat, const s_float& fWidth, const s_float& fHeight )
{
    pImpl_ = s_refptr<SpriteImpl>(new SpriteImpl(pManager, mMat, fWidth, fHeight));
}

Sprite::Sprite( s_ptr<GUIManager> pManager, const Material& mMat, const s_float& fU, const s_float& fV, const s_float& fWidth, const s_float& fHeight )
{
    pImpl_ = s_refptr<SpriteImpl>(new SpriteImpl(pManager, mMat, fU, fV, fWidth, fHeight));
}

Sprite::Sprite( s_ptr<GUIManager> pManager, const Color& mColor, const s_float& fWidth, const s_float& fHeight )
{
    pImpl_ = s_refptr<SpriteImpl>(new SpriteImpl(pManager, mColor, fWidth, fHeight));
}

Sprite::Sprite( s_ptr<GUIManager> pManager, s_ptr<RenderTarget> pRenderTarget )
{
    pImpl_ = s_refptr<SpriteImpl>(new SpriteImpl(pManager, pRenderTarget));
}

Sprite::Sprite( s_ptr<GUIManager> pManager, s_ptr<RenderTarget> pRenderTarget, const s_float& fWidth, const s_float& fHeight )
{
    pImpl_ = s_refptr<SpriteImpl>(new SpriteImpl(pManager, pRenderTarget, fWidth, fHeight));
}

void Sprite::Render( const s_float& fX, const s_float& fY ) const
{
    pImpl_->Render(fX, fY);
}

void Sprite::RenderEx( const s_float& fX, const s_float& fY, const s_float& fRot, const s_float& fHScale, const s_float& fVScale ) const
{
    pImpl_->RenderEx(fX, fY, fRot, fHScale, fVScale);
}

void Sprite::Render2V( const s_float& fX1, const s_float& fY1, const s_float& fX3, const s_float& fY3 )
{
    pImpl_->Render2V(fX1, fY1, fX3, fY3);
}

void Sprite::Render4V( const s_float& fX1, const s_float& fY1,
                       const s_float& fX2, const s_float& fY2,
                       const s_float& fX3, const s_float& fY3,
                       const s_float& fX4, const s_float& fY4 )
{
    pImpl_->Render4V(fX1, fY1, fX2, fY2, fX3, fY3, fX4, fY4);
}

void Sprite::RenderStatic() const
{
    pImpl_->RenderStatic();
}

void Sprite::SetQuad( const s_array<Vertex,4>& lVertexArray )
{
    pImpl_->SetQuad(lVertexArray);
}

void Sprite::SetColor( const Color& mColor, const s_uint& uiIndex )
{
    pImpl_->SetColor(mColor, uiIndex);
}

void Sprite::SetDesaturated( const s_bool& bDesaturated )
{
    pImpl_->SetDesaturated(bDesaturated);
}

void Sprite::SetBlendMode( BlendMode mBlendMode )
{
    pImpl_->SetBlendMode(mBlendMode);
}

void Sprite::SetHotSpot( const Vector2D& mHotSpot )
{
    pImpl_->SetHotSpot(mHotSpot);
}

void Sprite::SetHotSpot( const s_float& fX, const s_float& fY )
{
    SetHotSpot(Vector2D(fX, fY));
}

void Sprite::SetTextureRect( const s_array<s_float,4>& lTextureRect, const s_bool& bNormalized )
{
    pImpl_->SetTextureRect(lTextureRect, bNormalized);
}

void Sprite::SetTextureRect( const s_float& fX1, const s_float& fY1, const s_float& fX3, const s_float& fY3, const s_bool& bNormalized )
{
    SetTextureRect((fX1, fY1, fX3, fY3), bNormalized);
}

void Sprite::SetTextureCoords( const s_array<s_float,8>& lTextureCoords, const s_bool& bNormalized )
{
    pImpl_->SetTextureCoords(lTextureCoords, bNormalized);
}

void Sprite::SetTextureCoords( const s_float& fX1, const s_float& fY1, const s_float& fX2, const s_float& fY2, const s_float& fX3, const s_float& fY3, const s_float& fX4, const s_float& fY4, const s_bool& bNormalized )
{
    SetTextureRect((fX1, fY1, fX2, fY2, fX3, fY3, fX4, fY4), bNormalized);
}

void Sprite::SetDimensions(const s_float& fWidth, const s_float& fHeight )
{
    pImpl_->SetDimensions(fWidth, fHeight);
}

s_float Sprite::GetWidth() const
{
    return pImpl_->GetWidth();
}

s_float Sprite::GetHeight() const
{
    return pImpl_->GetHeight();
}

s_array<s_float,4> Sprite::GetTextureRect() const
{
    return pImpl_->GetTextureRect();
}

s_array<s_float,8> Sprite::GetTextureCoords( const s_bool& bNormalized ) const
{
    return pImpl_->GetTextureCoords(bNormalized);
}

Color Sprite::GetColor() const
{
    return pImpl_->GetColor();
}

BlendMode Sprite::GetBlendMode() const
{
    return pImpl_->GetBlendMode();
}

s_wptr<SpriteImpl> Sprite::GetImpl()
{
    return pImpl_;
}

GUI::Material Sprite::LoadTexture( const s_str& sFileName )
{
    return SpriteImpl::LoadTexture(sFileName);
}
