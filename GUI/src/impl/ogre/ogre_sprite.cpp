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
#include "impl/ogre/ogre_guimanager.h"
#include "impl/ogre/ogre_sprite.h"
#include "impl/ogre/ogre_material.h"

#include <OgreMaterial.h>
#include <OgreTechnique.h>
#include <OgrePass.h>

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str SpriteImpl::CLASS_NAME = "Sprite";

SpriteImpl::~SpriteImpl()
{
}

SpriteImpl::SpriteImpl( s_ptr<GUIManager> pManager ) : pManager_(pManager)
{
}

SpriteImpl::SpriteImpl( s_ptr<GUIManager> pManager, const s_str& sTextureFile ) :
    pManager_(pManager), sTextureFile_(sTextureFile)
{
    mQuad_.mMat = s_refptr<MaterialImpl>(new MaterialImpl(sTextureFile_));
    fTexWidth_ = fWidth_ = mQuad_.mMat.GetWidth();
    fTexHeight_ = fHeight_ = mQuad_.mMat.GetHeight();

    mQuad_.lVertexArray[0].mUVs = Vector2D(0.0f, 0.0f);
    mQuad_.lVertexArray[1].mUVs = Vector2D(1.0f, 0.0f);
    mQuad_.lVertexArray[2].mUVs = Vector2D(1.0f, 1.0f);
    mQuad_.lVertexArray[3].mUVs = Vector2D(0.0f, 1.0f);
}

SpriteImpl::SpriteImpl( s_ptr<GUIManager> pManager, const s_str& sTextureFile, const s_float& fWidth, const s_float& fHeight ) :
    pManager_(pManager), sTextureFile_(sTextureFile)
{
    mQuad_.mMat = s_refptr<MaterialImpl>(new MaterialImpl(sTextureFile_));
    fTexWidth_ = mQuad_.mMat.GetWidth();
    fTexHeight_ = mQuad_.mMat.GetHeight();
    fWidth_ = fWidth;
    fHeight_ = fHeight;

    mQuad_.lVertexArray[0].mUVs = Vector2D(0.0f,              0.0f);
    mQuad_.lVertexArray[1].mUVs = Vector2D(fWidth/fTexWidth_, 0.0f);
    mQuad_.lVertexArray[2].mUVs = Vector2D(fWidth/fTexWidth_, fHeight/fTexHeight_);
    mQuad_.lVertexArray[3].mUVs = Vector2D(0.0f,              fHeight/fTexHeight_);
}

SpriteImpl::SpriteImpl( s_ptr<GUIManager> pManager, const Material& mMat ) : pManager_(pManager)
{
    mQuad_.mMat = mMat;
    fTexWidth_ = fWidth_ = mMat.GetWidth();
    fTexHeight_ = fHeight_ = mMat.GetHeight();

    mQuad_.lVertexArray[0].mUVs = Vector2D(0.0f, 0.0f);
    mQuad_.lVertexArray[1].mUVs = Vector2D(1.0f, 0.0f);
    mQuad_.lVertexArray[2].mUVs = Vector2D(1.0f, 1.0f);
    mQuad_.lVertexArray[3].mUVs = Vector2D(0.0f, 1.0f);
}

SpriteImpl::SpriteImpl( s_ptr<GUIManager> pManager, const Material& mMat, const s_float& fWidth, const s_float& fHeight ) : pManager_(pManager)
{
    mQuad_.mMat = mMat;
    fTexWidth_ = mMat.GetWidth();
    fTexHeight_ = mMat.GetHeight();
    fWidth_ = fWidth;
    fHeight_ = fHeight;

    mQuad_.lVertexArray[0].mUVs = Vector2D(0.0f,              0.0f);
    mQuad_.lVertexArray[1].mUVs = Vector2D(fWidth/fTexWidth_, 0.0f);
    mQuad_.lVertexArray[2].mUVs = Vector2D(fWidth/fTexWidth_, fHeight/fTexHeight_);
    mQuad_.lVertexArray[3].mUVs = Vector2D(0.0f,              fHeight/fTexHeight_);
}

