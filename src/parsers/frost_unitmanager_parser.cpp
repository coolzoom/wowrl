/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           UnitManager source           */
/*                                        */
/*                                        */

#include "frost_unitmanager.h"

#include "frost_character.h"
#include "frost_creature.h"
#include "frost_modelmanager.h"
#include "frost_xml_document.h"

using namespace std;

namespace Frost
{
    s_bool UnitManager::ParseCharacterModels()
    {
        XML::Document mDoc("DB/CharacterModels.xml", "DB/CharacterModels.def");
        if (mDoc.Check())
        {
            s_ptr<XML::Block> pCharacterModel;
            foreach_named_block (pCharacterModel, "CharacterModel", mDoc.GetMainBlock())
            {
                s_str sModelName = pCharacterModel->GetAttribute("name");
                s_str sFileName = pCharacterModel->GetAttribute("file");
                ModelManager::GetSingleton()->LinkModelNameToFile(sModelName, sFileName);
            }
            return true;
        }
        else
            return false;
    }

    s_bool UnitManager::ParseRaces()
    {
        XML::Document mDoc("DB/Races.xml", "DB/Races.def");
        if (mDoc.Check())
        {
            s_ptr<XML::Block> pRace;
            foreach_named_block (pRace, "Race", mDoc.GetMainBlock())
            {
                Race mRace;
                mRace.sName = pRace->GetAttribute("name");

                // Male model
                s_ptr<XML::Block> pMaleModel = pRace->GetBlock("MaleModel");
                mRace.mMaleModelInfo.sModel = pMaleModel->GetAttribute("model");

                s_ptr<XML::Block> pBodyTextures = pMaleModel->GetBlock("BodyTextures");
                s_ptr<XML::Block> pTexture;
                foreach_named_block (pTexture, "Texture", pBodyTextures)
                {
                    mRace.mMaleModelInfo.lBodyTextureList.push_back(pTexture->GetAttribute("file"));
                }

                s_ptr<XML::Block> pHairTextures = pMaleModel->GetBlock("HairTextures");
                foreach_named_block (pTexture, "Texture", pHairTextures)
                {
                    mRace.mMaleModelInfo.lHairTextureList.push_back(pTexture->GetAttribute("file"));
                }

                s_ptr<XML::Block> pDefaultParts = pMaleModel->GetBlock("DefaultParts");
                s_ptr<XML::Block> pPart;
                if (pDefaultParts)
                {
                    foreach_named_block (pPart, "Part", pDefaultParts)
                    {
                        mRace.mMaleModelInfo.lDefaultPartList.push_back(s_uint(pPart->GetAttribute("val")));
                    }
                }

                // Female model
                s_ptr<XML::Block> pFemaleModel = pRace->GetBlock("FemaleModel");
                mRace.mFemaleModelInfo.sModel = pFemaleModel->GetAttribute("model");

                pBodyTextures = pFemaleModel->GetBlock("BodyTextures");
                foreach_named_block (pTexture, "Texture", pBodyTextures)
                {
                    mRace.mFemaleModelInfo.lBodyTextureList.push_back(pTexture->GetAttribute("file"));
                }

                pHairTextures = pFemaleModel->GetBlock("HairTextures");
                foreach_named_block (pTexture, "Texture", pHairTextures)
                {
                    mRace.mFemaleModelInfo.lHairTextureList.push_back(pTexture->GetAttribute("file"));
                }

                pDefaultParts = pFemaleModel->GetBlock("DefaultParts");
                if (pDefaultParts)
                {
                    foreach_named_block (pPart, "Part", pDefaultParts)
                    {
                        mRace.mFemaleModelInfo.lDefaultPartList.push_back(s_uint(pPart->GetAttribute("val")));
                    }
                }

                if (MAPFIND(mRace.sName, lRaceList_))
                {
                    Warning(CLASS_NAME,
                        "A race with the name \""+mRace.sName+"\" already exists. Previous one erased."
                    );
                }

                lRaceList_[mRace.sName] = mRace;
            }

            return true;
        }
        else
            return false;
    }

    s_bool UnitManager::ParseCreatureModels()
    {


        return true;
    }

    s_bool UnitManager::ParseSpellSchools()
    {
        XML::Document mDoc("DB/SpellSchools.xml", "DB/SpellSchools.def");
        if (mDoc.Check())
        {
            s_ptr<XML::Block> pSchool;
            foreach_named_block (pSchool, "School", mDoc.GetMainBlock())
            {
                s_str sSchool = pSchool->GetAttribute("name");
                sSchool.ToUpper();
                if (!VECTORFIND(sSchool, lSchoolList_))
                {
                    lSchoolList_.push_back(s_str::ToUpper(pSchool->GetAttribute("name")));
                }
                else
                {
                    Warning(CLASS_NAME,
                        "Spell school \""+sSchool+"\" has already been defined. Skipped."
                    );
                }
            }

            return true;
        }
        else
            return false;
    }
}



