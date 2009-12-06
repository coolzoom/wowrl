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
#include "scene/frost_terrainobstacle.h"
#include "scene/frost_planeobstacle.h"
#include "scene/frost_physicsmanager.h"

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
        /// Flags
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
                uint uiNX = s_uint(s_float::RoundUp(fXSize_*fDensity_)).Get();
                uint uiNZ = s_uint(s_float::RoundUp(fZSize_*fDensity_)).Get();
                float fXSize = fXSize_.Get();
                float fZSize = fZSize_.Get();
                uint uiVertexCount = uiNX*uiNZ;
                uint uiIndexCount = (uiNX-1)*(uiNZ-1)*6;
                uint uiParamNbr = 8;

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

                float fXIncrement = fXSize/(uiNX-1);
                float fZIncrement = fZSize/(uiNZ-1);

                s_array<float> lData; lData.Reserve(uiVertexCount*uiParamNbr);
                s_array<ushort> lIndices; lIndices.Reserve(uiIndexCount);

                for (uint i = 0; i < uiNX; ++i)
                {
                    float x = i*fXIncrement - fXSize/2.0f;
                    for (uint j = 0; j < uiNZ; ++j)
                    {
                        float z = j*fZIncrement - fZSize/2.0f;

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

                pObstacle_ = new PlaneObstacle(fXSize_, fZSize_);
            }
            else
            {
                File mFile(sTerrainFile_, File::I, true);

                if (!mFile.IsOpen())
                {
                    throw Exception(CLASS_NAME,
                        "\""+sTerrainFile_+"\" doesn't exist."
                    );
                }

                TerrainHeader mHeader;
                mFile.Read(mHeader);

                if (s_str(mHeader.sID, 4) != "FTER")
                {
                    throw Exception(CLASS_NAME,
                        "\""+sTerrainFile_+"\" is not a terrain file."
                    );
                }

                if (s_str(mHeader.sVersion, 4) > "0001")
                {
                    throw Exception(CLASS_NAME,
                        "\""+sTerrainFile_+"\" uses a too high file version ("+
                        s_str(mHeader.sVersion, 4)+" > 0001)."
                    );
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

                s_array<TerrainObstacle::Triangle> lTriangleArray;
                TerrainObstacle::Triangle mTri;

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

                    mTri.mP[0] = Vector(lData[i1*uiParamNbr+0], lData[i1*uiParamNbr+1], lData[i1*uiParamNbr+2]);
                    mTri.mP[1] = Vector(lData[i2*uiParamNbr+0], lData[i2*uiParamNbr+1], lData[i2*uiParamNbr+2]);
                    mTri.mP[2] = Vector(lData[i3*uiParamNbr+0], lData[i3*uiParamNbr+1], lData[i3*uiParamNbr+2]);

                    lTriangleArray.PushBack(mTri);

                    // Calculate the normal
                    Vector mNormal = (mTri.mP[1] - mTri.mP[0])^(mTri.mP[2] - mTri.mP[0]);

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

                s_array<s_float> lCollisionData;
                lCollisionData.Resize(uiNX*uiNZ);
                for (uint i = 0; i < uiNX*uiNZ; ++i)
                {
                    if (lPointList[i].ucFlags == 1)
                        lCollisionData[i] = s_float::NaN;
                    else
                        lCollisionData[i] = lPointList[i].fHeight;
                }

                pObstacle_ = new TerrainObstacle(lTriangleArray, this);
            }

            bLoaded_ = true;
        }
    }

    void TerrainChunk::Unload()
    {
        if (bLoaded_)
        {
            pEntity_->detatchFromParent();
            Engine::GetSingleton()->GetOgreSceneManager()->destroyEntity(pEntity_.Get());
            pEntity_ = nullptr;

            Ogre::MeshManager::getSingletonPtr()->remove(("ChunkMesh_"+uiID_).Get());
            pMesh_ = nullptr;

            Engine::GetSingleton()->GetOgreSceneManager()->destroySceneNode(pNode_.Get());
            pNode_ = nullptr;

            pObstacle_.Delete();

            bLoaded_ = false;
        }
    }

    void TerrainChunk::Show()
    {
        if (!bShown_)
        {
            Load();
            if (pEntity_)
                pEntity_->setVisible(true);

            PhysicsManager::GetSingleton()->AddObstacle(pObstacle_);
            bShown_ = true;
        }
    }

    void TerrainChunk::Hide()
    {
        if (bShown_)
        {
            if (pEntity_)
                pEntity_->setVisible(false);

            PhysicsManager::GetSingleton()->RemoveObstacle(pObstacle_);
            bShown_ = false;
        }
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

    s_float TerrainChunk::GetPointHeight(const s_float& fX, const s_float& fZ) const
    {
        if (bPlane_)
        {
            return mPosition_.Y();
        }
        else
        {
            s_ptr<TerrainObstacle> pTerrainObstacle = s_ptr<TerrainObstacle>::DynamicCast(pObstacle_);
            if (pTerrainObstacle)
            {
                return pTerrainObstacle->GetPointHeight(fX, fZ);
            }
            else
            {
                Warning(CLASS_NAME,
                    "Dynamic cast from Obstacle to TerrainObstacle failed."
                );
                return s_float::NaN;
            }
        }
    }

    s_refptr<Material> TerrainChunk::GetMaterial()
    {
        return pMat_;
    }

    const s_uint& TerrainChunk::GetID() const
    {
        return uiID_;
    }
}