SpriteImpl::SpriteImpl( s_ptr<GUIManager> pManager, const Material& mMat, const s_float& fU, const s_float& fV, const s_float& fWidth, const s_float& fHeight ) : pManager_(pManager)
{
    mQuad_.mMat = mMat;
    fTexWidth_ = mMat.GetWidth();
    fTexHeight_ = mMat.GetHeight();
    fWidth_ = fWidth;
    fHeight_ = fHeight;

    mQuad_.lVertexArray[0].mUVs = Vector2D(fU/fTexWidth_,          fV/fTexHeight_);
    mQuad_.lVertexArray[1].mUVs = Vector2D((fU+fWidth)/fTexWidth_, fV/fTexHeight_);
    mQuad_.lVertexArray[2].mUVs = Vector2D((fU+fWidth)/fTexWidth_, (fV+fHeight)/fTexHeight_);
    mQuad_.lVertexArray[3].mUVs = Vector2D(fU/fTexWidth_,          (fV+fHeight)/fTexHeight_);
}

SpriteImpl::SpriteImpl( s_ptr<GUIManager> pManager, const Color& mColor, const s_float& fWidth, const s_float& fHeight ) : pManager_(pManager)
{
    mQuad_.mMat = s_refptr<MaterialImpl>(new MaterialImpl(Color::WHITE));
    fTexWidth_ = 1.0f;
    fTexHeight_ = 1.0f;
    fWidth_ = fWidth;
    fHeight_ = fHeight;

    SetColor(mColor);
}

SpriteImpl::SpriteImpl( s_ptr<GUIManager> pManager, s_ptr<RenderTarget> pRenderTarget ) : pManager_(pManager)
{
    mQuad_.mMat = s_refptr<MaterialImpl>(new MaterialImpl(pRenderTarget));
    fTexWidth_ = s_float(pRenderTarget->GetRealWidth());
    fTexHeight_ = s_float(pRenderTarget->GetRealHeight());
    fWidth_ = s_float(pRenderTarget->GetWidth());
    fHeight_ = s_float(pRenderTarget->GetHeight());

    mQuad_.lVertexArray[0].mUVs = Vector2D(0.0f,               0.0f);
    mQuad_.lVertexArray[1].mUVs = Vector2D(fWidth_/fTexWidth_, 0.0f);
    mQuad_.lVertexArray[2].mUVs = Vector2D(fWidth_/fTexWidth_, fHeight_/fTexHeight_);
    mQuad_.lVertexArray[3].mUVs = Vector2D(0.0f,               fHeight_/fTexHeight_);
}

SpriteImpl::SpriteImpl( s_ptr<GUIManager> pManager, s_ptr<RenderTarget> pRenderTarget, const s_float& fWidth, const s_float& fHeight ) : pManager_(pManager)
{
    mQuad_.mMat = s_refptr<MaterialImpl>(new MaterialImpl(pRenderTarget));
    fTexWidth_ = s_float(pRenderTarget->GetRealWidth());
    fTexHeight_ = s_float(pRenderTarget->GetRealHeight());
    fWidth_ = fWidth;
    fHeight_ = fHeight;

    mQuad_.lVertexArray[0].mUVs = Vector2D(0.0f,              0.0f);
    mQuad_.lVertexArray[1].mUVs = Vector2D(fWidth/fTexWidth_, 0.0f);
    mQuad_.lVertexArray[2].mUVs = Vector2D(fWidth/fTexWidth_, fHeight/fTexHeight_);
    mQuad_.lVertexArray[3].mUVs = Vector2D(0.0f,              fHeight/fTexHeight_);
}

