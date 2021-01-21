/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*         ManualModelData source         */
/*                                        */
/*                                        */

#include "model/frost_manualmodeldata.h"
#include "model/frost_animmanager.h"
#include "model/frost_model.h"
#include "material/frost_materialmanager.h"
#include "material/frost_material.h"

#include <OgreBone.h>
#include <OgreMeshManager.h>
#include <OgreSubMesh.h>
#include <OgreSkeletonManager.h>
#include <OgreSkeleton.h>
#include <OgreAnimation.h>

/** \cond NOT_REMOVE_FROM_DOC
*/
struct AnimData
{
    Ogre::Vector3 mTranslation;
    Ogre::Vector3 mScaling;
    Ogre::Quaternion mRotation;
};
/** \endcond
*/

namespace Frost
{
    ManualModelData::ManualModelData( const ManualModel::Model& mModel, const s_str& sName ) :
        ModelData(sName)
    {
        // The main mesh
        pMesh_ = Ogre::MeshManager::getSingletonPtr()->createManual(sModelName_.Get(), "Frost").get();

        if (!mModel.lAnimList.IsEmpty())
        {
            // Create the skeleton
            Ogre::SkeletonPtr pSkel = Ogre::SkeletonManager::getSingleton().create(
                (sModelName_ + "_skel").Get(), "Frost", true
            );

            // Load animation sub sequences and get total animation time
            for (uint i = 0; i < mModel.lAnimList.GetSize().Get(); ++i)
            {
                const ManualModel::Animation& mMA = mModel.lAnimList[i];

                MeshAnimation mA;
                mA.uiID = mMA.uiID;
                mA.uiStart = mMA.uiBegin;
                mA.uiEnd = mMA.uiEnd;
                mA.bLoop = mMA.bLoop;

                float fLength = (mMA.uiEnd - mMA.uiBegin).Get()/1000.0f;

                // Create the main animation sequence
                mA.sName = "anim_" + s_str::Convert(s_uint(i));
                mA.pAnim = pSkel->createAnimation(mA.sName.Get(), fLength);

                lAnimList_[i] = mA;
            }

            // Load bones
            for (uint i = 0; i < mModel.lBoneList.GetSize().Get(); ++i)
            {
                const ManualModel::Bone& mB = mModel.lBoneList[i];

                Ogre::Bone* pBone;
                if (!mB.uiParent.IsValid())
                    pBone = pSkel->createBone(i);
                else
                    pBone = pSkel->getBone(mB.uiParent.Get())->createChild(i);

                pBone->setPosition(Vector::FrostToOgre(mB.mPos));

                s_map<s_uint, Vector>::const_iterator iterTrans = mB.lTranslationAnimation.Begin();
                s_map<s_uint, Vector>::const_iterator iterScale = mB.lScalingAnimation.Begin();
                s_map<s_uint, Ogre::Quaternion>::const_iterator iterRot = mB.lRotationAnimation.Begin();

                s_map<uint, AnimData> lData;
                for (uint j = 0; j < mB.lTimeline.GetSize().Get(); ++j, ++iterTrans, ++iterScale, ++iterRot)
                {
                    s_uint uiTime = mB.lTimeline[j];

                    AnimData mAD;
                    mAD.mTranslation = Vector::FrostToOgre(iterTrans->second);
                    mAD.mScaling = Vector::FrostToOgre(iterScale->second);
                    mAD.mRotation = iterRot->second;

                    lData[uiTime.Get()] = mAD;
                }

                // Create track
                uint  ulTime;
                float fKeyTime;
                s_map<s_uint, MeshAnimation>::iterator iterAnim;
                foreach (iterAnim, lAnimList_)
                {
                    MeshAnimation& mMA = iterAnim->second;
                    Ogre::NodeAnimationTrack* pTrack = mMA.pAnim->createNodeTrack(i, pBone);

                    s_map<uint, AnimData>::iterator iterData;
                    for (iterData = lData.LowerBound(mMA.uiStart.Get()); iterData != lData.End(); ++iterData)
                    {
                        ulTime = iterData->first;
                        if (ulTime <= mMA.uiEnd.Get())
                        {
                            fKeyTime = (ulTime - mMA.uiStart.Get())/1000.0f;
                            Ogre::TransformKeyFrame* pKeyFrame = pTrack->createNodeKeyFrame(fKeyTime);

                            pKeyFrame->setTranslate(iterData->second.mTranslation);
                            pKeyFrame->setScale(iterData->second.mScaling);
                            pKeyFrame->setRotation(iterData->second.mRotation);
                        }
                        else
                            break;
                    }
                }
            }

            pMesh_->_notifySkeleton(pSkel);
        }

        // Geometry
        for (uint i = 0; i < mModel.lSubMeshList.GetSize().Get(); ++i)
        {
            const ManualModel::SubMesh& mMSM = mModel.lSubMeshList[i];

            Ogre::SubMesh* pSub = pMesh_->createSubMesh(s_str::Convert(mMSM.uiID, 4).Get());

            if (!lModelPartList_.Find(mMSM.uiID))
                lModelPartList_[mMSM.uiID] = new ModelPart(nullptr, mMSM.uiID);

            lModelPartList_[mMSM.uiID]->AddSubEntity(i);

            pSub->useSharedVertices = false;

            uint nVertex = mMSM.lVertexList.GetSize().Get();
            float*  lSubVertices = new float[nVertex*6];
            float*  lSubUVs = new float[nVertex*2];

            for (uint j = 0; j < nVertex; ++j)
            {
                lSubVertices[j*6 + 0] = mMSM.lVertexList[j].mPos.X().Get();
                lSubVertices[j*6 + 1] = mMSM.lVertexList[j].mPos.Y().Get();
                lSubVertices[j*6 + 2] = mMSM.lVertexList[j].mPos.Z().Get();

                lSubVertices[j*6 + 3] = mMSM.lVertexList[j].mNormal.X().Get();
                lSubVertices[j*6 + 4] = mMSM.lVertexList[j].mNormal.Y().Get();
                lSubVertices[j*6 + 5] = mMSM.lVertexList[j].mNormal.Z().Get();

                lSubUVs[j*2 + 0] = mMSM.lVertexList[j].fU.Get();
                lSubUVs[j*2 + 1] = mMSM.lVertexList[j].fV.Get();

                Ogre::VertexBoneAssignment mVBA;
                mVBA.vertexIndex = j;
                for (uint k = 0; k < 4 ; ++k)
                {
                    mVBA.boneIndex = mMSM.lVertexList[j].lBoneIndexList[k].Get();
                    mVBA.weight = mMSM.lVertexList[j].lBoneWeightList[k].Get();
                    pSub->addBoneAssignment(mVBA);
                }
            }

            uint nIndice = mMSM.lTriangleList.GetSize().Get()*3;
            ushort* lSubIndices = new ushort[nIndice];

            for (uint j = 0; j < mMSM.lTriangleList.GetSize().Get(); ++j)
            {
                lSubIndices[3*j+0] = mMSM.lTriangleList[j].lVertexList[0].Get();
                lSubIndices[3*j+1] = mMSM.lTriangleList[j].lVertexList[1].Get();
                lSubIndices[3*j+2] = mMSM.lTriangleList[j].lVertexList[2].Get();
            }

            CreateBuffers_(mMSM.uiID.Get(), pSub, nVertex, nIndice, lSubVertices, lSubUVs, lSubIndices);

            pSub->_compileBoneAssignments();

            // Use default material
            pSub->setMaterialName(
                MaterialManager::GetSingleton()->GetDefault3D()->GetOgreMaterialName().Get()
            );

            delete[] lSubVertices;
            delete[] lSubUVs;
            delete[] lSubIndices;
        }

        pMesh_->_setBounds(AxisAlignedBox::FrostToOgre(mModel.mBoundingBox));

        pMesh_->_setBoundingSphereRadius(
            0.5f*std::max(
                mModel.mBoundingBox.GetMax().X().Get()-mModel.mBoundingBox.GetMin().X().Get(),
                std::max(
                    mModel.mBoundingBox.GetMax().Y().Get()-mModel.mBoundingBox.GetMin().Y().Get(),
                    mModel.mBoundingBox.GetMax().Z().Get()-mModel.mBoundingBox.GetMin().Z().Get()
                )
            )
        );

        // Notify Mesh object that it has been loaded
        pMesh_->load();
    }
}
