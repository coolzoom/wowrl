/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           UnitManager source           */
/*                                        */
/*                                        */

#include "scene/frost_zonemanager.h"
#include "scene/frost_zone.h"
#include "scene/frost_terrainchunk.h"
#include "scene/frost_doodad.h"
#include "material/frost_material.h"
#include "material/frost_materialmanager.h"
#include "xml/frost_xml_document.h"
#include "material/frost_shadermanager.h"
#include "material/frost_shader.h"

#include <OgreTextureManager.h>
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>
#include <OgrePass.h>

using namespace std;

namespace Frost
{
    void ZoneManager::ParseXMLFile_( const s_str& sFile, s_ptr<Zone> pCurrentZone )
    {
        XML::Document mDoc(sFile, "Zones/Zone.def");
        if (mDoc.Check())
        {
            s_ptr<XML::Block> pElemBlock;
            foreach_block (pElemBlock, mDoc.GetMainBlock())
            {
                if (pElemBlock->GetName() == "AmbientColor")
                {
                    pCurrentZone->SetAmbientColor(Color(
                        s_uchar(s_float(pElemBlock->GetAttribute("r"))*255),
                        s_uchar(s_float(pElemBlock->GetAttribute("g"))*255),
                        s_uchar(s_float(pElemBlock->GetAttribute("b"))*255)
                    ));
                }
                else if (pElemBlock->GetName() == "SunColor")
                {
                    pCurrentZone->SetSunColor(Color(
                        s_uchar(s_float(pElemBlock->GetAttribute("r"))*255),
                        s_uchar(s_float(pElemBlock->GetAttribute("g"))*255),
                        s_uchar(s_float(pElemBlock->GetAttribute("b"))*255)
                    ));
                }
                else if (pElemBlock->GetName() == "Script")
                {
                    pLua_->DoFile(pElemBlock->GetAttribute("file"));
                }
                else if (pElemBlock->GetName() == "Terrain")
                {
                    this->ParseTerrain_(pCurrentZone, pElemBlock);
                }
                else if (pElemBlock->GetName() == "Models")
                {
                    this->ParseModels_(pCurrentZone, pElemBlock);
                }
                else if (pElemBlock->GetName() == "Doodads")
                {
                    this->ParseDoodads_(pCurrentZone, pElemBlock);
                }
            }
        }
    }

    void ZoneManager::ParseModels_( s_ptr<Zone> pCurrentZone, s_ptr<XML::Block> pModelsBlock )
    {
        s_ptr<XML::Block> pModelBlock;
        foreach_block (pModelBlock, pModelsBlock)
        {
            s_str sModel = pModelBlock->GetAttribute("name");
            s_bool bResult = pCurrentZone->RegisterModel(
                sModel, pModelBlock->GetAttribute("file")
            );

            if (!bResult)
            {
                Warning(pModelBlock->GetFile()+":"+pModelBlock->GetLineNbr(),
                    "Couldn't link or preload model \""+sModel+"\"."
                );
            }
            else
            {
                s_ptr<XML::Block> pMaterialsBlock = pModelBlock->GetBlock("Materials");
                if (pMaterialsBlock)
                {
                    s_map< s_int, s_map<s_int, MaterialInfo> > lMatList;

                    s_ptr<XML::Block> pMaterialBlock;
                    foreach_block (pMaterialBlock, pMaterialsBlock)
                    {
                        s_int iID = s_int(pMaterialBlock->GetAttribute("subMeshID"));
                        s_int iEntityID = s_int(pMaterialBlock->GetAttribute("subEntityID"));
                        if (iID < 0)
                            iID = -1;
                        if (iEntityID < 0)
                            iEntityID = -1;

                        if (lMatList.Find(iID))
                        {
                            if (lMatList[iID].Find(iEntityID))
                            {
                                Warning(pMaterialBlock->GetFile()+":"+pMaterialBlock->GetLineNbr(),
                                    "A material has already been defined for \""+sModel+"\""+
                                    (iID < 0 ? "." : ("'s submesh "+iID+
                                    (iEntityID < 0 ? "." : ("(subentity "+iEntityID+")."))))
                                );
                                continue;
                            }
                        }

                        MaterialInfo mInfo;
                        s_ptr<XML::Block> pDiffuseBlock = pMaterialBlock->GetRadioBlock(1);
                        if (pDiffuseBlock->GetName() == "DiffuseColor")
                        {
                            mInfo.bDiffuseColor = true;
                            mInfo.mDiffuseColor = Color(
                                s_uchar(s_float(pDiffuseBlock->GetAttribute("a"))*255),
                                s_uchar(s_float(pDiffuseBlock->GetAttribute("r"))*255),
                                s_uchar(s_float(pDiffuseBlock->GetAttribute("g"))*255),
                                s_uchar(s_float(pDiffuseBlock->GetAttribute("b"))*255)
                            );
                        }
                        else
                        {
                            mInfo.bDiffuseColor = false;
                            mInfo.sDiffuseFile = pDiffuseBlock->GetAttribute("file");
                            mInfo.bAlphaReject = s_bool(pDiffuseBlock->GetAttribute("alphaReject"));
                        }

                        lMatList[iID][iEntityID] = mInfo;
                    }

                    pCurrentZone->SetMaterialInfo(sModel, lMatList);
                }
            }
        }
    }