void SpriteImpl::Render( const s_float& fX, const s_float& fY ) const
{
    mQuad_.lVertexArray[0].mPosition = Vector2D(fX-fHotSpotX_,         fY-fHotSpotY_);
    mQuad_.lVertexArray[1].mPosition = Vector2D(fX-fHotSpotX_+fWidth_, fY-fHotSpotY_);
    mQuad_.lVertexArray[2].mPosition = Vector2D(fX-fHotSpotX_+fWidth_, fY-fHotSpotY_+fHeight_);
    mQuad_.lVertexArray[3].mPosition = Vector2D(fX-fHotSpotX_,         fY-fHotSpotY_+fHeight_);

    if (pManager_)
        pManager_->GetImpl()->RenderQuad(mQuad_);
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

        mQuad_.lVertexArray[0].mPosition = Vector2D(x1*cost - y1*sint + fX, x1*sint + y1*cost + fY);
        mQuad_.lVertexArray[1].mPosition = Vector2D(x2*cost - y1*sint + fX, x1*sint + y1*cost + fY);
        mQuad_.lVertexArray[2].mPosition = Vector2D(x2*cost - y1*sint + fX, x2*sint + y2*cost + fY);
        mQuad_.lVertexArray[3].mPosition = Vector2D(x1*cost - y1*sint + fX, x2*sint + y2*cost + fY);
    }
    else
    {
        mQuad_.lVertexArray[0].mPosition = Vector2D(x1 + fX, y1 + fY);
        mQuad_.lVertexArray[1].mPosition = Vector2D(x2 + fX, y1 + fY);
        mQuad_.lVertexArray[2].mPosition = Vector2D(x2 + fX, y2 + fY);
        mQuad_.lVertexArray[3].mPosition = Vector2D(x1 + fX, y2 + fY);
    }

    if (pManager_)
        pManager_->GetImpl()->RenderQuad(mQuad_);
    else
        RenderQuad(mQuad_);
}

void SpriteImpl::Render2V( const s_float& fX1, const s_float& fY1, const s_float& fX3, const s_float& fY3 )
{
    mQuad_.lVertexArray[0].mPosition = Vector2D(fX1, fY1);
    mQuad_.lVertexArray[1].mPosition = Vector2D(fX3, fY1);
    mQuad_.lVertexArray[2].mPosition = Vector2D(fX3, fY3);
    mQuad_.lVertexArray[3].mPosition = Vector2D(fX1, fY3);

    if (pManager_)
        pManager_->GetImpl()->RenderQuad(mQuad_);
    else
        RenderQuad(mQuad_);
}

void SpriteImpl::Render4V( const s_float& fX1, const s_float& fY1,
                       const s_float& fX2, const s_float& fY2,
                       const s_float& fX3, const s_float& fY3,
                       const s_float& fX4, const s_float& fY4 )
{
    mQuad_.lVertexArray[0].mPosition = Vector2D(fX1, fY1);
    mQuad_.lVertexArray[1].mPosition = Vector2D(fX2, fY2);
    mQuad_.lVertexArray[2].mPosition = Vector2D(fX3, fY3);
    mQuad_.lVertexArray[3].mPosition = Vector2D(fX4, fY4);

    if (pManager_)
        pManager_->GetImpl()->RenderQuad(mQuad_);
    else
        RenderQuad(mQuad_);
}

void SpriteImpl::RenderStatic() const
{
    if (pManager_)
        pManager_->GetImpl()->RenderQuad(mQuad_);
    else
        RenderQuad(mQuad_);
}

void SpriteImpl::SetQuad( const s_array<Vertex,4>& lVertexArray )
{
    mQuad_.lVertexArray = lVertexArray;
}

void SpriteImpl::SetColor( const Color& mColor, const s_uint& uiIndex )
{
    if (uiIndex.IsValid())
        mQuad_.lVertexArray[uiIndex].mColor = mColor;
    else
    {
        for (uint i = 0; i < 4; ++i)
            mQuad_.lVertexArray[i].mColor = mColor;
    }
}

