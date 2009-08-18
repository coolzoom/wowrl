/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           UnitManager source           */
/*                                        */
/*                                        */

#include "scene/frost_zonemanager.h"
#include "scene/frost_zone.h"
#include "scene/frost_terrainchunk.h"
#include "material/frost_material.h"
#include "material/frost_materialmanager.h"
#include "xml/frost_xml_document.h"

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
                else if (pElemBlock->GetName() == "Script")
                {
                    pLua_->DoFile(pElemBlock->GetAttribute("file"));
                }
                else if (pElemBlock->GetName() == "Terrain")
                {
                    this->ParseTerrain_(pCurrentZone, pElemBlock);
                }
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

            // Bouding Box
            s_ptr<XML::Block> pBoundingBoxBlock = pChunkBlock->GetBlock("BoundingBox");
            pChunk->SetAlwaysVisible(s_bool(pBoundingBoxBlock->GetAttribute("alwaysVisible")));
            s_ptr<XML::Block> pSizeBlock = pBoundingBoxBlock->GetBlock("Size");
            if (!pChunk->IsAlwaysVisible() && pSizeBlock)
            {
                pChunk->SetSize(Vector(
                    s_float(pSizeBlock->GetAttribute("x")),
                    s_float(pSizeBlock->GetAttribute("y")),
                    s_float(pSizeBlock->GetAttribute("z"))
                ));
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
                s_bool bEnableSpecular = Engine::GetSingleton()->GetConstant("EnableSpecular").Get<s_bool>();
                s_ptr<XML::Block> pMaskBlock = pTexturesBlock->GetBlock("Mask");

                if (pMaskBlock && pTexturesBlock->GetChildNumber() > 2)
                {
                    s_str sMaskFile = pMaskBlock->GetAttribute("file");

                    s_ptr<Ogre::Material> pOgreMat = (Ogre::Material*)Ogre::MaterialManager::getSingleton().create(
                        ("_TCMat_"+uiChunkNbr).Get(), "Frost"
                    ).get();

                    s_ptr<Ogre::Pass> pPass = pOgreMat->getTechnique(0)->getPass(0);
                    pPass->setDiffuse(Ogre::ColourValue(1.0f,1.0f,1.0f));

                    s_uint uiLayerNbr = pTexturesBlock->GetChildNumber("Layer");

                    pPass->setVertexProgram(("Terrain_Splatting_"+uiLayerNbr+"_VS").Get());
                    pPass->setFragmentProgram(("Terrain_Splatting_"+uiLayerNbr+"_PS").Get());

                    Ogre::GpuProgramParametersSharedPtr pParams = pPass->getVertexProgramParameters();
                    pParams->setNamedAutoConstant(
                        "mWorldViewProj",
                        Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX
                    );

                    pParams->setNamedAutoConstant(
                        "mWorld",
                        Ogre::GpuProgramParameters::ACT_WORLD_MATRIX
                    );

                    pParams->setNamedAutoConstant(
                        "mLightPos",
                        Ogre::GpuProgramParameters::ACT_LIGHT_POSITION_ARRAY,
                        5
                    );
                    pParams->setNamedAutoConstant(
                        "mLightDiffuseColor",
                        Ogre::GpuProgramParameters::ACT_DERIVED_LIGHT_DIFFUSE_COLOUR_ARRAY,
                        5
                    );
                    pParams->setNamedAutoConstant(
                        "mLightAtten",
                        Ogre::GpuProgramParameters::ACT_LIGHT_ATTENUATION_ARRAY,
                        5
                    );
                    pParams->setNamedAutoConstant(
                        "mAmbient",
                        Ogre::GpuProgramParameters::ACT_DERIVED_SCENE_COLOUR
                    );
                    if (bEnableSpecular)
                    {
                        pParams->setNamedAutoConstant(
                            "mCamPos",
                            Ogre::GpuProgramParameters::ACT_CAMERA_POSITION
                        );
                    }

                    s_ptr<Ogre::TextureUnitState> pTUS = pPass->createTextureUnitState();
                    Ogre::TextureManager::getSingleton().load(sMaskFile.Get(), "Frost");
                    pTUS->setTextureName(sMaskFile.Get());
                    pTUS->setTextureFiltering(Ogre::TFO_ANISOTROPIC);

                    s_ptr<XML::Block> pLayerBlock;
                    s_uint uiLayer = 1;
                    foreach_named_block (pLayerBlock, "Layer", pTexturesBlock)
                    {
                        s_ptr<XML::Block> pDiffuseBlock = pLayerBlock->GetBlock("Diffuse");
                        s_ptr<Ogre::TextureUnitState> pTUS = pPass->createTextureUnitState();
                        s_str sFileName = pDiffuseBlock->GetAttribute("file");
                        Ogre::TextureManager::getSingleton().load(sFileName.Get(), "Frost");
                        pTUS->setTextureName(sFileName.Get());
                        pTUS->setTextureFiltering(Ogre::TFO_ANISOTROPIC);

                        if (Engine::GetSingleton()->GetRenderer() == "OpenGL")
                        {
                            pParams = pPass->getFragmentProgramParameters();
                            pParams->setNamedConstant(
                                ("mTexture"+uiLayer).Get(),
                                pPass->getNumTextureUnitStates()-1
                            );
                        }

                        s_ptr<XML::Block> pTillingBlock = pDiffuseBlock->GetBlock("Tilling");
                        if (pTillingBlock)
                        {
                            pTUS->setTextureScale(
                                s_float(pTillingBlock->GetAttribute("x")).Get(),
                                s_float(pTillingBlock->GetAttribute("y")).Get()
                            );

                            pParams = pPass->getVertexProgramParameters();
                            pParams->setNamedAutoConstant(
                                ("mTexCoordMat"+uiLayer).Get(),
                                Ogre::GpuProgramParameters::ACT_TEXTURE_MATRIX,
                                pPass->getNumTextureUnitStates()-1
                            );
                        }

                        s_ptr<XML::Block> pSpecularBlock = pLayerBlock->GetBlock("Specular");
                        if (pSpecularBlock && bEnableSpecular)
                        {
                            pTUS = pPass->createTextureUnitState();
                            sFileName = pSpecularBlock->GetAttribute("file");
                            Ogre::TextureManager::getSingleton().load(sFileName.Get(), "Frost");
                            pTUS->setTextureName(sFileName.Get());
                            pTUS->setTextureFiltering(Ogre::TFO_ANISOTROPIC);

                            if (Engine::GetSingleton()->GetRenderer() == "OpenGL")
                            {
                                pParams = pPass->getFragmentProgramParameters();
                                pParams->setNamedConstant(
                                    ("mTexture"+uiLayer+"S").Get(),
                                    pPass->getNumTextureUnitStates()-1
                                );
                            }
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
                        pTUS->setTextureFiltering(Ogre::TFO_ANISOTROPIC);

                        pPass->setVertexProgram("Terrain_Specular_VS");
                        pPass->setFragmentProgram("Terrain_Specular_PS");

                        Ogre::GpuProgramParametersSharedPtr pParams = pPass->getVertexProgramParameters();
                        pParams->setNamedAutoConstant(
                            "mWorldViewProj",
                            Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX
                        );

                        pParams->setNamedAutoConstant(
                            "mWorld",
                            Ogre::GpuProgramParameters::ACT_WORLD_MATRIX
                        );

                        pParams->setNamedAutoConstant(
                            "mLightPos",
                            Ogre::GpuProgramParameters::ACT_LIGHT_POSITION_ARRAY,
                            5
                        );
                        pParams->setNamedAutoConstant(
                            "mLightDiffuseColor",
                            Ogre::GpuProgramParameters::ACT_DERIVED_LIGHT_DIFFUSE_COLOUR_ARRAY,
                            5
                        );
                        pParams->setNamedAutoConstant(
                            "mLightAtten",
                            Ogre::GpuProgramParameters::ACT_LIGHT_ATTENUATION_ARRAY,
                            5
                        );
                        pParams->setNamedAutoConstant(
                            "mAmbient",
                            Ogre::GpuProgramParameters::ACT_DERIVED_SCENE_COLOUR
                        );
                        pParams->setNamedAutoConstant(
                            "mCamPos",
                            Ogre::GpuProgramParameters::ACT_CAMERA_POSITION
                        );
                        if (pTillingBlock)
                        {
                            pParams->setNamedAutoConstant(
                                "mTexCoordMat",
                                Ogre::GpuProgramParameters::ACT_TEXTURE_MATRIX,
                                0
                            );
                        }

                        if (Engine::GetSingleton()->GetRenderer() == "OpenGL")
                        {
                            pParams = pPass->getFragmentProgramParameters();
                            pParams->setNamedConstant("mTexture",  0);
                            pParams->setNamedConstant("mTextureS", 1);
                        }
                    }

                    pChunk->SetMaterial(pMat);
                }
            }

            ++uiChunkNbr;
        }
    }
}
