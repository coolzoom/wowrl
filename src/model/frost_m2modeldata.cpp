/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            ModelData source            */
/*                                        */
/*                                        */

#include "model/frost_m2modeldata.h"

#include "model/frost_modelmanager.h"
#include "model/frost_model.h"
#include "model/frost_animmanager.h"
#include "model/frost_m2modelstructs.h"
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

const float SCALE = 1.0f;

inline float ShortToFloat( const short& iValue )
{
    return (iValue > 0 ? iValue-32767 : iValue+32767)/32767.0f;
}

bool IsAnimated( char* sBuffer, ModelHeader mHeader )
{
    // Note : Copy/Pasted from WoWModelViewer
    // URL  : http://www.wowmodelviewer.org/

    // see if we have any animated bones
    ModelBoneDef *bo = (ModelBoneDef*)(sBuffer + mHeader.ofsBones);

    bool animGeometry = false;
    bool animBones = false;
    bool ind = false;

    ModelVertex *verts = (ModelVertex*)(sBuffer + mHeader.ofsVertices);
    for (uint i = 0; i < mHeader.nVertices && !animGeometry; i++)
    {
        for (int b = 0; b < 4; b++)
        {
            if (verts[i].weights[b] > 0)
            {
                ModelBoneDef &bb = bo[verts[i].bones[b]];
                if (bb.translation.type || bb.rotation.type || bb.scaling.type || (bb.flags&8))
                {
                    if (bb.flags & 8)
                    {
                        // if we have billboarding, the model will need per-instance animation
                        ind = true;
                    }
                    animGeometry = true;
                    break;
                }
            }
        }
    }

    if (animGeometry)
        animBones = true;
    else
    {
        for (uint i = 0; i < mHeader.nBones; i++)
        {
            ModelBoneDef &bb = bo[i];
            if (bb.translation.type || bb.rotation.type || bb.scaling.type)
            {
                animBones = true;
                break;
            }
        }
    }

    bool animTextures = mHeader.nTexAnims > 0;

    bool animMisc = mHeader.nCameras > 0          || // why waste time, pretty much all models with cameras need animation
                    mHeader.nLights > 0           || // same here
                    mHeader.nParticleEmitters > 0 ||
                    mHeader.nRibbonEmitters > 0;

    if (animMisc)
        animBones = true;

    // animated colors
    if (mHeader.nColors)
    {
        ModelColorDef *cols = (ModelColorDef*)(sBuffer + mHeader.ofsColors);
        for (uint i = 0; i < mHeader.nColors; i++)
        {
            if (cols[i].color.type != 0 || cols[i].opacity.type != 0)
            {
                animMisc = true;
                break;
            }
        }
    }

    // animated opacity
    if (mHeader.nTransparency && !animMisc)
    {
        ModelTransDef *trs = (ModelTransDef*)(sBuffer + mHeader.ofsTransparency);
        for (uint i = 0; i < mHeader.nTransparency; i++)
        {
            if (trs[i].trans.type != 0)
            {
                animMisc = true;
                break;
            }
        }
    }

    // guess not...
    return (animGeometry || animTextures || animMisc);
}

/** \cond NOT_REMOVE_FROM_DOC
*/
struct AnimData
{
    AnimData()
    {
        bTranslation = bScaling = bRotation = false;
    }

    Frost::s_bool bTranslation;
    Frost::s_bool bScaling;
    Frost::s_bool bRotation;
};
/** \endcond
*/

/// Linear interpolation in an std::map
template<class Elem>
Elem MapLERP(const std::map<uint, Elem>& lMap, uint uiValue)
{
    typename std::map<uint, Elem>::const_iterator iterFirst, iterSecond;
    iterSecond = lMap.lower_bound(uiValue);
    if (iterSecond == lMap.end())
    {
        --iterSecond;
        return iterSecond->second;
    }
    else if (iterSecond == lMap.begin())
    {
        return iterSecond->second;
    }
    else
    {
        iterFirst = iterSecond;
        --iterFirst;

        double dCoef = (uiValue - iterFirst->first);
        dCoef /= (iterSecond->first - iterFirst->first);

        return iterFirst->second +
               dCoef*(iterSecond->second - iterFirst->second);
    }
}