void SpriteImpl::SetDesaturated( const s_bool& bDesaturated )
{
    if (bDesaturated != bDesaturated_)
    {
        bDesaturated_ = bDesaturated;
        s_ptr<Ogre::Pass> pPass = mQuad_.mMat.GetImpl()->GetOgreMaterial()->getTechnique(0)->getPass(0);

        if (bDesaturated)
            pPass->setFragmentProgram("GUI_Desaturation_PS");
        else
            pPass->setFragmentProgram("");
    }
}

void SpriteImpl::SetBlendMode( BlendMode mBlendMode )
{

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
        mQuad_.lVertexArray[0].mUVs = Vector2D(lTextureRect[0], lTextureRect[1]);
        mQuad_.lVertexArray[1].mUVs = Vector2D(lTextureRect[2], lTextureRect[1]);
        mQuad_.lVertexArray[2].mUVs = Vector2D(lTextureRect[2], lTextureRect[3]);
        mQuad_.lVertexArray[3].mUVs = Vector2D(lTextureRect[0], lTextureRect[3]);
    }
    else
    {
        s_float fWidth = mQuad_.mMat.GetWidth();
        s_float fHeight = mQuad_.mMat.GetHeight();

        mQuad_.lVertexArray[0].mUVs = Vector2D(lTextureRect[0]/fWidth, lTextureRect[1]/fHeight);
        mQuad_.lVertexArray[1].mUVs = Vector2D(lTextureRect[2]/fWidth, lTextureRect[1]/fHeight);
        mQuad_.lVertexArray[2].mUVs = Vector2D(lTextureRect[2]/fWidth, lTextureRect[3]/fHeight);
        mQuad_.lVertexArray[3].mUVs = Vector2D(lTextureRect[0]/fWidth, lTextureRect[3]/fHeight);
    }
}

void SpriteImpl::SetTextureRect( const s_float& fX1, const s_float& fY1, const s_float& fX3, const s_float& fY3, const s_bool& bNormalized )
{
    if (bNormalized)
    {
        mQuad_.lVertexArray[0].mUVs = Vector2D(fX1, fY1);
        mQuad_.lVertexArray[1].mUVs = Vector2D(fX3, fY1);
        mQuad_.lVertexArray[2].mUVs = Vector2D(fX3, fY3);
        mQuad_.lVertexArray[3].mUVs = Vector2D(fX1, fY3);
    }
    else
    {
        s_float fWidth = mQuad_.mMat.GetWidth();
        s_float fHeight = mQuad_.mMat.GetHeight();

        mQuad_.lVertexArray[0].mUVs = Vector2D(fX1/fWidth, fY1/fHeight);
        mQuad_.lVertexArray[1].mUVs = Vector2D(fX3/fWidth, fY1/fHeight);
        mQuad_.lVertexArray[2].mUVs = Vector2D(fX3/fWidth, fY3/fHeight);
        mQuad_.lVertexArray[3].mUVs = Vector2D(fX1/fWidth, fY3/fHeight);
    }
}

void SpriteImpl::SetTextureCoords( const s_array<s_float,8>& lTextureRect, const s_bool& bNormalized )
{
    if (bNormalized)
    {
        mQuad_.lVertexArray[0].mUVs = Vector2D(lTextureRect[0], lTextureRect[1]);
        mQuad_.lVertexArray[1].mUVs = Vector2D(lTextureRect[2], lTextureRect[3]);
        mQuad_.lVertexArray[2].mUVs = Vector2D(lTextureRect[4], lTextureRect[5]);
        mQuad_.lVertexArray[3].mUVs = Vector2D(lTextureRect[6], lTextureRect[7]);
    }
    else
    {
        s_float fWidth = mQuad_.mMat.GetWidth();
        s_float fHeight = mQuad_.mMat.GetHeight();

        mQuad_.lVertexArray[0].mUVs = Vector2D(lTextureRect[0]/fWidth, lTextureRect[1]/fHeight);
        mQuad_.lVertexArray[1].mUVs = Vector2D(lTextureRect[2]/fWidth, lTextureRect[3]/fHeight);
        mQuad_.lVertexArray[2].mUVs = Vector2D(lTextureRect[4]/fWidth, lTextureRect[5]/fHeight);
        mQuad_.lVertexArray[3].mUVs = Vector2D(lTextureRect[6]/fWidth, lTextureRect[7]/fHeight);
    }
}

