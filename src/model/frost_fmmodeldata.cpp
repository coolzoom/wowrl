/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            ModelData source            */
/*                                        */
/*                                        */

#include "model/frost_fmmodeldata.h"

#include "model/frost_modelmanager.h"
#include "model/frost_model.h"
#include "model/frost_animmanager.h"
#include "model/frost_fmmodelstructs.h"
#include "material/frost_materialmanager.h"
#include "material/frost_material.h"

#include <OgreBone.h>
#include <OgreMeshManager.h>
#include <OgreSubMesh.h>
#include <OgreSkeletonManager.h>
#include <OgreSkeleton.h>
#include <OgreAnimation.h>

#include <utils/frost_utils_file.h>

using namespace std;

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
    const s_str FMModelData::CLASS_NAME = "FMModelData";

    FMModelData::FMModelData( const s_str& sFile ) :
        ModelData(sFile)
    {
        File mFile(sFile, File::I, true);
        if (!mFile.IsValid())
        {
            throw Exception(CLASS_NAME, "Model file \""+sFile+"\" couldn't be loaded.");
        }

        if (mFile.GetLength() < sizeof(ModelHeader))
        {
            throw Exception(CLASS_NAME, "Model file \""+sFile+"\" must be corrupted (incorrect size).");
        }

        ModelHeader mHeader;
        mFile.Read(mHeader);

        if (mHeader.id[0] != 'F' || mHeader.id[1] != 'M')
        {
            throw Exception(CLASS_NAME,
                "\""+sFile+"\" has wrong model type (FM expected, got "+s_str(mHeader.id)+")."
            );
        }

        if (mHeader.version[0] != '0' || mHeader.version[1] != '0' || mHeader.version[2] != '0' || mHeader.version[3] != '1')
        {
            throw Exception(CLASS_NAME,
                "\""+sFile+"\" has wrong model version (0001 expected, got "+
                s_uchar(mHeader.version[0])+""+s_uchar(mHeader.version[1])+""+
                s_uchar(mHeader.version[2])+""+s_uchar(mHeader.version[3])+")."
            );
        }

        // The main mesh
        pMesh_ = Ogre::MeshManager::getSingletonPtr()->createManual(sModelName_.Get(), "Frost").get();

        if (mHeader.nAnimation != 0)
        {
            // Create the skeleton
            Ogre::SkeletonPtr pSkel = Ogre::SkeletonManager::getSingleton().create(
                (sModelName_ + "_skel").Get(), "Frost", true
            );

            // Load animation sub sequences and get total animation time
            for (uint i = 0; i < mHeader.nAnimation; ++i)
            {
                ModelAnimation mMA;
                mFile.Read(mMA);

                MeshAnimation mA;
                mA.uiID = mMA.animID;
                mA.uiStart = mMA.timeStart;
                mA.uiEnd = mMA.timeEnd;
                mA.bLoop = mMA.loop;

                float fLength = mMA.timeEnd - mMA.timeStart;

                // Create the main animation sequence
                mA.sName = "anim_" + s_str(s_uint(i));
                mA.pAnim = pSkel->createAnimation(mA.sName.Get(), fLength/1000.0f);

                lAnimList_[i] = mA;
            }

            // Load bones
            ModelBone* lBoneList = new ModelBone[mHeader.nBone];
            for (uint i = 0; i < mHeader.nBone; ++i)
            {
                mFile.Read(lBoneList[i]);

                Ogre::Bone* pBone;
                if (lBoneList[i].parent == -1)
                    pBone = pSkel->createBone(i);
                else
                    pBone = pSkel->getBone(lBoneList[i].parent)->createChild(i);

                pBone->setPosition(Ogre::Vector3(lBoneList[i].pivot));

                ModelAnimationKey* lKeyList = new ModelAnimationKey[lBoneList[i].nKey];
                mFile.Read(lKeyList, lBoneList[i].nKey*sizeof(ModelAnimationKey));

                s_map<uint, AnimData> lData;
                for (uint j = 0; j < lBoneList[i].nKey; ++j)
                {
                    ModelAnimationKey& mKey = lKeyList[j];

                    AnimData mAD;
                    mAD.mTranslation = Ogre::Vector3(mKey.translation);
                    mAD.mScaling = Ogre::Vector3(mKey.scaling);
                    mAD.mRotation = Ogre::Quaternion(mKey.rotation);

                    lData[mKey.key] = mAD;
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

            delete[] lBoneList;

            pMesh_->_notifySkeleton(pSkel);
        }

        for (uint i = 0; i < mHeader.nSubMesh; ++i)
        {
            ModelSubMesh mMSM;
            mFile.Read(mMSM);

            Ogre::SubMesh* pSub = pMesh_->createSubMesh(s_str(s_uint(mMSM.id), 4).Get());

            if (!lModelPartList_.Find(mMSM.id))
                lModelPartList_[mMSM.id] = new ModelPart(nullptr, mMSM.id);

            lModelPartList_[mMSM.id]->AddSubEntity(i);

            pSub->useSharedVertices = false;

            float*  lSubVertices = new float[mMSM.nVertex*6];
            float*  lSubUVs = new float[mMSM.nVertex*2];

            ModelVertex* lVertexList = new ModelVertex[mMSM.nVertex];
            mFile.Read(lVertexList, mMSM.nVertex*sizeof(ModelVertex));

            for (uint j = 0; j < mMSM.nVertex; ++j)
            {
                lSubVertices[j*6 + 0] = lVertexList[j].pos[0];
                lSubVertices[j*6 + 1] = lVertexList[j].pos[1];
                lSubVertices[j*6 + 2] = lVertexList[j].pos[2];

                lSubVertices[j*6 + 3] = lVertexList[j].normal[0];
                lSubVertices[j*6 + 4] = lVertexList[j].normal[1];
                lSubVertices[j*6 + 5] = lVertexList[j].normal[2];

                lSubUVs[j*2 + 0] = lVertexList[j].texcoords[0];
                lSubUVs[j*2 + 1] = lVertexList[j].texcoords[1];

                Ogre::VertexBoneAssignment mVBA;
                mVBA.vertexIndex = j;
                for (uint k = 0; k < 4 ; ++k)
                {
                    mVBA.boneIndex = lVertexList[j].bones[k];
                    mVBA.weight = lVertexList[j].weights[k]/255.0f;
                    pSub->addBoneAssignment(mVBA);
                }
            }

            ushort* lSubIndices = new ushort[mMSM.nIndice];
            mFile.Read(lSubIndices, mMSM.nIndice*sizeof(ushort));

            CreateBuffers_(pSub, mMSM.nVertex, mMSM.nIndice, lSubVertices, lSubUVs, lSubIndices);

            pSub->_compileBoneAssignments();

            // Use default material
            pSub->setMaterialName(
                MaterialManager::GetSingleton()->GetDefault3D()->GetOgreMaterialName().Get()
            );

            delete[] lSubVertices;
            delete[] lSubUVs;
            delete[] lSubIndices;
            delete[] lVertexList;
        }

        pMesh_->_setBounds(Ogre::AxisAlignedBox(
            mHeader.bbox[0], mHeader.bbox[1], mHeader.bbox[2],
            mHeader.bbox[3], mHeader.bbox[4], mHeader.bbox[5]
        ));

        pMesh_->_setBoundingSphereRadius(
            0.5f*std::max(
                mHeader.bbox[3]-mHeader.bbox[0],
                std::max(
                    mHeader.bbox[4]-mHeader.bbox[1],
                    mHeader.bbox[5]-mHeader.bbox[2]
                )
            )
        );

        // Notify Mesh object that it has been loaded
        pMesh_->load();
    }

    FMModelData::~FMModelData()
    {
    }
}
