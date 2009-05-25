/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Material header             */
/*                                        */
/*                                        */

#include "frost_material.h"

#include <OgreMaterialManager.h>
#include <OgreMaterial.h>
#include <OgreTechnique.h>
#include <OgrePass.h>

#include "frost_decal.h"

using namespace std;

namespace Frost
{
    const s_str Material::CLASS_NAME = "Material";

    Material::Material( const s_uint& uiID, MaterialType mType, s_ptr<Ogre::Material> pOgreMat )
    {
        uiID_ = uiID;
        mType_ = mType;
        pOgreMat_ = pOgreMat;
        sName_ = pOgreMat_->getName();

        pDefaultPass_ = pOgreMat->getTechnique(0)->getPass(0);

        if ((mType != MATERIAL_2D_PLAIN) && (mType != MATERIAL_3D_PLAIN))
        {
            s_ptr<Ogre::Technique> pTech = pOgreMat->getTechnique(0);
            if (pTech)
            {
                for (uint i = 0; i < pTech->getNumPasses(); i++)
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
    }

    Material::~Material()
    {
        map< s_uint, s_ptr<Decal> >::iterator iterDecal;
        foreach (iterDecal, lDecalList_)
        {
            iterDecal->second.Delete();
        }

        if (mType_ != MATERIAL_UNKNOWN)
        {
            Ogre::MaterialManager::getSingleton().remove(pOgreMat_->getHandle());
        }
    }

    void Material::SetAlphaReject( const s_bool& bEnable )
    {
        if (!bAlphaReject_ && bEnable)
        {
            pDefaultPass_->setAlphaRejectSettings(Ogre::CMPF_GREATER, 200);
            pDefaultPass_->setCullingMode(Ogre::CULL_NONE);
            bAlphaReject_ = bEnable;
        }
        else if (bAlphaReject_ && !bEnable)
        {
            pDefaultPass_->setAlphaRejectFunction(Ogre::CMPF_ALWAYS_PASS);
            pDefaultPass_->setCullingMode(Ogre::CULL_CLOCKWISE);
            bAlphaReject_ = bEnable;
        }
    }

    void Material::SetHardwareSkinning( const s_bool& bEnable )
    {
        if (!bHardwareSkinning_ && bEnable)
        {
            pDefaultPass_->setVertexProgram("Skinning_VS");
            pDefaultPass_->setFragmentProgram("Skinning_PS");

            Ogre::GpuProgramParametersSharedPtr pParams = pDefaultPass_->getVertexProgramParameters();
            pParams->setNamedAutoConstant(
                "mViewProj",
                Ogre::GpuProgramParameters::ACT_VIEWPROJ_MATRIX
            );
            pParams->setNamedAutoConstant(
                "mBoneMat",
                Ogre::GpuProgramParameters::ACT_WORLD_MATRIX_ARRAY_3x4
            );

            pParams = pDefaultPass_->getFragmentProgramParameters();
            pParams->setNamedAutoConstant(
                "mLightPos",
                Ogre::GpuProgramParameters::ACT_LIGHT_POSITION_ARRAY,
                5
            );
            pParams->setNamedAutoConstant(
                "mLightDiffuseColor",
                Ogre::GpuProgramParameters::ACT_DERIVED_LIGHT_DIFFUSE_COLOUR_ARRAY,
                5
            );
            pParams->setNamedAutoConstant(
                "mLightAtten",
                Ogre::GpuProgramParameters::ACT_LIGHT_ATTENUATION_ARRAY,
                5
            );
            pParams->setNamedAutoConstant(
                "mAmbient",
                Ogre::GpuProgramParameters::ACT_DERIVED_SCENE_COLOUR
            );

            bHardwareSkinning_ = bEnable;
        }
        else if (bHardwareSkinning_ && !bEnable)
        {
            pDefaultPass_->setVertexProgram("");
            pDefaultPass_->setFragmentProgram("");

            bHardwareSkinning_ = bEnable;
        }
    }

    void Material::SetTilling( const s_float& fTileFactorH, const s_float& fTileFactorV )
    {
        pDefaultPass_->getTextureUnitState(0)->setTextureScale(
            fTileFactorH.Get(),
            fTileFactorV.Get()
        );
    }

    void Material::SetDiffuse( const Color& mColor )
    {
        pDefaultPass_->setDiffuse(
            mColor.GetR().Get()/255.0f,
            mColor.GetG().Get()/255.0f,
            mColor.GetB().Get()/255.0f,
            mColor.GetA().Get()/255.0f
        );
    }

    void Material::SetDesaturated( const s_bool& bIsDesaturated )
    {
        if (bIsDesaturated_ != bIsDesaturated)
        {
            bIsDesaturated_ = bIsDesaturated;

            if (bIsDesaturated_)
            {
                pDefaultPass_->setFragmentProgram("GUI_Desaturation_PS");
            }
            else
            {
                pDefaultPass_->setFragmentProgram("");
            }
        }
    }

    void Material::SetSelfIllumination( const Color& mColor )
    {
        pDefaultPass_->setSelfIllumination(
            mColor.GetR().Get()/255.0f,
            mColor.GetG().Get()/255.0f,
            mColor.GetB().Get()/255.0f
        );
    }

    void Material::SetAmbient( const Color& mColor )
    {
        pDefaultPass_->setAmbient(
            mColor.GetR().Get()/255.0f,
            mColor.GetG().Get()/255.0f,
            mColor.GetB().Get()/255.0f
        );
    }

    void Material::SetWireframe( const s_bool& bWireframe )
    {
        if (bWireframe)
            pDefaultPass_->setPolygonMode(Ogre::PM_WIREFRAME);
        else
            pDefaultPass_->setPolygonMode(Ogre::PM_SOLID);
    }

    s_ptr<Decal> Material::AddDecal( s_ptr<Decal> pDecal )
    {
        s_ptr<Decal> pNewDecal = new Decal(*pDecal, uiDecalCounter_, pOgreMat_);
        lDecalList_[uiDecalCounter_] = pNewDecal;
        uiDecalCounter_++;

        pNewDecal->Show();

        return pNewDecal;
    }

    s_ptr<Decal> Material::AddDecal( s_refptr<Decal> pDecal )
    {
        return this->AddDecal(pDecal.Get());
    }

    s_ptr<Decal> Material::AddDecal( const s_str& sTextureFile )
    {
        s_ptr<Decal> pDecal = new Decal(sTextureFile, uiDecalCounter_, pOgreMat_);
        lDecalList_[uiDecalCounter_] = pDecal;
        uiDecalCounter_++;

        pDecal->Show();

        return pDecal;
    }

    void Material::RemoveDecal( s_ptr<Decal> pDecal )
    {
        const s_uint& uiID = pDecal->GetID();
        map< s_uint, s_ptr<Decal> >::iterator iterDecal = lDecalList_.find(uiID);
        if (iterDecal != lDecalList_.end())
        {
            s_ptr<Decal> pListedDecal = lDecalList_[uiID];
            if (pListedDecal == pDecal)
            {
                pDecal.Delete();
                lDecalList_.erase(iterDecal);
            }
        }

        Warning(CLASS_NAME,
            "Trying to remove a Decal that doesn't belong to this Material "
            "("+sName_+")."
        );
    }

    const s_float& Material::GetWidth() const
    {
        return fWidth_;
    }

    const s_float& Material::GetHeight() const
    {
        return fHeight_;
    }

    s_ptr<Ogre::Material> Material::GetOgreMaterial()
    {
        return pOgreMat_;
    }

    void Material::SetDefaultPass( const s_uint& uiIndex )
    {
        if (uiIndex < pOgreMat_->getTechnique(0)->getNumPasses())
            pDefaultPass_ = pOgreMat_->getTechnique(0)->getPass(uiIndex.Get());
        else
        {
            Warning(CLASS_NAME,
                "Calling SetDefaultPass() with a too high index : "+uiIndex+" (only "+
                s_uint((uint)pOgreMat_->getTechnique(0)->getNumPasses())+" pass"+
                (s_uint((uint)pOgreMat_->getTechnique(0)->getNumPasses()) > 1).GetAsString("es", "")+
                " available)."
            );
        }
    }

    s_ptr<Ogre::Pass> Material::GetDefaultPass()
    {
        return pDefaultPass_;
    }

    const s_str& Material::GetOgreMaterialName() const
    {
        return sName_;
    }

    s_bool Material::IsRenderTarget() const
    {
        return (mType_ == MATERIAL_2D_RT);
    }

    s_bool Material::IsPlain() const
    {
        return (mType_ == MATERIAL_3D_PLAIN) || (mType_ == MATERIAL_2D_PLAIN);
    }
}