void SpriteImpl::SetTextureCoords( const s_float& fX1, const s_float& fY1, const s_float& fX2, const s_float& fY2, const s_float& fX3, const s_float& fY3, const s_float& fX4, const s_float& fY4, const s_bool& bNormalized )
{
    if (bNormalized)
    {
        mQuad_.lVertexArray[0].mUVs = Vector2D(fX1, fY1);
        mQuad_.lVertexArray[1].mUVs = Vector2D(fX2, fY2);
        mQuad_.lVertexArray[2].mUVs = Vector2D(fX3, fY3);
        mQuad_.lVertexArray[3].mUVs = Vector2D(fX4, fY4);
    }
    else
    {
        s_float fWidth = mQuad_.mMat.GetWidth();
        s_float fHeight = mQuad_.mMat.GetHeight();

        mQuad_.lVertexArray[0].mUVs = Vector2D(fX1/fWidth, fY1/fHeight);
        mQuad_.lVertexArray[1].mUVs = Vector2D(fX2/fWidth, fY2/fHeight);
        mQuad_.lVertexArray[2].mUVs = Vector2D(fX3/fWidth, fY3/fHeight);
        mQuad_.lVertexArray[3].mUVs = Vector2D(fX4/fWidth, fY4/fHeight);
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
    return mQuad_.lVertexArray[0].mUVs.X(), mQuad_.lVertexArray[0].mUVs.Y(),
           mQuad_.lVertexArray[2].mUVs.X(), mQuad_.lVertexArray[2].mUVs.Y();
}

s_array<s_float,8> SpriteImpl::GetTextureCoords( const s_bool& bNormalized ) const
{
    if (bNormalized)
    {
        return mQuad_.lVertexArray[0].mUVs.X(), mQuad_.lVertexArray[0].mUVs.Y(),
               mQuad_.lVertexArray[1].mUVs.X(), mQuad_.lVertexArray[1].mUVs.Y(),
               mQuad_.lVertexArray[2].mUVs.X(), mQuad_.lVertexArray[2].mUVs.Y(),
               mQuad_.lVertexArray[3].mUVs.X(), mQuad_.lVertexArray[3].mUVs.Y();
    }
    else
    {
        s_float fWidth = mQuad_.mMat.GetWidth();
        s_float fHeight = mQuad_.mMat.GetHeight();

        return mQuad_.lVertexArray[0].mUVs.X()*fWidth, mQuad_.lVertexArray[0].mUVs.Y()*fHeight,
               mQuad_.lVertexArray[1].mUVs.X()*fWidth, mQuad_.lVertexArray[1].mUVs.Y()*fHeight,
               mQuad_.lVertexArray[2].mUVs.X()*fWidth, mQuad_.lVertexArray[2].mUVs.Y()*fHeight,
               mQuad_.lVertexArray[3].mUVs.X()*fWidth, mQuad_.lVertexArray[3].mUVs.Y()*fHeight;
    }
}

Color SpriteImpl::GetColor() const
{
    return mQuad_.lVertexArray[0].mColor;
}

BlendMode SpriteImpl::GetBlendMode() const
{
    return BLEND_NORMAL;
}

const GUI::Material& SpriteImpl::GetMaterial() const
{
    return mQuad_.mMat;
}

GUI::Material SpriteImpl::LoadTexture( const s_str& sFileName )
{
    return s_refptr<MaterialImpl>(new MaterialImpl(sFileName));
}
