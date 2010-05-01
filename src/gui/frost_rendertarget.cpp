/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           RenderTarget source          */
/*                                        */
/*                                        */

#include "gui/frost_rendertarget.h"

#include "material/frost_materialmanager.h"
#include "material/frost_material.h"

#include <OgreTextureManager.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreRenderTarget.h>
#include <OgreRenderTargetListener.h>
#include <OgreRenderTexture.h>
#include <OgreRenderSystem.h>

using namespace std;

namespace Frost
{
    const s_str RenderTarget::CLASS_NAME = "RenderTarget";

    RenderTarget::RenderTarget( const s_uint& uiID, const s_uint& uiWidth, const s_uint& uiHeight, const PixelType& mType, const Usage& mUsage )
    {
        if (!uiWidth.IsValid() || !uiHeight.IsValid())
        {
            Error(CLASS_NAME,
                "Can't create a RenderTarget with invalid dimensions : "+uiWidth+" x "+uiHeight+"."
            );
            return;
        }

        uiID_ = uiID;
        sName_ = "_AutoNamedTarget:"+uiID_;
        uiWidth_ = uiWidth;
        uiHeight_ = uiHeight;
        uiRealWidth_ = uiWidth.GetNearestPowerOfTwo();
        uiRealHeight_ = uiHeight.GetNearestPowerOfTwo();
        mUsage_ = mUsage;

        switch (mType)
        {
            case PIXEL_ARGB : mPixelFormat_ = Ogre::PF_A8R8G8B8; break;
            case PIXEL_XRGB : mPixelFormat_ = Ogre::PF_X8R8G8B8; break;
            case PIXEL_FLOAT : mPixelFormat_ = Ogre::PF_FLOAT32_R; break;
            default : mPixelFormat_ = Ogre::PF_A8R8G8B8;
        }

        Ogre::TexturePtr pTexture = Ogre::TextureManager::getSingleton().createManual(
            sName_.Get(), "Frost", Ogre::TEX_TYPE_2D,
            uiRealWidth_.Get(), uiRealHeight_.Get(),
            0, mPixelFormat_, Ogre::TU_RENDERTARGET
        );
        hResourceHandle_ = pTexture->getHandle();

        pOgreRenderTarget_ = pTexture->getBuffer()->getRenderTarget();

        // Viewport for clearing or 2D drawing
        pOgreRenderTarget_->addViewport(nullptr, 0);

        if (mUsage_ == USAGE_2D)
        {
            pOgreRenderTarget_->setAutoUpdated(false);
        }
    }

    RenderTarget::RenderTarget( const s_uint& uiID, const s_str& sName, const s_uint& uiWidth, const s_uint& uiHeight, const PixelType& mType, const Usage& mUsage )
    {
        if (!uiWidth.IsValid() || !uiHeight.IsValid())
        {
            Error(CLASS_NAME,
                "Can't create a RenderTarget with invalid dimensions : "+uiWidth+" x "+uiHeight+"."
            );
            return;
        }

        uiID_ = uiID;
        sName_ = sName;
        uiWidth_ = uiWidth;
        uiHeight_ = uiHeight;
        uiRealWidth_ = uiWidth.GetNearestPowerOfTwo();
        uiRealHeight_ = uiHeight.GetNearestPowerOfTwo();
        mUsage_ = mUsage;

        switch (mType)
        {
            case PIXEL_ARGB : mPixelFormat_ = Ogre::PF_A8R8G8B8; break;
            case PIXEL_XRGB : mPixelFormat_ = Ogre::PF_X8R8G8B8; break;
            case PIXEL_FLOAT : mPixelFormat_ = Ogre::PF_FLOAT32_R; break;
            default : mPixelFormat_ = Ogre::PF_A8R8G8B8;
        }

        Ogre::TexturePtr pTexture = Ogre::TextureManager::getSingleton().createManual(
            sName_.Get(), "Frost", Ogre::TEX_TYPE_2D,
            uiRealWidth_.Get(), uiRealHeight_.Get(),
            0, mPixelFormat_, Ogre::TU_RENDERTARGET
        );
        hResourceHandle_ = pTexture->getHandle();

        pOgreRenderTarget_ = pTexture->getBuffer()->getRenderTarget();

        // Viewport for clearing or 2D drawing
        pOgreRenderTarget_->addViewport(nullptr, 0);

        if (mUsage_ == USAGE_2D)
        {
            pOgreRenderTarget_->setAutoUpdated(false);
        }
    }

