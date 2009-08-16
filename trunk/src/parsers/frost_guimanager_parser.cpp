/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           UnitManager source           */
/*                                        */
/*                                        */

#include "gui/frost_guimanager.h"

#include "gui/frost_anchor.h"
#include "gui/frost_uiobject.h"
#include "gui/frost_frame.h"
#include "gui/frost_backdrop.h"
#include "gui/frost_button.h"
#include "gui/frost_statusbar.h"
#include "gui/frost_slider.h"
#include "gui/frost_scrollingmessageframe.h"
#include "gui/frost_editbox.h"
#include "gui/frost_texture.h"
#include "gui/frost_fontstring.h"
#include "gui/frost_spritemanager.h"

#include "xml/frost_xml_document.h"

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
                        Warning(pMainBlock->GetFile()+":"+pMainBlock->GetLineNbr(),
                            "Can't find \""+pFrame->GetName()+"\"'s parent : \""+sParent+"\". "
                            "No parent given to that widget."
                        );
                    }
                }
                else
                {
                    Error(pMainBlock->GetFile()+":"+pMainBlock->GetLineNbr(),
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
                Warning(pMainBlock->GetFile()+":"+pMainBlock->GetLineNbr(),
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

        if (!pFrame->IsVirtual())
            pFrame->CreateGlue();

        s_ptr<GUI::Frame> pParent = s_ptr<GUI::Frame>::DynamicCast(pFrame->GetParent());
        if (pParent)
        {
            pParent->AddChild(pFrame);
            pFrame->SetLevel(pParent->GetFrameLevel() + 1);
        }

        s_str sInheritance = pMainBlock->GetAttribute("inherits");
        if (!sInheritance.IsEmpty())
        {
            s_ctnr<s_str> lObjects = sInheritance.Cut(",");
            s_ctnr<s_str>::iterator iter;
            foreach (iter, lObjects)
            {
                iter->Trim(' ');
                s_ptr<GUI::UIObject> pObj = this->GetUIObjectByName(*iter, true);
                if (pObj)
                {
                    if (pFrame->IsObjectType(pObj->GetObjectType()))
                    {
                        // Inherit from the other Frame
                        pFrame->CopyFrom(pObj);
                    }
                    else
                    {
                        Error(pMainBlock->GetFile()+":"+pMainBlock->GetLineNbr(),
                            "\""+pFrame->GetName()+"\" ("+pFrame->GetObjectType()+") cannot inherit "
                            "from \""+sInheritance+"\" ("+pObj->GetObjectType()+"). Skipped."
                        );
                    }
                }
                else
                {
                    Error(pMainBlock->GetFile()+":"+pMainBlock->GetLineNbr(),
                        "Couldn't find inherited object \""+*iter+"\". Skipped."
                    );
                }
            }
        }

        if ((pMainBlock->IsProvided("hidden") || sInheritance.IsEmpty()) &&
            (pMainBlock->GetAttribute("hidden") == "true"))
            pFrame->Hide();

        if ((pMainBlock->IsProvided("setAllPoints") || sInheritance.IsEmpty()) &&
            (pMainBlock->GetAttribute("setAllPoints") == "true"))
            pFrame->SetAllPoints(pParent);

        if (pMainBlock->IsProvided("alpha") || sInheritance.IsEmpty())
            pFrame->SetAlpha(s_float(pMainBlock->GetAttribute("alpha")));
        if (pMainBlock->IsProvided("toplevel") || sInheritance.IsEmpty())
            pFrame->SetTopLevel(s_bool(pMainBlock->GetAttribute("toplevel")));
        if (pMainBlock->IsProvided("movable") || sInheritance.IsEmpty())
            pFrame->SetMovable(s_bool(pMainBlock->GetAttribute("movable")));
        if (pMainBlock->IsProvided("resizable") || sInheritance.IsEmpty())
            pFrame->SetResizable(s_bool(pMainBlock->GetAttribute("resizable")));
        if (pMainBlock->IsProvided("frameStrata") || sInheritance.IsEmpty())
            pFrame->SetFrameStrata(pMainBlock->GetAttribute("frameStrata"));
        if (pMainBlock->IsProvided("enableMouse") || sInheritance.IsEmpty())
            pFrame->EnableMouse(s_bool(pMainBlock->GetAttribute("enableMouse")));
        if (pMainBlock->IsProvided("enableKeyboard") || sInheritance.IsEmpty())
            pFrame->EnableKeyboard(s_bool(pMainBlock->GetAttribute("enableKeyboard")));
        if (pMainBlock->IsProvided("clampedToScreen") || sInheritance.IsEmpty())
            pFrame->SetClampedToScreen(s_bool(pMainBlock->GetAttribute("clampedToScreen")));

        return true;
    }

    s_bool GUIManager::ParseTextureAttributes_( s_ptr<GUI::Texture> pTexture, s_ptr<XML::Block> pMainBlock )
    {
        s_str sName = pMainBlock->GetAttribute("name");
        s_bool bVirtual = s_bool(pMainBlock->GetAttribute("virtual"));
        s_ptr<GUI::Frame> pParent = s_ptr<GUI::Frame>::DynamicCast(pTexture->GetParent());
        if (!sName.IsEmpty(true))
        {
            if ( bVirtual || (pParent && pParent->IsVirtual()) )
                pTexture->SetVirtual();

            pTexture->SetName(sName);
        }
        else
        {
            Error(pMainBlock->GetFile()+":"+pMainBlock->GetLineNbr(),
                "Can't create an UIObject with a blank name. Skipped."
            );
            return false;
        }

        if (!this->AddUIObject(pTexture))
            return false;

        if (!pTexture->IsVirtual())
            pTexture->CreateGlue();

        if (pParent)
            pParent->AddRegion(pTexture);

        s_str sInheritance = pMainBlock->GetAttribute("inherits");
        if (!sInheritance.IsEmpty())
        {
            s_ctnr<s_str> lObjects = sInheritance.Cut(",");
            s_ctnr<s_str>::iterator iter;
            foreach (iter, lObjects)
            {
                iter->Trim(' ');
                s_ptr<GUI::UIObject> pObj = this->GetUIObjectByName(*iter, true);
                if (pObj)
                {
                    if (pTexture->IsObjectType(pObj->GetObjectType()))
                    {
                        // Inherit from the other Texture
                        pTexture->CopyFrom(pObj);
                    }
                    else
                    {
                        Error(pMainBlock->GetFile()+":"+pMainBlock->GetLineNbr(),
                            "\""+pTexture->GetName()+"\" ("+pTexture->GetObjectType()+") cannot inherit "
                            "from \""+sInheritance+"\" ("+pObj->GetObjectType()+"). Skipped."
                        );
                    }
                }
                else
                {
                    Error(pMainBlock->GetFile()+":"+pMainBlock->GetLineNbr(),
                        "Couldn't find inherited object \""+*iter+"\". Skipped."
                    );
                }
            }
        }

        if ((pMainBlock->IsProvided("hidden") || sInheritance.IsEmpty()) &&
            (pMainBlock->GetAttribute("hidden") == "true"))
            pTexture->Hide();

        if ((pMainBlock->IsProvided("setAllPoints") || sInheritance.IsEmpty()) &&
            (pMainBlock->GetAttribute("setAllPoints") == "true"))
            pTexture->SetAllPoints(pParent);

        s_str sFile = pMainBlock->GetAttribute("file");
        if (!sFile.IsEmpty())
            pTexture->SetTexture(sFile);

        //pTexture->SetBlendMode(pMainBlock->GetAttribute("alphaMode"));

        return true;
    }

    s_bool GUIManager::ParseFontStringAttributes_( s_ptr<GUI::FontString> pFontString, s_ptr<XML::Block> pMainBlock )
    {
        s_str sName = pMainBlock->GetAttribute("name");
        s_bool bVirtual = s_bool(pMainBlock->GetAttribute("virtual"));
        s_ptr<GUI::Frame> pParent = s_ptr<GUI::Frame>::DynamicCast(pFontString->GetParent());
        if (!sName.IsEmpty(true))
        {
            if ( bVirtual || (pParent && pParent->IsVirtual()) )
                pFontString->SetVirtual();

            pFontString->SetName(sName);
        }
        else
        {
            Error(pMainBlock->GetFile()+":"+pMainBlock->GetLineNbr(),
                "Can't create an UIObject with a blank name. Skipped."
            );
            return false;
        }

        if (!this->AddUIObject(pFontString))
            return false;

        if (!pFontString->IsVirtual())
            pFontString->CreateGlue();

        if (pParent)
            pParent->AddRegion(pFontString);

        s_str sInheritance = pMainBlock->GetAttribute("inherits");
        if (!sInheritance.IsEmpty())
        {
            s_ctnr<s_str> lObjects = sInheritance.Cut(",");
            s_ctnr<s_str>::iterator iter;
            foreach (iter, lObjects)
            {
                iter->Trim(' ');
                s_ptr<GUI::UIObject> pObj = this->GetUIObjectByName(*iter, true);
                if (pObj)
                {
                    if (pFontString->IsObjectType(pObj->GetObjectType()))
                    {
                        // Inherit from the other FontString
                        pFontString->CopyFrom(pObj);
                    }
                    else
                    {
                        Error(pMainBlock->GetFile()+":"+pMainBlock->GetLineNbr(),
                            "\""+pFontString->GetName()+"\" ("+pFontString->GetObjectType()+") cannot inherit "
                            "from \""+sInheritance+"\" ("+pObj->GetObjectType()+"). Skipped."
                        );
                    }
                }
                else
                {
                    Error(pMainBlock->GetFile()+":"+pMainBlock->GetLineNbr(),
                        "Couldn't find inherited object \""+*iter+"\". Skipped."
                    );
                }
            }
        }

        if ((pMainBlock->IsProvided("hidden") || sInheritance.IsEmpty()) &&
            (pMainBlock->GetAttribute("hidden") == "true"))
            pFontString->Hide();

        if ((pMainBlock->IsProvided("setAllPoints") || sInheritance.IsEmpty()) &&
            (pMainBlock->GetAttribute("setAllPoints") == "true"))
            pFontString->SetAllPoints(pParent);

        pFontString->SetFont(
            pMainBlock->GetAttribute("font"),
            s_uint(pMainBlock->GetAttribute("fontHeight"))
        );

        if (pMainBlock->IsProvided("text") || sInheritance.IsEmpty())
            pFontString->SetText(pMainBlock->GetAttribute("text"));
        if (pMainBlock->IsProvided("nonspacewrap") || sInheritance.IsEmpty())
            pFontString->SetNonSpaceWrap(s_bool(pMainBlock->GetAttribute("nonspacewrap")));
        if (pMainBlock->IsProvided("spacing") || sInheritance.IsEmpty())
            pFontString->SetSpacing(s_float(pMainBlock->GetAttribute("spacing")));

        if (pMainBlock->IsProvided("outline") || sInheritance.IsEmpty())
        {
            const s_str& sOutline = pMainBlock->GetAttribute("outline");
            if ( (sOutline == "NORMAL") || (sOutline == "THICK") )
                pFontString->SetOutlined(true);
            else if (sOutline == "NONE")
                pFontString->SetOutlined(false);
            else
            {
                Warning(pMainBlock->GetFile()+":"+pMainBlock->GetLineNbr(),
                    "Unkown outline type for "+pFontString->GetName()+" : \""+sOutline+"\"."
                );
            }
        }

        if (pMainBlock->IsProvided("justifyH") || sInheritance.IsEmpty())
        {
            const s_str& sJustifyH = pMainBlock->GetAttribute("justifyH");
            if (sJustifyH == "LEFT")
                pFontString->SetJustifyH(Text::ALIGN_LEFT);
            else if (sJustifyH == "CENTER")
                pFontString->SetJustifyH(Text::ALIGN_CENTER);
            else if (sJustifyH == "RIGHT")
                pFontString->SetJustifyH(Text::ALIGN_RIGHT);
            else
            {
                Warning(pMainBlock->GetFile()+":"+pMainBlock->GetLineNbr(),
                    "Unkown horizontal justify behavior for "+pFontString->GetName()+
                    " : \""+sJustifyH+"\"."
                );
            }
        }

        if (pMainBlock->IsProvided("justifyV") || sInheritance.IsEmpty())
        {
            const s_str& sJustifyV = pMainBlock->GetAttribute("justifyV");
            if (sJustifyV == "TOP")
                pFontString->SetJustifyV(Text::ALIGN_TOP);
            else if (sJustifyV == "MIDDLE")
                pFontString->SetJustifyV(Text::ALIGN_MIDDLE);
            else if (sJustifyV == "BOTTOM")
                pFontString->SetJustifyV(Text::ALIGN_BOTTOM);
            else
            {
                Warning(pMainBlock->GetFile()+":"+pMainBlock->GetLineNbr(),
                    "Unkown vertical justify behavior for "+pFontString->GetName()+
                    " : \""+sJustifyV+"\"."
                );
            }
        }

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
            s_ctnr<s_str> lFoundPoints;
            s_ptr<XML::Block> pAnchorBlock;
            foreach_block (pAnchorBlock, pAnchorsBlock)
            {
                s_str sPoint = pAnchorBlock->GetAttribute("point");
                s_str sParent = pAnchorBlock->GetAttribute("relativeTo");
                s_str sRelativePoint = pAnchorBlock->GetAttribute("relativePoint");

                if (lFoundPoints.Find(sPoint))
                {
                    Warning(pAnchorsBlock->GetFile()+":"+pAnchorsBlock->GetLineNbr(),
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

    s_bool GUIManager::ParseResizeBoundsBlock_( s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock )
    {
        s_ptr<XML::Block> pResizeBoundsBlock = pMainBlock->GetBlock("ResizeBounds");
        if (pResizeBoundsBlock)
        {
            s_ptr<XML::Block> pMinBlock = pResizeBoundsBlock->GetBlock("Min");
            if (pMinBlock)
            {
                s_ptr<XML::Block> pDimBlock = pMinBlock->GetRadioBlock();
                if (pDimBlock->GetName() == "AbsDimension")
                {
                    s_int iX = s_int(pDimBlock->GetAttribute("x"));
                    s_int iY = s_int(pDimBlock->GetAttribute("y"));

                    if (iX < 0)
                        iX.SetInfinitePlus();
                    if (iY < 0)
                        iX.SetInfinitePlus();

                    pFrame->SetMinResize(s_uint(iX), s_uint(iY));
                }
                else if (pDimBlock->GetName() == "RelDimension")
                {
                    Warning(pDimBlock->GetFile()+":"+pDimBlock->GetLineNbr(),
                        "\"RelDimension\" for ResizeBounds:Min is not yet supported. Skipped"
                    );
                }
            }

            s_ptr<XML::Block> pMaxBlock = pResizeBoundsBlock->GetBlock("Max");
            if (pMaxBlock)
            {
                s_ptr<XML::Block> pDimBlock = pMaxBlock->GetRadioBlock();
                if (pDimBlock->GetName() == "AbsDimension")
                {
                    s_int iX = s_int(pDimBlock->GetAttribute("x"));
                    s_int iY = s_int(pDimBlock->GetAttribute("y"));

                    if (iX < 0)
                        iX.SetInfinitePlus();
                    if (iY < 0)
                        iX.SetInfinitePlus();

                    pFrame->SetMaxResize(s_uint(iX), s_uint(iY));
                }
                else if (pDimBlock->GetName() == "RelDimension")
                {
                    Warning(pDimBlock->GetFile()+":"+pDimBlock->GetLineNbr(),
                        "\"RelDimension\" for ResizeBounds:Max is not yet supported. Skipped"
                    );
                }
            }
        }
        return true;
    }

    s_bool GUIManager::ParseTitleRegionBlock_( s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock )
    {
        s_ptr<XML::Block> pTitleRegionBlock = pMainBlock->GetBlock("TitleRegion");
        if (pTitleRegionBlock)
        {
            pFrame->CreateTitleRegion();
            s_ptr<GUI::LayeredRegion> pTitleRegion = pFrame->GetTitleRegion();
            if (pTitleRegion)
            {
                if (pTitleRegionBlock->GetAttribute("setAllPoints") == "true")
                    pTitleRegion->SetAllPoints(pFrame);

                ParseSizeBlock_(pTitleRegion, pTitleRegionBlock);
                ParseAnchorsBlock_(pTitleRegion, pTitleRegionBlock);
            }
        }
        return true;
    }

    s_bool GUIManager::ParseBackdropBlock_( s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock )
    {
        s_ptr<XML::Block> pBackdropBlock = pMainBlock->GetBlock("Backdrop");
        if (pBackdropBlock)
        {
            s_ptr<GUI::Backdrop> pBackdrop = new GUI::Backdrop(pFrame);

            pBackdrop->SetBackground(pBackdropBlock->GetAttribute("bgFile"));
            pBackdrop->SetEdge(pBackdropBlock->GetAttribute("edgeFile"));
            pBackdrop->SetBackgroundTilling(s_bool(pBackdropBlock->GetAttribute("tile")));

            s_ptr<XML::Block> pBGInsetsBlock = pBackdropBlock->GetBlock("BackgroundInsets");
            if (pBGInsetsBlock)
            {
                s_ptr<XML::Block> pInsetBlock = pBGInsetsBlock->GetRadioBlock();
                if (pInsetBlock->GetName() == "AbsInset")
                {
                    pBackdrop->SetBackgroundInsets(
                        s_int(pInsetBlock->GetAttribute("left")),
                        s_int(pInsetBlock->GetAttribute("right")),
                        s_int(pInsetBlock->GetAttribute("top")),
                        s_int(pInsetBlock->GetAttribute("bottom"))
                    );
                }
                else
                {
                    Warning(pInsetBlock->GetFile()+":"+pInsetBlock->GetLineNbr(),
                        "RelInset for Backdrop:BackgroundInsets is not yet supported ("+pFrame->GetName()+")."
                    );
                }
            }

            s_ptr<XML::Block> pEdgeSizeBlock = pBackdropBlock->GetBlock("EdgeSize");
            if (pEdgeSizeBlock)
            {
                s_ptr<XML::Block> pSizeBlock = pEdgeSizeBlock->GetRadioBlock();
                if (pSizeBlock->GetName() == "AbsValue")
                {
                    pBackdrop->SetEdgeSize(s_uint(pSizeBlock->GetAttribute("x")));
                }
                else
                {
                    Warning(pSizeBlock->GetFile()+":"+pSizeBlock->GetLineNbr(),
                        "RelValue for Backdrop:EdgeSize is not yet supported ("+pFrame->GetName()+")."
                    );
                }
            }

            s_ptr<XML::Block> pTileSizeBlock = pBackdropBlock->GetBlock("TileSize");
            if (pTileSizeBlock)
            {
                s_ptr<XML::Block> pTileBlock = pTileSizeBlock->GetRadioBlock();
                if (pTileBlock->GetName() == "AbsValue")
                {
                    pBackdrop->SetTileSize(s_uint(pTileBlock->GetAttribute("x")));
                }
                else
                {
                    Warning(pTileBlock->GetFile()+":"+pTileBlock->GetLineNbr(),
                        "RelValue for Backdrop:TileSize is not yet supported ("+pFrame->GetName()+")."
                    );
                }
            }

            pFrame->SetBackdrop(pBackdrop);
        }

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
                s_ptr<XML::Block> pRegionBlock;
                foreach_block (pRegionBlock, pLayerBlock)
                {
                    if (pRegionBlock->GetName() == "FontString")
                    {
                        s_ptr<GUI::FontString> pFontString = new GUI::FontString();
                        if (!this->ParseFontStringBlock_(pFontString, pFrame, sLevel, pRegionBlock))
                            pFontString.Delete();
                    }
                    else if (pRegionBlock->GetName() == "Texture")
                    {
                        s_ptr<GUI::Texture> pTexture = new GUI::Texture();
                        if (!this->ParseTextureBlock_(pTexture, pFrame, sLevel, pRegionBlock))
                            pTexture.Delete();
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
                    this->ParseFrameBlock_(pFrame, pElemBlock);
                }
                else if (pElemBlock->GetName() == "Button")
                {
                    this->ParseButtonBlock_(pFrame, pElemBlock);
                }
                else if (pElemBlock->GetName() == "EditBox")
                {
                    this->ParseEditBoxBlock_(pFrame, pElemBlock);
                }
                else if (pElemBlock->GetName() == "ScrollingMessageFrame")
                {
                    this->ParseSMFBlock_(pFrame, pElemBlock);
                }
                else if (pElemBlock->GetName() == "Slider")
                {
                    this->ParseSliderBlock_(pFrame, pElemBlock);
                }
                else if (pElemBlock->GetName() == "StatusBar")
                {
                    this->ParseStatusBarBlock_(pFrame, pElemBlock);
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
                pFrame->DefineScript(pScriptBlock->GetName(), pScriptBlock->GetValue());
            }
        }
        return true;
    }

    s_bool GUIManager::ParseTexCoordsBlock_( s_ptr<GUI::Texture> pTexture, s_ptr<XML::Block> pTextureBlock )
    {
        s_ptr<XML::Block> pTexCoordsBlock = pTextureBlock->GetBlock("TexCoords");
        if (pTexCoordsBlock)
        {
            pTexture->SetTexCoord(s_array<s_float,4>((
                s_float(pTexCoordsBlock->GetAttribute("left")),
                s_float(pTexCoordsBlock->GetAttribute("right")),
                s_float(pTexCoordsBlock->GetAttribute("top")),
                s_float(pTexCoordsBlock->GetAttribute("bottom"))
            )));
        }

        return true;
    }

    s_bool GUIManager::ParseTextureColorBlock_( s_ptr<GUI::Texture> pTexture, s_ptr<XML::Block> pTextureBlock )
    {
        s_ptr<XML::Block> pColorBlock = pTextureBlock->GetBlock("Color");
        if (pColorBlock)
        {
            pTexture->SetColor(Color(
                s_uchar(s_float(pColorBlock->GetAttribute("a"))*255.0f),
                s_uchar(s_float(pColorBlock->GetAttribute("r"))*255.0f),
                s_uchar(s_float(pColorBlock->GetAttribute("g"))*255.0f),
                s_uchar(s_float(pColorBlock->GetAttribute("b"))*255.0f)
            ));
        }
        return true;
    }

    s_bool GUIManager::ParseGradientBlock_( s_ptr<GUI::Texture> pTexture, s_ptr<XML::Block> pTextureBlock )
    {
        s_ptr<XML::Block> pGradientBlock = pTextureBlock->GetBlock("Gradient");
        if (pGradientBlock)
        {
            s_str sOrientation = pGradientBlock->GetAttribute("orientation");
            GUI::GradientOrientation mOrient;
            if (sOrientation == "HORIZONTAL")
                mOrient = GUI::ORIENTATION_HORIZONTAL;
            else if (sOrientation == "VERTICAL")
                mOrient = GUI::ORIENTATION_VERTICAL;
            else
            {
                Error(pGradientBlock->GetFile()+":"+pGradientBlock->GetLineNbr(),
                    "Unknown gradient orientation for "+pTexture->GetName()+" : \""+sOrientation+"\"."
                );
                return false;
            }

            Color mMinColor, mMaxColor;
            s_ptr<XML::Block> pMinColorBlock = pGradientBlock->GetBlock("MinColor");
            mMinColor.SetA(s_uchar(255.0f*s_float(pMinColorBlock->GetAttribute("a"))));
            mMinColor.SetR(s_uchar(255.0f*s_float(pMinColorBlock->GetAttribute("r"))));
            mMinColor.SetG(s_uchar(255.0f*s_float(pMinColorBlock->GetAttribute("g"))));
            mMinColor.SetB(s_uchar(255.0f*s_float(pMinColorBlock->GetAttribute("b"))));


            s_ptr<XML::Block> pMaxColorBlock = pGradientBlock->GetBlock("MaxColor");
            mMaxColor.SetA(s_uchar(255.0f*s_float(pMaxColorBlock->GetAttribute("a"))));
            mMaxColor.SetR(s_uchar(255.0f*s_float(pMaxColorBlock->GetAttribute("r"))));
            mMaxColor.SetG(s_uchar(255.0f*s_float(pMaxColorBlock->GetAttribute("g"))));
            mMaxColor.SetB(s_uchar(255.0f*s_float(pMaxColorBlock->GetAttribute("b"))));

            pTexture->SetGradient(GUI::Gradient(mOrient, mMinColor, mMaxColor));
        }
        return true;
    }


    s_bool GUIManager::ParseFontStringColorBlock_( s_ptr<GUI::FontString> pFontString, s_ptr<XML::Block> pFontStringBlock )
    {
        s_ptr<XML::Block> pColorBlock = pFontStringBlock->GetBlock("Color");
        if (pColorBlock)
        {
            pFontString->SetTextColor(Color(
                s_uchar(s_float(pColorBlock->GetAttribute("a"))*255.0f),
                s_uchar(s_float(pColorBlock->GetAttribute("r"))*255.0f),
                s_uchar(s_float(pColorBlock->GetAttribute("g"))*255.0f),
                s_uchar(s_float(pColorBlock->GetAttribute("b"))*255.0f)
            ));
        }
        return true;
    }


    s_bool GUIManager::ParseShadowBlock_( s_ptr<GUI::FontString> pFontString, s_ptr<XML::Block> pFontStringBlock )
    {
        s_ptr<XML::Block> pShadowBlock = pFontStringBlock->GetBlock("Shadow");
        if (pShadowBlock)
        {
            pFontString->SetShadow(true);
            s_ptr<XML::Block> pColorBlock = pShadowBlock->GetBlock("Color");
            if (pColorBlock)
            {
                pFontString->SetShadowColor(Color(
                    s_uchar(s_float(pColorBlock->GetAttribute("a"))*255.0f),
                    s_uchar(s_float(pColorBlock->GetAttribute("r"))*255.0f),
                    s_uchar(s_float(pColorBlock->GetAttribute("g"))*255.0f),
                    s_uchar(s_float(pColorBlock->GetAttribute("b"))*255.0f)
                ));
            }

            s_ptr<XML::Block> pOffsetBlock = pShadowBlock->GetBlock("Offset");
            if (pOffsetBlock)
            {
                pFontString->SetShadowOffsets(
                    s_int(pOffsetBlock->GetAttribute("x")),
                    s_int(pOffsetBlock->GetAttribute("y"))
                );
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
            pFrame.Delete(); return false;
        }

        this->ParseSizeBlock_(pFrame, pWidgetBlock);
        this->ParseResizeBoundsBlock_(pFrame, pWidgetBlock);
        this->ParseAnchorsBlock_(pFrame, pWidgetBlock);
        this->ParseTitleRegionBlock_(pFrame, pWidgetBlock);
        this->ParseBackdropBlock_(pFrame, pWidgetBlock);
        this->ParseHitRectInsetsBlock_(pFrame, pWidgetBlock);
        this->ParseLayersBlock_(pFrame, pWidgetBlock);
        this->ParseFramesBlock_(pFrame, pWidgetBlock);
        this->ParseScriptsBlock_(pFrame, pWidgetBlock);

        return true;
    }

    s_bool GUIManager::ParseButtonBlock_( s_ptr<GUI::Frame> pParent, s_ptr<XML::Block> pWidgetBlock )
    {
        s_ptr<GUI::Button> pButton = new GUI::Button();

        if (pParent)
            pButton->SetParent(pParent);

        // Parse attributes
        if (!this->ParseFrameAttributes_(pButton, pWidgetBlock))
        {
            pButton.Delete(); return false;
        }

        this->ParseSizeBlock_(pButton, pWidgetBlock);
        this->ParseResizeBoundsBlock_(pButton, pWidgetBlock);
        this->ParseAnchorsBlock_(pButton, pWidgetBlock);
        this->ParseTitleRegionBlock_(pButton, pWidgetBlock);
        this->ParseBackdropBlock_(pButton, pWidgetBlock);
        this->ParseHitRectInsetsBlock_(pButton, pWidgetBlock);
        this->ParseLayersBlock_(pButton, pWidgetBlock);
        this->ParseFramesBlock_(pButton, pWidgetBlock);
        this->ParseScriptsBlock_(pButton, pWidgetBlock);

        s_ptr<XML::Block> pNormalTextureBlock = pWidgetBlock->GetBlock("NormalTexture");
        if (pNormalTextureBlock)
        {
            pButton->CreateNormalTexture();
            s_ptr<GUI::Texture> pTexture = pButton->GetNormalTexture();
            if (pTexture)
                this->ParseTextureBlock_(pTexture, pButton, "BORDER", pNormalTextureBlock);
        }

        s_ptr<XML::Block> pPushedTextureBlock = pWidgetBlock->GetBlock("PushedTexture");
        if (pPushedTextureBlock)
        {
            pButton->CreatePushedTexture();
            s_ptr<GUI::Texture> pTexture = pButton->GetPushedTexture();
            if (pTexture)
                this->ParseTextureBlock_(pTexture, pButton, "BORDER", pPushedTextureBlock);
        }

        s_ptr<XML::Block> pDisabledTextureBlock = pWidgetBlock->GetBlock("DisabledTexture");
        if (pDisabledTextureBlock)
        {
            pButton->CreateDisabledTexture();
            s_ptr<GUI::Texture> pTexture = pButton->GetDisabledTexture();
            if (pTexture)
                this->ParseTextureBlock_(pTexture, pButton, "BORDER", pDisabledTextureBlock);
        }

        s_ptr<XML::Block> pHighlightTextureBlock = pWidgetBlock->GetBlock("HighlightTexture");
        if (pHighlightTextureBlock)
        {
            pButton->CreateHighlightTexture();
            s_ptr<GUI::Texture> pTexture = pButton->GetHighlightTexture();
            if (pTexture)
                this->ParseTextureBlock_(pTexture, pButton, "HIGHLIGHT", pHighlightTextureBlock);
        }


        s_ptr<XML::Block> pNormalTextBlock = pWidgetBlock->GetBlock("NormalText");
        if (pNormalTextBlock)
        {
            pButton->CreateNormalText();
            s_ptr<GUI::FontString> pFontString = pButton->GetNormalText();
            if (pFontString)
                this->ParseFontStringBlock_(pFontString, pButton, "ARTWORK", pNormalTextBlock);
        }

        s_ptr<XML::Block> pHighlightTextBlock = pWidgetBlock->GetBlock("HighlightText");
        if (pHighlightTextBlock)
        {
            pButton->CreateHighlightText();
            s_ptr<GUI::FontString> pFontString = pButton->GetHighlightText();
            if (pFontString)
                this->ParseFontStringBlock_(pFontString, pButton, "ARTWORK", pHighlightTextBlock);
        }

        s_ptr<XML::Block> pDisabledTextBlock = pWidgetBlock->GetBlock("DisabledText");
        if (pDisabledTextBlock)
        {
            pButton->CreateDisabledText();
            s_ptr<GUI::FontString> pFontString = pButton->GetDisabledText();
            if (pFontString)
                this->ParseFontStringBlock_(pFontString, pButton, "ARTWORK", pDisabledTextBlock);
        }

        if ((pWidgetBlock->IsProvided("text") || !pWidgetBlock->IsProvided("inherits")))
            pButton->SetText(pWidgetBlock->GetAttribute("text"));

        return true;
    }

    s_bool GUIManager::ParseEditBoxBlock_( s_ptr<GUI::Frame> pParent, s_ptr<XML::Block> pWidgetBlock )
    {
        // TODO : parse EditBox
        s_ptr<GUI::EditBox> pEditBox = new GUI::EditBox();

        if (pParent)
            pEditBox->SetParent(pParent);

        // Parse attributes
        if (!this->ParseFrameAttributes_(pEditBox, pWidgetBlock))
        {
            pEditBox.Delete(); return false;
        }

        this->ParseSizeBlock_(pEditBox, pWidgetBlock);
        this->ParseResizeBoundsBlock_(pEditBox, pWidgetBlock);
        this->ParseAnchorsBlock_(pEditBox, pWidgetBlock);
        this->ParseTitleRegionBlock_(pEditBox, pWidgetBlock);
        this->ParseBackdropBlock_(pEditBox, pWidgetBlock);
        this->ParseHitRectInsetsBlock_(pEditBox, pWidgetBlock);
        this->ParseLayersBlock_(pEditBox, pWidgetBlock);
        this->ParseFramesBlock_(pEditBox, pWidgetBlock);
        this->ParseScriptsBlock_(pEditBox, pWidgetBlock);

        return true;
    }

    s_bool GUIManager::ParseSMFBlock_( s_ptr<GUI::Frame> pParent, s_ptr<XML::Block> pWidgetBlock )
    {
        // TODO : parse SMF
        s_ptr<GUI::ScrollingMessageFrame> pSMF = new GUI::ScrollingMessageFrame();

        if (pParent)
            pSMF->SetParent(pParent);

        // Parse attributes
        if (!this->ParseFrameAttributes_(pSMF, pWidgetBlock))
        {
            pSMF.Delete(); return false;
        }

        this->ParseSizeBlock_(pSMF, pWidgetBlock);
        this->ParseResizeBoundsBlock_(pSMF, pWidgetBlock);
        this->ParseAnchorsBlock_(pSMF, pWidgetBlock);
        this->ParseTitleRegionBlock_(pSMF, pWidgetBlock);
        this->ParseBackdropBlock_(pSMF, pWidgetBlock);
        this->ParseHitRectInsetsBlock_(pSMF, pWidgetBlock);
        this->ParseLayersBlock_(pSMF, pWidgetBlock);
        this->ParseFramesBlock_(pSMF, pWidgetBlock);
        this->ParseScriptsBlock_(pSMF, pWidgetBlock);

        return true;
    }

    s_bool GUIManager::ParseSliderBlock_( s_ptr<GUI::Frame> pParent, s_ptr<XML::Block> pWidgetBlock )
    {
        // TODO : parse Slider
        s_ptr<GUI::Slider> pSlider = new GUI::Slider();

        if (pParent)
            pSlider->SetParent(pParent);

        // Parse attributes
        if (!this->ParseFrameAttributes_(pSlider, pWidgetBlock))
        {
            pSlider.Delete(); return false;
        }

        this->ParseSizeBlock_(pSlider, pWidgetBlock);
        this->ParseResizeBoundsBlock_(pSlider, pWidgetBlock);
        this->ParseAnchorsBlock_(pSlider, pWidgetBlock);
        this->ParseTitleRegionBlock_(pSlider, pWidgetBlock);
        this->ParseBackdropBlock_(pSlider, pWidgetBlock);
        this->ParseHitRectInsetsBlock_(pSlider, pWidgetBlock);
        this->ParseLayersBlock_(pSlider, pWidgetBlock);
        this->ParseFramesBlock_(pSlider, pWidgetBlock);
        this->ParseScriptsBlock_(pSlider, pWidgetBlock);

        return true;
    }

    s_bool GUIManager::ParseStatusBarBlock_( s_ptr<GUI::Frame> pParent, s_ptr<XML::Block> pWidgetBlock )
    {
        // TODO : parse StatusBar
        s_ptr<GUI::StatusBar> pStatusBar = new GUI::StatusBar();

        if (pParent)
            pStatusBar->SetParent(pParent);

        // Parse attributes
        if (!this->ParseFrameAttributes_(pStatusBar, pWidgetBlock))
        {
            pStatusBar.Delete(); return false;
        }

        this->ParseSizeBlock_(pStatusBar, pWidgetBlock);
        this->ParseResizeBoundsBlock_(pStatusBar, pWidgetBlock);
        this->ParseAnchorsBlock_(pStatusBar, pWidgetBlock);
        this->ParseTitleRegionBlock_(pStatusBar, pWidgetBlock);
        this->ParseBackdropBlock_(pStatusBar, pWidgetBlock);
        this->ParseHitRectInsetsBlock_(pStatusBar, pWidgetBlock);
        this->ParseLayersBlock_(pStatusBar, pWidgetBlock);
        this->ParseFramesBlock_(pStatusBar, pWidgetBlock);
        this->ParseScriptsBlock_(pStatusBar, pWidgetBlock);

        return true;
    }

    s_bool GUIManager::ParseFontStringBlock_( s_ptr<GUI::FontString> pFontString, s_ptr<GUI::Frame> pParent, const s_str& sLevel, s_ptr<XML::Block> pArtBlock )
    {
        pFontString->SetDrawLayer(sLevel);

        if (pParent)
            pFontString->SetParent(pParent);

        // Parse attributes
        if (!this->ParseFontStringAttributes_(pFontString, pArtBlock))
            return false;

        // Parse Size
        if (!this->ParseSizeBlock_(pFontString, pArtBlock))
            return false;

        // Parse Anchors
        if (!this->ParseAnchorsBlock_(pFontString, pArtBlock))
            return false;

        // Parse Color
        if (!this->ParseFontStringColorBlock_(pFontString, pArtBlock))
            return false;

        // Parse Shadow
        if (!this->ParseShadowBlock_(pFontString, pArtBlock))
            return false;

        return true;
    }

    s_bool GUIManager::ParseTextureBlock_( s_ptr<GUI::Texture> pTexture, s_ptr<GUI::Frame> pParent, const s_str& sLevel, s_ptr<XML::Block> pArtBlock )
    {
        pTexture->SetDrawLayer(sLevel);

        if (pParent)
            pTexture->SetParent(pParent);

        // Parse attributes
        if (!this->ParseTextureAttributes_(pTexture, pArtBlock))
            return false;

        // Parse Size
        if (!this->ParseSizeBlock_(pTexture, pArtBlock))
            return false;

        // Parse Anchors
        if (!this->ParseAnchorsBlock_(pTexture, pArtBlock))
            return false;

        // Parse TexCoords
        if (!this->ParseTexCoordsBlock_(pTexture, pArtBlock))
            return false;

        // Parse Color
        if (!this->ParseTextureColorBlock_(pTexture, pArtBlock))
            return false;

        // Parse Gradient
        if (!this->ParseGradientBlock_(pTexture, pArtBlock))
            return false;

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
                    this->ParseXMLFile_(pAddOn->sFolder + "/" + pElemBlock->GetAttribute("file"), pAddOn);
                }
                else if (pElemBlock->GetName() == "Frame")
                {
                    this->ParseFrameBlock_(NULL, pElemBlock);
                }
                else if (pElemBlock->GetName() == "Button")
                {
                    this->ParseButtonBlock_(NULL, pElemBlock);
                }
                else if (pElemBlock->GetName() == "EditBox")
                {
                    this->ParseEditBoxBlock_(NULL, pElemBlock);
                }
                else if (pElemBlock->GetName() == "ScrollingMessageFrame")
                {
                    this->ParseSMFBlock_(NULL, pElemBlock);
                }
                else if (pElemBlock->GetName() == "Slider")
                {
                    this->ParseSliderBlock_(NULL, pElemBlock);
                }
                else if (pElemBlock->GetName() == "StatusBar")
                {
                    this->ParseStatusBarBlock_(NULL, pElemBlock);
                }
            }
        }
    }
}
