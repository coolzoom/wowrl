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

        pDefaultPass_ = pOgreMat->getTechnique(0)->getPass(0);

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

    s_wptr<Decal> Material::AddDecal( s_wptr<Decal> pDecal )
    {
        if (s_refptr<Decal> pLocked = pDecal.Lock())
        {
            s_refptr<Decal> pNewDecal = s_refptr<Decal>(new Decal(*pLocked, pOgreMat_));
            lDecalList_[pNewDecal->GetID()] = pNewDecal;

            pNewDecal->Show();

            return pNewDecal;
        }
        else
            return s_wptr<Decal>();
    }

    s_wptr<Decal> Material::AddDecal( const s_str& sTextureFile )
    {
        s_refptr<Decal> pDecal = s_refptr<Decal>(new Decal(sTextureFile, pOgreMat_));
        lDecalList_[pDecal->GetID()] = pDecal;

        pDecal->Show();

        return pDecal;
    }

    void Material::RemoveDecal( s_wptr<Decal> pDecal )
    {
        if (s_refptr<Decal> pLocked = pDecal.Lock())
        {
            s_map< s_uint, s_refptr<Decal> >::iterator iterDecal = lDecalList_.Get(pLocked->GetID());
            if (iterDecal != lDecalList_.End())
            {
                if (iterDecal->second == pLocked)
                {
                    lDecalList_.Erase(iterDecal);
                    return;
                }
            }

            Warning(CLASS_NAME,
                "Trying to remove a Decal that doesn't belong to this Material "
                "("+sName_+")."
            );
        }
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
        if (pVS_)
            pVS_->UnBind(pDefaultPass_);

        if (pVS && pVS->IsValid())
        {
            pVS->BindTo(pDefaultPass_);
            pVS_ = pVS;
        }
        else
        {
            pDefaultPass_->setVertexProgram("");
            pVS_ = nullptr;
        }
    }

    void Material::RemoveVertexShader()
    {
        if (pVS_)
            pVS_->UnBind(pDefaultPass_);

        pDefaultPass_->setVertexProgram("");

        pVS_ = nullptr;
    }

    void Material::SetPixelShader( s_ptr<PixelShader> pPS )
    {
        if (pPS_)
            pPS_->UnBind(pDefaultPass_);

        if (pPS && pPS->IsValid())
        {
            pPS->BindTo(pDefaultPass_);
            pPS_ = pPS;
        }
        else
        {
            pDefaultPass_->setFragmentProgram("");
            pPS_ = nullptr;
        }
    }

    void Material::RemovePixelShader()
    {
        if (pPS_)
            pPS_->UnBind(pDefaultPass_);

        pDefaultPass_->setFragmentProgram("");

        pPS_ = nullptr;
    }

    void Material::SetShaders( const s_str& sSName )
    {
        SetVertexShader(sSName);
        SetPixelShader(sSName);
    }

    void Material::SetVertexShader( const s_str& sVSName )
    {
        s_ptr<VertexShader> pVS = ShaderManager::GetSingleton()->GetVertexShader(sVSName);
        SetVertexShader(pVS);
    }

    void Material::SetPixelShader( const s_str& sPSName )
    {
        s_ptr<PixelShader> pPS = ShaderManager::GetSingleton()->GetPixelShader(sPSName);
        SetPixelShader(pPS);
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
                s_str(pOgreMat_->getTechnique(0)->getNumPasses() > 1 ? "es" : "")+
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
        return (mType_ == TYPE_2D_RT);
    }

    s_bool Material::IsPlain() const
    {
        return (mType_ == TYPE_3D_PLAIN) || (mType_ == TYPE_2D_PLAIN);
    }
}
