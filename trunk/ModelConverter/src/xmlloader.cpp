/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#include "xmlloader.h"

#include <frost_utils_file.h>
#include <frost_xml_document.h>

using namespace std;

namespace Frost
{
    const s_str XMLLoader::CLASS_NAME = "XMLLoader";

    Vector ReadVector(s_ptr<XML::Block> pBlock)
    {
        return Vector(
            s_float(pBlock->GetAttribute("x")),
            s_float(pBlock->GetAttribute("y")),
            s_float(pBlock->GetAttribute("z"))
        );
    }

    Triangle ReadTriangle(s_ptr<XML::Block> pBlock)
    {
        return Triangle(
            s_uint(pBlock->GetAttribute("v1")),
            s_uint(pBlock->GetAttribute("v2")),
            s_uint(pBlock->GetAttribute("v3"))
        );
    }

    Quaternion ReadQuaternion(s_ptr<XML::Block> pBlock)
    {
        return Quaternion(
            s_float(pBlock->GetAttribute("w")),
            s_float(pBlock->GetAttribute("x")),
            s_float(pBlock->GetAttribute("y")),
            s_float(pBlock->GetAttribute("z"))
        );
    }

    ModelData XMLLoader::LoadModelData( const s_str& sFile )
    {
        ModelData mData;

        if (!File::Exists(sFile))
        {
            throw Exception(CLASS_NAME, "Model file \""+sFile+"\" couldn't be loaded.");
        }

        XML::Document mDoc(sFile, "XML.def");
        if (!mDoc.Check())
        {
            throw Exception(CLASS_NAME, "Error while loading XML file, aborting.");
        }

        s_ptr<XML::Block> pBoundingBox = mDoc.GetMainBlock()->GetBlock("BoundingBox");
        mData.mBoundingBox = AxisAlignedBox(
            ReadVector(pBoundingBox->GetBlock("Min")),
            ReadVector(pBoundingBox->GetBlock("Max"))
        );

        s_ptr<XML::Block> pAnimationData = mDoc.GetMainBlock()->GetBlock("AnimationData");
        if (pAnimationData)
        {
            s_ptr<XML::Block> pAnimations = pAnimationData->GetBlock("Animations");
            s_ptr<XML::Block> pAnimation;
            mData.lAnimList.Reserve(pAnimations->GetChildNumber());
            foreach_block (pAnimation, pAnimations)
            {
                Animation mA;
                mA.uiID =    s_uint(pAnimation->GetAttribute("id"));
                mA.uiBegin = s_uint(pAnimation->GetAttribute("begin"));
                mA.uiEnd =   s_uint(pAnimation->GetAttribute("end"));
                mA.bLoop =   s_bool(pAnimation->GetAttribute("loop"));

                mData.lAnimList.PushBack(mA);
            }

            s_ptr<XML::Block> pBones = pAnimationData->GetBlock("Bones");
            s_ptr<XML::Block> pBone;
            mData.lBoneList.Reserve(pBones->GetChildNumber());
            foreach_block (pBone, pBones)
            {
                Bone mB;
                mB.mPos = ReadVector(pBone->GetBlock("Position"));
                s_int iParent = s_int(pBone->GetAttribute("parent"));
                if (iParent >= 0)
                {
                    mB.uiParent = s_uint(iParent);
                    mB.mPos += mData.lBoneList[mB.uiParent].mPos;
                }
                else
                    mB.uiParent = s_uint::NaN;

                s_ptr<XML::Block> pTimeline = pBone->GetBlock("Timeline");
                if (pTimeline)
                {
                    mB.lTimeline.Reserve(pTimeline->GetChildNumber());
                    s_ptr<XML::Block> pKey;
                    foreach_block (pKey, pTimeline)
                    {
                        s_uint uiKey = s_uint(pKey->GetAttribute("time"));
                        mB.lTimeline.PushBack(uiKey);

                        s_ptr<XML::Block> pTranslation = pKey->GetBlock("Translation");
                        if (pTranslation)
                            mB.lTranslationAnimation[uiKey] = ReadVector(pTranslation);

                        s_ptr<XML::Block> pScaling = pKey->GetBlock("Scaling");
                        if (pScaling)
                            mB.lScalingAnimation[uiKey] = ReadVector(pScaling);

                        s_ptr<XML::Block> pRotation = pKey->GetBlock("Rotation");
                        if (pRotation)
                            mB.lRotationAnimation[uiKey] = ReadQuaternion(pRotation);
                    }
                }

                mData.lBoneList.PushBack(mB);
            }
        }

        s_ptr<XML::Block> pSubMeshes = mDoc.GetMainBlock()->GetBlock("SubMeshes");
        s_ptr<XML::Block> pSubMesh;
        mData.lSubMeshList.Reserve(pSubMeshes->GetChildNumber());
        foreach_block (pSubMesh, pSubMeshes)
        {
            SubMesh mSM;
            mSM.uiID = s_uint(pSubMesh->GetAttribute("id"));

            s_ptr<XML::Block> pVertexPool = pSubMesh->GetBlock("VertexPool");
            s_ptr<XML::Block> pVertex;
            mSM.lVertexList.Reserve(pVertexPool->GetChildNumber());
            foreach_block (pVertex, pVertexPool)
            {
                Vertex mV;

                mV.mPos = ReadVector(pVertex->GetBlock("Position"));
                mV.mNormal = ReadVector(pVertex->GetBlock("Normal"));

                s_ptr<XML::Block> pTextureCoordinates = pVertex->GetBlock("TextureCoordinates");
                mV.fU = s_float(pTextureCoordinates->GetAttribute("u"));
                mV.fV = s_float(pTextureCoordinates->GetAttribute("v"));

                s_ptr<XML::Block> pBones = pVertex->GetBlock("Bones");
                if (pBones)
                {
                    s_ptr<XML::Block> pBone;
                    uint j = 0;
                    foreach_block (pBone, pBones)
                    {
                        mV.lBoneIndexList[j]  = s_uint(pBone->GetAttribute("id"));
                        mV.lBoneWeightList[j] = s_float(pBone->GetAttribute("weight"));

                        ++j;
                    }
                }

                mSM.lVertexList.PushBack(mV);
            }

            s_ptr<XML::Block> pTriangles = pSubMesh->GetBlock("Triangles");
            s_ptr<XML::Block> pTriangle;
            mSM.lTriangleList.Reserve(pTriangles->GetChildNumber());
            foreach_block (pTriangle, pTriangles)
            {
                mSM.lTriangleList.PushBack(ReadTriangle(pTriangle));
            }

            mData.lSubMeshList.PushBack(mSM);
        }

        return mData;
    }

