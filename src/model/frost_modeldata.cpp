/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            ModelData source            */
/*                                        */
/*                                        */

#include "model/frost_modeldata.h"

#include "model/frost_model.h"
#include "model/frost_animmanager.h"
#include "material/frost_material.h"

#include <OgreSubMesh.h>
#include <OgreHardwareBufferManager.h>

using namespace std;

namespace Frost
{
    const s_str ModelData::CLASS_NAME = "ModelData";

    ModelData::ModelData( const s_str& sFile )
    {
        sModelName_ = sFile;
    }

    ModelData::~ModelData()
    {
        s_map< s_uint, s_ptr<ModelPart> >::iterator iterPart;
        foreach (iterPart, lModelPartList_)
        {
            iterPart->second.Delete();
        }
    }

    void ModelData::CreateBuffers_( const s_uint& uiSubID, Ogre::SubMesh* pSub, uint uiVertexNbr, uint uiIndexNbr, float* lVertices, float* lUVs, ushort* lIndices )
    {
        Ogre::VertexData* pData = new Ogre::VertexData();
        pSub->vertexData = pData;
        pData->vertexCount = uiVertexNbr;
        Ogre::VertexDeclaration* pDecl = pData->vertexDeclaration;

        // Position and normals
        uint uiOffset = 0;
        pDecl->addElement(0, uiOffset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
        uiOffset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
        pDecl->addElement(0, uiOffset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
        uiOffset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

        Ogre::HardwareVertexBufferSharedPtr pVBuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            uiOffset, uiVertexNbr, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY
        );
        pVBuf->writeData(0, pVBuf->getSizeInBytes(), lVertices, true);
        Ogre::VertexBufferBinding* pBind = pData->vertexBufferBinding;
        pBind->setBinding(0, pVBuf);

        // UVs
        pDecl->addElement(1, 0, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
        pVBuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            pDecl->getVertexSize(1), uiVertexNbr,
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY
        );
        pVBuf->writeData(0, pVBuf->getSizeInBytes(), lUVs, true);
        pBind->setBinding(1, pVBuf);

        // Indices
        Ogre::HardwareIndexBufferSharedPtr pIBuf = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
            Ogre::HardwareIndexBuffer::IT_16BIT, uiIndexNbr,
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY
        );
        pIBuf->writeData(0, pIBuf->getSizeInBytes(), lIndices, true);

        pSub->indexData->indexBuffer = pIBuf;
        pSub->indexData->indexCount = uiIndexNbr;
        pSub->indexData->indexStart = 0;

        // Build triangle list for collision
        MeshObstacle::Triangle mTri;
        s_array<MeshObstacle::Triangle>& lArray = lTriangleList_[uiSubID];
        lArray.Reserve(uiIndexNbr/3);
        for (uint i = 0; i < uiIndexNbr; i += 3)
        {
            ushort i1 = lIndices[i+0];
            ushort i2 = lIndices[i+1];
            ushort i3 = lIndices[i+2];

            mTri.mP[0] = Vector(lVertices[i1*6+0], lVertices[i1*6+1], lVertices[i1*6+2]);
            mTri.mP[1] = Vector(lVertices[i2*6+0], lVertices[i2*6+1], lVertices[i2*6+2]);
            mTri.mP[2] = Vector(lVertices[i3*6+0], lVertices[i3*6+1], lVertices[i3*6+2]);
            lArray.PushBack(mTri);
        }

        // Build local bounding box
        AxisAlignedBox& mBox = lBoxList_[uiSubID];
        Vector mVec(lVertices[0], lVertices[1], lVertices[2]);
        mBox.SetMin(mVec);
        mBox.SetMax(mVec);
        for (uint i = 6; i < uiVertexNbr; ++i)
        {
            mBox.Include(Vector(lVertices[i*6+0], lVertices[i*6+1], lVertices[i*6+2]));
        }
    }
}
