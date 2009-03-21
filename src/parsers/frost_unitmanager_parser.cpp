/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           UnitManager source           */
/*                                        */
/*                                        */

#include "frost_unitmanager.h"

#include "frost_character.h"
#include "frost_creature.h"
#include "frost_healthtype.h"
#include "frost_powertype.h"
#include "frost_modelmanager.h"
#include "frost_xml_document.h"

using namespace std;

namespace Frost
{
    s_bool UnitManager::ParseCharacterModels_()
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

    s_bool UnitManager::ParseRaces_()
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
                        "A Race with the name \""+mRace.sName+"\" already exists. Previous one erased."
                    );
                }

                lRaceList_[mRace.sName] = mRace;
            }

            return true;
        }
        else
            return false;
    }

    s_bool UnitManager::ParseClasses_()
    {
        XML::Document mDoc("DB/Classes.xml", "DB/Classes.def");
        if (mDoc.Check())
        {
            s_ptr<XML::Block> pClass;
            foreach_named_block (pClass, "Class", mDoc.GetMainBlock())
            {
                Class mClass;
                mClass.sName = pClass->GetAttribute("name");
                mClass.sHealthType = pClass->GetAttribute("healthType");
                mClass.sPowerType = pClass->GetAttribute("powerType");

                if (MAPFIND(mClass.sName, lClassList_))
                {
                    Warning(CLASS_NAME,
                        "A Class with the name \""+mClass.sName+"\" already exists. Previous one erased."
                    );
                }

                lClassList_[mClass.sName] = mClass;
            }

            return true;
        }
        else
            return false;
    }

    s_bool UnitManager::ParseHealthTypes_()
    {
        XML::Document mDoc("DB/HealthTypes.xml", "DB/HealthTypes.def");
        if (mDoc.Check())
        {
            s_ptr<XML::Block> pHealthType;
            foreach_named_block (pHealthType, "HealthType", mDoc.GetMainBlock())
            {
                HealthType mHealthType;
                mHealthType.sName = pHealthType->GetAttribute("name");

                const s_str& sRegenType = pHealthType->GetAttribute("regenType");
                if (sRegenType == "NONE")
                {
                    mHealthType.mRegenType = HEALTH_REGEN_NONE;
                }
                else if (sRegenType == "TICK")
                {
                    mHealthType.mRegenType = HEALTH_REGEN_TICK;
                    mHealthType.fRegenTickDuration = s_float(pHealthType->GetAttribute("tickDuration"));
                }
                else if (sRegenType == "CONTINUOUS")
                {
                    mHealthType.mRegenType = HEALTH_REGEN_CONTINUOUS;
                    mHealthType.fRegenRate = s_float(pHealthType->GetAttribute("rate"));
                }
                else
                {
                    Error(CLASS_NAME,
                        "Unknown regeneration type : \""+sRegenType+"\". Assuming \"NONE\"."
                    );
                    mHealthType.mRegenType = HEALTH_REGEN_NONE;
                }

                // Create the Lua object
                pLua_->PushString(mHealthType.sName);
                pLua_->SetGlobal(mHealthType.sName);

                s_ptr<XML::Block> pScripts = pHealthType->GetBlock("Scripts");
                if (pScripts)
                {
                    s_ptr<XML::Block> pScript;
                    foreach_block (pScript, pScripts)
                    {
                        // Tell this HealthType it can use the script
                        s_str sTruncatedScriptName = pScript->GetName();
                        sTruncatedScriptName.EraseFromStart(2);
                        mHealthType.lDefinedScriptList.push_back(sTruncatedScriptName);

                        // Register it in Lua
                        s_str sStr;
                        sStr += "function " + mHealthType.sName + ":" + pScript->GetName() + "()\n";
                        sStr += pScript->GetValue() + "\n";
                        sStr += "end";
                        pLua_->DoString(sStr);
                    }
                }

                if (MAPFIND(mHealthType.sName, lHealthTypeList_))
                {
                    Warning(CLASS_NAME,
                        "A HealthType with the name \""+mHealthType.sName+"\" already exists. Previous one erased."
                    );
                }

                lHealthTypeList_[mHealthType.sName] = mHealthType;
            }

            return true;
        }
        else
            return false;
    }

    s_bool UnitManager::ParsePowerTypes_()
    {
        XML::Document mDoc("DB/PowerTypes.xml", "DB/PowerTypes.def");
        if (mDoc.Check())
        {
            s_ptr<XML::Block> pPowerType;
            foreach_named_block (pPowerType, "PowerType", mDoc.GetMainBlock())
            {
                PowerType mPowerType;
                mPowerType.sName = pPowerType->GetAttribute("name");

                const s_str& sRegenType = pPowerType->GetAttribute("regenType");
                if (sRegenType == "NONE")
                {
                    mPowerType.mRegenType = POWER_REGEN_NONE;
                }
                else if (sRegenType == "TICK")
                {
                    mPowerType.mRegenType = POWER_REGEN_TICK;
                    mPowerType.fRegenTickDuration = s_float(pPowerType->GetAttribute("tickDuration"));
                }
                else if (sRegenType == "CONTINUOUS")
                {
                    mPowerType.mRegenType = POWER_REGEN_CONTINUOUS;
                    mPowerType.fRegenRate = s_float(pPowerType->GetAttribute("rate"));
                }
                else
                {
                    Error(CLASS_NAME,
                        "Unknown regeneration type : \""+sRegenType+"\". Assuming \"NONE\"."
                    );
                    mPowerType.mRegenType = POWER_REGEN_NONE;
                }

                // Create the Lua object
                pLua_->PushString(mPowerType.sName);
                pLua_->SetGlobal(mPowerType.sName);

                s_ptr<XML::Block> pScripts = pPowerType->GetBlock("Scripts");
                if (pScripts)
                {
                    s_ptr<XML::Block> pScript;
                    foreach_block (pScript, pScripts)
                    {
                        // Tell this PowerType it can use the script
                        s_str sTruncatedScriptName = pScript->GetName();
                        sTruncatedScriptName.EraseFromStart(2);
                        mPowerType.lDefinedScriptList.push_back(sTruncatedScriptName);

                        // Register it in Lua
                        s_str sStr;
                        sStr += "function " + mPowerType.sName + ":" + pScript->GetName() + "()\n";
                        sStr += pScript->GetValue() + "\n";
                        sStr += "end";
                        pLua_->DoString(sStr);
                    }
                }

                if (MAPFIND(mPowerType.sName, lPowerTypeList_))
                {
                    Warning(CLASS_NAME,
                        "A PowerType with the name \""+mPowerType.sName+"\" already exists. Previous one erased."
                    );
                }

                lPowerTypeList_[mPowerType.sName] = mPowerType;
            }

            return true;
        }
        else
            return false;
    }

    s_bool UnitManager::ParseCreatureModels_()
    {


        return true;
    }

    s_bool UnitManager::ParseSpellSchools_()
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