    void XMLLoader::WriteModelData( const ModelData& mData, const s_str& sFile )
    {
        File mFile(sFile, File::O);
        s_str sTab = "";

        mFile.WriteLine("<Model format=\"1.0\">");

            sTab += "    ";
            mFile.WriteLine(sTab+"<BoundingBox>");
                sTab += "    ";
                mFile.WriteLine(sTab+"<Min x=\""+
                    mData.mBoundingBox.GetMin().X()+"\" y=\""+
                    mData.mBoundingBox.GetMin().Y()+"\" z=\""+
                    mData.mBoundingBox.GetMin().Z()+"\"/>"
                );
                mFile.WriteLine(sTab+"<Max x=\""+
                    mData.mBoundingBox.GetMax().X()+"\" y=\""+
                    mData.mBoundingBox.GetMax().Y()+"\" z=\""+
                    mData.mBoundingBox.GetMax().Z()+"\"/>"
                );
                sTab.EraseFromEnd(4);
            mFile.WriteLine(sTab+"</BoundingBox>");

            s_bool bAnimation = true;
            if (mData.lAnimList.GetSize() != 0)
            {
                mFile.WriteLine(sTab+"<AnimationData>");
                sTab += "    ";
                    mFile.WriteLine(sTab+"<Animations>");
                    sTab += "    ";
                    s_array<Animation>::const_iterator iterAnim;
                    foreach (iterAnim, mData.lAnimList)
                    {
                        mFile.WriteLine(sTab+"<Animation id=\""+
                            iterAnim->uiID+"\" begin=\""+
                            iterAnim->uiBegin+"\" end=\""+
                            iterAnim->uiEnd+"\" loop=\""+
                            iterAnim->bLoop+"\"/>");
                    }
                    sTab.EraseFromEnd(4);
                    mFile.WriteLine(sTab+"</Animations>");

                    mFile.WriteLine(sTab+"<Bones>");
                    sTab += "    ";
                    s_array<Bone>::const_iterator iterBone;
                    foreach (iterBone, mData.lBoneList)
                    {
                        mFile.WriteLine(sTab+"<Bone"+
                            (iterBone->uiParent.IsValid() ? " parent=\""+iterBone->uiParent+"\"" : "")+">"
                        );
                        sTab += "    ";
                            Vector mPos = iterBone->mPos;
                            if (iterBone->uiParent.IsValid())
                                mPos -= mData.lBoneList[iterBone->uiParent].mPos;
                            mFile.WriteLine(sTab+"<Position x=\""+
                                iterBone->mPos.X()+"\" y=\""+
                                iterBone->mPos.Y()+"\" z=\""+
                                iterBone->mPos.Z()+"\"/>"
                            );
                            mFile.WriteLine(sTab+"<Timeline>");
                            sTab += "    ";
                            s_array<s_uint>::const_iterator iterKey;
                            foreach (iterKey, iterBone->lTimeline)
                            {
                                mFile.WriteLine(sTab+"<Key time=\""+(*iterKey)+"\">");
                                sTab += "    ";
                                s_map<s_uint, Vector>::const_iterator iterTAnim = iterBone->lTranslationAnimation.Get(*iterKey);
                                if (iterTAnim != iterBone->lTranslationAnimation.End())
                                {
                                    mFile.WriteLine(sTab+"<Translation x=\""+
                                        iterTAnim->second.X()+"\" y=\""+
                                        iterTAnim->second.Y()+"\" z=\""+
                                        iterTAnim->second.Z()+"\"/>"
                                    );
                                }
                                s_map<s_uint, Vector>::const_iterator iterSAnim = iterBone->lScalingAnimation.Get(*iterKey);
                                if (iterSAnim != iterBone->lScalingAnimation.End())
                                {
                                    mFile.WriteLine(sTab+"<Scaling x=\""+
                                        iterSAnim->second.X()+"\" y=\""+
                                        iterSAnim->second.Y()+"\" z=\""+
                                        iterSAnim->second.Z()+"\"/>"
                                    );
                                }
                                s_map<s_uint, Quaternion>::const_iterator iterRAnim = iterBone->lRotationAnimation.Get(*iterKey);
                                if (iterRAnim != iterBone->lRotationAnimation.End())
                                {
                                    mFile.WriteLine(sTab+"<Rotation w=\""+
                                        iterRAnim->second.fW+"\" x=\""+
                                        iterRAnim->second.fX+"\" y=\""+
                                        iterRAnim->second.fY+"\" z=\""+
                                        iterRAnim->second.fZ+"\"/>"
                                    );
                                }
                                sTab.EraseFromEnd(4);
                                mFile.WriteLine(sTab+"</Key>");
                            }
                            sTab.EraseFromEnd(4);
                            mFile.WriteLine(sTab+"</Timeline>");
                        sTab.EraseFromEnd(4);
                        mFile.WriteLine(sTab+"</Bone>");
                    }
                    sTab.EraseFromEnd(4);
                    mFile.WriteLine(sTab+"</Bones>");
                sTab.EraseFromEnd(4);
                mFile.WriteLine(sTab+"</AnimationData>");
            }
            else
                bAnimation = false;

            mFile.WriteLine(sTab+"<SubMeshes>");
            sTab += "    ";
            s_array<SubMesh>::const_iterator iterSub;
            foreach (iterSub, mData.lSubMeshList)
            {
                mFile.WriteLine(sTab+"<SubMesh id=\""+iterSub->uiID+"\">");
                    sTab += "    ";
                    mFile.WriteLine(sTab+"<VertexPool>");
                    sTab += "    ";
                    s_array<Vertex>::const_iterator iterVertex;
                    foreach (iterVertex, iterSub->lVertexList)
                    {
                        mFile.WriteLine(sTab+"<Vertex>");
                            sTab += "    ";
                            mFile.WriteLine(sTab+"<Position x=\""+
                                iterVertex->mPos.X()+"\" y=\""+
                                iterVertex->mPos.Y()+"\" z=\""+
                                iterVertex->mPos.Z()+"\"/>"
                            );
                            mFile.WriteLine(sTab+"<Normal x=\""+
                                iterVertex->mNormal.X()+"\" y=\""+
                                iterVertex->mNormal.Y()+"\" z=\""+
                                iterVertex->mNormal.Z()+"\"/>"
                            );
                            mFile.WriteLine(sTab+"<TextureCoordinates u=\""+
                                iterVertex->fU+"\" v=\""+
                                iterVertex->fV+"\"/>"
                            );
                            if (bAnimation)
                            {
                                mFile.WriteLine(sTab+"<Bones>");
                                sTab += "    ";
                                for (s_uint i; i < 4; ++i)
                                {
                                    if (fabs(iterVertex->lBoneWeightList[i]) > 0.0001)
                                    {
                                        mFile.WriteLine(sTab+"<Bone id=\""+
                                            iterVertex->lBoneIndexList[i]+"\" weight=\""+
                                            iterVertex->lBoneWeightList[i]+"\"/>"
                                        );
                                    }
                                }
                                sTab.EraseFromEnd(4);
                                mFile.WriteLine(sTab+"</Bones>");
                            }
                            sTab.EraseFromEnd(4);
                        mFile.WriteLine(sTab+"</Vertex>");
                    }
                    sTab.EraseFromEnd(4);
                    mFile.WriteLine(sTab+"</VertexPool>");
                    mFile.WriteLine(sTab+"<Triangles>");
                    sTab += "    ";
                    s_array<Triangle>::const_iterator iterTriangle;
                    foreach (iterTriangle, iterSub->lTriangleList)
                    {
                        mFile.WriteLine(sTab+"<Triangle v1=\""+
                            iterTriangle->lVertexList[0]+"\" v2=\""+
                            iterTriangle->lVertexList[1]+"\" v3=\""+
                            iterTriangle->lVertexList[2]+"\"/>"
                        );
                    }
                    sTab.EraseFromEnd(4);
                    mFile.WriteLine(sTab+"</Triangles>");
                    sTab.EraseFromEnd(4);
                mFile.WriteLine(sTab+"</SubMesh>");
            }
            sTab.EraseFromEnd(4);
            mFile.WriteLine(sTab+"</SubMeshes>");
        mFile.WriteLine("</Model>");
    }
}
