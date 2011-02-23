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

#include <frost_utils_file.h>
#include <frost_xml_document.h>

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreSubEntity.h>

using namespace std;

namespace Frost
{
    const s_str Model::CLASS_NAME = "Model";

    Model::Model( const ModelData& mData, const s_str& sEntityName, s_ptr<Ogre::SceneManager> pSceneManager ) : MovableObject(s_uint::NaN, pSceneManager)
    {
        pModelData_ = &mData;
        pMesh_ = mData.pMesh_;
        sOgreModelName_ = mData.sModelName_;
        sEntityName_ = sEntityName;

        pEntity_ = pSceneManager_->createEntity(sEntityName_.Get(), sOgreModelName_.Get());

        pNode_->attachObject(pEntity_.Get());

        bHasAnimation_ = !mData.lAnimList_.IsEmpty();

        pAnimMgr_ = s_refptr<AnimManager>(new AnimManager(this, mData.lAnimList_));

        s_map< s_uint, s_ptr<ModelPart> >::const_iterator iterMP;
        foreach (iterMP, mData.lModelPartList_)
        {
            lModelPartList_[iterMP->first] = new ModelPart(*iterMP->second, this);
        }

        if (pMesh_)
            mBoundingBox_ = AxisAlignedBox::OgreToFrost(pMesh_->getBounds());
    }

    Model::~Model()
    {
        s_map< s_uint, s_ptr<Obstacle> >::iterator iterObs;
        foreach (iterObs, lObstacleList_)
        {
            iterObs->second.Delete();
        }

        s_map< s_uint, s_ptr<ModelPart> >::iterator iterPart;
        foreach (iterPart, lModelPartList_)
        {
            iterPart->second.Delete();
        }

        if (pEntity_)
        {
            pEntity_->detatchFromParent();
            pSceneManager_->destroyEntity(pEntity_.Get());
        }
    }

    const s_str& Model::GetOgreModelName() const
    {
        return sOgreModelName_;
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
                "ModelPart "+s_str::Convert(uiID, 4)+" doesn't seem to exist for \""+sOgreModelName_+"\"."
            );
            return nullptr;
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

    void Model::Show( s_bool bSubMeshes )
    {
        if (bSubMeshes)
        {
            for (uint i = 0; i < pEntity_->getNumSubEntities(); ++i)
            {
                pEntity_->getSubEntity(i)->setVisible(true);
            }
        }

        pEntity_->setVisible(true);
    }

    void Model::Hide( s_bool bSubMeshes )
    {
        if (bSubMeshes)
        {
            for (uint i = 0; i < pEntity_->getNumSubEntities(); ++i)
            {
                pEntity_->getSubEntity(i)->setVisible(false);
            }
        }

        pEntity_->setVisible(false);
    }

    void Model::Highlight()
    {
        if (!bHighlighted_)
        {
            bHighlighted_ = true;
            if (pMaterial_)
                pMaterial_->SetSelfIllumination(Color(128, 128, 128));

            s_map< s_uint, s_ptr<ModelPart> >::iterator iterMP;
            foreach (iterMP, lModelPartList_)
            {
                iterMP->second->Highlight();
            }
        }
    }

    void Model::Unlight()
    {
        if (bHighlighted_)
        {
            bHighlighted_ = false;
            if (pMaterial_)
                pMaterial_->SetSelfIllumination(Color(0, 0, 0));

            s_map< s_uint, s_ptr<ModelPart> >::iterator iterMP;
            foreach (iterMP, lModelPartList_)
            {
                iterMP->second->Unlight();
            }
        }
    }

    const s_bool& Model::HasAnimation() const
    {
        return bHasAnimation_;
    }

    s_wptr<AnimManager> Model::GetAnimMgr()
    {
        return pAnimMgr_;
    }

    void Model::CreateObstacles()
    {
        s_map< s_uint, s_ptr<ModelPart> >::const_iterator iterMP;
        foreach (iterMP, lModelPartList_)
        {
            CreateObstacle(iterMP->first);
        }
    }

    void Model::CreateObstacle( const s_uint& uiSubMeshID )
    {
        if (lObstacleList_[uiSubMeshID] == nullptr)
        {
            s_ptr<MeshObstacle> pObs = new MeshObstacle(
                pModelData_->lTriangleList_.Get(uiSubMeshID)->second,
                pModelData_->lBoxList_.Get(uiSubMeshID)->second
            );

            pObs->AttachTo(this);

            lObstacleList_[uiSubMeshID] = pObs;
        }
    }

    s_ptr<Obstacle> Model::GetObstacle( const s_uint& uiSubMeshID ) const
    {
        s_map< s_uint, s_ptr<Obstacle> >::const_iterator iter = lObstacleList_.Get(uiSubMeshID);
        if (iter != lObstacleList_.End())
            return iter->second;
        else
            return nullptr;
    }

    const s_map< s_uint, s_ptr<Obstacle> >& Model::GetObstacleList() const
    {
        return lObstacleList_;
    }

    void Model::SetMaterial( s_refptr<Material> pMat )
    {
        pMaterial_ = pMat;
        for (uint i = 0; i < pEntity_->getNumSubEntities(); ++i)
        {
            pEntity_->getSubEntity(i)->setMaterialName(pMat->GetOgreMaterialName().Get());
        }
    }

    void Model::SetCustomShaderParameter( const s_uint& uiID, const Ogre::Vector4& mVec )
    {
        for (uint i = 0; i < pEntity_->getNumSubEntities(); ++i)
        {
            pEntity_->getSubEntity(i)->setCustomParameter(uiID.Get(), mVec);
        }
    }

