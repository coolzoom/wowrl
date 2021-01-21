/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           TerrainChunk source          */
/*                                        */
/*                                        */

#include "scene/frost_terrainchunk.h"

#include "material/frost_materialmanager.h"
#include "material/frost_material.h"
#include "material/frost_shadermanager.h"
#include "material/frost_shader.h"
#include "scene/frost_terrainobstacle.h"
#include "scene/frost_planeobstacle.h"
#include "scene/frost_physicsmanager.h"
#include "frost_engine.h"

#include <OgreSceneManager.h>
#include <OgreMeshManager.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>
#include <OgreHardwareBufferManager.h>
#include <OgreEntity.h>
#include <OgreTextureManager.h>
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>
#include <OgrePass.h>

#include <algorithm>

#include <frost_utils_file.h>

using namespace std;

namespace Frost
{
    struct TerrainHeader
    {
        /// Makes sure we are opening a terrain file
        char sID[2];
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
        /// Vertex normal
        float fNormal[3];
        /// Texture coordinates
        float fUVs[4];
        /// Flags
        uchar ucFlags;
    };

    const s_str TerrainChunk::CLASS_NAME = "TerrainChunk";

    TerrainChunk::TerrainChunk(const s_uint& uiID) : uiID_(uiID)
    {
    }

    TerrainChunk::~TerrainChunk()
    {
        Unload();
    }

    void TerrainChunk::Load()
    {
        if (!bLoaded_)
        {
            Log<3>("Loading terrain chunk : "+uiID_);
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
                    Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0
                );
                pDecl->addElement(
                    0, (3+3+2)*Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT1),
                    Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 1
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

                        // Texture coordinates (normalized)
                        lData.PushBack(i/(float)(uiNX-1));
                        lData.PushBack(j/(float)(uiNZ-1));
                        // Texture coordinates (constant scale)
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

                mBoundingBox_ = AxisAlignedBox(
                    Vector(-fXSize/2.0f, -0.1, -fZSize/2.0f),
                    Vector( fXSize/2.0f,  0.1,  fZSize/2.0f)
                );

                pMesh_->_setBounds(AxisAlignedBox::FrostToOgre(mBoundingBox_));

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

                s_ptr<MovableObstacle> pMObstacle = new PlaneObstacle(fXSize_, fZSize_);
                pMObstacle->SetPosition(mPosition_);

                pObstacle_ = pMObstacle;
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

                if (mHeader.sID[0] != 'F' || mHeader.sID[1] != 'T')
                {
                    throw Exception(CLASS_NAME,
                        "\""+sTerrainFile_+"\" is not a terrain file."
                    );
                }

                s_str sVersion;
                for (int i = 0; i < 4; ++i)
                    sVersion.PushBack(mHeader.sVersion[i]);

                if (sVersion > "0001")
                {
                    throw Exception(CLASS_NAME,
                        "\""+sTerrainFile_+"\" uses a too high file version ("+sVersion+" > 0001)."
                    );
                }

                uint uiNX = mHeader.uiNumXPoint;
                uint uiNZ = mHeader.uiNumZPoint;
                float fXSize = mHeader.fXSize;
                float fYSize = mHeader.fYSize;
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
                    Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0
                );
                pDecl->addElement(
                    0, (3+3+2)*Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT1),
                    Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 1
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
                mFile.Close();

                uint uiParamNbr = 3+3+2+2;
                s_array<float> lData; lData.Resize(uiVertexCount*uiParamNbr);
                s_array<ushort> lIndices; lIndices.Resize(uiIndexCount);

                s_array<s_float> lHeightData;
                lHeightData.Resize(uiNX*uiNZ);

                float fNX = uiNX;
                float fNZ = uiNZ;

                float fOffX = (uiNX % 2 == 0) ? 0.5f - 1.0f/(2.0f*uiNX) : 0.5f;
                float fOffZ = (uiNZ % 2 == 0) ? 0.5f - 1.0f/(2.0f*uiNZ) : 0.5f;

