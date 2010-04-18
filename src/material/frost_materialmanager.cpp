/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*         MaterialManager header         */
/*                                        */
/*                                        */

#include "material/frost_materialmanager.h"
#include "material/frost_material.h"
#include "gui/frost_rendertarget.h"

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
        Log("Closing "+CLASS_NAME+"...");
    }

    void MaterialManager::Initialize()
    {
        pDefault2D_ = CreateMaterial2D("Default2D", 255, 255, 255);
        pDefault3D_ = CreateMaterial3D("Default3D", 255, 255, 255);
        pDefault3D_->SetShaders("SimpleColor");
        pDefault3D_->SetSelfIllumination(Color(128, 128, 128));
    }

    s_refptr<Material> MaterialManager::CreateMaterial( s_ptr<Ogre::Material> pOgreMat )
    {
        s_refptr<Material> pMat(new Material(uiCounter_, Material::TYPE_UNKNOWN, pOgreMat));
        ++uiCounter_;

        return pMat;
    }

    s_refptr<Material> MaterialManager::CreateMaterial2D( s_ptr<Ogre::Material> pOgreMat )
    {
        s_refptr<Material> pMat(new Material(uiCounter_, Material::TYPE_UNKNOWN, pOgreMat));
        ++uiCounter_;

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

    s_refptr<Material> MaterialManager::CreateMaterial2D( const s_str& sName, const s_uchar& ucR, const s_uchar& ucG, const s_uchar& ucB )
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
            Ogre::ColourValue(ucR.Get()/255.0f, ucG.Get()/255.0f, ucB.Get()/255.0f)
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

        s_refptr<Material> pMat(new Material(uiCounter_, Material::TYPE_2D_PLAIN, pOgreMat));
        ++uiCounter_;

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

        s_refptr<Material> pMat(new Material(uiCounter_, Material::TYPE_2D, pOgreMat));
        ++uiCounter_;

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

        s_refptr<Material> pMat(new Material(uiCounter_, Material::TYPE_2D_RT, pOgreMat));
        ++uiCounter_;

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

        s_refptr<Material> pMat(new Material(uiCounter_, Material::TYPE_2D_RT, pOgreMat));
        ++uiCounter_;

        return pMat;
    }

    s_refptr<Material> MaterialManager::GetDefault2D()
    {
        return pDefault2D_;
    }

    s_refptr<Material> MaterialManager::CreateMaterial3D( const s_str& sName, const s_uchar& ucR, const s_uchar& ucG, const s_uchar& ucB, const s_uchar& ucA )
    {
        s_ptr<Ogre::Material> pOgreMat = (Ogre::Material*)Ogre::MaterialManager::getSingleton().create(
            sName.Get(), "Frost"
        ).get();

        s_refptr<Material> pMat(new Material(uiCounter_, Material::TYPE_3D_PLAIN, pOgreMat));
        ++uiCounter_;

        pMat->SetDiffuse(Color(ucA, ucR, ucG, ucB));

        return pMat;
    }

    s_refptr<Material> MaterialManager::CreateMaterial3D( const s_str& sName, const Color& mColor )
    {
        return CreateMaterial3D(sName, mColor.GetR(), mColor.GetG(), mColor.GetB(), mColor.GetA());
    }

    s_refptr<Material> MaterialManager::CreateMaterial3D( const s_uchar& ucR, const s_uchar& ucG, const s_uchar& ucB, const s_uchar& ucA )
    {
        return CreateMaterial3D("3D_"+uiCounter_, ucR, ucG, ucB, ucA);
    }

    s_refptr<Material> MaterialManager::CreateMaterial3D( const Color& mColor )
    {
        return CreateMaterial3D("3D_"+uiCounter_, mColor.GetR(), mColor.GetG(), mColor.GetB(), mColor.GetA());
    }

    s_refptr<Material> MaterialManager::CreateMaterial3D( const s_str& sName, const s_str& sFileName )
    {
        s_ptr<Ogre::Material> pOgreMat = (Ogre::Material*)Ogre::MaterialManager::getSingleton().create(
            sName.Get(), "Frost"
        ).get();

        Ogre::TextureManager::getSingleton().load(sFileName.Get(), "Frost");

        s_ptr<Ogre::Pass> pPass = pOgreMat->getTechnique(0)->getPass(0);
        pPass->setDiffuse(Ogre::ColourValue(1.0f,1.0f,1.0f));
        pPass->createTextureUnitState()->setTextureName(sFileName.Get());

        pOgreMat->load();

        s_refptr<Material> pMat(new Material(uiCounter_, Material::TYPE_3D, pOgreMat));
        ++uiCounter_;

        return pMat;
    }

    s_refptr<Material> MaterialManager::CreateMaterial3D( const s_str& sFileName )
    {
        return CreateMaterial3D(sFileName+"_3D_"+uiCounter_, sFileName);
    }

    s_refptr<Material> MaterialManager::CreateMaterial3D()
    {
        s_ptr<Ogre::Material> pOgreMat = (Ogre::Material*)Ogre::MaterialManager::getSingleton().create(
            ("3D_"+uiCounter_).Get(), "Frost"
        ).get();

        s_refptr<Material> pMat(new Material(uiCounter_, Material::TYPE_3D, pOgreMat));
        ++uiCounter_;

        return pMat;
    }

    s_refptr<Material> MaterialManager::GetDefault3D()
    {
        return pDefault3D_;
    }
}
