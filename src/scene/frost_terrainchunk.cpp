/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           TerrainChunk source          */
/*                                        */
/*                                        */

#include "scene/frost_terrainchunk.h"

#include "scene/frost_zone.h"
#include "material/frost_materialmanager.h"
#include "material/frost_material.h"

#include <OgreSceneManager.h>
#include <OgreMeshManager.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>
#include <OgreHardwareBufferManager.h>
#include <OgreEntity.h>

using namespace std;

namespace Frost
{
    struct TerrainHeader
    {
        /// Makes sure we are opening a terrain file
        char sID[4];
        /// Makes sure the loading code supports this file
        char sVersion[4];
        /// The dimension of the terrain in world unit on the X axis
        float fXSize;
        /// The dimension of the terrain in world unit on the Y axis
        float fYSize;
        /// The dimension of the terrain in world unit on the Y axis
        float fZSize;
        /// The dimension of the terrain in number of points on the X axis
        uint uiNumXPoint;
        /// The dimension of the terrain in number of points on the Z axis
        uint uiNumZPoint;
        /// The offset at which the loader can get the first point
        uint uiOffPoints;
    };

    struct TerrainVertex
    {
        /// Vertex height
        float fHeight;
        /// Flags;
        uchar ucFlags;
    };

    const s_str TerrainChunk::CLASS_NAME = "TerrainChunk";

    TerrainChunk::TerrainChunk( s_ptr<Zone> pZone ) :
        pZone_(pZone)
    {
        uiID_ = pZone_->AddChunk(this);
    }

    TerrainChunk::~TerrainChunk()
    {
        Unload();
    }

    void TerrainChunk::Load()
    {
        if (!bLoaded_)
        {
            s_ptr<Ogre::SceneManager> pSceneMgr = Engine::GetSingleton()->GetOgreSceneManager();

            if (bPlane_)
            {
                uint uiNX = s_uint(s_float::RoundDown(fXSize_*fDensity_)).Get();
                uint uiNZ = s_uint(s_float::RoundDown(fZSize_*fDensity_)).Get();
                float fXSize = fXSize_.Get();
                float fZSize = fZSize_.Get();
                uint uiVertexCount = uiNX*uiNZ;
                uint uiIndexCount = (uiNX-1)*(uiNZ-1)*6;

                pMesh_ = Ogre::MeshManager::getSingletonPtr()->createManual(
                    ("ChunkMesh_"+uiID_).Get(), "Frost"
                ).get();
                Ogre::SubMesh* pSubMesh = pMesh_->createSubMesh();
                pSubMesh->useSharedVertices = true;
                Ogre::VertexData* pVertexData = new Ogre::VertexData();
                pMesh_->sharedVertexData = pVertexData;
                pVertexData->vertexCount = uiVertexCount;
                Ogre::VertexDeclaration* pDecl = pVertexData->vertexDeclaration;
                pDecl->addElement(
                    0, 0,
                    Ogre::VET_FLOAT3, Ogre::VES_POSITION
                );
                pDecl->addElement(
                    0, (3)*Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT1),
                    Ogre::VET_FLOAT3, Ogre::VES_NORMAL
                );
                pDecl->addElement(
                    0, (3+3)*Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT1),
                    Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES
                );

                Ogre::HardwareVertexBufferSharedPtr pVBuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                    pDecl->getVertexSize(0),
                    pVertexData->vertexCount,
                    Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY
                );

                pSubMesh->indexData->indexCount = uiIndexCount;
                pSubMesh->indexData->indexStart = 0;

