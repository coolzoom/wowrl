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

    void ModelPart::Show()
    {
        s_ctnr<s_uint>::iterator iterSub;
        foreach (iterSub, lSubEntity_)
        {
            pParent_->GetEntity()->getSubEntity(iterSub->Get())->setVisible(true);
        }
    }

    void ModelPart::Hide()
    {
        s_ctnr<s_uint>::iterator iterSub;
        foreach (iterSub, lSubEntity_)
        {
            pParent_->GetEntity()->getSubEntity(iterSub->Get())->setVisible(false);
        }
    }

    void ModelPart::SetMaterial(s_refptr<Material> pMat)
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

    s_wptr<Material> ModelPart::GetMaterial()
    {
        return pMaterial_;
    }

    void ModelPart::SetCustomShaderParameter(const s_uint& uiID, const Ogre::Vector4& mVec)
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
}
