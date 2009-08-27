/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            GUI Structs source          */
/*                                        */
/*                                        */

#include "gui/frost_guistructs.h"

#include "material/frost_materialmanager.h"
#include "material/frost_material.h"

#include <OgreTextureManager.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreRenderTarget.h>
#include <OgreRenderTargetListener.h>
#include <OgreRenderTexture.h>

using namespace std;

namespace Frost
{
    const s_str RenderTarget::CLASS_NAME = "RenderTarget";

    Quad::Quad()
    {
        pMat = MaterialManager::GetSingleton()->GetDefault2D();
    }

    RenderTarget::RenderTarget( const s_uint& uiID, const s_uint& uiWidth, const s_uint& uiHeight, const PixelType& mType, const Usage& mUsage )
    {
        uiID_ = uiID;
        sName_ = "_AutoNamedTarget:"+uiID_;
        uiWidth_ = uiWidth;
        uiHeight_ = uiHeight;
        uiRealWidth_ = uiWidth.GetNearestPowerOfTwo();
        uiRealHeight_ = uiHeight.GetNearestPowerOfTwo();

        Ogre::PixelFormat mPF;
        switch (mType)
        {
            case PIXEL_ARGB : mPF = Ogre::PF_A8R8G8B8; break;
            case PIXEL_XRGB : mPF = Ogre::PF_X8R8G8B8; break;
            case PIXEL_FLOAT : mPF = Ogre::PF_FLOAT32_R; break;
            default : mPF = Ogre::PF_A8R8G8B8;
        }

        Ogre::TexturePtr pTexture = Ogre::TextureManager::getSingleton().createManual(
            sName_.Get(), "Frost", Ogre::TEX_TYPE_2D,
            uiRealWidth_.Get(), uiRealHeight_.Get(),
            0, mPF, Ogre::TU_RENDERTARGET
        );
        hResourceHandle_ = pTexture->getHandle();

        pOgreRenderTarget_ = pTexture->getBuffer()->getRenderTarget();

        if (mUsage == USAGE_2D)
        {
            pOgreRenderTarget_->addViewport(0);
            pOgreRenderTarget_->setAutoUpdated(false);
        }
    }

    RenderTarget::RenderTarget( const s_uint& uiID, const s_str& sName, const s_uint& uiWidth, const s_uint& uiHeight, const PixelType& mType, const Usage& mUsage )
    {
        uiID_ = uiID;
        sName_ = sName;
        uiWidth_ = uiWidth;
        uiHeight_ = uiHeight;
        uiRealWidth_ = uiWidth.GetNearestPowerOfTwo();
        uiRealHeight_ = uiHeight.GetNearestPowerOfTwo();

        Ogre::PixelFormat mPF;
        switch (mType)
        {
            case PIXEL_ARGB : mPF = Ogre::PF_A8R8G8B8; break;
            case PIXEL_XRGB : mPF = Ogre::PF_X8R8G8B8; break;
            case PIXEL_FLOAT : mPF = Ogre::PF_FLOAT32_R; break;
            default : mPF = Ogre::PF_A8R8G8B8;
        }

        Ogre::TexturePtr pTexture = Ogre::TextureManager::getSingleton().createManual(
            sName_.Get(), "Frost", Ogre::TEX_TYPE_2D,
            uiRealWidth_.Get(), uiRealHeight_.Get(),
            0, mPF, Ogre::TU_RENDERTARGET
        );
        hResourceHandle_ = pTexture->getHandle();

        pOgreRenderTarget_ = pTexture->getBuffer()->getRenderTarget();

        if (mUsage == USAGE_2D)
        {
            pOgreRenderTarget_->addViewport(0);
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
                ("_AutoNamedTarget:"+uiID_).Get(), "Frost", Ogre::TEX_TYPE_2D,
                uiRealWidth_.Get(), uiRealHeight_.Get(),
                0, Ogre::PF_A8B8G8R8, Ogre::TU_RENDERTARGET
            );
            hResourceHandle_ = pTexture->getHandle();

            pOgreRenderTarget_ = pTexture->getBuffer()->getRenderTarget();
            pOgreRenderTarget_->addViewport(0);
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
