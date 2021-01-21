/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#include "fmloader.h"
#include "fmmodelstructs.h"

#include <frost_utils_file.h>

using namespace std;

namespace Frost
{
    const s_str FMLoader::CLASS_NAME = "FMLoader";

    ModelData FMLoader::LoadModelData( const s_str& sFile )
    {
        ModelData mData;

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

        mData.mBoundingBox = AxisAlignedBox(
            Vector(mHeader.bbox[0], mHeader.bbox[1], mHeader.bbox[2]),
            Vector(mHeader.bbox[3], mHeader.bbox[4], mHeader.bbox[5])
        );

        mData.lAnimList.Reserve(mHeader.nAnimation);
        for (uint i = 0; i < mHeader.nAnimation; ++i)
        {
            ModelAnimation mMA;
            mFile.Read(mMA);

            Animation mA;
            mA.uiID = mMA.animID;
            mA.uiBegin = mMA.timeStart;
            mA.uiEnd = mMA.timeEnd;
            mA.bLoop = mMA.loop;

            mData.lAnimList.PushBack(mA);
        }

        mData.lBoneList.Reserve(mHeader.nBone);
        for (uint i = 0; i < mHeader.nBone; ++i)
        {
            ModelBone mMB;
            mFile.Read(mMB);

            Bone mB;
            mB.mPos = Vector(mMB.pivot[0], mMB.pivot[1], mMB.pivot[2]);
            mB.uiParent = (mMB.parent > 0 ? mMB.parent : s_uint::NaN);

            if (mB.uiParent.IsValid())
                mB.mPos += mData.lBoneList[mB.uiParent].mPos;

            mB.lTimeline.Reserve(mMB.nKey);
            for (int j = 0; j < mMB.nKey; ++j)
            {
                ModelAnimationKey mKey;
                mFile.Read(mKey);

                mB.lTimeline.PushBack(mKey.key);
                mB.lTranslationAnimation[mKey.key] = Vector(
                    mKey.translation[0], mKey.translation[1], mKey.translation[2]
                );
                mB.lScalingAnimation[mKey.key] = Vector(
                    mKey.scaling[0], mKey.scaling[1], mKey.scaling[2]
                );
                mB.lRotationAnimation[mKey.key] = Quaternion(
                    mKey.rotation[0], mKey.rotation[1], mKey.rotation[2], mKey.rotation[3]
                );
            }

            mData.lBoneList.PushBack(mB);
        }

        mData.lSubMeshList.Reserve(mHeader.nSubMesh);
        for (uint i = 0; i < mHeader.nSubMesh; ++i)
        {
            ModelSubMesh mMSM;
            mFile.Read(mMSM);

            SubMesh mSM;
            mSM.uiID = mMSM.id;

            ModelVertex* lVertexList = new ModelVertex[mMSM.nVertex];
            mFile.Read(lVertexList, mMSM.nVertex*sizeof(ModelVertex));

            mSM.lVertexList.Reserve(mMSM.nVertex);
            for (uint j = 0; j < mMSM.nVertex; ++j)
            {
                Vertex mV;
                ModelVertex& mMV = lVertexList[j];

                mV.mPos =    Vector(mMV.pos[0],    mMV.pos[1],    mMV.pos[2]);
                mV.mNormal = Vector(mMV.normal[0], mMV.normal[1], mMV.normal[2]);

                mV.fU = mMV.texcoords[0];
                mV.fV = mMV.texcoords[1];

                mV.lBoneWeightList[0] = mMV.weights[0]/255.0f;
                mV.lBoneWeightList[1] = mMV.weights[1]/255.0f;
                mV.lBoneWeightList[2] = mMV.weights[2]/255.0f;
                mV.lBoneWeightList[3] = mMV.weights[3]/255.0f;

                mV.lBoneIndexList[0] = mMV.bones[0];
                mV.lBoneIndexList[1] = mMV.bones[1];
                mV.lBoneIndexList[2] = mMV.bones[2];
                mV.lBoneIndexList[3] = mMV.bones[3];

                mSM.lVertexList.PushBack(mV);
            }

            ushort* lTriangleList = new ushort[mMSM.nIndice];
            mFile.Read(lTriangleList, mMSM.nIndice*sizeof(ushort));

            mSM.lTriangleList.Reserve(mMSM.nIndice/3);
            for (uint j = 0; j < mMSM.nIndice; j+=3)
            {
                mSM.lTriangleList.PushBack(Triangle(
                    lTriangleList[j+0], lTriangleList[j+1], lTriangleList[j+2]
                ));
            }

            mData.lSubMeshList.PushBack(mSM);

            mFile.SetReadPos(mMSM.oNextSubMesh);

            delete[] lVertexList;
            delete[] lTriangleList;
        }

        return mData;
    }

