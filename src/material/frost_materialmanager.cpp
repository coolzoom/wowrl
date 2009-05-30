/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*         MaterialManager header         */
/*                                        */
/*                                        */

#include "material/frost_materialmanager.h"
#include "material/frost_material.h"
#include "gui/frost_spritemanager.h"

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
    const s_str MaterialManager::CLASS_NAME = "MaterialManager";

    MaterialManager::MaterialManager()
    {
    }

    MaterialManager::~MaterialManager()
    {
    }

    void MaterialManager::Initialize()
    {
        pDefault2D_ = CreateMaterial2D("Default2D", 255, 255, 255);
        pDefault3D_ = CreateMaterial3D("Default3D", 255, 255, 255);
    }

    s_refptr<Material> MaterialManager::CreateMaterial( s_ptr<Ogre::Material> pOgreMat )
    {
        s_refptr<Material> pMat(new Material(uiCounter_, MATERIAL_UNKNOWN, pOgreMat));
        uiCounter_++;

        return pMat;
    }

    s_refptr<Material> MaterialManager::CreateMaterial2D(s_ptr<Ogre::Material> pOgreMat)
    {
        s_refptr<Material> pMat(new Material(uiCounter_, MATERIAL_UNKNOWN, pOgreMat));
        uiCounter_++;

        if (pOgreMat->getTechnique(0)->getNumPasses() == 1)
        {
            /*pOgreMat->getTechnique(0)->createPass();
            pOgreMat->getTechnique(0)->movePass(0, 1);*/

            s_ptr<Ogre::Pass> pPass = pOgreMat->getTechnique(0)->getPass(0);
            /*pPass->setSeparateSceneBlending(
                Ogre::SBF_ZERO, Ogre::SBF_DEST_ALPHA, // color
                Ogre::SBF_ZERO, Ogre::SBF_ONE         // alpha
            );

            pPass = pOgreMat->getTechnique(0)->getPass(1);*/
            pPass->setDiffuse(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
            pPass->setAlphaRejectFunction(Ogre::CMPF_ALWAYS_PASS);
            pPass->setSeparateSceneBlending(
                Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA, // color
                Ogre::SBF_ONE, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA           // alpha
            );

            pOgreMat->load();
            pOgreMat->setLightingEnabled(false);
            pOgreMat->setDepthCheckEnabled(false);
            pOgreMat->setCullingMode(Ogre::CULL_NONE);

            //pMat->SetDefaultPass(1);
        }

        return pMat;
    }

    s_refptr<Material> MaterialManager::CreateMaterial2D( const s_str& sName, const s_uint& uiR, const s_uint& uiG, const s_uint& uiB )
    {
        s_ptr<Ogre::Material> pOgreMat = (Ogre::Material*)Ogre::MaterialManager::getSingleton().create(
            sName.Get(), "Frost"
        ).get();

        s_ptr<Ogre::Pass> pPass = pOgreMat->getTechnique(0)->getPass(0);
        /*pPass->setSeparateSceneBlending(
            Ogre::SBF_ZERO, Ogre::SBF_DEST_ALPHA, // color
            Ogre::SBF_ZERO, Ogre::SBF_ONE         // alpha
        );

        pPass = pOgreMat->getTechnique(0)->createPass();*/
        pPass->setDiffuse(
            Ogre::ColourValue(uiR.Get()/255.0f, uiG.Get()/255.0f, uiB.Get()/255.0f)
        );
        pPass->setAlphaRejectFunction(Ogre::CMPF_ALWAYS_PASS);
        pPass->setCullingMode(Ogre::CULL_NONE);
        pPass->setSeparateSceneBlending(
            Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA, // color
            Ogre::SBF_ONE, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA           // alpha
        );

        pOgreMat->load();
        pOgreMat->setLightingEnabled(false);
        pOgreMat->setDepthCheckEnabled(false);
        pOgreMat->setCullingMode(Ogre::CULL_NONE);

        s_refptr<Material> pMat(new Material(uiCounter_, MATERIAL_2D_PLAIN, pOgreMat));
        uiCounter_++;

        return pMat;
    }

    s_refptr<Material> MaterialManager::CreateMaterial2D( const s_str& sName, const Color& mColor )
    {
        return this->CreateMaterial2D(sName, mColor.GetR(), mColor.GetG(), mColor.GetB());
    }

    s_refptr<Material> MaterialManager::CreateMaterial2D( const s_str& sFileName )
    {
        s_ptr<Ogre::Material> pOgreMat = (Ogre::Material*)Ogre::MaterialManager::getSingleton().create(
            (sFileName+"_2D_"+uiCounter_).Get(), "Frost"
        ).get();

        Ogre::TextureManager::getSingleton().load(sFileName.Get(), "Frost");

        s_ptr<Ogre::Pass> pPass = pOgreMat->getTechnique(0)->getPass(0);
        /*pPass->setSeparateSceneBlending(
            Ogre::SBF_ZERO, Ogre::SBF_DEST_ALPHA, // color
            Ogre::SBF_ZERO, Ogre::SBF_ONE         // alpha
        );

        pPass = pOgreMat->getTechnique(0)->createPass();*/

        pPass->setDiffuse(Ogre::ColourValue(1.0f,1.0f,1.0f));
        pPass->createTextureUnitState()->setTextureName(sFileName.Get());
        pPass->setTextureFiltering(Ogre::TFO_NONE);
        pPass->setSeparateSceneBlending(
            Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA, // color
            Ogre::SBF_ONE, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA           // alpha
        );

        pOgreMat->load();
        pOgreMat->setLightingEnabled(false);
        pOgreMat->setDepthCheckEnabled(false);
        pOgreMat->setCullingMode(Ogre::CULL_NONE);

        s_refptr<Material> pMat(new Material(uiCounter_, MATERIAL_2D, pOgreMat));
        uiCounter_++;

        return pMat;
    }

    s_refptr<Material> MaterialManager::CreateMaterial2DFromRT( const s_str& sRenderTargetName )
    {
        s_ptr<Ogre::Material> pOgreMat = (Ogre::Material*)Ogre::MaterialManager::getSingleton().create(
            (sRenderTargetName+"_2D_"+uiCounter_).Get(), "Frost"
        ).get();

        s_ptr<Ogre::Pass> pPass = pOgreMat->getTechnique(0)->getPass(0);
        pPass->setDiffuse(Ogre::ColourValue(1.0f,1.0f,1.0f));
        pPass->createTextureUnitState()->setTextureName(sRenderTargetName.Get());
        pPass->setTextureFiltering(Ogre::TFO_NONE);
        pPass->setSeparateSceneBlending(
            Ogre::SBF_ONE, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA, // color
            Ogre::SBF_ONE, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA  // alpha
        );

        pOgreMat->load();
        pOgreMat->setLightingEnabled(false);
        pOgreMat->setDepthCheckEnabled(false);
        pOgreMat->setCullingMode(Ogre::CULL_NONE);

        s_refptr<Material> pMat(new Material(uiCounter_, MATERIAL_2D_RT, pOgreMat));
        uiCounter_++;

        return pMat;
    }

    s_refptr<Material> MaterialManager::CreateMaterial2DFromRT( s_ptr<RenderTarget> pRenderTarget )
    {
        s_str sRenderTargetName = pRenderTarget->GetName();
        s_ptr<Ogre::Material> pOgreMat = (Ogre::Material*)Ogre::MaterialManager::getSingleton().create(
            (sRenderTargetName+"_2D_"+uiCounter_).Get(), "Frost"
        ).get();

        s_ptr<Ogre::Pass> pPass = pOgreMat->getTechnique(0)->getPass(0);
        pPass->setDiffuse(Ogre::ColourValue(1.0f,1.0f,1.0f));
        pPass->createTextureUnitState()->setTextureName(sRenderTargetName.Get());
        pPass->setTextureFiltering(Ogre::TFO_NONE);
        pPass->setSeparateSceneBlending(
            Ogre::SBF_ONE, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA, // color
            Ogre::SBF_ONE, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA  // alpha
        );

        pOgreMat->load();
        pOgreMat->setLightingEnabled(false);
        pOgreMat->setDepthCheckEnabled(false);
        pOgreMat->setCullingMode(Ogre::CULL_NONE);

        s_refptr<Material> pMat(new Material(uiCounter_, MATERIAL_2D_RT, pOgreMat));
        uiCounter_++;

        return pMat;
    }

    s_refptr<Material> MaterialManager::GetDefault2D()
    {
        return pDefault2D_;
    }

    s_refptr<Material> MaterialManager::CreateMaterial3D( const s_str& sName, const s_uint& uiR, const s_uint& uiG, const s_uint& uiB )
    {
        s_ptr<Ogre::Material> pOgreMat = (Ogre::Material*)Ogre::MaterialManager::getSingleton().create(
            sName.Get(), "Frost"
        ).get();

        pOgreMat->getTechnique(0)->getPass(0)->setDiffuse(
            Ogre::ColourValue(uiR.Get()/255.0f, uiG.Get()/255.0f, uiB.Get()/255.0f)
        );

        s_refptr<Material> pMat(new Material(uiCounter_, MATERIAL_3D_PLAIN, pOgreMat));
        uiCounter_++;

        return pMat;
    }

    s_refptr<Material> MaterialManager::CreateMaterial3D( const s_str& sFileName )
    {
        s_ptr<Ogre::Material> pOgreMat = (Ogre::Material*)Ogre::MaterialManager::getSingleton().create(
            (sFileName+"_3D_"+uiCounter_).Get(), "Frost"
        ).get();

        Ogre::TextureManager::getSingleton().load(sFileName.Get(), "Frost");

        s_ptr<Ogre::Pass> pPass = pOgreMat->getTechnique(0)->getPass(0);
        pPass->setDiffuse(Ogre::ColourValue(1.0f,1.0f,1.0f));
        pPass->createTextureUnitState()->setTextureName(sFileName.Get());

        pOgreMat->load();

        s_refptr<Material> pMat(new Material(uiCounter_, MATERIAL_3D, pOgreMat));
        uiCounter_++;

        return pMat;
    }

    s_refptr<Material> MaterialManager::GetDefault3D()
    {
        return pDefault3D_;
    }
}
