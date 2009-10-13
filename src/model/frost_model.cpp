/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Model source              */
/*                                        */
/*  ## : Contains the member functions    */
/*  of the Model class.                   */
/*       Models are loaded from M2 files  */
/*  and used to render characters, items, */
/*  doodads, ...                          */
/*                                        */

#include "model/frost_model.h"

#include "model/frost_modeldata.h"
#include "model/frost_animmanager.h"
#include "material/frost_material.h"
#include "material/frost_materialmanager.h"

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreSubEntity.h>

using namespace std;

namespace Frost
{
    const s_str Model::CLASS_NAME = "Model";

    Model::Model( const ModelData& mData, const s_str& sEntityName ) : MovableObject()
    {
        pMesh_ = mData.pMesh_;
        sModelName_ = mData.sModelName_;
        sEntityName_ = sEntityName;

        s_ptr<Ogre::SceneManager> pSceneMgr = Engine::GetSingleton()->GetOgreSceneManager();
        pEntity_ = pSceneMgr->createEntity(sEntityName_.Get(), sModelName_.Get());

        pNode_->attachObject(pEntity_.Get());

        pAnimMgr_ = s_refptr<AnimManager>(new AnimManager(this, mData.lAnimList_));

        s_map< s_uint, s_ptr<ModelPart> >::const_iterator iterMP;
        foreach (iterMP, mData.lModelPartList_)
        {
            lModelPartList_[iterMP->first] = new ModelPart(*iterMP->second, this);
        }
    }

    Model::~Model()
    {
        s_map< s_uint, s_ptr<ModelPart> >::iterator iterPart;
        foreach (iterPart, lModelPartList_)
        {
            iterPart->second.Delete();
        }

        if (pEntity_)
        {
            pEntity_->detatchFromParent();
            Engine::GetSingleton()->GetOgreSceneManager()->destroyEntity(
                pEntity_.Get()
            );
        }
    }

    const s_str& Model::GetModelName() const
    {
        return sModelName_;
    }

    const s_str& Model::GetEntityName() const
    {
        return sEntityName_;
    }

    s_ptr<ModelPart> Model::GetModelPart( const s_uint& uiID )
    {
        if (lModelPartList_.Find(uiID))
        {
            return lModelPartList_[uiID];
        }
        else
        {
            Warning(CLASS_NAME,
                "ModelPart "+s_str(uiID, 4)+" doesn't seem to exist for \""+sModelName_+"\"."
            );
            return NULL;
        }
    }

    s_ptr<Ogre::Entity> Model::GetEntity()
    {
        return pEntity_;
    }


    s_ptr<Ogre::Mesh> Model::GetMesh()
    {
        return pMesh_;
    }

    void Model::SetUserObject(s_ptr<Ogre::UserDefinedObject> pObj)
    {
        pEntity_->setUserObject(pObj.Get());
    }

    void Model::Show()
    {
        pEntity_->setVisible(true);
    }

    void Model::Hide( s_bool bSubMeshes )
    {
        if (bSubMeshes)
        {
            for (uint i = 0; i < pEntity_->getNumSubEntities(); i++)
            {
                pEntity_->getSubEntity(i)->setVisible(false);
            }
        }

        pEntity_->setVisible(false);
    }

    s_refptr<AnimManager> Model::GetAnimMgr()
    {
        return pAnimMgr_;
    }

    void Model::SetMaterial( s_refptr<Material> pMat )
    {
        pMaterial_ = pMat;
        for (uint i = 0; i < pEntity_->getNumSubEntities(); i++)
        {
            pEntity_->getSubEntity(i)->setMaterialName(pMat->GetOgreMaterialName().Get());
        }
    }

    void Model::SetCustomShaderParameter(const s_uint& uiID, const Ogre::Vector4& mVec)
    {
        for (uint i = 0; i < pEntity_->getNumSubEntities(); i++)
        {
            pEntity_->getSubEntity(i)->setCustomParameter(uiID.Get(), mVec);
        }
    }

    s_refptr<Material> Model::GetMaterial()
    {
        return pMaterial_;
    }

    const s_map< s_uint, s_ptr<ModelPart> >& Model::GetModelPartList() const
    {
        return lModelPartList_;
    }

    void Model::Update(const s_float& fDelta)
    {
        /*Vector mTemp = GetPosition();
        MovableObject::Update(fDelta);
        mTemp -= GetPosition();
        if (!mTemp.IsNull())
        {
            SetDirection(mTemp);
        }*/

        pAnimMgr_->Update(fDelta);
    }
}