    void ZoneManager::ParseDoodads_( s_ptr<Zone> pCurrentZone, s_ptr<XML::Block> pDoodadsBlock )
    {
        s_ptr<XML::Block> pDoodadBlock;
        foreach_block (pDoodadBlock, pDoodadsBlock)
        {
            s_ptr<Doodad> pDoodad = pCurrentZone->AddDoodad(
                pDoodadBlock->GetAttribute("name"),
                pDoodadBlock->GetAttribute("model")
            );

            if (s_bool(pDoodadBlock->GetAttribute("hidden")))
                pDoodad->Hide();

            s_ptr<XML::Block> pPosition = pDoodadBlock->GetBlock("Position");
            pDoodad->SetPosition(Vector(
                s_float(pPosition->GetAttribute("x")),
                s_float(pPosition->GetAttribute("y")),
                s_float(pPosition->GetAttribute("z"))
            ));

            s_ptr<XML::Block> pScale = pDoodadBlock->GetBlock("Scale");
            if (pScale)
            {
                pDoodad->SetScale(Vector(
                    s_float(pScale->GetAttribute("x")),
                    s_float(pScale->GetAttribute("y")),
                    s_float(pScale->GetAttribute("z"))
                ));
            }

            s_ptr<XML::Block> pOrientation = pDoodadBlock->GetBlock("Orientation");
            if (pOrientation)
            {
                pDoodad->SetOrientation(
                    s_float(pScale->GetAttribute("x")),
                    s_float(pScale->GetAttribute("y")),
                    s_float(pScale->GetAttribute("z")),
                    s_float(pScale->GetAttribute("w"))
                );
            }
        }
    }

