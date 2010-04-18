/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Material header             */
/*                                        */
/*                                        */

#include "material/frost_material.h"

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

    Material::Material( const s_uint& uiID, Type mType, s_ptr<Ogre::Material> pOgreMat )
    {
        uiID_ = uiID;
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
        if (mType_ != TYPE_UNKNOWN)
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
            pDefaultPass_->pPass->setAlphaRejectSettings(Ogre::CMPF_GREATER, 200);
            pDefaultPass_->pPass->setCullingMode(Ogre::CULL_NONE);
            pDefaultPass_->bAlphaReject = true;
        }
        else if (pDefaultPass_->bAlphaReject && !bEnable)
        {
            pDefaultPass_->pPass->setAlphaRejectFunction(Ogre::CMPF_ALWAYS_PASS);
            pDefaultPass_->pPass->setCullingMode(Ogre::CULL_CLOCKWISE);
            pDefaultPass_->bAlphaReject = false;
        }
    }

    void Material::SetTilling( const s_float& fTileFactorH, const s_float& fTileFactorV )
    {
        pDefaultPass_->pPass->getTextureUnitState(0)->setTextureScale(
            fTileFactorH.Get(),
            fTileFactorV.Get()
        );
    }

    void Material::SetDiffuse( const Color& mColor )
    {
        if (mColor.GetA() != 255)
            pDefaultPass_->pPass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

        pDefaultPass_->pPass->setDiffuse(
            mColor.GetR().Get()/255.0f,
            mColor.GetG().Get()/255.0f,
            mColor.GetB().Get()/255.0f,
            mColor.GetA().Get()/255.0f
        );
    }

    void Material::SetSelfIllumination( const Color& mColor )
    {
        pDefaultPass_->pPass->setSelfIllumination(
            mColor.GetR().Get()/255.0f,
            mColor.GetG().Get()/255.0f,
            mColor.GetB().Get()/255.0f
        );
    }

    void Material::SetAmbient( const Color& mColor )
    {
        pDefaultPass_->pPass->setAmbient(
            mColor.GetR().Get()/255.0f,
            mColor.GetG().Get()/255.0f,
            mColor.GetB().Get()/255.0f
        );
    }

    void Material::SetWireframe( const s_bool& bWireframe )
    {
        if (bWireframe)
            pDefaultPass_->pPass->setPolygonMode(Ogre::PM_WIREFRAME);
        else
            pDefaultPass_->pPass->setPolygonMode(Ogre::PM_SOLID);
    }

    void Material::SetDepthCheck( const s_bool& bDepthCheck )
    {
        pDefaultPass_->pPass->setDepthCheckEnabled(bDepthCheck);
    }

    void Material::SetDepthWrite( const s_bool& bDepthWrite )
    {
        pDefaultPass_->pPass->setDepthWriteEnabled(bDepthWrite);
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

    void Material::SetVertexShader( s_ptr<VertexShader> pVS )
    {
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
        if (pDefaultPass_->pVS)
            pDefaultPass_->pVS->UnBind(pDefaultPass_->pPass);

        pDefaultPass_->pPass->setVertexProgram("");

        pDefaultPass_->pVS = nullptr;
    }

    void Material::SetPixelShader( s_ptr<PixelShader> pPS )
    {
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
        s_ptr<Ogre::Pass> pPass = pOgreMat_->getTechnique(0)->createPass();
        lPassList_.PushBack(PassInfo());
        lPassList_.Back().pPass = pPass;

        return pPass->getIndex();
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

    const s_str& Material::GetOgreMaterialName() const
    {
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
}