    const AxisAlignedBox& Model::GetBoundingBox() const
    {
        return mBoundingBox_;
    }

    s_wptr<Material> Model::GetMaterial()
    {
        return pMaterial_;
    }

    const s_map< s_uint, s_ptr<ModelPart> >& Model::GetModelPartList() const
    {
        return lModelPartList_;
    }

    void Model::Update(const s_float& fDelta)
    {
        pAnimMgr_->Update(fDelta);
    }


    ModelMaterial::ModelMaterial()
    {
    }

    ModelMaterial::ModelMaterial( s_ptr<XML::Block> pBlock )
    {
        s_ptr<XML::Block> pMaterialBlock;
        foreach_block (pMaterialBlock, pBlock)
        {
            s_int iID = s_int(pMaterialBlock->GetAttribute("subMeshID"));
            s_int iEntityID = s_int(pMaterialBlock->GetAttribute("subEntityID"));
            if (iID < 0)
                iID = -1;
            if (iEntityID < 0)
                iEntityID = -1;

            MaterialDefinition mMatDef;
            s_ptr<XML::Block> pDiffuseBlock = pMaterialBlock->GetRadioBlock();
            if (pDiffuseBlock->GetName() == "DiffuseColor")
            {
                mMatDef.SetColor(Color(
                    s_float(pDiffuseBlock->GetAttribute("a")),
                    s_float(pDiffuseBlock->GetAttribute("r")),
                    s_float(pDiffuseBlock->GetAttribute("g")),
                    s_float(pDiffuseBlock->GetAttribute("b"))
                ));
            }
            else
            {
                mMatDef.SetTextureFile(pDiffuseBlock->GetAttribute("file"));
                mMatDef.SetAlphaReject(s_bool(pDiffuseBlock->GetAttribute("alphaReject")));
            }

            AddMaterialDefinition(mMatDef, iID, iEntityID);
        }
    }

    void ModelMaterial::AddMaterialDefinition( const MaterialDefinition& mMatDef, const s_int& iSubMesh, const s_int& iSubEntity )
    {
        lMaterialList_[iSubMesh][iSubEntity] = mMatDef;
    }

    void ModelMaterial::Clear( const s_int& iSubMesh, const s_int& iSubEntity )
    {
        if (iSubMesh == -1)
            lMaterialList_.Clear();
        else
        {
            if (iSubEntity == -1)
                lMaterialList_.Erase(iSubMesh);
            else
                lMaterialList_[iSubMesh].Erase(iSubEntity);
        }
    }

    void ModelMaterial::ApplyOn( s_wptr<Model> pModel, const s_bool& bPostProcess ) const
    {
        if (s_refptr<Model> pLocked = pModel.Lock())
        {
            s_map< s_int, s_map<s_int, MaterialDefinition> >::const_iterator iterSubMesh;
            foreach (iterSubMesh, lMaterialList_)
            {
                s_map<s_int, MaterialDefinition>::const_iterator iterSubEntity;
                foreach (iterSubEntity, iterSubMesh->second)
                {
                    s_refptr<Material> pMat = iterSubEntity->second.CreateMaterial(bPostProcess);

                    if (iterSubMesh->first == -1)
                    {
                        pLocked->SetMaterial(pMat);
                    }
                    else
                    {
                        s_ptr<ModelPart> pPart = pLocked->GetModelPart(s_uint(iterSubMesh->first));
                        if (pPart)
                        {
                            if (iterSubEntity->first == -1)
                            {
                                pPart->SetMaterial(pMat);
                            }
                            else
                            {
                                pPart->SetMaterial(pMat, s_uint(iterSubEntity->first));
                            }
                        }
                    }
                }
            }
        }
    }

    s_str ModelMaterial::Serialize() const
    {
        s_map< s_int, s_map<s_int, MaterialDefinition> >::const_iterator iterSubMesh;
        s_str sString = "<Materials>\n";
        foreach (iterSubMesh, lMaterialList_)
        {
            s_map<s_int, MaterialDefinition>::const_iterator iterSubEntity;
            foreach (iterSubEntity, iterSubMesh->second)
            {
                const MaterialDefinition& mMatDef = iterSubEntity->second;
                sString += "<Material subMeshID=\""+iterSubMesh->first+"\""
                           " subEntityID=\""+iterSubEntity->first+"\">\n";
                sString += mMatDef.Serialize();
                sString += "\n</Material>\n";
            }
        }
        sString += "</Materials>";

        return sString;
    }

    void ModelMaterial::SerializeIn( File& mFile ) const
    {
        s_map< s_int, s_map<s_int, MaterialDefinition> >::const_iterator iterSubMesh;
        mFile.WriteLine("<Materials>");
            mFile.AddTab();
            foreach (iterSubMesh, lMaterialList_)
            {
                s_map<s_int, MaterialDefinition>::const_iterator iterSubEntity;
                foreach (iterSubEntity, iterSubMesh->second)
                {
                    const MaterialDefinition& mMatDef = iterSubEntity->second;
                    mFile.WriteLine("<Material subMeshID=\""+iterSubMesh->first+
                                    "\" subEntityID=\""+iterSubEntity->first+"\">");
                    mFile.AddTab();
                        mMatDef.SerializeIn(mFile);
                    mFile.RemoveTab();
                    mFile.WriteLine("</Material>");
                }
            }
            mFile.RemoveTab();
        mFile.WriteLine("</Materials>");
    }
}
