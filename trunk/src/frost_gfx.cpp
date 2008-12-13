/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GFX source                */
/*                                        */
/*                                        */


#include "frost_gfx.h"
#include "frost_material.h"

#include <OgreMaterialManager.h>
#include <OgreTextureManager.h>
#include <OgreMaterial.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreRenderTarget.h>
#include <OgreRenderTexture.h>

using namespace std;

namespace Frost
{
    const s_str RenderTarget::CLASS_NAME = "RenderTarget";

    Quad::Quad()
    {
    }

    RenderTarget GFX::CreateRenderTarget( const s_str& sTargetName, const s_uint& uiWidth, const s_uint& uiHeight )
    {
        RenderTarget mRTarget;
        mRTarget.pResourcePtr = Ogre::TextureManager::getSingleton().createManual(
            sTargetName.Get(), "Frost", Ogre::TEX_TYPE_2D,
            uiWidth.GetNearestPowerOfTwo().Get(), uiHeight.GetNearestPowerOfTwo().Get(),
            0, Ogre::PF_A8B8G8R8, Ogre::TU_RENDERTARGET
        );

        mRTarget.pOgreRenderTarget = mRTarget.pResourcePtr->getBuffer()->getRenderTarget();
        mRTarget.pOgreRenderTarget->addViewport(0);
        mRTarget.uiWidth = uiWidth;
        mRTarget.uiHeight = uiHeight;

        return mRTarget;
    }

    void GFX::DeleteRenderTarget( const s_str& sTargetName )
    {
        Ogre::TextureManager::getSingleton().remove(
            sTargetName.Get()
        );
    }

    void GFX::DeleteRenderTarget( RenderTarget& mTarget )
    {
        Ogre::TextureManager::getSingleton().remove(
            mTarget.pResourcePtr->getHandle()
        );
        mTarget.pOgreRenderTarget = NULL;
    }

    void GFX::FrostToOgrePosition( s_float &fX, s_float &fY, s_float &fZ )
    {
        s_float fTemp = fX;
        fX = fY;
        fY = fZ;
        fZ = fTemp;
    }

    void GFX::OgreToFrostPosition( s_float &fX, s_float &fY, s_float &fZ )
    {
        s_float fTemp = fX;
        fX = fZ;
        fZ = fY;
        fY = fTemp;
    }

    Ogre::ColourValue GFX::FrostToOgreColor( const Color& mColor )
    {
        Ogre::ColourValue mOgreColor;
        mOgreColor.setAsARGB(mColor.Get().Get());
        return mOgreColor;
    }
}
