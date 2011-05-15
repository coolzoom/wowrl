/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Material header             */
/*                                        */
/*                                        */

#include "material/frost_material.h"
#include "material/frost_materialmanager.h"

#include <frost_utils_file.h>

#include <OgreMaterialManager.h>
#include <OgreMaterial.h>
#include <OgreTechnique.h>
#include <OgrePass.h>

#include "material/frost_decal.h"
#include "material/frost_shadermanager.h"
#include "material/frost_shader.h"

using namespace std;

namespace Frost
{
    const s_str Material::CLASS_NAME = "Material";

    Material::Material( const s_uint& uiID, Type mType, s_ptr<Ogre::Material> pOgreMat, const s_bool& bVanilla )
    {
        uiID_ = uiID;
        bVanilla_ = bVanilla;
        mType_ = mType;
        pOgreMat_ = pOgreMat;
        sName_ = pOgreMat_->getName();

        lPassList_.PushBack(PassInfo());
        lPassList_[0].pPass = pOgreMat->getTechnique(0)->getPass(0);
        pDefaultPass_ = &lPassList_[0];

        if ((mType != TYPE_2D_PLAIN) && (mType != TYPE_3D_PLAIN))
        {
            s_ptr<Ogre::Technique> pTech = pOgreMat->getTechnique(0);
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
    }

    Material::~Material()
    {
        if (bOwner_ && !bVanilla_)
        {
            RemoveVertexShader();
            RemovePixelShader();

            Ogre::MaterialManager::getSingleton().remove(pOgreMat_->getHandle());
        }
    }

    void Material::SetAlphaReject( const s_bool& bEnable )
    {
        if (!pDefaultPass_->bAlphaReject && bEnable)
        {
            CheckVanilla_();
            pDefaultPass_->pPass->setAlphaRejectSettings(Ogre::CMPF_GREATER, 200);
            pDefaultPass_->pPass->setCullingMode(Ogre::CULL_NONE);
            pDefaultPass_->bAlphaReject = true;
        }
        else if (pDefaultPass_->bAlphaReject && !bEnable)
        {
            CheckVanilla_();
            pDefaultPass_->pPass->setAlphaRejectFunction(Ogre::CMPF_ALWAYS_PASS);
            pDefaultPass_->pPass->setCullingMode(Ogre::CULL_CLOCKWISE);
            pDefaultPass_->bAlphaReject = false;
        }
    }

    void Material::SetTilling( const s_float& fTileFactorH, const s_float& fTileFactorV )
    {
        CheckVanilla_();
        pDefaultPass_->pPass->getTextureUnitState(0)->setTextureScale(
            fTileFactorH.Get(),
            fTileFactorV.Get()
        );
    }

    void Material::SetDiffuse( const Color& mColor )
    {
        CheckVanilla_();

        if (mColor.GetA() != 1.0f)
            pDefaultPass_->pPass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

        pDefaultPass_->pPass->setDiffuse(
            mColor.GetR().Get(),
            mColor.GetG().Get(),
            mColor.GetB().Get(),
            mColor.GetA().Get()
        );
    }

    void Material::SetSelfIllumination( const Color& mColor )
    {
        CheckVanilla_();

        pDefaultPass_->pPass->setSelfIllumination(
            mColor.GetR().Get(),
            mColor.GetG().Get(),
            mColor.GetB().Get()
        );
    }

    void Material::SetAmbient( const Color& mColor )
    {
        CheckVanilla_();

        pDefaultPass_->pPass->setAmbient(
            mColor.GetR().Get(),
            mColor.GetG().Get(),
            mColor.GetB().Get()
        );
    }

    void Material::SetWireframe( const s_bool& bWireframe )
    {
        CheckVanilla_();

        if (bWireframe)
            pDefaultPass_->pPass->setPolygonMode(Ogre::PM_WIREFRAME);
        else
            pDefaultPass_->pPass->setPolygonMode(Ogre::PM_SOLID);
    }

    void Material::SetDepthCheck( const s_bool& bDepthCheck )
    {
        CheckVanilla_();
        pDefaultPass_->pPass->setDepthCheckEnabled(bDepthCheck);
    }

    void Material::SetDepthWrite( const s_bool& bDepthWrite )
    {
        CheckVanilla_();
        pDefaultPass_->pPass->setDepthWriteEnabled(bDepthWrite);
    }

    void Material::SetCulling( const s_bool& bCulling )
    {
        CheckVanilla_();

        if (bCulling)
            pDefaultPass_->pPass->setCullingMode(Ogre::CULL_CLOCKWISE);
        else
            pDefaultPass_->pPass->setCullingMode(Ogre::CULL_NONE);
    }

    const s_float& Material::GetWidth() const
    {
        return fWidth_;
    }

    const s_float& Material::GetHeight() const
    {
        return fHeight_;
    }

    s_ptr<Ogre::Material> Material::GetOgreMaterial( GetMaterialFlag mFlag )
    {
        switch (mFlag)
        {
            case FLAG_NONE : break;
            case FLAG_TRANSFER_OWNERSHIP : bOwner_ = false; break;
        }

        return pOgreMat_;
    }

    void Material::SetVertexShader( s_ptr<VertexShader> pVS )
    {
        CheckVanilla_();
        if (pDefaultPass_->pVS)
            pDefaultPass_->pVS->UnBind(pDefaultPass_->pPass);

        if (pVS && pVS->IsValid())
        {
            pVS->BindTo(pDefaultPass_->pPass);
            pDefaultPass_->pVS = pVS;
        }
        else
        {
            pDefaultPass_->pPass->setVertexProgram("");
            pDefaultPass_->pVS = nullptr;
        }
    }

    void Material::RemoveVertexShader()
    {
        CheckVanilla_();
        if (pDefaultPass_->pVS)
            pDefaultPass_->pVS->UnBind(pDefaultPass_->pPass);

        pDefaultPass_->pPass->setVertexProgram("");

        pDefaultPass_->pVS = nullptr;
    }

    void Material::SetPixelShader( s_ptr<PixelShader> pPS )
    {
        CheckVanilla_();
        if (pDefaultPass_->pPS)
            pDefaultPass_->pPS->UnBind(pDefaultPass_->pPass);

        if (pPS && pPS->IsValid())
        {
            pPS->BindTo(pDefaultPass_->pPass);
            pDefaultPass_->pPS = pPS;
        }
        else
        {
            pDefaultPass_->pPass->setFragmentProgram("");
            pDefaultPass_->pPS = nullptr;
        }
    }

    void Material::RemovePixelShader()
    {
        CheckVanilla_();
        if (pDefaultPass_->pPS)
            pDefaultPass_->pPS->UnBind(pDefaultPass_->pPass);

        pDefaultPass_->pPass->setFragmentProgram("");

        pDefaultPass_->pPS = nullptr;
    }

    void Material::SetShaders( const s_str& sSName )
    {
        SetVertexShader(sSName);
        SetPixelShader(sSName);
    }

    void Material::SetVertexShader( const s_str& sVSName )
    {
        SetVertexShader(ShaderManager::GetSingleton()->GetVertexShader(sVSName));
    }

    void Material::SetPixelShader( const s_str& sPSName )
    {
        SetPixelShader(ShaderManager::GetSingleton()->GetPixelShader(sPSName));
    }

    s_uint Material::CreatePass()
    {
        CheckVanilla_();
        s_ptr<Ogre::Pass> pPass = pOgreMat_->getTechnique(0)->createPass();
        PassInfo mPI;
        mPI.pPass = pPass;
        mPI.uiPassID = lPassList_.GetSize();

        lPassList_.PushBack(mPI);

        return mPI.uiPassID;
    }

    void Material::SetDefaultPass( const s_uint& uiIndex )
    {
        if (uiIndex < lPassList_.GetSize())
            pDefaultPass_ = &lPassList_[uiIndex];
        else
        {
            Warning(CLASS_NAME,
                "Calling SetDefaultPass() with a too high index : "+uiIndex+" (only "+
                lPassList_.GetSize()+" pass"+s_str(lPassList_.GetSize() > 1 ? "es" : "")+
                " available)."
            );
        }
    }

    s_ptr<Ogre::Pass> Material::GetDefaultPass()
    {
        return pDefaultPass_->pPass;
    }

    const s_str& Material::GetOgreMaterialName( GetMaterialFlag mFlag ) const
    {
        switch (mFlag)
        {
            case FLAG_NONE : break;
            case FLAG_TRANSFER_OWNERSHIP : bOwner_ = false; break;
        }

        return sName_;
    }

    s_bool Material::IsRenderTarget() const
    {
        return (mType_ == TYPE_2D_RT);
    }

    s_bool Material::IsPlain() const
    {
        return (mType_ == TYPE_3D_PLAIN) || (mType_ == TYPE_2D_PLAIN);
    }

    const s_uint& Material::GetID() const
    {
        return uiID_;
    }

    void Material::CheckVanilla_()
    {
        if (bVanilla_)
        {
            sName_ = "ClonedMaterial_"+uiID_;
            pOgreMat_ = pOgreMat_->clone(sName_.Get()).get();
            pOgreMat_->load();

            s_ctnr<PassInfo>::iterator iterPI;
            foreach (iterPI, lPassList_)
            {
                iterPI->pPass = pOgreMat_->getTechnique(0)->getPass(iterPI->uiPassID.Get());
                if (iterPI->pVS && iterPI->pVS->IsValid())
                {
                    iterPI->pPass->setVertexProgram("");
                    iterPI->pVS->BindTo(iterPI->pPass);
                }
                if (iterPI->pPS && iterPI->pPS->IsValid())
                {
                    iterPI->pPass->setFragmentProgram("");
                    iterPI->pPS->BindTo(iterPI->pPass);
                }
            }

            bVanilla_ = false;
        }
    }

    MaterialDefinition::MaterialDefinition() : mType_(TYPE_COLOR), mColor_(Color::WHITE)
    {
    }

    MaterialDefinition::MaterialDefinition( const s_str& sFile, const s_bool& bAlphaReject ) :
        mType_(TYPE_TEXTURE), sTextureFile_(sFile), bAlphaReject_(bAlphaReject)
    {
    }

    MaterialDefinition::MaterialDefinition( const Color& mColor ) :
        mType_(TYPE_COLOR), mColor_(mColor)
    {
    }

    void MaterialDefinition::SetTextureFile( const s_str& sFile )
    {
        sTextureFile_ = sFile;
        mType_ = TYPE_TEXTURE;
    }

    void MaterialDefinition::SetColor( const Color& mColor )
    {
        mColor_ = mColor;
        mType_ = TYPE_COLOR;
    }

    void MaterialDefinition::SetAlphaReject( const s_bool& bAlphaReject )
    {
        bAlphaReject_ = bAlphaReject;
    }

    s_refptr<Material> MaterialDefinition::CreateMaterial( const s_bool& bPostProcess ) const
    {
        s_refptr<Material> pMat;

        switch (mType_)
        {
            case TYPE_COLOR :
                pMat = MaterialManager::GetSingleton()->CreateMaterial3D(mColor_);
                if (bPostProcess)
                    pMat->SetShaders("SimpleColor");
                else
                    pMat->SetShaders("SimpleColorNoPP");
                break;

            case TYPE_TEXTURE :
                pMat = MaterialManager::GetSingleton()->CreateMaterial3D(sTextureFile_);
                if (bPostProcess)
                    pMat->SetShaders("SimpleTexture");
                else
                    pMat->SetShaders("SimpleTextureNoPP");
                break;
        }

        if (bAlphaReject_)
            pMat->SetAlphaReject(true);

        return pMat;
    }

    s_str SerializeColorRGB( const Color& mColor, const s_str& sTag )
    {
        return "<"+sTag+" r=\""+mColor.GetR()+"\""
                        " g=\""+mColor.GetG()+"\""
                        " b=\""+mColor.GetB()+"\"/>";
    }

    s_str MaterialDefinition::Serialize() const
    {
        switch (mType_)
        {
            case TYPE_COLOR :
                return SerializeColorRGB(mColor_, "DiffuseColor");

            case TYPE_TEXTURE :
                return "<DiffuseTexture file=\""+sTextureFile_+"\""
                       +(bAlphaReject_ ? " alphaReject=\"true\"" : "")+"/>";
            default : return "";
        }
    }

    void MaterialDefinition::SerializeIn( File& mFile ) const
    {
        switch (mType_)
        {
            case TYPE_COLOR :
                mFile.WriteLine(SerializeColorRGB(mColor_, "DiffuseColor"));
                break;

            case TYPE_TEXTURE :
                mFile.WriteLine("<DiffuseTexture file=\""+sTextureFile_+"\""
                            +(bAlphaReject_ ? " alphaReject=\"true\"" : "")+"/>");
                break;
        }
    }
}