    RenderTarget::~RenderTarget()
    {
        Ogre::TextureManager::getSingleton().remove(hResourceHandle_);

        s_ctnr< s_ptr<Ogre::RenderTargetListener> >::iterator iter;
        foreach (iter, lListenerList_)
        {
            iter->Delete();
        }
    }

    void RenderTarget::AddListener( s_ptr<Ogre::RenderTargetListener> pListener )
    {
        pOgreRenderTarget_->addListener(pListener.Get());
        lListenerList_.PushBack(pListener);
    }

    void RenderTarget::Clear( const Color& mColor )
    {
        Engine::GetSingleton()->GetRenderSystem()->_setViewport(
            pOgreRenderTarget_->getViewport(0)
        );
        Ogre::ColourValue mCV;
        mCV.setAsABGR(mColor.GetPacked().Get());
        Engine::GetSingleton()->GetRenderSystem()->clearFrameBuffer(
            Ogre::FBT_COLOUR | Ogre::FBT_DEPTH, mCV
        );
    }

    const s_uint& RenderTarget::GetWidth() const
    {
        return uiWidth_;
    }

    const s_uint& RenderTarget::GetHeight() const
    {
        return uiHeight_;
    }

    s_bool RenderTarget::SetDimensions( const s_uint& uiWidth, const s_uint& uiHeight )
    {
        if (!uiWidth.IsValid() || !uiHeight.IsValid())
        {
            Error(CLASS_NAME,
                "Can't create a RenderTarget with invalid dimensions : "+uiWidth+" x "+uiHeight+"."
            );
            return;
        }

        s_uint uiNewWidth = uiWidth.GetNearestPowerOfTwo();
        s_uint uiNewHeight = uiHeight.GetNearestPowerOfTwo();
        if ((uiRealWidth_ < uiNewWidth) ||
            (uiRealHeight_ < uiNewHeight))
        {
            // The current render target is too small...
            // Let's delete it and create a bigger one
            Ogre::TextureManager::getSingleton().remove(
                hResourceHandle_
            );
            uiRealWidth_ = uiNewWidth;
            uiRealHeight_ = uiNewHeight;
            Ogre::TexturePtr pTexture = Ogre::TextureManager::getSingleton().createManual(
                sName_.Get(), "Frost", Ogre::TEX_TYPE_2D,
                uiRealWidth_.Get(), uiRealHeight_.Get(),
                0, mPixelFormat_, Ogre::TU_RENDERTARGET
            );
            hResourceHandle_ = pTexture->getHandle();

            pOgreRenderTarget_ = pTexture->getBuffer()->getRenderTarget();

            // Viewport for clearing or 2D drawing
            pOgreRenderTarget_->addViewport(nullptr, 0);

            if (mUsage_ == USAGE_2D)
            {
                pOgreRenderTarget_->setAutoUpdated(false);
            }

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

    const s_uint& RenderTarget::GetRealWidth() const
    {
        return uiRealWidth_;
    }

    const s_uint& RenderTarget::GetRealHeight() const
    {
        return uiRealHeight_;
    }

    s_ptr<Ogre::RenderTarget> RenderTarget::GetOgreRenderTarget()
    {
        return pOgreRenderTarget_;
    }

    const s_uint& RenderTarget::GetID()
    {
        return uiID_;
    }

    const s_str& RenderTarget::GetName()
    {
        return sName_;
    }
}