                s_float fYMin = s_float::INFPLUS;
                s_float fYMax = s_float::INFMINUS;

                for (uint x = 0; x < uiNX; ++x)
                {
                    for (uint z = 0; z < uiNZ; ++z)
                    {
                        uint j = x*uiNZ + z;
                        uint i = j*uiParamNbr;

                        float fHeight = lPointList[j].fHeight;

                        if (lPointList[j].ucFlags == 1)
                            lHeightData[j] = s_float::NaN;
                        else
                        {
                            s_float fTemp = lHeightData[j] = fHeight*fYSize;
                            if (fTemp > fYMax)
                                fYMax = fTemp;

                            if (fTemp < fYMin)
                                fYMin = fTemp;
                        }

                        // Position
                        lData[i+0] = fXSize*(x/(fNX-1) - fOffX);
                        lData[i+1] = fHeight*fYSize;
                        lData[i+2] = fZSize*(z/(fNZ-1) - fOffZ);

                        // Normal
                        lData[i+3] = lPointList[j].fNormal[0];
                        lData[i+4] = lPointList[j].fNormal[1];
                        lData[i+5] = lPointList[j].fNormal[2];

                        // Texture coordinates (normalized)
                        lData[i+6] = lPointList[j].fUVs[0];
                        lData[i+7] = lPointList[j].fUVs[1];
                        // Texture coordinates (constant scale)
                        lData[i+8] = lPointList[j].fUVs[2];
                        lData[i+9] = lPointList[j].fUVs[3];
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
                lTriangleArray.Reserve(uiIndexCount/3);
                TerrainObstacle::Triangle mTri;

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
                }

                pVBuf->writeData(0, pVBuf->getSizeInBytes(), lData.GetClassicArray(), true);
                Ogre::VertexBufferBinding* pBind = pVertexData->vertexBufferBinding;
                pBind->setBinding(0, pVBuf);

                pIBuf->writeData(0, pIBuf->getSizeInBytes(), lIndices.GetClassicArray(), true);

                if (!pMat_)
                    pMat_ = MaterialManager::GetSingleton()->GetDefault3D();

                pSubMesh->setMaterialName(pMat_->GetOgreMaterial()->getName());

                mBoundingBox_ = AxisAlignedBox(
                    Vector(-fXSize*fOffX, fYMin-1, -fZSize*fOffZ),
                    Vector( fXSize*(1.0f-fOffX), fYMax+1,  fZSize*(1.0f-fOffZ))
                );

                pMesh_->_setBounds(AxisAlignedBox::FrostToOgre(mBoundingBox_));

                pMesh_->_setBoundingSphereRadius(
                    std::max(fXSize*(1.0f-fOffX), fZSize*(1.0f-fOffZ))
                );

                pMesh_->load();

                pEntity_ = pSceneMgr->createEntity(
                    ("ChunkEntity_"+uiID_).Get(), ("ChunkMesh_"+uiID_).Get()
                );

                pNode_ = pSceneMgr->getRootSceneNode()->createChildSceneNode(
                    Vector::FrostToOgre(mPosition_)
                );
                pNode_->attachObject(pEntity_.Get());

                pObstacle_ = new TerrainObstacle(
                    lTriangleArray, lHeightData, uiNX, uiNZ,
                    fXSize, fZSize, fOffX, fOffZ, this
                );
            }

