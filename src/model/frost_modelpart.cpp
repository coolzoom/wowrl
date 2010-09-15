/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            ModelPart source            */
/*                                        */
/*  ## : It's just a submesh.             */
/*                                        */

#include "model/frost_model.h"
#include "material/frost_material.h"

#include <OgreEntity.h>
#include <OgreSubEntity.h>

using namespace std;

namespace Frost
{
    const s_str ModelPart::CLASS_NAME = "ModelPart";

    ModelPart::ModelPart( s_ptr<Model> pParent, const s_uint& uiID )
    {
        pParent_ = pParent;
        uiID_ = uiID;
    }

    ModelPart::ModelPart( const ModelPart& mMP, s_ptr<Model> pParent )
    {
        pParent_ = pParent;
        uiID_ = mMP.uiID_;
        lSubEntity_ = mMP.lSubEntity_;
    }

    void ModelPart::Show( const s_uint& uiSubEntity )
    {
        if (uiSubEntity.IsValid())
        {
            if (uiSubEntity < lSubEntity_.GetSize())
                pParent_->GetEntity()->getSubEntity(uiSubEntity.Get())->setVisible(true);
        }
        else
        {
            s_ctnr<s_uint>::iterator iterSub;
            foreach (iterSub, lSubEntity_)
            {
                pParent_->GetEntity()->getSubEntity(iterSub->Get())->setVisible(true);
            }
        }
    }

    void ModelPart::Hide( const s_uint& uiSubEntity )
    {
        if (uiSubEntity.IsValid())
        {
            if (uiSubEntity < lSubEntity_.GetSize())
                pParent_->GetEntity()->getSubEntity(uiSubEntity.Get())->setVisible(false);
        }
        else
        {
            s_ctnr<s_uint>::iterator iterSub;
            foreach (iterSub, lSubEntity_)
            {
                pParent_->GetEntity()->getSubEntity(iterSub->Get())->setVisible(false);
            }
        }
    }

    void ModelPart::Highlight()
    {
        if (!bHighlighted_)
        {
            bHighlighted_ = true;
            if (pMaterial_)
                pMaterial_->SetSelfIllumination(Color(128, 128, 128));

            s_map< s_uint, s_refptr<Material> >::iterator iterMat;
            foreach (iterMat, lSubEntityMaterialList_)
            {
                iterMat->second->SetSelfIllumination(Color(128, 128, 128));
            }
        }
    }

    void ModelPart::Unlight()
    {
        if (bHighlighted_)
        {
            bHighlighted_ = false;
            if (pMaterial_)
                pMaterial_->SetSelfIllumination(Color(0, 0, 0));

            s_map< s_uint, s_refptr<Material> >::iterator iterMat;
            foreach (iterMat, lSubEntityMaterialList_)
            {
                iterMat->second->SetSelfIllumination(Color(0, 0, 0));
            }
        }
    }

    void ModelPart::SetMaterial( s_refptr<Material> pMat, const s_uint& uiSubEntity )
    {
        if (uiSubEntity.IsValid())
        {
            if (uiSubEntity >= lSubEntity_.GetSize())
            {
                Warning(CLASS_NAME, "Trying to set the material of subentity "+uiSubEntity+", but this ModelPart\n"
                    "("+pParent_->GetOgreModelName()+":"+uiID_+") only contains "+lSubEntity_.GetSize()+" subentities."
                );
            }
            else
            {
                lSubEntityMaterialList_[uiSubEntity] = pMat;
                pParent_->GetEntity()->getSubEntity(lSubEntity_[uiSubEntity].Get())->setMaterialName(
                    pMat->GetOgreMaterialName().Get()
                );
            }
        }
        else
        {
            pMaterial_ = pMat;
            s_ctnr<s_uint>::iterator iterSub;
            foreach (iterSub, lSubEntity_)
            {
                pParent_->GetEntity()->getSubEntity(iterSub->Get())->setMaterialName(
                    pMat->GetOgreMaterialName().Get()
                );
            }
        }
    }

    s_wptr<Material> ModelPart::GetMaterial()
    {
        return pMaterial_;
    }

    void ModelPart::SetCustomShaderParameter( const s_uint& uiID, const Ogre::Vector4& mVec )
    {
        s_ctnr<s_uint>::iterator iterSub;
        foreach (iterSub, lSubEntity_)
        {
            pParent_->GetEntity()->getSubEntity(iterSub->Get())->setCustomParameter(
                uiID.Get(), mVec
            );
        }
    }

    void ModelPart::AddSubEntity( const s_uint& uiID )
    {
        lSubEntity_.PushBack(uiID);
    }


    const s_ctnr<s_uint>& ModelPart::GetSubEntityList() const
    {
        return lSubEntity_;
    }
}