    template<class Elem>
    Elem MapLERP(const s_map<s_uint, Elem>& lMap, s_uint uiValue)
    {
        typename s_map<s_uint, Elem>::const_iterator iterFirst, iterSecond;
        iterSecond = lMap.LowerBound(uiValue);
        if (iterSecond == lMap.End())
        {
            --iterSecond;
            return iterSecond->second;
        }
        else if (iterSecond == lMap.Begin())
        {
            return iterSecond->second;
        }
        else
        {
            iterFirst = iterSecond;
            --iterFirst;

            s_float fCoef = s_float(uiValue - iterFirst->first);
            fCoef /= s_float(iterSecond->first - iterFirst->first);

            return iterFirst->second +
                   fCoef*(iterSecond->second - iterFirst->second);
        }
    }

    void FMLoader::WriteModelData( const ModelData& mData, const s_str& sFile )
    {
        File mFile(sFile, File::O, true);

        ModelHeader mHeader;
        mHeader.id[0] = 'F';
        mHeader.id[1] = 'M';
        mHeader.version[0] = '0';
        mHeader.version[1] = '0';
        mHeader.version[2] = '0';
        mHeader.version[3] = '1';
        mHeader.nSubMesh = mData.lSubMeshList.GetSize().Get();
        mHeader.nAnimation = mData.lAnimList.GetSize().Get();
        mHeader.nBone = mData.lBoneList.GetSize().Get();
        mHeader.bbox[0] = mData.mBoundingBox.GetMin().X().Get();
        mHeader.bbox[1] = mData.mBoundingBox.GetMin().Y().Get();
        mHeader.bbox[2] = mData.mBoundingBox.GetMin().Z().Get();
        mHeader.bbox[3] = mData.mBoundingBox.GetMax().X().Get();
        mHeader.bbox[4] = mData.mBoundingBox.GetMax().Y().Get();
        mHeader.bbox[5] = mData.mBoundingBox.GetMax().Z().Get();
        mFile.Write(mHeader);

        s_array<Animation>::const_iterator iterAnim;
        foreach (iterAnim, mData.lAnimList)
        {
            ModelAnimation mAnim;

            mAnim.animID = iterAnim->uiID.Get();
            mAnim.timeStart = iterAnim->uiBegin.Get();
            mAnim.timeEnd = iterAnim->uiEnd.Get();
            mAnim.loop = iterAnim->bLoop;

            mFile.Write(mAnim);
        }

        s_array<Bone>::const_iterator iterBone;
        foreach (iterBone, mData.lBoneList)
        {
            ModelBone mBone;

            Vector mRelativePos;
            if (iterBone->uiParent.IsValid())
                mRelativePos = iterBone->mPos - mData.lBoneList[iterBone->uiParent].mPos;
            else
                mRelativePos = iterBone->mPos;

            mBone.pivot[0] = mRelativePos.X().Get();
            mBone.pivot[1] = mRelativePos.Y().Get();
            mBone.pivot[2] = mRelativePos.Z().Get();
            mBone.nKey = iterBone->lTimeline.GetSize().Get();
            mBone.parent = (iterBone->uiParent.IsValid() ? iterBone->uiParent.Get() : -1);
            mBone.oNextBone = mFile.GetReadPos().Get() + sizeof(ModelBone) + mBone.nKey*sizeof(ModelAnimationKey);

            ModelAnimationKey* lKeyList = new ModelAnimationKey[mBone.nKey];

            uint i = 0;
            s_array<s_uint>::const_iterator iterKey;
            foreach (iterKey, iterBone->lTimeline)
            {
                ModelAnimationKey& mKey = lKeyList[i];
                mKey.key = iterKey->Get();

                Vector mTranslation;
                s_map<s_uint, Vector>::const_iterator iter1 = iterBone->lTranslationAnimation.Get(*iterKey);
                if (iter1 != iterBone->lTranslationAnimation.End())
                    mTranslation = iter1->second;
                else if (!iterBone->lTranslationAnimation.IsEmpty())
                    mTranslation = MapLERP(iterBone->lTranslationAnimation, *iterKey);

                mKey.translation[0] = mTranslation.X().Get();
                mKey.translation[1] = mTranslation.Y().Get();
                mKey.translation[2] = mTranslation.Z().Get();

                Vector mScaling(1.0f, 1.0f, 1.0f);
                iter1 = iterBone->lScalingAnimation.Get(*iterKey);
                if (iter1 != iterBone->lScalingAnimation.End())
                    mScaling = iter1->second;
                else if (!iterBone->lScalingAnimation.IsEmpty())
                    mScaling = MapLERP(iterBone->lScalingAnimation, *iterKey);

                mKey.scaling[0] = mScaling.X().Get();
                mKey.scaling[1] = mScaling.Y().Get();
                mKey.scaling[2] = mScaling.Z().Get();

                Quaternion mRotation(1.0f, 0.0f, 0.0f, 0.0f);
                s_map<s_uint, Quaternion>::const_iterator iter2 = iterBone->lRotationAnimation.Get(*iterKey);
                if (iter2 != iterBone->lRotationAnimation.End())
                    mRotation = iter2->second;
                else if (!iterBone->lRotationAnimation.IsEmpty())
                    mRotation = MapLERP(iterBone->lRotationAnimation, *iterKey);

                mKey.rotation[0] = mRotation.fW.Get();
                mKey.rotation[1] = mRotation.fX.Get();
                mKey.rotation[2] = mRotation.fY.Get();
                mKey.rotation[3] = mRotation.fZ.Get();

                ++i;
            }

            mFile.Write(mBone);
            mFile.Write(lKeyList, mBone.nKey*sizeof(ModelAnimationKey));

            delete[] lKeyList;
        }

        s_array<SubMesh>::const_iterator iterSub;
        foreach (iterSub, mData.lSubMeshList)
        {
            ModelSubMesh mSub;
            mSub.id = iterSub->uiID.Get();
            mSub.nVertex = iterSub->lVertexList.GetSize().Get();
            mSub.nIndice = 3*iterSub->lTriangleList.GetSize().Get();

            ModelVertex* lVertexList = new ModelVertex[mSub.nVertex];
            for (uint i = 0; i < mSub.nVertex; ++i)
            {
                ModelVertex& mv = lVertexList[i];
                const Vertex& v = iterSub->lVertexList[i];

                mv.pos[0] = v.mPos.X().Get();
                mv.pos[1] = v.mPos.Y().Get();
                mv.pos[2] = v.mPos.Z().Get();

                mv.normal[0] = v.mNormal.X().Get();
                mv.normal[1] = v.mNormal.Y().Get();
                mv.normal[2] = v.mNormal.Z().Get();

                mv.texcoords[0] = v.fU.Get();
                mv.texcoords[1] = v.fV.Get();

                mv.weights[0] = v.lBoneWeightList[0].Get()*255;
                mv.weights[1] = v.lBoneWeightList[1].Get()*255;
                mv.weights[2] = v.lBoneWeightList[2].Get()*255;
                mv.weights[3] = v.lBoneWeightList[3].Get()*255;

                mv.bones[0] = v.lBoneIndexList[0].Get();
                mv.bones[1] = v.lBoneIndexList[1].Get();
                mv.bones[2] = v.lBoneIndexList[2].Get();
                mv.bones[3] = v.lBoneIndexList[3].Get();
            }

            ushort* lTriangleList = new ushort[mSub.nIndice];
            uint uiTriangleNbr = mSub.nIndice/3;
            for (uint i = 0; i < uiTriangleNbr; ++i)
            {
                const Triangle& t = iterSub->lTriangleList[i];

                lTriangleList[i*3 + 0] = t.lVertexList[0].Get();
                lTriangleList[i*3 + 1] = t.lVertexList[1].Get();
                lTriangleList[i*3 + 2] = t.lVertexList[2].Get();
            }

            mSub.oNextSubMesh = mFile.GetWritePos().Get() +
                sizeof(ModelSubMesh) +
                mSub.nVertex*sizeof(ModelVertex) +
                mSub.nIndice*sizeof(ushort);

            mFile.Write(mSub);
            mFile.Write(lVertexList, mSub.nVertex*sizeof(ModelVertex));
            mFile.Write(lTriangleList, mSub.nIndice*sizeof(ushort));

            delete[] lVertexList;
            delete[] lTriangleList;
        }
    }
}