            bLoaded_ = true;
        }
    }

    void TerrainChunk::Unload()
    {
        if (bLoaded_)
        {
            pObstacle_.Delete();

            pEntity_->detatchFromParent();
            Engine::GetSingleton()->GetOgreSceneManager()->destroyEntity(pEntity_.Get());
            pEntity_ = nullptr;

            Ogre::MeshManager::getSingletonPtr()->remove(("ChunkMesh_"+uiID_).Get());
            pMesh_ = nullptr;

            Engine::GetSingleton()->GetOgreSceneManager()->destroySceneNode(pNode_.Get());
            pNode_ = nullptr;

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

    const s_str& TerrainChunk::GetTerrainFile() const
    {
        return sTerrainFile_;
    }

    void TerrainChunk::SetPlane( const s_float& fXSize, const s_float& fZSize, const s_float& fDensity )
    {
        bPlane_ = true;

        fXSize_ = fXSize;
        fZSize_ = fZSize;
        fDensity_ = fDensity;
    }

    const s_float& TerrainChunk::GetPlaneXSize() const
    {
        return fXSize_;
    }

    const s_float& TerrainChunk::GetPlaneZSize() const
    {
        return fZSize_;
    }

    const s_float& TerrainChunk::GetPlaneDensity() const
    {
        return fDensity_;
    }

    const s_bool& TerrainChunk::IsPlanar() const
    {
        return bPlane_;
    }

    void TerrainChunk::SetMaterialInfo( const MaterialInfo& mMatInfo )
    {
        mMatInfo_ = mMatInfo;

        s_bool bEnableSpecular = Engine::GetSingleton()->GetBoolConstant("EnableSpecular");

        s_str sTFO = Engine::GetSingleton()->GetStringConstant("TerrainTextureFiltering");
        Ogre::TextureFilterOptions mTFO;
        if (sTFO == "ANISOTROPIC")
            mTFO = Ogre::TFO_ANISOTROPIC;
        else if (sTFO == "BILINEAR")
            mTFO = Ogre::TFO_BILINEAR;
        else
        {
            Warning("Engine",
                "Unknown value for \"TerrainTextureFiltering\" : \""+sTFO+"\". "
                "Using no filtering."
            );
            mTFO = Ogre::TFO_NONE;
        }

        if (mMatInfo_.sMaskFile.IsEmpty())
        {
            const MaterialInfo::Layer& mLayer = mMatInfo_.lLayerList[0];
            pMat_ = MaterialManager::GetSingleton()->CreateMaterial3D(mLayer.sDiffuseFile);
            pMat_->GetDefaultPass()->getTextureUnitState(0)->setTextureFiltering(mTFO);
            pMat_->SetShaders("Terrain");
            pMat_->SetTilling(mLayer.fXTilling, mLayer.fZTilling);

            if (!mLayer.sSpecularFile.IsEmpty() && bEnableSpecular)
            {
                s_ptr<Ogre::Pass> pPass = pMat_->GetDefaultPass();
                s_ptr<Ogre::TextureUnitState> pTUS = pPass->createTextureUnitState();
                Ogre::TextureManager::getSingleton().load(mLayer.sSpecularFile.Get(), "Frost");
                pTUS->setTextureName(mLayer.sSpecularFile.Get());
                pTUS->setTextureFiltering(mTFO);
            }
        }
        else
        {
            s_ptr<Ogre::Material> pOgreMat = dynamic_cast<Ogre::Material*>(Ogre::MaterialManager::getSingleton().create(
                ("_TCMat_"+uiID_).Get(), "Frost"
            ).get());

            s_ptr<Ogre::Pass> pPass = pOgreMat->getTechnique(0)->getPass(0);
            pPass->setDiffuse(Ogre::ColourValue(1.0f,1.0f,1.0f));

            s_ptr<VertexShader> pVS = ShaderManager::GetSingleton()->GetVertexShader(
                "Terrain_Splatting_"+mMatInfo_.uiLayerCount
            );
            s_ptr<PixelShader> pPS = ShaderManager::GetSingleton()->GetPixelShader(
                "Terrain_Splatting_"+mMatInfo_.uiLayerCount
            );
            pVS->BindTo(pPass);
            pPS->BindTo(pPass);

            s_ptr<Ogre::TextureUnitState> pTUS = pPass->createTextureUnitState();
            Ogre::TextureManager::getSingleton().load(mMatInfo_.sMaskFile.Get(), "Frost");
            pTUS->setTextureName(mMatInfo_.sMaskFile.Get());
            pTUS->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);

            for (s_uint i = 0; i < mMatInfo_.uiLayerCount; ++i)
            {
                const MaterialInfo::Layer& mLayer = mMatInfo_.lLayerList[i];
                Ogre::TextureManager::getSingleton().load(mLayer.sDiffuseFile.Get(), "Frost");

                s_ptr<Ogre::TextureUnitState> pTUS = pPass->createTextureUnitState();
                pTUS->setTextureName(mLayer.sDiffuseFile.Get());
                pTUS->setTextureFiltering(mTFO);
                pTUS->setTextureScale(mLayer.fXTilling.Get(), mLayer.fZTilling.Get());

                if (!mLayer.sSpecularFile.IsEmpty() && bEnableSpecular)
                {
                    Ogre::TextureManager::getSingleton().load(mLayer.sSpecularFile.Get(), "Frost");
                    pTUS = pPass->createTextureUnitState();
                    pTUS->setTextureName(mLayer.sSpecularFile.Get());
                    pTUS->setTextureFiltering(mTFO);
                }
            }

            pMat_ = MaterialManager::GetSingleton()->CreateMaterial(pOgreMat);
        }
    }

    const TerrainChunk::MaterialInfo& TerrainChunk::GetMaterialInfo() const
    {
        return mMatInfo_;
    }

    void TerrainChunk::SetAlwaysVisible( const s_bool& bAlwaysVisible )
    {
        bAlwaysVisible_ = bAlwaysVisible;

        if (bAlwaysVisible_)
            mVisibilityBoundingBox_ = AxisAlignedBox();
    }

    const s_bool& TerrainChunk::IsAlwaysVisible() const
    {
        return bAlwaysVisible_;
    }

    void TerrainChunk::SetPosition( const Vector& mPosition )
    {
        mPosition_ = mPosition;
    }

    void TerrainChunk::SetVisibilityBox( const AxisAlignedBox& mBox )
    {
        bAlwaysVisible_ = mBox.IsInfinite();
        mVisibilityBoundingBox_ = mBox;
    }

    const Vector& TerrainChunk::GetPosition() const
    {
        return mPosition_;
    }

    AxisAlignedBox TerrainChunk::GetVisibilityBox( const s_bool& bLocalSpace ) const
    {
        if (bLocalSpace)
            return mVisibilityBoundingBox_;
        else
            return mVisibilityBoundingBox_ + mPosition_;
    }

    AxisAlignedBox TerrainChunk::GetBoundingBox( const s_bool& bLocalSpace ) const
    {
        if (bLocalSpace)
            return mBoundingBox_;
        else
            return mBoundingBox_ + mPosition_;
    }

    s_bool TerrainChunk::ContainsPoint( const s_float& fX, const s_float& fZ ) const
    {
        Vector mTemp = Vector(
            fX - mPosition_.X(),
            (mBoundingBox_.GetMax().Y() + mBoundingBox_.GetMin().Y())/2.0f,
            fZ - mPosition_.Z()
        );

        return mBoundingBox_.Contains(mTemp);
    }

    s_float TerrainChunk::GetPointHeight( const s_float& fX, const s_float& fZ ) const
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
                return pTerrainObstacle->GetPointHeight(fX, fZ) + mPosition_.Y();
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

    s_bool TerrainChunk::GetRayIntersection( const Vector& mRayOrigin, const Vector& mRayDirection, Vector& mPosition ) const
    {
        if (bLoaded_)
        {
            return pObstacle_->GetRayIntersection(mRayOrigin, mRayDirection, mPosition);
        }
        else
            return false;
    }

    s_wptr<Material> TerrainChunk::GetMaterial()
    {
        return pMat_;
    }

    const s_uint& TerrainChunk::GetID() const
    {
        return uiID_;
    }
}
