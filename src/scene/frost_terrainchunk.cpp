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

                float fXIncrement = fXSize_.Get()/uiNX;
                float fZIncrement = fZSize_.Get()/uiNZ;

                lVertexList_.Reserve(uiIndexCount*6);
                s_array<float> lData; lData.Reserve(uiVertexCount*(3+3+2));
                s_array<ushort> lIndices; lIndices.Reserve(uiIndexCount);

                float fXSize = fXSize_.Get();
                float fZSize = fZSize_.Get();
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
