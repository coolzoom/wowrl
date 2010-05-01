/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           ZoneManager source           */
/*                                        */
/*                                        */

#include "scene/frost_zonemanager.h"
#include "scene/frost_zone.h"
#include "scene/frost_terrainchunk.h"
#include "scene/frost_doodad.h"
#include "material/frost_material.h"
#include "material/frost_materialmanager.h"
#include "xml/frost_xml_document.h"

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

                        if (lMatList.Find(iID) && lMatList[iID].Find(iEntityID))
                        {
                            Warning(pMaterialBlock->GetFile()+":"+pMaterialBlock->GetLineNbr(),
                                "A material has already been defined for \""+sModel+"\""+
                                (iID < 0 ? "." : ("'s submesh "+iID+
                                (iEntityID < 0 ? "." : ("(subentity "+iEntityID+")."))))
                            );
                            continue;
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
                    s_float(pOrientation->GetAttribute("x")),
                    s_float(pOrientation->GetAttribute("y")),
                    s_float(pOrientation->GetAttribute("z")),
                    s_float(pOrientation->GetAttribute("w"))
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
                pChunk->SetVisibilityBox(AxisAlignedBox(-mSize, mSize));
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
                TerrainChunk::MaterialInfo mMatInfo;
                s_bool bError = false;
                s_ptr<XML::Block> pMaskBlock = pTexturesBlock->GetBlock("Mask");

                if (pMaskBlock && pTexturesBlock->GetChildNumber() > 2)
                {
                    mMatInfo.sMaskFile = pMaskBlock->GetAttribute("file");
                    if (!File::Exists(mMatInfo.sMaskFile))
                    {
                        Warning(CLASS_NAME,
                            "Parsing "+pMaskBlock->GetFile()+" :\n"
                            "Couldn't find texture : \""+mMatInfo.sMaskFile+"\""
                        );
                        bError = true;
                    }

                    mMatInfo.uiLayerCount = pTexturesBlock->GetChildNumber("Layer");

                    s_uint uiLayer = 0;
                    s_ptr<XML::Block> pLayerBlock;
                    foreach_named_block (pLayerBlock, "Layer", pTexturesBlock)
                    {
                        TerrainChunk::MaterialInfo::Layer& mLayer = mMatInfo.lLayerList[uiLayer];

                        s_ptr<XML::Block> pDiffuseBlock = pLayerBlock->GetBlock("Diffuse");
                        mLayer.sDiffuseFile = pDiffuseBlock->GetAttribute("file");
                        if (!File::Exists(mLayer.sDiffuseFile))
                        {
                            Warning(CLASS_NAME,
                                "Parsing "+pLayerBlock->GetFile()+" :\n"
                                "Couldn't find texture : \""+mLayer.sDiffuseFile+"\""
                            );
                            bError = true;
                        }

                        s_ptr<XML::Block> pTillingBlock = pDiffuseBlock->GetBlock("Tilling");
                        if (pTillingBlock)
                        {
                            mLayer.fXTilling = s_float(pTillingBlock->GetAttribute("x"));
                            mLayer.fZTilling = s_float(pTillingBlock->GetAttribute("z"));
                        }

                        s_ptr<XML::Block> pSpecularBlock = pLayerBlock->GetBlock("Specular");
                        if (pSpecularBlock)
                        {
                            mLayer.sSpecularFile = pSpecularBlock->GetAttribute("file");
                            if (!File::Exists(mLayer.sSpecularFile))
                            {
                                Warning(CLASS_NAME,
                                    "Parsing "+pLayerBlock->GetFile()+" :\n"
                                    "Couldn't find texture : \""+mLayer.sSpecularFile+"\""
                                );
                                bError = true;
                            }
                        }

                        ++uiLayer;
                    }

                    if (!bError)
                        pChunk->SetMaterialInfo(mMatInfo);
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

                    mMatInfo.uiLayerCount = 1;
                    TerrainChunk::MaterialInfo::Layer& mLayer = mMatInfo.lLayerList[0];

                    s_ptr<XML::Block> pDiffuseBlock = pLayerBlock->GetBlock("Diffuse");
                    mLayer.sDiffuseFile = pDiffuseBlock->GetAttribute("file");
                    if (!File::Exists(mLayer.sDiffuseFile))
                    {
                        Warning(CLASS_NAME,
                            "Parsing "+pLayerBlock->GetFile()+" :\n"
                            "Couldn't find texture : \""+mLayer.sDiffuseFile+"\""
                        );
                        bError = true;
                    }

                    s_ptr<XML::Block> pTillingBlock = pDiffuseBlock->GetBlock("Tilling");
                    if (pTillingBlock)
                    {
                        mLayer.fXTilling = s_float(pTillingBlock->GetAttribute("x"));
                        mLayer.fZTilling = s_float(pTillingBlock->GetAttribute("z"));
                    }

                    s_ptr<XML::Block> pSpecularBlock = pLayerBlock->GetBlock("Specular");
                    if (pSpecularBlock)
                    {
                        mLayer.sSpecularFile = pSpecularBlock->GetAttribute("file");
                        if (!File::Exists(mLayer.sSpecularFile))
                        {
                            Warning(CLASS_NAME,
                                "Parsing "+pSpecularBlock->GetFile()+" :\n"
                                "Couldn't find texture : \""+mLayer.sSpecularFile+"\""
                            );
                            bError = true;
                        }
                    }

                    if (!bError)
                        pChunk->SetMaterialInfo(mMatInfo);
                }
            }

            ++uiChunkNbr;
        }
    }
}
