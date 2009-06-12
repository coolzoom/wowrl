/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           UnitManager source           */
/*                                        */
/*                                        */

#include "gameplay/frost_gameplaymanager.h"
#include "gameplay/frost_gameplay.h"
#include "xml/frost_xml_document.h"

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
                    pLua_->DoFile("Gameplays/" + pElemBlock->GetAttribute("file"));
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
            s_ptr<Gameplay> pGameplay = new Gameplay(sName);
            lGameplayList_[sName] = pGameplay;

            pGameplay->CreateGlue();

            s_ptr<XML::Block> pSelectionBlock = pGameplayBlock->GetBlock("Selection");
            if (pSelectionBlock)
            {
                pGameplay->SetAllowSingleSelection(s_bool(pSelectionBlock->GetAttribute("allowSingle")));
                pGameplay->SetAllowMultipleSelection(s_bool(pSelectionBlock->GetAttribute("allowMultiple")));
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
                    pLua_->DoString(sStr);

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
