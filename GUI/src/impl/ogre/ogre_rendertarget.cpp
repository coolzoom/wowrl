/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           RenderTarget source          */
/*                                        */
/*                                        */

#include "frost_rendertarget.h"
#include "impl/ogre/ogre_rendertarget.h"

#include <frost_utils_color.h>

#include <OgreTextureManager.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreRenderTarget.h>
#include <OgreRenderTargetListener.h>
#include <OgreRenderTexture.h>
#include <OgreRenderSystem.h>
#include <OgreRoot.h>

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str RenderTargetImpl::CLASS_NAME = "GUI::RenderTargetImpl";

RenderTargetImpl::RenderTargetImpl( const s_uint& uiWidth, const s_uint& uiHeight )
{
    if (!uiWidth.IsValid() || !uiHeight.IsValid())
    {
        throw Exception(CLASS_NAME,
            "Can't create a RenderTarget with invalid dimensions : "+uiWidth+" x "+uiHeight+"."
        );
    }

    static s_uint uiID;

    sName_ = "_AutoNamedGUITarget:"+uiID;
    ++uiID;

    uiWidth_ = uiWidth;
    uiHeight_ = uiHeight;
    uiRealWidth_ = uiWidth.GetNearestPowerOfTwo();
    uiRealHeight_ = uiHeight.GetNearestPowerOfTwo();

    Ogre::TexturePtr pTexture = Ogre::TextureManager::getSingleton().createManual(
        sName_.Get(), "Frost", Ogre::TEX_TYPE_2D,
        uiRealWidth_.Get(), uiRealHeight_.Get(),
        0, Ogre::PF_A8R8G8B8, Ogre::TU_RENDERTARGET
    );
    hResourceHandle_ = pTexture->getHandle();

    pOgreRenderTarget_ = pTexture->getBuffer()->getRenderTarget();

    // Viewport for clearing or 2D drawing
    pOgreRenderTarget_->addViewport(nullptr, 0);
    pOgreRenderTarget_->setAutoUpdated(false);
}

RenderTargetImpl::~RenderTargetImpl()
{
    Ogre::TextureManager::getSingleton().remove(hResourceHandle_);
}

void RenderTargetImpl::Clear( const Color& mColor )
{
    Ogre::Root::getSingleton().getRenderSystem()->_setViewport(
        pOgreRenderTarget_->getViewport(0)
    );
    Ogre::ColourValue mCV;
    mCV.setAsABGR(mColor.GetPacked().Get());
    Ogre::Root::getSingleton().getRenderSystem()->clearFrameBuffer(
        Ogre::FBT_COLOUR | Ogre::FBT_DEPTH, mCV
    );
}

const s_uint& RenderTargetImpl::GetWidth() const
{
    return uiWidth_;
}

const s_uint& RenderTargetImpl::GetHeight() const
{
    return uiHeight_;
}

s_bool RenderTargetImpl::SetDimensions( const s_uint& uiWidth, const s_uint& uiHeight )
{
    if (!uiWidth.IsValid() || !uiHeight.IsValid())
        {
            Error(CLASS_NAME,
                "Can't create a RenderTarget with invalid dimensions : "+uiWidth+" x "+uiHeight+"."
            );
            return false;
        }

        s_uint uiNewWidth = uiWidth.GetNearestPowerOfTwo();
        s_uint uiNewHeight = uiHeight.GetNearestPowerOfTwo();
        if ((uiRealWidth_ < uiNewWidth) ||
            (uiRealHeight_ < uiNewHeight))
        {
            // The current render target is too small...
            // Let's delete it and create a bigger one
            Ogre::TextureManager::getSingleton().remove(hResourceHandle_);
            uiRealWidth_ = uiNewWidth;
            uiRealHeight_ = uiNewHeight;
            Ogre::TexturePtr pTexture = Ogre::TextureManager::getSingleton().createManual(
                sName_.Get(), "Frost", Ogre::TEX_TYPE_2D,
                uiRealWidth_.Get(), uiRealHeight_.Get(),
                0, Ogre::PF_A8R8G8B8, Ogre::TU_RENDERTARGET
            );
            hResourceHandle_ = pTexture->getHandle();

            pOgreRenderTarget_ = pTexture->getBuffer()->getRenderTarget();

            // Viewport for clearing or 2D drawing
            pOgreRenderTarget_->addViewport(nullptr, 0);
            pOgreRenderTarget_->setAutoUpdated(false);

            return true;
        }
        else
        {
            // There was already one, with a correct size
            // Just change its virtual size
            uiWidth_ = uiWidth;
            uiHeight_ = uiHeight;
            return false;
        }
}

const s_uint& RenderTargetImpl::GetRealWidth() const
{
    return uiRealWidth_;
}

const s_uint& RenderTargetImpl::GetRealHeight() const
{
    return uiRealHeight_;
}

const s_str& RenderTargetImpl::GetName() const
{
    return sName_;
}

s_ptr<Ogre::RenderTarget> RenderTargetImpl::GetOgreRenderTarget() const
{
    return pOgreRenderTarget_;
}

void RenderTargetImpl::SaveContentToFile( const s_str& sFileName ) const
{
    if (pOgreRenderTarget_)
        pOgreRenderTarget_->writeContentsToFile(sFileName.Get());
}
