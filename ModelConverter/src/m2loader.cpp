/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#include "m2loader.h"
#include "m2modelstructs.h"

#include <string.h>

using namespace std;

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
    const s_str M2Loader::CLASS_NAME = "M2Loader";

    ModelData M2Loader::LoadModelData( const s_str& sFile )
    {
        ModelData mData;

        File mFile(sFile, File::I, true);
        if (!mFile.IsValid())
        {
            throw Exception(CLASS_NAME, "Model file \""+sFile+"\" couldn't be loaded.");
        }

        s_uint uiLength = mFile.GetLength();

        if (uiLength < sizeof(ModelHeader))
        {
            throw Exception(CLASS_NAME, "Model file \""+sFile+"\" must be corrupted (incorrect size).");
        }

        char* sBuffer = new char[uiLength.Get()];
        mFile.Read(sBuffer, uiLength);
        mFile.Close();

        // Parse the header
        ModelHeader mHeader;
        memcpy(&mHeader, sBuffer, sizeof(ModelHeader));

        // Error check
        if (mHeader.id[0] != 'M' || mHeader.id[1] != 'D' || mHeader.id[2] != '2' || mHeader.id[3] != '0')
        {
            throw Exception(CLASS_NAME,
                "\""+sFile+"\" has wrong model type (MD20 expected, got "+s_str(mHeader.id)+")."
            );
        }

        if (mHeader.version[0] != 4 || mHeader.version[1] != 1 || mHeader.version[2] != 0 || mHeader.version[3] != 0)
        {
            throw Exception(CLASS_NAME,
                "\""+sFile+"\" has wrong model version (4100 expected, got "+
                s_int(s_uchar(mHeader.version[0]))+""+s_int(s_uchar(mHeader.version[1]))+""+
                s_int(s_uchar(mHeader.version[2]))+""+s_int(s_uchar(mHeader.version[3]))+")."
            );
        }

        // We first load animation data
        bool bAnimated = IsAnimated(sBuffer, mHeader);

        if (bAnimated)
        {
            // Load animation sub sequences and get total animation time
            ModelAnimation* lAnims = (ModelAnimation*)(sBuffer + mHeader.ofsAnimations);
            mData.lAnimList.Reserve(mHeader.nAnimations);
            for (uint i = 0; i < mHeader.nAnimations; ++i)
            {
                Animation mA;
                mA.uiID = lAnims[i].animID;
                mA.bLoop = (lAnims[i].loopType == 0);
                mA.uiBegin = lAnims[i].timeStart;
                mA.uiEnd = lAnims[i].timeEnd;

                mData.lAnimList.PushBack(mA);
            }

            // Load bones
            ModelBoneDef* lBones = (ModelBoneDef*)(sBuffer + mHeader.ofsBones);
            mData.lBoneList.Reserve(mHeader.nBones);
            for (uint i = 0; i < mHeader.nBones; ++i)
            {
                Bone mB;

                if (lBones[i].parent == -1)
                {
                    mB.uiParent = s_uint::NaN;
                    mB.mPos = Vector(
                       -lBones[i].pivot[1],
                        lBones[i].pivot[2],
                       -lBones[i].pivot[0]
                    );
                }
                else
                {
                    mB.uiParent = lBones[i].parent;
                    mB.mPos = Vector(
                       -lBones[i].pivot[1],
                        lBones[i].pivot[2],
                       -lBones[i].pivot[0]
                    );
                }

                // Pre-load animation data
                map<uint, AnimData> lData;
                // Translation
                uint* lTimes = (uint*)(sBuffer + lBones[i].translation.ofsTimes);
                float* lValues = (float*)(sBuffer + lBones[i].translation.ofsKeys);

                for (uint j = 0; j < lBones[i].translation.nTimes; ++j)
                {
                    mB.lTranslationAnimation[lTimes[j]] = Vector(
                       -lValues[j*3+1],
                        lValues[j*3+2],
                       -lValues[j*3+0]
                    );

                    if (!mB.lTimeline.Find(lTimes[j]))
                        mB.lTimeline.PushBack(lTimes[j]);
                }

                // Scaling
                lTimes = (uint*)(sBuffer + lBones[i].scaling.ofsTimes);
                lValues = (float*)(sBuffer + lBones[i].scaling.ofsKeys);

                for (uint j = 0; j < lBones[i].scaling.nTimes; ++j)
                {
                    // (x, y, z)    =>    (y, z, -x)
                    mB.lScalingAnimation[lTimes[j]] = Vector(
                        lValues[j*3+1],
                        lValues[j*3+2],
                       -lValues[j*3+0]
                    );

                    if (!mB.lTimeline.Find(lTimes[j]))
                        mB.lTimeline.PushBack(lTimes[j]);
                }

                // Rotation
                lTimes = (uint*)(sBuffer + lBones[i].rotation.ofsTimes);
                short* lSValues = (short*)(sBuffer + lBones[i].rotation.ofsKeys);

                for (uint j = 0; j < lBones[i].rotation.nTimes; ++j)
                {
                    // (x, y, z, w)    =>    (w, -y, z, -x)
                    mB.lRotationAnimation[lTimes[j]] = Quaternion(
                        ShortToFloat(lSValues[j*4+3]),
                       -ShortToFloat(lSValues[j*4+1]),
                        ShortToFloat(lSValues[j*4+2]),
                       -ShortToFloat(lSValues[j*4+0])
                    );

                    if (!mB.lTimeline.Find(lTimes[j]))
                        mB.lTimeline.PushBack(lTimes[j]);
                }

                mB.lTimeline.Sort();

                mData.lBoneList.PushBack(mB);
            }
        }

        // Then, geometry
        ModelVertex* lOrigVertices = (ModelVertex*)(sBuffer + mHeader.ofsVertices);

        s_float fXMax(s_float::FLOAT_INF_MINUS);
        s_float fYMax(s_float::FLOAT_INF_MINUS);
        s_float fZMax(s_float::FLOAT_INF_MINUS);
        s_float fXMin(s_float::FLOAT_INF_PLUS);
        s_float fYMin(s_float::FLOAT_INF_PLUS);
        s_float fZMin(s_float::FLOAT_INF_PLUS);

        s_array<Vertex> lGlobalVertexList;
        lGlobalVertexList.Reserve(mHeader.nVertices);
        for (uint i = 0; i < mHeader.nVertices; ++i)
        {
            Vertex mV;

            mV.mPos = Vector(
                -lOrigVertices[i].pos[1],
                 lOrigVertices[i].pos[2],
                -lOrigVertices[i].pos[0]
            );

            mV.mNormal = Vector(
                -lOrigVertices[i].normal[1],
                 lOrigVertices[i].normal[2],
                -lOrigVertices[i].normal[0]
            );

            mV.fU = lOrigVertices[i].texcoords[0];
            mV.fV = lOrigVertices[i].texcoords[1];

            for (uint j = 0; j < 4; ++j)
            {
                mV.lBoneIndexList[j] = lOrigVertices[i].bones[j];
                mV.lBoneWeightList[j] = lOrigVertices[i].weights[j]/255.0f;
            }

            fXMin = s_float::Min(fXMin, mV.mPos.X());
            fXMax = s_float::Max(fXMax, mV.mPos.X());
            fYMin = s_float::Min(fYMin, mV.mPos.Y());
            fYMax = s_float::Max(fYMax, mV.mPos.Y());
            fZMin = s_float::Min(fZMin, mV.mPos.Z());
            fZMax = s_float::Max(fZMax, mV.mPos.Z());

            lGlobalVertexList.PushBack(mV);
        }

        ModelView* lView = (ModelView*)(sBuffer + mHeader.ofsViews);

        // Worst LOD
        //int v = 0;
        // Best LOD
        uint uiLOD = mHeader.nViews-1;

        // Get vertex indices
        ushort* lIndexLookup = (ushort*)(sBuffer + lView[uiLOD].ofsIndex);
        ushort* lTriangles = (ushort*)(sBuffer + lView[uiLOD].ofsTris);
        s_array<s_uint> lGIndices; lGIndices.Reserve(lView[uiLOD].nTris);
        for (uint i = 0; i < lView[uiLOD].nTris; ++i)
        {
            lGIndices.PushBack(lIndexLookup[lTriangles[i]]);
        }

        // Get submeshes
        uint uiMeshNbr = lView[uiLOD].nSub;
        ModelSubMesh* lSubMesh = (ModelSubMesh*)(sBuffer + lView[uiLOD].ofsSub);

        mData.lSubMeshList.Reserve(uiMeshNbr);
        for (uint i = 0; i < uiMeshNbr; ++i)
        {
            SubMesh mSM;
            mSM.uiID = lSubMesh[i].id;

            uint uiIndexNbr = lSubMesh[i].nTris;

            s_array<s_uint> lSubIndices; lSubIndices.Reserve(uiIndexNbr);
            s_map<s_uint, s_uint> lLocalVertices;
            s_map<s_uint, s_uint> lIndexMap;
            s_map<s_uint, s_uint>::iterator iter;

            // Build a temporary index list
            for (uint j = 0; j < uiIndexNbr; ++j)
            {
                s_uint uiInd = lGIndices[lSubMesh[i].ofsTris+j];
                lSubIndices.PushBack(uiInd);

                if (lLocalVertices.Get(uiInd) == lLocalVertices.End())
                    lLocalVertices[uiInd] = uiInd;
            }

            // Build the vertex list
            s_map<s_uint, s_uint>::iterator iterV;
            uint k = 0;
            mSM.lVertexList.Reserve(lLocalVertices.GetSize());
            foreach (iterV, lLocalVertices)
            {
                s_uint uiInd = iterV->second;
                mSM.lVertexList.PushBack(lGlobalVertexList[uiInd]);

                lIndexMap[uiInd] = k;
                ++k;
            }

            // Update the index list
            mSM.lTriangleList.Reserve(uiIndexNbr/3);
            for (k = 0; k < uiIndexNbr; k+=3)
            {
                mSM.lTriangleList.PushBack(Triangle(
                    lIndexMap[lSubIndices[k]],
                    lIndexMap[lSubIndices[k+1]],
                    lIndexMap[lSubIndices[k+2]]
                ));
            }

            mData.lSubMeshList.PushBack(mSM);
        }

        // Set bounding information for culling and collisions
        mData.mBoundingBox = AxisAlignedBox(
            Vector(fXMin, fYMin, fZMin),
            Vector(fXMax, fYMax, fZMax)
        );

        delete[] sBuffer;

        return mData;
    }
}



