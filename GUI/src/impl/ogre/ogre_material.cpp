/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Material source            */
/*                                        */
/*                                        */

#include "impl/ogre/ogre_material.h"
#include "frost_rendertarget.h"
#include "impl/ogre/ogre_rendertarget.h"

#include <frost_utils_file.h>
#include <frost_utils_color.h>

#include <OgreMaterialManager.h>
#include <OgreTextureManager.h>
#include <OgreMaterial.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgreRenderTarget.h>

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

MaterialImpl::MaterialImpl() : fWidth_(1.0f), fHeight_(1.0f)
{
}

MaterialImpl::MaterialImpl( s_ptr<Ogre::Material> pMat )
{
    pMat_ = pMat;
    GetDimensions_();
}

MaterialImpl::MaterialImpl( s_ptr<Ogre::Material> pMat, const s_float& fWidth, const s_float& fHeight )
{
    pMat_ = pMat;
    fWidth_ = fWidth;
    fHeight_ = fHeight;
}

MaterialImpl::MaterialImpl( const s_str& sFileName )
{
    pMat_ = (Ogre::Material*)Ogre::MaterialManager::getSingleton().create(
        (sFileName+"_2D_"+GetNewID_()).Get(), "Frost"
    ).get();

    if (File::Exists(sFileName))
    {
        Ogre::TextureManager::getSingleton().load(sFileName.Get(), "Frost");

        s_ptr<Ogre::Pass> pPass = pMat_->getTechnique(0)->getPass(0);

        pPass->setDiffuse(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
        pPass->createTextureUnitState()->setTextureName(sFileName.Get());
        pPass->setTextureFiltering(Ogre::TFO_NONE);
        pPass->setSeparateSceneBlending(
            Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA, // color
            Ogre::SBF_ONE, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA           // alpha
        );
    }
    else
    {
        Error("MaterialImpl", "Couldn't load texture : \""+sFileName+"\".");
    }

    pMat_->load();
    pMat_->setLightingEnabled(false);
    pMat_->setDepthCheckEnabled(false);
    pMat_->setCullingMode(Ogre::CULL_NONE);

    GetDimensions_();
}

MaterialImpl::MaterialImpl( const Color& mColor ) : fWidth_(1.0f), fHeight_(1.0f)
{
    pMat_ = (Ogre::Material*)Ogre::MaterialManager::getSingleton().create(
        ("2D_"+GetNewID_()).Get(), "Frost"
    ).get();

    s_ptr<Ogre::Pass> pPass = pMat_->getTechnique(0)->getPass(0);
    pPass->setDiffuse(
        Ogre::ColourValue(mColor.GetR().Get(), mColor.GetG().Get(), mColor.GetB().Get())
    );
    pPass->setAlphaRejectFunction(Ogre::CMPF_ALWAYS_PASS);
    pPass->setCullingMode(Ogre::CULL_NONE);
    pPass->setSeparateSceneBlending(
        Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA, // color
        Ogre::SBF_ONE, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA           // alpha
    );

    pMat_->load();
    pMat_->setLightingEnabled(false);
    pMat_->setDepthCheckEnabled(false);
    pMat_->setCullingMode(Ogre::CULL_NONE);
}

MaterialImpl::MaterialImpl( s_ptr<RenderTarget> pRenderTarget )
{
    s_str sRenderTargetName = pRenderTarget->GetImpl()->GetName();
    pMat_ = (Ogre::Material*)Ogre::MaterialManager::getSingleton().create(
        (sRenderTargetName+"_2D_"+GetNewID_()).Get(), "Frost"
    ).get();

    s_ptr<Ogre::Pass> pPass = pMat_->getTechnique(0)->getPass(0);
    pPass->setDiffuse(Ogre::ColourValue(1.0f,1.0f,1.0f));
    pPass->createTextureUnitState()->setTextureName(sRenderTargetName.Get());
    pPass->setTextureFiltering(Ogre::TFO_NONE);
    pPass->setSeparateSceneBlending(
        Ogre::SBF_ONE, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA, // color
        Ogre::SBF_ONE, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA  // alpha
    );

    pMat_->load();
    pMat_->setLightingEnabled(false);
    pMat_->setDepthCheckEnabled(false);
    pMat_->setCullingMode(Ogre::CULL_NONE);

    fWidth_ = s_float(pRenderTarget->GetRealWidth());
    fHeight_ = s_float(pRenderTarget->GetRealHeight());
}

MaterialImpl::~MaterialImpl()
{
    if (pMat_)
        Ogre::MaterialManager::getSingleton().remove(pMat_->getHandle());
}

s_float MaterialImpl::GetWidth() const
{
    return fWidth_;
}

s_float MaterialImpl::GetHeight() const
{
    return fHeight_;
}

s_ptr<Ogre::Material> MaterialImpl::GetOgreMaterial() const
{
    return pMat_;
}

s_uint MaterialImpl::GetNewID_()
{
    static s_uint uiID;
    return uiID++;
}

void MaterialImpl::GetDimensions_()
{
    s_ptr<Ogre::Technique> pTech = pMat_->getTechnique(0);
    if (pTech)
    {
        for (uint i = 0; i < pTech->getNumPasses(); ++i)
        {
            s_ptr<Ogre::Pass> pPass = pTech->getPass(i);
            if (pPass)
            {
                if (pPass->getNumTextureUnitStates() != 0)
                {
                    s_ptr<Ogre::TextureUnitState> pTUS = pPass->getTextureUnitState(0);
                    if (pTUS)
                    {
                        std::pair<size_t, size_t> mDim = pTUS->getTextureDimensions();
                        fWidth_ = mDim.first;
                        fHeight_ = mDim.second;
                    }
                    break;
                }
            }
        }
    }
}
