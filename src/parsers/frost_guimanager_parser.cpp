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

        if (!pFrame->IsVirtual())
            pFrame->CreateGlue();

        s_ptr<GUI::Frame> pParent = s_ptr<GUI::Frame>::DynamicCast(pFrame->GetParent());
        if (pParent)
            pParent->AddChild(pFrame);

        s_str sInheritance = pMainBlock->GetAttribute("inherits");
        if (!sInheritance.IsEmpty())
        {
            s_ptr<GUI::UIObject> pObj = this->GetUIObjectByName(sInheritance, true);
            if (pObj)
            {
                if (pFrame->IsObjectType(pObj->GetObjectType()))
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
        if (pMainBlock->GetAttribute("setAllPoints") == "true")
            pFrame->SetAllPoints(pParent);
        pFrame->SetAlpha(s_float(pMainBlock->GetAttribute("alpha")));
        pFrame->SetTopLevel(s_bool(pMainBlock->GetAttribute("toplevel")));
        pFrame->SetMovable(s_bool(pMainBlock->GetAttribute("movable")));
        pFrame->SetResizable(s_bool(pMainBlock->GetAttribute("resizable")));
        pFrame->SetFrameStrata(pMainBlock->GetAttribute("frameStrata"));
        pFrame->EnableMouse(s_bool(pMainBlock->GetAttribute("enableMouse")));
        pFrame->EnableKeyboard(s_bool(pMainBlock->GetAttribute("enableKeyboard")));
        pFrame->SetClampedToScreen(s_bool(pMainBlock->GetAttribute("clampedToScreen")));

        return true;
    }

    s_bool GUIManager::ParseTextureAttributes_( s_ptr<GUI::Texture> pTexture, s_ptr<XML::Block> pMainBlock )
    {
        // TODO : Implementer l'héritage pour Texture
        s_str sName = pMainBlock->GetAttribute("name");
        if (!sName.IsEmpty(true))
        {
            pTexture->SetName(sName);
        }
        else
        {
            Error(CLASS_NAME,
                "Can't create an UIObject with a blank name. Skipped."
            );
            return false;
        }

        if (!this->AddUIObject(pTexture))
            return false;

        s_ptr<GUI::Frame> pParent = s_ptr<GUI::Frame>::DynamicCast(pTexture->GetParent());

        if (pParent && !pParent->IsVirtual())
            pTexture->CreateGlue();

        if (pParent)
            pParent->AddRegion(pTexture);

        if (pMainBlock->GetAttribute("hidden") == "true")
            pTexture->Hide();
        if (pMainBlock->GetAttribute("setAllPoints") == "true")
            pTexture->SetAllPoints(pParent);

        s_str sFile = pMainBlock->GetAttribute("file");
        if (!sFile.IsEmpty())
            pTexture->SetTexture(sFile);

        //pTexture->SetBlendMode(pMainBlock->GetAttribute("alphaMode"));

        return true;
    }

    s_bool GUIManager::ParseFontStringAttributes_( s_ptr<GUI::FontString> pFontString, s_ptr<XML::Block> pMainBlock )
    {
        // TODO : Implementer l'héritage pour FontString
        s_str sName = pMainBlock->GetAttribute("name");
        if (!sName.IsEmpty(true))
        {
            pFontString->SetName(sName);
        }
        else
        {
            Error(CLASS_NAME,
                "Can't create an UIObject with a blank name. Skipped."
            );
            return false;
        }

        if (!this->AddUIObject(pFontString))
            return false;

        s_ptr<GUI::Frame> pParent = s_ptr<GUI::Frame>::DynamicCast(pFontString->GetParent());

        if (pParent && !pParent->IsVirtual())
            pFontString->CreateGlue();

        if (pParent)
            pParent->AddRegion(pFontString);

        if (pMainBlock->GetAttribute("hidden") == "true")
            pFontString->Hide();
        if (pMainBlock->GetAttribute("setAllPoints") == "true")
            pFontString->SetAllPoints(pParent);

        pFontString->SetFont(
            pMainBlock->GetAttribute("font"),
            s_uint(pMainBlock->GetAttribute("fontHeight"))
        );

        pFontString->SetText(pMainBlock->GetAttribute("text"));
        pFontString->SetNonSpaceWrap(s_bool(pMainBlock->GetAttribute("nonspacewrap")));
        pFontString->SetSpacing(s_float(pMainBlock->GetAttribute("spacing")));

        const s_str& sOutline = pMainBlock->GetAttribute("outline");
        if ( (sOutline == "NORMAL") || (sOutline == "THICK") )
            pFontString->SetOutlined(true);
        else if (sOutline == "NONE")
            pFontString->SetOutlined(false);
        else
        {
            Warning(CLASS_NAME,
                "Unkown outline type for "+pFontString->GetName()+" : \""+sOutline+"\"."
            );
        }

        const s_str& sJustifyH = pMainBlock->GetAttribute("justifyH");
        if (sJustifyH == "LEFT")
            pFontString->SetJustifyH(Text::ALIGN_LEFT);
        else if (sJustifyH == "CENTER")
            pFontString->SetJustifyH(Text::ALIGN_CENTER);
        else if (sJustifyH == "RIGHT")
            pFontString->SetJustifyH(Text::ALIGN_RIGHT);
        else
        {
            Warning(CLASS_NAME,
                "Unkown horizontal justify behavior for "+pFontString->GetName()+
                " : \""+sJustifyH+"\"."
            );
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
                    Warning(CLASS_NAME,
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
                    Warning(CLASS_NAME,
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
                    Warning(CLASS_NAME,
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
                        this->ParseFontStringBlock_(pFrame, sLevel, pRegionBlock);
                    }
                    else if (pRegionBlock->GetName() == "Texture")
                    {
                        this->ParseTextureBlock_(pFrame, sLevel, pRegionBlock);
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
                // Create the function in Lua
                s_str sStr;
                sStr += "function " + pFrame->GetName() + ":" + pScriptBlock->GetName() + "()\n";
                sStr += pScriptBlock->GetValue() + "\n";
                sStr += "end";
                pLua_->DoString(sStr);

                // Tell the Frame it can use it
                pFrame->NotifyScriptDefined(pScriptBlock->GetName());
            }
        }
        return true;
    }

    s_bool GUIManager::ParseTexCoordsBlock_( s_ptr<GUI::Texture> pTexture, s_ptr<XML::Block> pTextureBlock )
    {
        // TODO : parse TexCoords
        return true;
    }

    s_bool GUIManager::ParseTextureColorBlock_( s_ptr<GUI::Texture> pTexture, s_ptr<XML::Block> pTextureBlock )
    {
        s_ptr<XML::Block> pColorBlock = pTextureBlock->GetBlock("Color");
        if (pColorBlock)
        {
            pTexture->SetColor(Color(
                s_uint(s_float(pColorBlock->GetAttribute("a"))*255),
                s_uint(s_float(pColorBlock->GetAttribute("r"))*255),
                s_uint(s_float(pColorBlock->GetAttribute("g"))*255),
                s_uint(s_float(pColorBlock->GetAttribute("b"))*255)
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
                Error(CLASS_NAME,
                    "Unknown gradient orientation for "+pTexture->GetName()+" : \""+sOrientation+"\"."
                );
                return false;
            }

            Color mMinColor, mMaxColor;
            s_ptr<XML::Block> pMinColorBlock = pGradientBlock->GetBlock("MinColor");
            if (pMinColorBlock)
            {
                mMinColor.SetA(s_uint(255*s_float(pMinColorBlock->GetAttribute("a"))));
                mMinColor.SetR(s_uint(255*s_float(pMinColorBlock->GetAttribute("r"))));
                mMinColor.SetG(s_uint(255*s_float(pMinColorBlock->GetAttribute("g"))));
                mMinColor.SetB(s_uint(255*s_float(pMinColorBlock->GetAttribute("b"))));
            }
            else
            {
                Error(CLASS_NAME,
                    "Missins \"MinColor\" block in "+pTexture->GetName()+"'s gradient."
                );
                return false;
            }

            s_ptr<XML::Block> pMaxColorBlock = pGradientBlock->GetBlock("MaxColor");
            if (pMaxColorBlock)
            {
                mMaxColor.SetA(s_uint(255*s_float(pMaxColorBlock->GetAttribute("a"))));
                mMaxColor.SetR(s_uint(255*s_float(pMaxColorBlock->GetAttribute("r"))));
                mMaxColor.SetG(s_uint(255*s_float(pMaxColorBlock->GetAttribute("g"))));
                mMaxColor.SetB(s_uint(255*s_float(pMaxColorBlock->GetAttribute("b"))));
            }
            else
            {
                Error(CLASS_NAME,
                    "Missins \"MaxColor\" block in "+pTexture->GetName()+"'s gradient."
                );
                return false;
            }

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
                s_uint(s_float(pColorBlock->GetAttribute("a"))*255),
                s_uint(s_float(pColorBlock->GetAttribute("r"))*255),
                s_uint(s_float(pColorBlock->GetAttribute("g"))*255),
                s_uint(s_float(pColorBlock->GetAttribute("b"))*255)
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
                    s_uint(s_float(pColorBlock->GetAttribute("a"))*255),
                    s_uint(s_float(pColorBlock->GetAttribute("r"))*255),
                    s_uint(s_float(pColorBlock->GetAttribute("g"))*255),
                    s_uint(s_float(pColorBlock->GetAttribute("b"))*255)
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
        s_ptr<GUI::FontString> pFontString = new GUI::FontString();
        pFontString->SetDrawLayer(sLevel);

        if (pParent)
            pFontString->SetParent(pParent);

        // Parse attributes
        if (!this->ParseFontStringAttributes_(pFontString, pArtBlock))
        {
            pFontString.Delete(); return false;
        }

        // Parse Size
        if (!this->ParseSizeBlock_(pFontString, pArtBlock))
        {
            pFontString.Delete(); return false;
        }

        // Parse Anchors
        if (!this->ParseAnchorsBlock_(pFontString, pArtBlock))
        {
            pFontString.Delete(); return false;
        }

        // Parse Color
        if (!this->ParseFontStringColorBlock_(pFontString, pArtBlock))
        {
            pFontString.Delete(); return false;
        }

        // Parse Shadow
        if (!this->ParseShadowBlock_(pFontString, pArtBlock))
        {
            pFontString.Delete(); return false;
        }

        return true;
    }

    s_bool GUIManager::ParseTextureBlock_( s_ptr<GUI::Frame> pParent, const s_str& sLevel, s_ptr<XML::Block> pArtBlock )
    {
        s_ptr<GUI::Texture> pTexture = new GUI::Texture();
        pTexture->SetDrawLayer(sLevel);

        if (pParent)
            pTexture->SetParent(pParent);

        // Parse attributes
        if (!this->ParseTextureAttributes_(pTexture, pArtBlock))
        {
            pTexture.Delete(); return false;
        }

        // Parse Size
        if (!this->ParseSizeBlock_(pTexture, pArtBlock))
        {
            pTexture.Delete(); return false;
        }

        // Parse Anchors
        if (!this->ParseAnchorsBlock_(pTexture, pArtBlock))
        {
            pTexture.Delete(); return false;
        }

        // Parse TexCoords
        if (!this->ParseTexCoordsBlock_(pTexture, pArtBlock))
        {
            pTexture.Delete(); return false;
        }

        // Parse Color
        if (!this->ParseTextureColorBlock_(pTexture, pArtBlock))
        {
            pTexture.Delete(); return false;
        }

        // Parse Gradient
        if (!this->ParseGradientBlock_(pTexture, pArtBlock))
        {
            pTexture.Delete(); return false;
        }

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
