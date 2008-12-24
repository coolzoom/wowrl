/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           UnitManager source           */
/*                                        */
/*                                        */

#include "frost_guimanager.h"

#include "frost_gui.h"
#include "gui/frost_gui_uiobject.h"
#include "gui/frost_gui_frame.h"

#include "frost_xml_document.h"

using namespace std;

namespace Frost
{
    s_bool GUIManager::ParseFrameAttributes_( s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock )
    {
        s_str sParent = pMainBlock->GetAttribute("parent");
        if (!sParent.IsEmpty())
            pFrame->SetParent(this->GetUIObjectByName(sParent));

        pFrame->SetName(pMainBlock->GetAttribute("name"));
        if (!this->AddUIObject(pFrame))
            return false;

        if (pMainBlock->GetAttribute("hidden") == "true")
            pFrame->Hide();
        pFrame->SetAlpha(s_float(pMainBlock->GetAttribute("alpha")));
        pFrame->SetTopLevel(s_bool(pMainBlock->GetAttribute("toplevel")));
        pFrame->SetMovable(s_bool(pMainBlock->GetAttribute("movable")));
        pFrame->SetResizable(s_bool(pMainBlock->GetAttribute("resizable")));
        pFrame->SetFrameStrata(pMainBlock->GetAttribute("frameStrata"));
        pFrame->EnableMouse(s_bool(pMainBlock->GetAttribute("enableMouse")));
        pFrame->EnableKeyboard(s_bool(pMainBlock->GetAttribute("enableKeyboard")));

        return true;
    }

    s_bool GUIManager::ParseSizeBlock_( s_ptr<GUI::UIObject> pObject, s_ptr<XML::Block> pMainBlock )
    {
        s_ptr<XML::Block> pSizeBlock = pMainBlock->GetBlock("Size");
        if (pSizeBlock)
        {
            s_ptr<XML::Block> pDimBlock = pSizeBlock->GetRadioBlock();
            if (pDimBlock->GetName() == "AbsDimension")
            {
                pObject->SetAbsWidth(s_uint(pDimBlock->GetAttribute("x")));
                pObject->SetAbsHeight(s_uint(pDimBlock->GetAttribute("y")));
            }
            else if (pDimBlock->GetName() == "RelDimension")
            {
                pObject->SetRelWidth(s_float(pDimBlock->GetAttribute("x")));
                pObject->SetRelHeight(s_float(pDimBlock->GetAttribute("y")));
            }
        }

        return true;
    }

    s_bool GUIManager::ParseAnchorsBlock_( s_ptr<GUI::UIObject> pObject, s_ptr<XML::Block> pMainBlock )
    {
        s_ptr<XML::Block> pAnchorsBlock = pMainBlock->GetBlock("Anchors");
        if (pAnchorsBlock)
        {
            vector<s_str> lFoundPoints;
            s_ptr<XML::Block> pAnchorBlock;
            foreach_block (pAnchorBlock, pAnchorsBlock)
            {
                s_str sPoint = pAnchorBlock->GetAttribute("point");
                s_str sParent = pAnchorBlock->GetAttribute("relativeTo");
                s_str sRelativePoint = pAnchorBlock->GetAttribute("relativePoint");

                if (VECTORFIND(sPoint, lFoundPoints))
                {
                    Warning(CLASS_NAME,
                        "Anchor point \""+sPoint+"\" has already been defined "
                        "for \""+pObject->GetName()+"\". Skipped."
                    );
                }
                else
                {
                    s_ptr<GUI::UIObject> pParent;
                    if (!sParent.IsEmpty())
                    {
                        pParent = this->GetUIObjectByName(sParent);

                        if (!pParent)
                        {
                            Warning(CLASS_NAME,
                                "Can't find UIObject \""+sParent+"\" while defining anchors for \""+
                                pObject->GetName()+"\". Assuming no parent has been given."
                            );
                        }
                    }

                    s_ptr<XML::Block> pOffsetBlock = pAnchorBlock->GetBlock("Offset");
                    if (pOffsetBlock)
                    {
                        s_ptr<XML::Block> pDimBlock = pOffsetBlock->GetRadioBlock();
                        if (pDimBlock->GetName() == "AbsDimension")
                        {
                            pObject->SetAbsPoint(
                                GUI::Anchor::GetAnchorPoint(sPoint),
                                pParent,
                                GUI::Anchor::GetAnchorPoint(sRelativePoint),
                                s_int(pDimBlock->GetAttribute("x")),
                                s_int(pDimBlock->GetAttribute("y"))
                            );
                        }
                        else if (pDimBlock->GetName() == "RelDimension")
                        {
                            pObject->SetRelPoint(
                                GUI::Anchor::GetAnchorPoint(sPoint),
                                pParent,
                                GUI::Anchor::GetAnchorPoint(sRelativePoint),
                                s_float(pDimBlock->GetAttribute("x")),
                                s_float(pDimBlock->GetAttribute("y"))
                            );
                        }
                    }
                    else
                    {
                        pObject->SetAbsPoint(
                            GUI::Anchor::GetAnchorPoint(sPoint),
                            pParent,
                            GUI::Anchor::GetAnchorPoint(sRelativePoint),
                            0, 0
                        );
                    }
                }
            }
        }

        return true;
    }

    void GUIManager::ParseXMLFile( const s_str& sFile )
    {
        XML::Document mDoc(sFile, "Interface/UI.def");
        if (mDoc.Check())
        {
            s_ptr<XML::Block> pScriptBlock;
            foreach_named_block (pScriptBlock, "Script", mDoc.GetMainBlock())
            {
                Lua::DoFile(pLua_, pScriptBlock->GetAttribute("file"));
            }

            s_ptr<XML::Block> pElemBlock;
            foreach_named_block (pElemBlock, "Frame", mDoc.GetMainBlock())
            {
                s_ptr<GUI::Frame> pFrame = new GUI::Frame();

                if (!this->ParseFrameAttributes_(pFrame, pElemBlock))
                    return;

                if (!this->ParseSizeBlock_(pFrame, pElemBlock))
                    return;

                if (!this->ParseAnchorsBlock_(pFrame, pElemBlock))
                    return;
            }
        }
    }
}
