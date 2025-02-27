/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          GameplayManager source        */
/*                                        */
/*                                        */

#include "gameplay/frost_gameplaymanager.h"
#include "gameplay/frost_gameplay.h"

#include <frost_xml_document.h>

using namespace std;

namespace Frost
{
    void GameplayManager::ParseXMLFile_( const s_str& sFile )
    {
        XML::Document mDoc(sFile, "Gameplays/Gameplay.def");
        if (mDoc.Check())
        {
            s_ptr<XML::Block> pElemBlock;
            foreach_block (pElemBlock, mDoc.GetMainBlock())
            {
                if (pElemBlock->GetName() == "Script")
                {
                    try
                    {
                        pLua_->DoFile("Gameplays/" + pElemBlock->GetAttribute("file"));
                    }
                    catch (LuaException& e)
                    {
                        Warning(CLASS_NAME, e.GetDescription());
                    }
                }
                else if (pElemBlock->GetName() == "Gameplay")
                {
                    this->ParseGameplayBlock_(pElemBlock);
                }
            }
        }
    }

    void GameplayManager::ParseGameplayBlock_( s_ptr<XML::Block> pGameplayBlock )
    {
        s_str sName = pGameplayBlock->GetAttribute("name");
        if (!GetGameplay(sName))
        {
            Log<2>("    Loading gameplay : "+sName);
            s_ptr<Gameplay> pGameplay = new Gameplay(sName);
            lGameplayList_[sName] = pGameplay;

            pGameplay->CreateGlue();

            s_ptr<XML::Block> pSelectionBlock = pGameplayBlock->GetBlock("Selection");
            if (pSelectionBlock)
            {
                s_str sSelection = pSelectionBlock->GetAttribute("friendly");
                Gameplay::SelectionType mType;
                if (sSelection == "NONE")
                    mType = Gameplay::SELECTION_NONE;
                else if (sSelection == "SINGLE")
                    mType = Gameplay::SELECTION_SINGLE;
                else if (sSelection == "MULTIPLE")
                    mType = Gameplay::SELECTION_MULTIPLE;
                else
                {
                    Warning(CLASS_NAME,
                        "Unknown selection type : \""+sSelection+"\". Using \"NONE\"."
                    );
                    mType = Gameplay::SELECTION_NONE;
                }
                pGameplay->SetFriendlySelection(mType);

                sSelection = pSelectionBlock->GetAttribute("hostile");
                if (sSelection == "NONE")
                    mType = Gameplay::SELECTION_NONE;
                else if (sSelection == "SINGLE")
                    mType = Gameplay::SELECTION_SINGLE;
                else if (sSelection == "MULTIPLE")
                    mType = Gameplay::SELECTION_MULTIPLE;
                else
                {
                    Warning(CLASS_NAME,
                        "Unknown selection type : \""+sSelection+"\". Using \"NONE\"."
                    );
                    mType = Gameplay::SELECTION_NONE;
                }
                pGameplay->SetHostileSelection(mType);
            }

            s_ptr<XML::Block> pScriptsBlock = pGameplayBlock->GetBlock("Scripts");
            if (pScriptsBlock)
            {
                s_ptr<XML::Block> pScriptBlock;
                foreach_block (pScriptBlock, pScriptsBlock)
                {
                    // Create the function in Lua
                    s_str sStr;
                    sStr += "function " + pGameplay->GetName() + ":" + pScriptBlock->GetName() + "()\n";
                    sStr += pScriptBlock->GetValue() + "\n";
                    sStr += "end";

                    try
                    {
                        pLua_->DoString(sStr);
                    }
                    catch (LuaException& e)
                    {
                        Error(CLASS_NAME, "Parsing gameplay \""+pGameplay->GetName()+"\", "
                            "defining "+pScriptBlock->GetName()+" handler :\n"+e.GetDescription()
                        );
                    }

                    // Tell the Frame it can use it
                    pGameplay->NotifyScriptDefined(pScriptBlock->GetName());
                }
            }

            pGameplay->On("Load");
        }
        else
        {
            Error(CLASS_NAME,
                "A Gameplay with the name \""+sName+"\" already exists. Skipping."
            );
        }
    }
}