    void ZoneManager::ParseTerrain_( s_ptr<Zone> pCurrentZone, s_ptr<XML::Block> pTerrainBlock )
    {
        s_ptr<XML::Block> pChunksBlock = pTerrainBlock->GetBlock("Chunks");
        s_ptr<XML::Block> pChunkBlock;
        s_uint uiChunkNbr;
        foreach_block (pChunkBlock, pChunksBlock)
        {
            s_ptr<TerrainChunk> pChunk = new TerrainChunk(pCurrentZone);

            // Position
            s_ptr<XML::Block> pPositionBlock = pChunkBlock->GetBlock("Position");
            if (pPositionBlock)
            {
                pChunk->SetPosition(Vector(
                    s_float(pPositionBlock->GetAttribute("x")),
                    s_float(pPositionBlock->GetAttribute("y")),
                    s_float(pPositionBlock->GetAttribute("z"))
                ));
            }

            // Bounding Box
            s_ptr<XML::Block> pBoundingBoxBlock = pChunkBlock->GetBlock("BoundingBox");
            pChunk->SetAlwaysVisible(s_bool(pBoundingBoxBlock->GetAttribute("alwaysVisible")));
            s_ptr<XML::Block> pSizeBlock = pBoundingBoxBlock->GetBlock("Size");
            if (!pChunk->IsAlwaysVisible() && pSizeBlock)
            {
                Vector mSize(
                    s_float(pSizeBlock->GetAttribute("x")),
                    s_float(pSizeBlock->GetAttribute("y")),
                    s_float(pSizeBlock->GetAttribute("z"))
                );
                pChunk->SetBoundingBox(AxisAlignedBox(-mSize, mSize));
            }

            // Mesh Data
            s_ptr<XML::Block> pDataBlock = pChunkBlock->GetBlock("Data");
            s_ptr<XML::Block> pChildBlock = pDataBlock->GetRadioBlock();
            if (pChildBlock->GetName() == "TerrainFile")
            {
                pChunk->SetTerrainFile(pChildBlock->GetAttribute("file"));
            }
            else
            {
                pChunk->SetPlane(
                    s_float(pChildBlock->GetAttribute("xSize")),
                    s_float(pChildBlock->GetAttribute("zSize")),
                    s_float(pChildBlock->GetAttribute("density"))
                );
            }

            // Textures
            s_ptr<XML::Block> pTexturesBlock = pChunkBlock->GetBlock("Textures");
            if (pTexturesBlock)
            {
                s_bool bEnableSpecular = Engine::GetSingleton()->GetBoolConstant("EnableSpecular");
                s_ptr<XML::Block> pMaskBlock = pTexturesBlock->GetBlock("Mask");

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

                if (pMaskBlock && pTexturesBlock->GetChildNumber() > 2)
                {
                    s_str sMaskFile = pMaskBlock->GetAttribute("file");

                    s_ptr<Ogre::Material> pOgreMat = (Ogre::Material*)Ogre::MaterialManager::getSingleton().create(
                        ("_TCMat_"+uiChunkNbr).Get(), "Frost"
                    ).get();

                    s_ptr<Ogre::Pass> pPass = pOgreMat->getTechnique(0)->getPass(0);
                    pPass->setDiffuse(Ogre::ColourValue(1.0f,1.0f,1.0f));

                    s_uint uiLayerNbr = pTexturesBlock->GetChildNumber("Layer");

                    s_ptr<VertexShader> pVS = ShaderManager::GetSingleton()->GetVertexShader("Terrain_Splatting_"+uiLayerNbr);
                    s_ptr<PixelShader> pPS = ShaderManager::GetSingleton()->GetPixelShader("Terrain_Splatting_"+uiLayerNbr);
                    pVS->BindTo(pPass);
                    pPS->BindTo(pPass);

                    s_ptr<Ogre::TextureUnitState> pTUS = pPass->createTextureUnitState();
                    Ogre::TextureManager::getSingleton().load(sMaskFile.Get(), "Frost");
                    pTUS->setTextureName(sMaskFile.Get());

                    s_ptr<XML::Block> pLayerBlock;
                    s_uint uiLayer = 1;
                    foreach_named_block (pLayerBlock, "Layer", pTexturesBlock)
                    {
                        s_ptr<XML::Block> pDiffuseBlock = pLayerBlock->GetBlock("Diffuse");
                        s_ptr<Ogre::TextureUnitState> pTUS = pPass->createTextureUnitState();
                        s_str sFileName = pDiffuseBlock->GetAttribute("file");
                        Ogre::TextureManager::getSingleton().load(sFileName.Get(), "Frost");
                        pTUS->setTextureName(sFileName.Get());
                        pTUS->setTextureFiltering(mTFO);

                        s_ptr<XML::Block> pTillingBlock = pDiffuseBlock->GetBlock("Tilling");
                        if (pTillingBlock)
                        {
                            pTUS->setTextureScale(
                                s_float(pTillingBlock->GetAttribute("x")).Get(),
                                s_float(pTillingBlock->GetAttribute("y")).Get()
                            );
                        }

                        s_ptr<XML::Block> pSpecularBlock = pLayerBlock->GetBlock("Specular");
                        if (pSpecularBlock && bEnableSpecular)
                        {
                            pTUS = pPass->createTextureUnitState();
                            sFileName = pSpecularBlock->GetAttribute("file");
                            Ogre::TextureManager::getSingleton().load(sFileName.Get(), "Frost");
                            pTUS->setTextureName(sFileName.Get());
                            pTUS->setTextureFiltering(mTFO);
                        }

                        ++uiLayer;
                    }

                    pChunk->SetMaterial(MaterialManager::GetSingleton()->CreateMaterial(pOgreMat));
                }
                else if (pTexturesBlock->GetBlock("Layer"))
                {
                    s_ptr<XML::Block> pLayerBlock;
                    if (pTexturesBlock->GetChildNumber() == 1)
                    {
                        pLayerBlock = pTexturesBlock->GetBlock("Layer");
                    }
                    else
                    {
                        if (pMaskBlock)
                            Warning(CLASS_NAME,
                                "In zone \""+pCurrentZone->GetName()+"\" : mask texture provided with "
                                "only one texture to blend for chunk "+uiChunkNbr+". Mask texture ignored."
                            );
                        else
                            Warning(CLASS_NAME,
                                "In zone \""+pCurrentZone->GetName()+"\" : no mask texture, but "
                                "several texture layers provided for chunk "+uiChunkNbr+". Only "
                                "the first layer will be used."
                            );
                        pLayerBlock = pTexturesBlock->First("Layer");
                    }

                    s_ptr<XML::Block> pDiffuseBlock = pLayerBlock->GetBlock("Diffuse");
                    s_refptr<Material> pMat = MaterialManager::GetSingleton()->CreateMaterial3D(
                        pDiffuseBlock->GetAttribute("file")
                    );
                    pMat->GetDefaultPass()->getTextureUnitState(0)->setTextureFiltering(mTFO);
                    pMat->SetShaders("Terrain");

                    s_ptr<XML::Block> pTillingBlock = pDiffuseBlock->GetBlock("Tilling");
                    if (pTillingBlock)
                    {
                        pMat->SetTilling(
                            s_float(pTillingBlock->GetAttribute("x")),
                            s_float(pTillingBlock->GetAttribute("y"))
                        );
                    }

                    s_ptr<XML::Block> pSpecularBlock = pLayerBlock->GetBlock("Specular");
                    if (pSpecularBlock && bEnableSpecular)
                    {
                        s_ptr<Ogre::Pass> pPass = pMat->GetDefaultPass();
                        s_ptr<Ogre::TextureUnitState> pTUS = pPass->createTextureUnitState();
                        s_str sFileName = pSpecularBlock->GetAttribute("file");
                        Ogre::TextureManager::getSingleton().load(sFileName.Get(), "Frost");
                        pTUS->setTextureName(sFileName.Get());
                        pTUS->setTextureFiltering(mTFO);
                    }

                    pChunk->SetMaterial(pMat);
                }
            }

            ++uiChunkNbr;
        }
    }
}
