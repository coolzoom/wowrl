/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Material header             */
/*                                        */
/*                                        */

#include "frost_material.h"

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
    }

    void Material::SetAlphaReject( const s_bool& bEnable )
    {
        if (!bAlphaReject_ && bEnable)
        {
            s_ptr<Ogre::Pass> pPass = pOgreMat_->getTechnique(0)->getPass(0);
            pPass->setAlphaRejectSettings(Ogre::CMPF_GREATER, 200);
            pPass->setCullingMode(Ogre::CULL_NONE);
            bAlphaReject_ = bEnable;
        }
        else if (bAlphaReject_ && !bEnable)
        {
            s_ptr<Ogre::Pass> pPass = pOgreMat_->getTechnique(0)->getPass(0);
            pPass->setAlphaRejectFunction(Ogre::CMPF_ALWAYS_PASS);
            pPass->setCullingMode(Ogre::CULL_CLOCKWISE);
            bAlphaReject_ = bEnable;
        }
    }

    void Material::SetHardwareSkinning( const s_bool& bEnable )
    {
        if (!bHardwareSkinning_ && bEnable)
        {
            s_ptr<Ogre::Pass> pPass = pOgreMat_->getTechnique(0)->getPass(0);
            pPass->setVertexProgram("Skinning_VS");
            pPass->setFragmentProgram("Skinning_PS");

            Ogre::GpuProgramParametersSharedPtr pParams = pPass->getVertexProgramParameters();
            pParams->setNamedAutoConstant(
                "mViewProj",
                Ogre::GpuProgramParameters::ACT_VIEWPROJ_MATRIX
            );
            pParams->setNamedAutoConstant(
                "mBoneMat",
                Ogre::GpuProgramParameters::ACT_WORLD_MATRIX_ARRAY_3x4
            );

            pParams = pPass->getFragmentProgramParameters();
            pParams->setNamedAutoConstant(
                "mLightPos",
                Ogre::GpuProgramParameters::ACT_LIGHT_POSITION_ARRAY,
                5
            );
            pParams->setNamedAutoConstant(
                "mLightDiffuseColor",
                Ogre::GpuProgramParameters::ACT_LIGHT_DIFFUSE_COLOUR_ARRAY,
                5
            );
            pParams->setNamedAutoConstant(
                "mLightAtten",
                Ogre::GpuProgramParameters::ACT_LIGHT_ATTENUATION_ARRAY,
                5
            );
            pParams->setNamedAutoConstant(
                "mAmbient",
                Ogre::GpuProgramParameters::ACT_AMBIENT_LIGHT_COLOUR
            );

            bHardwareSkinning_ = bEnable;
        }
        else if (bHardwareSkinning_ && !bEnable)
        {
            s_ptr<Ogre::Pass> pPass = pOgreMat_->getTechnique(0)->getPass(0);
            pPass->setVertexProgram("");
            pPass->setFragmentProgram("");

            bHardwareSkinning_ = bEnable;
        }
    }

    void Material::SetTilling( const s_float& fTileFactorH, const s_float& fTileFactorV )
    {
        pOgreMat_->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureScale(
            fTileFactorH.Get(),
            fTileFactorV.Get()
        );
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