namespace Frost
{
    const s_str M2ModelData::CLASS_NAME = "M2ModelData";

    M2ModelData::M2ModelData( const s_str& sFile ) :
        ModelData(sFile)
    {
        // Note : Inspired from WoWModelViewer
        // URL  : http://www.wowmodelviewer.org/

        File mFile(sFile, File::I, true);
        if (!mFile.IsValid())
        {
            throw ModelLoadingException(CLASS_NAME, "Model file \""+sFile+"\" couldn't be loaded.");
        }

        s_uint uiLength = mFile.GetLength();

        if (uiLength < sizeof(ModelHeader))
        {
            throw ModelLoadingException(CLASS_NAME, "Model file \""+sFile+"\" must be corrupted (incorrect size).");
        }

        char* sBuffer = new char[uiLength.Get()];
        mFile.Read(sBuffer, uiLength);
        mFile.Close();

        // Parse the header
        ModelHeader mHeader;
        memcpy(&mHeader, sBuffer, sizeof(ModelHeader));

        // Error check
        if (mHeader.id[0] != 'M' && mHeader.id[1] != 'D' && mHeader.id[2] != '2' && mHeader.id[3] != '0')
        {
            throw ModelLoadingException(CLASS_NAME,
                "\""+sFile+"\" has wrong model type (MD20 expected, got "+s_str(mHeader.id)+")."
            );
        }

        if (mHeader.version[0] != 4 || mHeader.version[1] != 1 || mHeader.version[2] != 0 || mHeader.version[3] != 0)
        {
            throw ModelLoadingException(CLASS_NAME,
                "\""+sFile+"\" has wrong model version (4100 expected, got "+
                s_int(s_uchar(mHeader.version[0]))+""+s_int(s_uchar(mHeader.version[1]))+""+
                s_int(s_uchar(mHeader.version[2]))+""+s_int(s_uchar(mHeader.version[3]))+")."
            );
        }

        // The main mesh
        pMesh_ = Ogre::MeshManager::getSingletonPtr()->createManual(sModelName_.Get(), "Frost").get();

        // We first load animation data
        bool bAnimated = IsAnimated(sBuffer, mHeader);

        if (bAnimated)
        {
            // Create the skeleton
            Ogre::SkeletonPtr pSkel = Ogre::SkeletonManager::getSingleton().create(
                (sModelName_ + "_skel").Get(), "Frost", true
            );

            // Load animation sub sequences and get total animation time
            ModelAnimation* lAnims = (ModelAnimation*)(sBuffer + mHeader.ofsAnimations);
            for (uint i = 0; i < mHeader.nAnimations; i++)
            {
                MeshAnimation mMA;
                mMA.uiID = lAnims[i].animID;
                mMA.bLoop = (lAnims[i].loopType == 0);
                mMA.uiStart = lAnims[i].timeStart;
                mMA.uiEnd = lAnims[i].timeEnd;

                float fLength = lAnims[i].timeEnd - lAnims[i].timeStart;

                // Create the main animation sequence
                mMA.sName = "anim_" + s_str(s_uint(i));
                mMA.pAnim = pSkel->createAnimation(mMA.sName.Get(), fLength/1000.0f);

                lAnimList_[i] = mMA;
            }

            // Load bones
            ModelBoneDef* lBones = (ModelBoneDef*)(sBuffer + mHeader.ofsBones);
            for (uint i = 0; i < mHeader.nBones; i++)
            {
                Ogre::Bone* pBone;
                Ogre::Vector3 mParentPos;
                Ogre::Vector3 mBonePos;

                if (lBones[i].parent == -1)
                {
                    // This is a root bone
                    pBone = pSkel->createBone(i);

                    mParentPos = Ogre::Vector3(0, 0, 0);
                }
                else
                {
                    // This is a child bone
                    Ogre::Bone* pParent = pSkel->getBone(lBones[i].parent);
                    pBone = pParent->createChild(i);

                    mParentPos = Ogre::Vector3(
                       -lBones[lBones[i].parent].pivot[1]*SCALE,
                        lBones[lBones[i].parent].pivot[2]*SCALE,
                       -lBones[lBones[i].parent].pivot[0]*SCALE
                    );
                }

                // Position is relative to parent in Ogre
                mBonePos = Ogre::Vector3(
                   -lBones[i].pivot[1]*SCALE,
                    lBones[i].pivot[2]*SCALE,
                   -lBones[i].pivot[0]*SCALE
                );
                pBone->setPosition(mBonePos - mParentPos);

                // Pre-load animation data
                map<uint, Ogre::Vector3> lTranslation;
                map<uint, Ogre::Vector3> lScaling;
                map<uint, Ogre::Quaternion> lRotation;

                // Pre-load animation data
                map<uint, AnimData> lData;
                // Translation
                uint* lTimes = (uint*)(sBuffer + lBones[i].translation.ofsTimes);
                float* lValues = (float*)(sBuffer + lBones[i].translation.ofsKeys);

                for (uint j = 0; j < lBones[i].translation.nTimes; j++)
                {
                    // (x, y, z)    =>    (-y, z, -x)
                    lTranslation[lTimes[j]] = Ogre::Vector3(
                       -lValues[j*3+1]*SCALE,
                        lValues[j*3+2]*SCALE,
                       -lValues[j*3+0]*SCALE
                    );
                    lData[lTimes[j]].bTranslation = true;
                }

                // Scaling
                lTimes = (uint*)(sBuffer + lBones[i].scaling.ofsTimes);
                lValues = (float*)(sBuffer + lBones[i].scaling.ofsKeys);

                for (uint j = 0; j < lBones[i].scaling.nTimes; j++)
                {
                    // (x, y, z)    =>    (y, z, -x)
                    lScaling[lTimes[j]] = Ogre::Vector3(
                        lValues[j*3+1],
                        lValues[j*3+2],
                       -lValues[j*3+0]
                    );
                    lData[lTimes[j]].bScaling = true;
                }

                // Rotation
                lTimes = (uint*)(sBuffer + lBones[i].rotation.ofsTimes);
                short* lSValues = (short*)(sBuffer + lBones[i].rotation.ofsKeys);

                for (uint j = 0; j < lBones[i].rotation.nTimes; j++)
                {
                    // (x, y, z, w)    =>    (w, -y, z, -x)
                    lRotation[lTimes[j]] = Ogre::Quaternion(
                        ShortToFloat(lSValues[j*4+3]),
                       -ShortToFloat(lSValues[j*4+1]),
                        ShortToFloat(lSValues[j*4+2]),
                       -ShortToFloat(lSValues[j*4+0])
                    );
                    lData[lTimes[j]].bRotation = true;
                }

                // Create track
                uint  ulTime;
                float fKeyTime;
                s_map<s_uint, MeshAnimation>::iterator iterAnim;
                foreach (iterAnim, lAnimList_)
                {
                    MeshAnimation* pMA = &iterAnim->second;
                    Ogre::NodeAnimationTrack* pTrack = pMA->pAnim->createNodeTrack(i, pBone);

                    map<uint, AnimData>::iterator iterAnim = lData.lower_bound(pMA->uiStart.Get());
                    for (; iterAnim != lData.end(); iterAnim++)
                    {
                        ulTime = iterAnim->first;
                        if (ulTime <= pMA->uiEnd.Get())
                        {
                            fKeyTime = (ulTime - pMA->uiStart.Get())/1000.0f;
                            Ogre::TransformKeyFrame* pKeyFrame = pTrack->createNodeKeyFrame(fKeyTime);

                            Ogre::Vector3 mTranslation;
                            Ogre::Vector3 mScaling(1.0f, 1.0f, 1.0f);
                            Ogre::Quaternion mRotation;

                            if (iterAnim->second.bTranslation)
                                mTranslation = lTranslation[ulTime];
                            else if (!lTranslation.empty())
                                mTranslation = MapLERP(lTranslation, ulTime);

                            if (iterAnim->second.bScaling)
                                mScaling = lScaling[ulTime];
                            else if (!lScaling.empty())
                                mScaling = MapLERP(lScaling, ulTime);

                            if (iterAnim->second.bRotation)
                                mRotation = lRotation[ulTime];
                            else if (!lRotation.empty())
                                mRotation = MapLERP(lRotation, ulTime);

                            pKeyFrame->setTranslate(mTranslation);
                            pKeyFrame->setScale(mScaling);
                            pKeyFrame->setRotation(mRotation);
                        }
                        else
                            break;
                    }
                }
            }

            pMesh_->_notifySkeleton(pSkel);
        }

        // Then, geometry
        ModelVertex* lOrigVertices = (ModelVertex*)(sBuffer + mHeader.ofsVertices);
        uint uiParamNbr = 3+3;
        float* lVertices = new float[mHeader.nVertices*uiParamNbr];
        short* lBonesInd = new short[mHeader.nVertices*4];
        float* lBonesWgt = new float[mHeader.nVertices*4];
        float* lUVs = new float[mHeader.nVertices*2];

        s_float fXMax(s_float::FLOAT_INF_MINUS);
        s_float fYMax(s_float::FLOAT_INF_MINUS);
        s_float fZMax(s_float::FLOAT_INF_MINUS);
        s_float fXMin(s_float::FLOAT_INF_PLUS);
        s_float fYMin(s_float::FLOAT_INF_PLUS);
        s_float fZMin(s_float::FLOAT_INF_PLUS);

        for (uint i = 0; i < mHeader.nVertices; i++)
        {
            lVertices[i*uiParamNbr+0] = -lOrigVertices[i].pos[1]*SCALE;
            lVertices[i*uiParamNbr+1] = lOrigVertices[i].pos[2]*SCALE;
            lVertices[i*uiParamNbr+2] = -lOrigVertices[i].pos[0]*SCALE;

            lVertices[i*uiParamNbr+3] = -lOrigVertices[i].normal[1];
            lVertices[i*uiParamNbr+4] = lOrigVertices[i].normal[2];
            lVertices[i*uiParamNbr+5] = -lOrigVertices[i].normal[0];

            lUVs[i*2+0] = lOrigVertices[i].texcoords[0];
            lUVs[i*2+1] = lOrigVertices[i].texcoords[1];

            lBonesInd[i*4+0] = lOrigVertices[i].bones[0];
            lBonesInd[i*4+1] = lOrigVertices[i].bones[1];
            lBonesInd[i*4+2] = lOrigVertices[i].bones[2];
            lBonesInd[i*4+3] = lOrigVertices[i].bones[3];

            lBonesWgt[i*4+0] = lOrigVertices[i].weights[0]/255.0f;
            lBonesWgt[i*4+1] = lOrigVertices[i].weights[1]/255.0f;
            lBonesWgt[i*4+2] = lOrigVertices[i].weights[2]/255.0f;
            lBonesWgt[i*4+3] = lOrigVertices[i].weights[3]/255.0f;

            fXMin = s_float::Min(fXMin, lVertices[i*uiParamNbr+0]);
            fXMax = s_float::Max(fXMax, lVertices[i*uiParamNbr+0]);
            fYMin = s_float::Min(fYMin, lVertices[i*uiParamNbr+1]);
            fYMax = s_float::Max(fYMax, lVertices[i*uiParamNbr+1]);
            fZMin = s_float::Min(fZMin, lVertices[i*uiParamNbr+2]);
            fZMax = s_float::Max(fZMax, lVertices[i*uiParamNbr+2]);
        }

        ModelView* lView = (ModelView*)(sBuffer + mHeader.ofsViews);

        // Worst LOD
        //int v = 0;
        // Best LOD
        uint uiLOD = mHeader.nViews-1;

        // Get vertex indices
        ushort* lIndexLookup = (ushort*)(sBuffer + lView[uiLOD].ofsIndex);
        ushort* lTriangles = (ushort*)(sBuffer + lView[uiLOD].ofsTris);
        ushort* lGIndices = new ushort[lView[uiLOD].nTris];
        for (uint i = 0; i < lView[uiLOD].nTris; i++)
        {
            lGIndices[i] = lIndexLookup[lTriangles[i]];
        }

        // Get submeshes
        uint uiMeshNbr = lView[uiLOD].nSub;
        ModelSubMesh* lSubMesh = (ModelSubMesh*)(sBuffer + lView[uiLOD].ofsSub);
        for (uint i = 0; i < uiMeshNbr; i++)
        {
            Ogre::SubMesh* pSub = pMesh_->createSubMesh(s_str(s_uint(lSubMesh[i].id), 4).Get());

            if (!lModelPartList_.Find(lSubMesh[i].id))
                lModelPartList_[lSubMesh[i].id] = new ModelPart(nullptr, lSubMesh[i].id);

            lModelPartList_[lSubMesh[i].id]->AddSubEntity(i);

            pSub->useSharedVertices = false;

            uint uiIndexNbr = lSubMesh[i].nTris;

            ushort* lSubIndices = new ushort[uiIndexNbr];
            map<uint, uint> lLocalVertices;
            map<uint, uint> lIndexMap;
            map<uint, uint> lReversedIndexMap;
            map<uint, uint>::iterator iter;

            // Build a temporary index list
            for (uint j = 0; j < uiIndexNbr; j++)
            {
                uint uiInd = lSubIndices[j] = lGIndices[lSubMesh[i].ofsTris+j];

                iter = lLocalVertices.find(uiInd);
                if (iter == lLocalVertices.end())
                    lLocalVertices.insert(make_pair(uiInd, uiInd));
            }

            uint uiVertexNbr = lLocalVertices.size();

            float* lSubVertices = new float[uiVertexNbr*uiParamNbr];
            float* lSubUVs = new float[uiVertexNbr*2];

            // Build the vertex list
            map<uint, uint>::iterator iterV;
            uint k = 0;
            foreach (iterV, lLocalVertices)
            {
                uint uiInd = iterV->second;
                // Position
                lSubVertices[k*uiParamNbr+0] = lVertices[uiInd*uiParamNbr+0];
                lSubVertices[k*uiParamNbr+1] = lVertices[uiInd*uiParamNbr+1];
                lSubVertices[k*uiParamNbr+2] = lVertices[uiInd*uiParamNbr+2];

                // Normals
                lSubVertices[k*uiParamNbr+3] = lVertices[uiInd*uiParamNbr+3];
                lSubVertices[k*uiParamNbr+4] = lVertices[uiInd*uiParamNbr+4];
                lSubVertices[k*uiParamNbr+5] = lVertices[uiInd*uiParamNbr+5];

                // UVs
                lSubUVs[k*2+0] = lUVs[uiInd*2+0];
                lSubUVs[k*2+1] = lUVs[uiInd*2+1];

                lIndexMap[uiInd] = k;
                lReversedIndexMap[k] = uiInd;
                k++;
            }

            // Update the index list
            for (k = 0; k < uiIndexNbr; k++)
            {
                lSubIndices[k] = lIndexMap[lSubIndices[k]];
            }

            CreateBuffers_(pSub, uiVertexNbr, uiIndexNbr, lSubVertices, lSubUVs, lSubIndices);

            // Use default material
            pSub->setMaterialName(
                MaterialManager::GetSingleton()->GetDefault3D()->GetOgreMaterialName().Get()
            );

            // Then create vertex/bone assignements
            for (uint j = 0; j < uiVertexNbr; j++)
            {
                Ogre::VertexBoneAssignment mVBA;
                mVBA.vertexIndex = j;
                uint iGIndex = lReversedIndexMap[j];
                for (uint k = 0; k < 4 ; k++)
                {
                    mVBA.boneIndex = lBonesInd[iGIndex*4+k];
                    mVBA.weight = lBonesWgt[iGIndex*4+k];
                    pSub->addBoneAssignment(mVBA);
                }
            }

            pSub->_compileBoneAssignments();

            delete[] lSubIndices;
            delete[] lSubVertices;
            delete[] lSubUVs;
        }

        // Set bounding information for culling and collisions
        pMesh_->_setBounds(Ogre::AxisAlignedBox(
            fXMin.Get(), fYMin.Get(), fZMin.Get(),
            fXMax.Get(), fYMax.Get(), fZMax.Get()
        ));

        pMesh_->_setBoundingSphereRadius(
            std::max(
                fXMax-fXMin,
                std::max(fYMax-fYMin, fZMax-fZMin)
            ).Get()/2.0f
        );

        // Notify Mesh object that it has been loaded
        pMesh_->load();

        delete[] lVertices;
        delete[] lUVs;
        delete[] lBonesInd;
        delete[] lBonesWgt;
        delete[] lGIndices;
        delete[] sBuffer;
    }

    M2ModelData::~M2ModelData()
    {
    }
}
