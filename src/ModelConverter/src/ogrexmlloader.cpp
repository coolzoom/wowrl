/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#include "ogrexmlloader.h"

#include <frost_utils_file.h>
#include <frost_xml_document.h>

using namespace std;

namespace Frost
{
    const s_str OgreXMLLoader::CLASS_NAME = "OgreXMLLoader";

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

    ModelData OgreXMLLoader::LoadModelData( const s_str& sFile )
    {
        ModelData mData;

        if (!File::Exists(sFile))
        {
            throw Exception(CLASS_NAME, "Model file \""+sFile+"\" couldn't be loaded.");
        }

        XML::Document mDoc(sFile, "OgreXML.def");
        if (!mDoc.Check())
        {
            throw Exception(CLASS_NAME, "Error while loading Ogre XML file, aborting.");
        }

        s_float fXMax(s_float::FLOAT_INF_MINUS);
        s_float fYMax(s_float::FLOAT_INF_MINUS);
        s_float fZMax(s_float::FLOAT_INF_MINUS);
        s_float fXMin(s_float::FLOAT_INF_PLUS);
        s_float fYMin(s_float::FLOAT_INF_PLUS);
        s_float fZMin(s_float::FLOAT_INF_PLUS);

        s_ptr<XML::Block> pSubMeshes = mDoc.GetMainBlock()->GetBlock("submeshes");
        s_ptr<XML::Block> pSubMesh;
        mData.lSubMeshList.Reserve(pSubMeshes->GetChildNumber());
        s_uint uiID = 0;
        foreach_block (pSubMesh, pSubMeshes)
        {
            SubMesh mSM;
            mSM.uiID = uiID;
            ++uiID;

            s_ptr<XML::Block> pTriangles = pSubMesh->GetBlock("faces");
            s_ptr<XML::Block> pTriangle;
            mSM.lTriangleList.Reserve(pTriangles->GetChildNumber());
            foreach_block (pTriangle, pTriangles)
            {
                mSM.lTriangleList.PushBack(ReadTriangle(pTriangle));
            }

            s_ptr<XML::Block> pVertexPool = pSubMesh->GetBlock("geometry")->GetBlock("vertexbuffer");
            s_ptr<XML::Block> pVertex;
            mSM.lVertexList.Reserve(pVertexPool->GetChildNumber());
            foreach_block (pVertex, pVertexPool)
            {
                Vertex mV;

                mV.mPos = ReadVector(pVertex->GetBlock("position"));
                mV.mNormal = ReadVector(pVertex->GetBlock("normal"));

                s_ptr<XML::Block> pTextureCoordinates = pVertex->GetBlock("texcoord");
                mV.fU = s_float(pTextureCoordinates->GetAttribute("u"));
                mV.fV = s_float(pTextureCoordinates->GetAttribute("v"));

                fXMin = s_float::Min(fXMin, mV.mPos.X());
                fXMax = s_float::Max(fXMax, mV.mPos.X());
                fYMin = s_float::Min(fYMin, mV.mPos.Y());
                fYMax = s_float::Max(fYMax, mV.mPos.Y());
                fZMin = s_float::Min(fZMin, mV.mPos.Z());
                fZMax = s_float::Max(fZMax, mV.mPos.Z());

                mSM.lVertexList.PushBack(mV);
            }

            mData.lSubMeshList.PushBack(mSM);
        }

        mData.mBoundingBox = AxisAlignedBox(
            Vector(fXMin, fYMin, fZMin),
            Vector(fXMax, fYMax, fZMax)
        );

        return mData;
    }
}
