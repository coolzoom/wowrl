/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           UnitManager source           */
/*                                        */
/*                                        */

#include "frost_guimanager.h"

#include "gui/frost_gui_anchor.h"
#include "gui/frost_gui_uiobject.h"
#include "gui/frost_gui_frame.h"

#include "frost_xml_document.h"

using namespace std;

namespace Frost
{
    s_bool GUIManager::ParseFrameAttributes_( s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock )
    {
        s_str sParent = pMainBlock->GetAttribute("parent");
        s_bool bVirtual = s_bool(pMainBlock->GetAttribute("virtual"));

        if (!sParent.IsEmpty())
        {
            if (!pFrame->GetParent())
            {
                s_ptr<GUI::UIObject> pParent = this->GetUIObjectByName(sParent);
                s_str sName = pMainBlock->GetAttribute("name");
                if (!sName.IsEmpty(true))
                {
                    if (pParent)
                    {
                        pFrame->SetParent(pParent);
                        if ( bVirtual || pParent->IsVirtual() )
                            pFrame->SetVirtual();
                        pFrame->SetName(sName);
                    }
                    else
                    {
                        if (bVirtual)
                            pFrame->SetVirtual();
                        pFrame->SetName(sName);
                        Warning(CLASS_NAME,
                            "Can't find \""+pFrame->GetName()+"\"'s parent : \""+sParent+"\". "
                            "No parent given to that widget."
                        );
                    }
                }
                else
                {
                    Error(CLASS_NAME,
                        "Can't create an UIObject with a blank name. Skipped."
                    );
                    return false;
                }
            }
            else
            {
                if ( bVirtual || pFrame->GetParent()->IsVirtual() )
                    pFrame->SetVirtual();
                pFrame->SetName(pMainBlock->GetAttribute("name"));
                Warning(CLASS_NAME,
                    "Can't use the \"parent\" attribute on \""+pFrame->GetName()+"\", "
                    "because it is a nested UIObject. Attribute ignored."
                );
            }
        }
        else
        {
            if ( bVirtual  || (pFrame->GetParent() && pFrame->GetParent()->IsVirtual()) )
                pFrame->SetVirtual();
            pFrame->SetName(pMainBlock->GetAttribute("name"));
        }

        if (!this->AddUIObject(pFrame))
            return false;

        pFrame->CreateGlue();

        s_ptr<GUI::Frame> pParent = (GUI::Frame*)pFrame->GetParent().Get();
        if (pParent)
            pParent->AddChild(pFrame);

        s_str sInheritance = pMainBlock->GetAttribute("inherits");
        if (!sInheritance.IsEmpty())
        {
            s_ptr<GUI::UIObject> pObj = this->GetUIObjectByName(sInheritance, true);
            if (pObj)
            {
                if (pObj->GetParent())
                {
                    Error(CLASS_NAME,
                        "\""+pFrame->GetName()+"\" cannot inherit "
                        "from \""+sInheritance+"\" because this virtual "
                        +pObj->GetObjectType()+" has a parent. Skipped."
                    );
                    return false;
                }
                else
                {
                    const vector<s_str>& lFrameTypes = pFrame->GetObjectTypeList();
                    if (VECTORFIND(pObj->GetObjectType(), lFrameTypes))
                    {
                        // Inherit from the other Frame
                        pFrame->CopyFrom(pObj);
                    }
                    else
                    {
                        Error(CLASS_NAME,
                            "\""+pFrame->GetName()+"\" ("+pFrame->GetObjectType()+") cannot inherit "
                            "from \""+sInheritance+"\" ("+pObj->GetObjectType()+"). Skipped."
                        );
                        return false;
                    }
                }
            }
            else
            {
                Error(CLASS_NAME,
                    "Couldn't find inherited object \""+sInheritance+"\". Skipped."
                );
                return false;
            }
        }

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
                s_int iX = s_int(pDimBlock->GetAttribute("x"));
                s_int iY = s_int(pDimBlock->GetAttribute("y"));
                if (iX >= 0)
                    pObject->SetAbsWidth(s_uint(iX));
                if (iY >= 0)
                    pObject->SetAbsHeight(s_uint(iY));
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
                    if (sRelativePoint.IsEmpty())
                        sRelativePoint = sPoint;

                    GUI::Anchor mAnchor(
                        pObject,
                        GUI::Anchor::GetAnchorPoint(sPoint),
                        NULL,
                        GUI::Anchor::GetAnchorPoint(sRelativePoint)
                    );

                    mAnchor.SetParentRawName(sParent);

                    s_ptr<XML::Block> pOffsetBlock = pAnchorBlock->GetBlock("Offset");
                    if (pOffsetBlock)
                    {
                        s_ptr<XML::Block> pDimBlock = pOffsetBlock->GetRadioBlock();
                        if (pDimBlock->GetName() == "AbsDimension")
                        {
                            mAnchor.SetAbsOffset(
                                s_int(pDimBlock->GetAttribute("x")),
                                s_int(pDimBlock->GetAttribute("y"))
                            );
                        }
                        else if (pDimBlock->GetName() == "RelDimension")
                        {
                            mAnchor.SetRelOffset(
                                s_float(pDimBlock->GetAttribute("x")),
                                s_float(pDimBlock->GetAttribute("y"))
                            );
                        }
                    }

                    pObject->SetPoint(mAnchor);
                }
            }
        }

        return true;
    }

    s_bool GUIManager::ParseTitleRegionBlock_( s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock )
    {
        // TODO : parse TitleRegion
        return true;
    }

    s_bool GUIManager::ParseBackdropBlock_( s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock )
    {
        // TODO : parse Backdrop
        return true;
    }

    s_bool GUIManager::ParseHitRectInsetsBlock_( s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock )
    {
        s_ptr<XML::Block> pHitRectBlock = pMainBlock->GetBlock("HitRectInsets");
        if (pHitRectBlock)
        {
            s_ptr<XML::Block> pInsetBlock = pHitRectBlock->GetRadioBlock();
            if (pInsetBlock->GetName() == "AbsInset")
            {
                pFrame->SetAbsHitRectInsets(
                    s_int(pInsetBlock->GetAttribute("left")),
                    s_int(pInsetBlock->GetAttribute("right")),
                    s_int(pInsetBlock->GetAttribute("top")),
                    s_int(pInsetBlock->GetAttribute("bottom"))
                );
            }
            else if (pInsetBlock->GetName() == "RelInset")
            {
                pFrame->SetRelHitRectInsets(
                    s_float(pInsetBlock->GetAttribute("left")),
                    s_float(pInsetBlock->GetAttribute("right")),
                    s_float(pInsetBlock->GetAttribute("top")),
                    s_float(pInsetBlock->GetAttribute("bottom"))
                );
            }
        }

        return true;
    }

    s_bool GUIManager::ParseLayersBlock_( s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock )
    {
        s_ptr<XML::Block> pLayersBlock = pMainBlock->GetBlock("Layers");
        if (pLayersBlock)
        {
            s_ptr<XML::Block> pLayerBlock;
            foreach_block (pLayerBlock, pLayersBlock)
            {
                s_str sLevel = pLayerBlock->GetAttribute("level");
                s_ptr<XML::Block> pArtBlock;
                foreach_block (pArtBlock, pLayerBlock)
                {
                    if (pArtBlock->GetName() == "FontString")
                    {
                        if (!this->ParseFontStringBlock_(pFrame, sLevel, pArtBlock))
                            return false;
                    }
                    else if (pArtBlock->GetName() == "Texture")
                    {
                        if (!this->ParseTextureBlock_(pFrame, sLevel, pArtBlock))
                            return false;
                    }
                }
            }
        }

        return true;
    }

    s_bool GUIManager::ParseFramesBlock_( s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock )
    {
        s_ptr<XML::Block> pFramesBlock = pMainBlock->GetBlock("Frames");
        if (pFramesBlock)
        {
            s_ptr<XML::Block> pElemBlock;
            foreach_block (pElemBlock, pFramesBlock)
            {
                if (pElemBlock->GetName() == "Frame")
                {
                    if (!this->ParseFrameBlock_(pFrame, pElemBlock))
                        return false;
                }
                else if (pElemBlock->GetName() == "Button")
                {
                    if (!this->ParseButtonBlock_(pFrame, pElemBlock))
                        return false;
                }
                else if (pElemBlock->GetName() == "EditBox")
                {
                    if (!this->ParseEditBoxBlock_(pFrame, pElemBlock))
                        return false;
                }
                else if (pElemBlock->GetName() == "ScrollingMessageFrame")
                {
                    if (!this->ParseSMFBlock_(pFrame, pElemBlock))
                        return false;
                }
                else if (pElemBlock->GetName() == "Slider")
                {
                    if (!this->ParseSliderBlock_(pFrame, pElemBlock))
                        return false;
                }
                else if (pElemBlock->GetName() == "StatusBar")
                {
                    if (!this->ParseStatusBarBlock_(pFrame, pElemBlock))
                        return false;
                }
            }
        }

        return true;
    }

    s_bool GUIManager::ParseScriptsBlock_( s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock )
    {
        s_ptr<XML::Block> pScriptsBlock = pMainBlock->GetBlock("Scripts");
        if (pScriptsBlock)
        {
            s_ptr<XML::Block> pScriptBlock;
            foreach_block (pScriptBlock, pScriptsBlock)
            {
                // Create the function in Lua
                s_str sStr;
                sStr += "function "+ pFrame->GetName() + ":" + pScriptBlock->GetName() + "()\n";
                sStr += pScriptBlock->GetValue() + "\n";
                sStr += "end";
                pLua_->DoString(sStr);

                // Tell the Frame it can use it
                pFrame->NotifyScriptDefined(pScriptBlock->GetName());
            }
        }
        return true;
    }

    s_bool GUIManager::ParseFrameBlock_( s_ptr<GUI::Frame> pParent, s_ptr<XML::Block> pWidgetBlock )
    {
        s_ptr<GUI::Frame> pFrame = new GUI::Frame();

        if (pParent)
            pFrame->SetParent(pParent);

        // Parse attributes
        if (!this->ParseFrameAttributes_(pFrame, pWidgetBlock))
        {
            delete pFrame.Get(); return false;
        }

        // Parse Size
        if (!this->ParseSizeBlock_(pFrame, pWidgetBlock))
        {
            delete pFrame.Get(); return false;
        }

        // Parse Anchors
        if (!this->ParseAnchorsBlock_(pFrame, pWidgetBlock))
        {
            delete pFrame.Get(); return false;
        }

        // Parse TitleRegion
        if (!this->ParseTitleRegionBlock_(pFrame, pWidgetBlock))
        {
            delete pFrame.Get(); return false;
        }

        // Parse Backdrop
        if (!this->ParseBackdropBlock_(pFrame, pWidgetBlock))
        {
            delete pFrame.Get(); return false;
        }

        // Parse HitRectInsets
        if (!this->ParseHitRectInsetsBlock_(pFrame, pWidgetBlock))
        {
            delete pFrame.Get(); return false;
        }

        // Parse Layers
        if (!this->ParseLayersBlock_(pFrame, pWidgetBlock))
        {
            delete pFrame.Get(); return false;
        }

        // Parse Frames
        if (!this->ParseFramesBlock_(pFrame, pWidgetBlock))
        {
            delete pFrame.Get(); return false;
        }

        // Parse Scripts
        if (!this->ParseScriptsBlock_(pFrame, pWidgetBlock))
        {
            delete pFrame.Get(); return false;
        }

        return true;
    }

    s_bool GUIManager::ParseButtonBlock_( s_ptr<GUI::Frame> pParent, s_ptr<XML::Block> pWidgetBlock )
    {
        // TODO : parse Button
        return true;
    }

    s_bool GUIManager::ParseEditBoxBlock_( s_ptr<GUI::Frame> pParent, s_ptr<XML::Block> pWidgetBlock )
    {
        // TODO : parse EditBox
        return true;
    }

    s_bool GUIManager::ParseSMFBlock_( s_ptr<GUI::Frame> pParent, s_ptr<XML::Block> pWidgetBlock )
    {
        // TODO : parse SMF
        return true;
    }

    s_bool GUIManager::ParseSliderBlock_( s_ptr<GUI::Frame> pParent, s_ptr<XML::Block> pWidgetBlock )
    {
        // TODO : parse Slider
        return true;
    }

    s_bool GUIManager::ParseStatusBarBlock_( s_ptr<GUI::Frame> pParent, s_ptr<XML::Block> pWidgetBlock )
    {
        // TODO : parse StatusBar
        return true;
    }

    s_bool GUIManager::ParseFontStringBlock_( s_ptr<GUI::Frame> pParent, const s_str& sLevel, s_ptr<XML::Block> pArtBlock )
    {
        // TODO : parse FontString
        return true;
    }

    s_bool GUIManager::ParseTextureBlock_( s_ptr<GUI::Frame> pParent, const s_str& sLevel, s_ptr<XML::Block> pArtBlock )
    {
        // TODO : parse Texture
        return true;
    }

    void GUIManager::ParseXMLFile_( const s_str& sFile, s_ptr<AddOn> pAddOn )
    {
        XML::Document mDoc(sFile, "Interface/UI.def");
        if (mDoc.Check())
        {
            s_ptr<XML::Block> pElemBlock;
            foreach_block (pElemBlock, mDoc.GetMainBlock())
            {
                if (pElemBlock->GetName() == "Script")
                {
                    pLua_->DoFile(pAddOn->sFolder + "/" + pElemBlock->GetAttribute("file"));
                }
                else if (pElemBlock->GetName() == "Include")
                {
                    // TODO : parse Include
                }
                else if (pElemBlock->GetName() == "Frame")
                {
                    if (!this->ParseFrameBlock_(NULL, pElemBlock))
                        return;
                }
                else if (pElemBlock->GetName() == "Button")
                {
                    if (!this->ParseButtonBlock_(NULL, pElemBlock))
                        return;
                }
                else if (pElemBlock->GetName() == "EditBox")
                {
                    if (!this->ParseEditBoxBlock_(NULL, pElemBlock))
                        return;
                }
                else if (pElemBlock->GetName() == "ScrollingMessageFrame")
                {
                    if (!this->ParseSMFBlock_(NULL, pElemBlock))
                        return;
                }
                else if (pElemBlock->GetName() == "Slider")
                {
                    if (!this->ParseSliderBlock_(NULL, pElemBlock))
                        return;
                }
                else if (pElemBlock->GetName() == "StatusBar")
                {
                    if (!this->ParseStatusBarBlock_(NULL, pElemBlock))
                        return;
                }
            }
        }
    }
}