                Ogre::HardwareIndexBufferSharedPtr pIBuf = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
                    Ogre::HardwareIndexBuffer::IT_16BIT,
                    pSubMesh->indexData->indexCount,
                    Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY
                );

                pSubMesh->indexData->indexBuffer = pIBuf;

                float fXIncrement = fXSize/uiNX;
                float fZIncrement = fZSize/uiNZ;

                lVertexList_.Reserve(uiIndexCount*6);
                s_array<float> lData; lData.Reserve(uiVertexCount*(3+3+2));
                s_array<ushort> lIndices; lIndices.Reserve(uiIndexCount);

                for (float x = -fXSize/2.0f; x < fXSize/2.0f; x += fXIncrement)
                {
                    for (float z = -fZSize/2.0f; z < fZSize/2.0f; z += fZIncrement)
                    {
                        // Position
                        lData.PushBack(x);
                        lData.PushBack(0.0f);
                        lData.PushBack(z);

                        // Normal
                        lData.PushBack(0.0f);
                        lData.PushBack(1.0f);
                        lData.PushBack(0.0f);

                        // Texture coordinates
                        lData.PushBack((x+fXSize/2.0f)/fXSize);
                        lData.PushBack((z+fZSize/2.0f)/fZSize);
                    }
                }

                for (uint x = 0; x < uiNX-1; ++x)
                {
                    for (uint z = 0; z < uiNZ-1; ++z)
                    {
                        // Indices for the first triangle
                        lIndices.PushBack(x*uiNZ+z);
                        lIndices.PushBack(x*uiNZ+z+1);
                        lIndices.PushBack((x+1)*uiNZ+z+1);

                        // Indices for the second triangle
                        lIndices.PushBack(x*uiNZ+z);
                        lIndices.PushBack((x+1)*uiNZ+z+1);
                        lIndices.PushBack((x+1)*uiNZ+z);
                    }
                }

                pVBuf->writeData(0, pVBuf->getSizeInBytes(), lData.GetClassicArray(), true);
                Ogre::VertexBufferBinding* pBind = pVertexData->vertexBufferBinding;
                pBind->setBinding(0, pVBuf);

                pIBuf->writeData(0, pIBuf->getSizeInBytes(), lIndices.GetClassicArray(), true);

                if (!pMat_)
                    pMat_ = MaterialManager::GetSingleton()->GetDefault3D();

                pSubMesh->setMaterialName(pMat_->GetOgreMaterial()->getName());

                pMesh_->_setBounds(Ogre::AxisAlignedBox(
                    -fXSize/2.0f, -0.1, -fZSize/2.0f,
                    fXSize/2.0f,  0.1,  fZSize/2.0f
                ));

                pMesh_->_setBoundingSphereRadius(
                    std::max(fXSize, fZSize)/2.0f
                );

                pMesh_->load();

                pEntity_ = pSceneMgr->createEntity(
                    ("ChunkEntity_"+uiID_).Get(), ("ChunkMesh_"+uiID_).Get()
                );

                pNode_ = pSceneMgr->getRootSceneNode()->createChildSceneNode(
                    Vector::FrostToOgre(mPosition_)
                );
                pNode_->attachObject(pEntity_.Get());
            }
            else
            {
                File mFile(sTerrainFile_, File::I, true);

                if (!mFile.IsOpen())
                {
                    Error(CLASS_NAME,
                        "\""+sTerrainFile_+"\" doesn't exist."
                    );
                    return;
                }

                TerrainHeader mHeader;
                mFile.Read(mHeader);

                if (s_str(mHeader.sID, 4) != "FTER")
                {
                    Error(CLASS_NAME,
                        "\""+sTerrainFile_+"\" is not a terrain file."
                    );
                    return;
                }

                if (s_str(mHeader.sVersion, 4) > "0001")
                {
                    Error(CLASS_NAME,
                        "\""+sTerrainFile_+"\" uses a too high file version ("+
                        s_str(mHeader.sVersion, 4)+" > 0001)."
                    );
                    return;
                }

                uint uiNX = mHeader.uiNumXPoint;
                uint uiNZ = mHeader.uiNumZPoint;
                float fXSize = mHeader.fXSize;
                float fZSize = mHeader.fZSize;
                uint uiVertexCount = uiNX*uiNZ;
                uint uiIndexCount = (uiNX-1)*(uiNZ-1)*6;

                pMesh_ = Ogre::MeshManager::getSingletonPtr()->createManual(
                    ("ChunkMesh_"+uiID_).Get(), "Frost"
                ).get();
                Ogre::SubMesh* pSubMesh = pMesh_->createSubMesh();
                pSubMesh->useSharedVertices = true;
                Ogre::VertexData* pVertexData = new Ogre::VertexData();
                pMesh_->sharedVertexData = pVertexData;
                pVertexData->vertexCount = uiVertexCount;
                Ogre::VertexDeclaration* pDecl = pVertexData->vertexDeclaration;
                pDecl->addElement(
                    0, 0,
                    Ogre::VET_FLOAT3, Ogre::VES_POSITION
                );
                pDecl->addElement(
                    0, (3)*Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT1),
                    Ogre::VET_FLOAT3, Ogre::VES_NORMAL
                );
                pDecl->addElement(
                    0, (3+3)*Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT1),
                    Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES
                );

                Ogre::HardwareVertexBufferSharedPtr pVBuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                    pDecl->getVertexSize(0),
                    pVertexData->vertexCount,
                    Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY
                );

                pSubMesh->indexData->indexCount = uiIndexCount;
                pSubMesh->indexData->indexStart = 0;

                Ogre::HardwareIndexBufferSharedPtr pIBuf = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
                    Ogre::HardwareIndexBuffer::IT_16BIT,
                    pSubMesh->indexData->indexCount,
                    Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY
                );

                pSubMesh->indexData->indexBuffer = pIBuf;

                s_array<TerrainVertex> lPointList; lPointList.Resize(uiNX*uiNZ);
                mFile.Read(lPointList.GetClassicArray(), uiNX*uiNZ*sizeof(TerrainVertex));

                lVertexList_.Reserve(uiIndexCount*6);
                uint uiParamNbr = 3+3+2;
                s_array<float> lData; lData.Resize(uiVertexCount*uiParamNbr);
                s_array<ushort> lIndices; lIndices.Resize(uiIndexCount);

                for (uint x = 0; x < uiNX; ++x)
                {
                    for (uint z = 0; z < uiNZ; ++z)
                    {
                        uint i = (x*uiNZ+z)*uiParamNbr;

                        // Position
                        lData[i+0] = (fXSize*x)/uiNX-fXSize/2.0f;
                        lData[i+1] = lPointList[x*uiNZ+z].fHeight;
                        lData[i+2] = (fZSize*z)/uiNZ-fZSize/2.0f;

                        // Texture coordinates
                        lData[i+6] = x/(float)uiNX;
                        lData[i+7] = z/(float)uiNZ;
                    }
                }

                for (uint x = 0; x < uiNX-1; ++x)
                {
                    for (uint z = 0; z < uiNZ-1; ++z)
                    {
                        uint i = (x*(uiNZ-1) + z)*6;

                        // Indices for the first triangle
                        lIndices[i+0] = x*uiNZ+z;
                        lIndices[i+1] = x*uiNZ+z+1;
                        lIndices[i+2] = (x+1)*uiNZ+z+1;

                        // Indices for the second triangle
                        lIndices[i+3] = x*uiNZ+z;
                        lIndices[i+4] = (x+1)*uiNZ+z+1;
                        lIndices[i+5] = (x+1)*uiNZ+z;
                    }
                }

                // Normals
                // Calculation code taken from :
                // http://www.devmaster.net/forums/showthread.php?t=1783
                s_array<Vector> lNArray; lNArray.Resize(uiIndexCount);
                for (uint i = 0; i < uiIndexCount; i += 3)
                {
                    // Get the three vertices that make the faces
                    ushort i1 = lIndices[i+0];
                    ushort i2 = lIndices[i+1];
                    ushort i3 = lIndices[i+2];

                    Vector mV1 = Vector(lData[i1*uiParamNbr+0], lData[i1*uiParamNbr+1], lData[i1*uiParamNbr+2]);
                    Vector mV2 = Vector(lData[i2*uiParamNbr+0], lData[i2*uiParamNbr+1], lData[i2*uiParamNbr+2]);
                    Vector mV3 = Vector(lData[i3*uiParamNbr+0], lData[i3*uiParamNbr+1], lData[i3*uiParamNbr+2]);

                    // Calculate the normal
                    Vector mNormal = (mV2-mV1)^(mV3-mV1);

                    // Sum up the face's normal for each of the vertices that make up the face.
                    lNArray[i1] += mNormal;
                    lNArray[i2] += mNormal;
                    lNArray[i3] += mNormal;
                }

                // Normals
                for (uint x = 0; x < uiNX; ++x)
                {
                    for (uint z = 0; z < uiNZ; ++z)
                    {
                        uint i1 = (x*uiNZ+z)*uiParamNbr;
                        uint i2 = x*uiNZ+z;

                        // Normalize the sum at the very end
                        lNArray[i2].Normalize();

                        lData[i1+3] = lNArray[i2].X().Get();
                        lData[i1+4] = lNArray[i2].Y().Get();
                        lData[i1+5] = lNArray[i2].Z().Get();
                    }
                }

                pVBuf->writeData(0, pVBuf->getSizeInBytes(), lData.GetClassicArray(), true);
                Ogre::VertexBufferBinding* pBind = pVertexData->vertexBufferBinding;
                pBind->setBinding(0, pVBuf);

                pIBuf->writeData(0, pIBuf->getSizeInBytes(), lIndices.GetClassicArray(), true);

                if (!pMat_)
                    pMat_ = MaterialManager::GetSingleton()->GetDefault3D();

                pSubMesh->setMaterialName(pMat_->GetOgreMaterial()->getName());

                pMesh_->_setBounds(Ogre::AxisAlignedBox(
                    -fXSize/2.0f, -5.0f, -fZSize/2.0f,
                    fXSize/2.0f,  5.0f,  fZSize/2.0f
                ));

                pMesh_->_setBoundingSphereRadius(
                    std::max(fXSize, fZSize)/2.0f
                );

                pMesh_->load();

                pEntity_ = pSceneMgr->createEntity(
                    ("ChunkEntity_"+uiID_).Get(), ("ChunkMesh_"+uiID_).Get()
                );

                pNode_ = pSceneMgr->getRootSceneNode()->createChildSceneNode(
                    Vector::FrostToOgre(mPosition_)
                );
                pNode_->attachObject(pEntity_.Get());
            }

            bLoaded_ = true;
        }
    }

    void TerrainChunk::Unload()
    {
        if (bLoaded_)
        {
            lVertexList_.Clear();

            pEntity_->detatchFromParent();
            Engine::GetSingleton()->GetOgreSceneManager()->destroyEntity(pEntity_.Get());
            pEntity_ = NULL;

            Ogre::MeshManager::getSingletonPtr()->remove(("ChunkMesh_"+uiID_).Get());
            pMesh_ = NULL;

            Engine::GetSingleton()->GetOgreSceneManager()->destroySceneNode(pNode_.Get());
            pNode_ = NULL;

            bLoaded_ = false;
        }
    }

    void TerrainChunk::Show()
    {
        Load();
        if (pEntity_)
            pEntity_->setVisible(true);
    }

    void TerrainChunk::Hide()
    {
        if (pEntity_)
            pEntity_->setVisible(false);
    }

    s_bool TerrainChunk::IsShown()
    {
        return bShown_;
    }

    void TerrainChunk::SetTerrainFile( const s_str& sTerrainFile )
    {
        bPlane_ = false;

        sTerrainFile_ = sTerrainFile;
    }

    void TerrainChunk::SetPlane( const s_float& fXSize, const s_float& fZSize, const s_float& fDensity )
    {
        bPlane_ = true;

        fXSize_ = fXSize;
        fZSize_ = fZSize;
        fDensity_ = fDensity;
    }

    void TerrainChunk::SetMaterial( s_refptr<Material> pMat )
    {
        pMat_ = pMat;
    }

    void TerrainChunk::SetAlwaysVisible( const s_bool& bAlwaysVisible )
    {
        bAlwaysVisible_ = bAlwaysVisible;

        if (bAlwaysVisible_)
        {
            mSize_ = Vector(s_float::INFPLUS);
        }
    }

    const s_bool& TerrainChunk::IsAlwaysVisible() const
    {
        return bAlwaysVisible_;
    }

    void TerrainChunk::SetPosition(const Vector& mPosition)
    {
        mPosition_ = mPosition;
    }

    void TerrainChunk::SetSize(const Vector& mSize)
    {
        mSize_ = mSize;
    }

    const Vector& TerrainChunk::GetPosition() const
    {
        return mPosition_;
    }

    const Vector& TerrainChunk::GetSize() const
    {
        return mSize_;
    }

    const s_uint& TerrainChunk::GetID() const
    {
        return uiID_;
    }
}
