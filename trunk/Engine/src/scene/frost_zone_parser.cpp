/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Zone source               */
/*                                        */
/*                                        */

#include "scene/frost_zone.h"
#include "scene/frost_zonemanager.h"
#include "scene/frost_doodad.h"
#include <scene/frost_terrainchunk.h>
#include <material/frost_material.h>
#include <material/frost_materialmanager.h>
#include <model/frost_model.h>
#include <frost_xml_document.h>
#include <frost_utils_file.h>

using namespace std;

namespace Frost
{
    void Zone::ParseXMLFile( const s_str& sFile )
    {
        XML::Document mDoc(sFile, "Zones/Zone.def");
        if (mDoc.Check())
        {
            sName_ = mDoc.GetMainBlock()->GetAttribute("name");

            s_ptr<XML::Block> pElemBlock;
            foreach_block (pElemBlock, mDoc.GetMainBlock())
            {
                if (pElemBlock->GetName() == "AmbientColor")
                {
                    SetAmbientColor(Color(
                        s_float(pElemBlock->GetAttribute("r")),
                        s_float(pElemBlock->GetAttribute("g")),
                        s_float(pElemBlock->GetAttribute("b"))
                    ));
                }
                else if (pElemBlock->GetName() == "SunColor")
                {
                    SetSunColor(Color(
                        s_float(pElemBlock->GetAttribute("r")),
                        s_float(pElemBlock->GetAttribute("g")),
                        s_float(pElemBlock->GetAttribute("b"))
                    ));
                }
                else if (pElemBlock->GetName() == "Script")
                {
                    try
                    {
                        ZoneManager::GetSingleton()->GetLua()->DoFile(pElemBlock->GetAttribute("file"));
                    }
                    catch (LuaException& e)
                    {
                        Warning(CLASS_NAME, e.GetDescription());
                    }
                }
                else if (pElemBlock->GetName() == "Terrain")
                {
                    ParseTerrain_(pElemBlock);
                }
                else if (pElemBlock->GetName() == "Models")
                {
                    ParseModels_(pElemBlock);
                }
                else if (pElemBlock->GetName() == "Doodads")
                {
                    ParseDoodads_(pElemBlock);
                }
            }
        }
    }

    void Zone::ParseModels_( s_ptr<XML::Block> pModelsBlock )
    {
        s_ptr<XML::Block> pModelBlock;
        foreach_block (pModelBlock, pModelsBlock)
        {
            s_str sModel = pModelBlock->GetAttribute("name");
            s_bool bResult = RegisterModel(
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
                    ModelMaterial mModelMat(pMaterialsBlock);
                    SetMaterialInfo(sModel, mModelMat);
                }
            }
        }
    }

    void Zone::ParseDoodads_( s_ptr<XML::Block> pDoodadsBlock )
    {
        s_ptr<XML::Block> pDoodadBlock;
        foreach_block (pDoodadBlock, pDoodadsBlock)
        {
            s_ptr<Doodad> pDoodad = AddDoodad(
                pDoodadBlock->GetAttribute("name"),
                pDoodadBlock->GetAttribute("model")
            );

            if (s_bool(pDoodadBlock->GetAttribute("collisions")))
                pDoodad->EnableCollisions();

            if (s_bool(pDoodadBlock->GetAttribute("hidden")))
                pDoodad->Hide();
            else
                pDoodad->Show();

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
                pDoodad->SetOrientation(Ogre::Quaternion(
                    s_float(pOrientation->GetAttribute("w")).Get(),
                    s_float(pOrientation->GetAttribute("x")).Get(),
                    s_float(pOrientation->GetAttribute("y")).Get(),
                    s_float(pOrientation->GetAttribute("z")).Get()
                ));
            }
        }
    }

    void Zone::ParseTerrain_( s_ptr<XML::Block> pTerrainBlock )
    {
        s_ptr<XML::Block> pChunksBlock = pTerrainBlock->GetBlock("Chunks");
        s_ptr<XML::Block> pChunkBlock;
        s_uint uiChunkNbr;
        foreach_block (pChunkBlock, pChunksBlock)
        {
            s_ptr<TerrainChunk> pChunk = new TerrainChunk(lChunkList_.GetSize());

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
                                "In zone \""+sName_+"\" : mask texture provided with "
                                "only one texture to blend for chunk "+uiChunkNbr+". Mask texture ignored."
                            );
                        else
                            Warning(CLASS_NAME,
                                "In zone \""+sName_+"\" : no mask texture, but "
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

            AddChunk(pChunk);
            ++uiChunkNbr;
        }
    }
}
