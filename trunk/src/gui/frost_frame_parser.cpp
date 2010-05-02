/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_frame.h"
#include "gui/frost_backdrop.h"
#include "gui/frost_layeredregion.h"

#include "xml/frost_xml_document.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

void Frame::ParseBlock( s_ptr<XML::Block> pBlock )
{
    ParseAttributes_(pBlock);

    ParseSizeBlock_(pBlock);
    ParseResizeBoundsBlock_(pBlock);
    ParseAnchorsBlock_(pBlock);
    ParseTitleRegionBlock_(pBlock);
    ParseBackdropBlock_(pBlock);
    ParseHitRectInsetsBlock_(pBlock);

    ParseLayersBlock_(pBlock);
    ParseFramesBlock_(pBlock);
    ParseScriptsBlock_(pBlock);
}

void Frame::ParseAttributes_( s_ptr<XML::Block> pBlock )
{
    s_str sParent = pBlock->GetAttribute("parent");
    s_bool bVirtual = s_bool(pBlock->GetAttribute("virtual"));

    if (!sParent.IsEmpty())
    {
        if (!pParent_)
        {
            s_ptr<GUI::UIObject> pParent = GUIManager::GetSingleton()->GetUIObjectByName(sParent);
            s_str sName = pBlock->GetAttribute("name");
            if (!GUIManager::GetSingleton()->CheckUIObjectName(sName))
            {
                throw GUIException(pBlock->GetFile()+":"+pBlock->GetLineNbr(),
                    "Can't create an UIObject with an incorrect name. Skipped."
                );
            }

            if (!sName.IsEmpty(true))
            {
                if (pParent)
                {
                    SetParent(pParent);
                    if ( bVirtual || pParent->IsVirtual() )
                        SetVirtual();
                    SetName(sName);
                }
                else
                {
                    if (bVirtual)
                        SetVirtual();
                    SetName(sName);
                    Warning(pBlock->GetFile()+":"+pBlock->GetLineNbr(),
                        "Can't find \""+GetName()+"\"'s parent : \""+sParent+"\". "
                        "No parent given to that widget."
                    );
                }
            }
            else
            {
                throw GUIException(pBlock->GetFile()+":"+pBlock->GetLineNbr(),
                    "Can't create an UIObject with a blank name. Skipped."
                );
            }
        }
        else
        {
            if ( bVirtual || pParent_->IsVirtual() )
                SetVirtual();

            SetName(pBlock->GetAttribute("name"));
            Warning(pBlock->GetFile()+":"+pBlock->GetLineNbr(),
                "Can't use the \"parent\" attribute on \""+GetName()+"\", "
                "because it is a nested UIObject. Attribute ignored."
            );
        }
    }
    else
    {
        if ( bVirtual  || (pParent_ && pParent_->IsVirtual()) )
            SetVirtual();
        SetName(pBlock->GetAttribute("name"));
    }

    if (GUIManager::GetSingleton()->GetUIObjectByName(sName_))
    {
        throw GUIException(pBlock->GetFile()+":"+pBlock->GetLineNbr(),
            s_str(bVirtual ? "A virtual" : "An")+" object with the name \""+
            sName_+"\" already exists. Skipped."
        );
    }

    GUIManager::GetSingleton()->AddUIObject(this);

    CreateGlue();

    if (pParentFrame_)
    {
        pParentFrame_->AddChild(this);
        SetLevel(pParentFrame_->GetFrameLevel() + 1);
    }
    else
        SetLevel(0);

    s_str sInheritance = pBlock->GetAttribute("inherits");
    if (!sInheritance.IsEmpty(true))
    {
        s_ctnr<s_str> lObjects = sInheritance.Cut(",");
        s_ctnr<s_str>::iterator iter;
        foreach (iter, lObjects)
        {
            iter->Trim(' ');
            s_ptr<GUI::UIObject> pObj = GUIManager::GetSingleton()->GetUIObjectByName(*iter, true);
            if (pObj)
            {
                if (IsObjectType(pObj->GetObjectType()))
                {
                    // Inherit from the other Frame
                    CopyFrom(pObj);
                }
                else
                {
                    Warning(pBlock->GetFile()+":"+pBlock->GetLineNbr(),
                        "\""+sName_+"\" ("+lType_.Back()+") cannot inherit "
                        "from \""+(*iter)+"\" ("+pObj->GetObjectType()+"). Inheritance skipped."
                    );
                }
            }
            else
            {
                Warning(pBlock->GetFile()+":"+pBlock->GetLineNbr(),
                    "Couldn't find inherited object \""+*iter+"\". Inheritance skipped."
                );
            }
        }
    }

    if ((pBlock->IsProvided("hidden") || !bInherits_) &&
        (s_bool(pBlock->GetAttribute("hidden"))))
        Hide();

    if ((pBlock->IsProvided("setAllPoints") || !bInherits_) &&
        (s_bool(pBlock->GetAttribute("setAllPoints"))))
        SetAllPoints("$parent");

    if (pBlock->IsProvided("alpha") || !bInherits_)
        SetAlpha(s_float(pBlock->GetAttribute("alpha")));
    if (pBlock->IsProvided("topLevel") || !bInherits_)
        SetTopLevel(s_bool(pBlock->GetAttribute("topLevel")));
    if (pBlock->IsProvided("movable") || !bInherits_)
        SetMovable(s_bool(pBlock->GetAttribute("movable")));
    if (pBlock->IsProvided("resizable") || !bInherits_)
        SetResizable(s_bool(pBlock->GetAttribute("resizable")));
    if (pBlock->IsProvided("frameStrata") || !bInherits_)
        SetFrameStrata(pBlock->GetAttribute("frameStrata"));
    if (pBlock->IsProvided("enableMouse") || !bInherits_)
        EnableMouse(s_bool(pBlock->GetAttribute("enableMouse")));
    if (pBlock->IsProvided("enableMouseWheel") || !bInherits_)
        EnableMouseWheel(s_bool(pBlock->GetAttribute("enableMouseWheel")));
    if (pBlock->IsProvided("enableKeyboard") || !bInherits_)
        EnableKeyboard(s_bool(pBlock->GetAttribute("enableKeyboard")));
    if (pBlock->IsProvided("clampedToScreen") || !bInherits_)
        SetClampedToScreen(s_bool(pBlock->GetAttribute("clampedToScreen")));
}

void Frame::ParseResizeBoundsBlock_( s_ptr<XML::Block> pBlock )
{
    s_ptr<XML::Block> pResizeBoundsBlock = pBlock->GetBlock("ResizeBounds");
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

                SetMinResize(s_uint(iX), s_uint(iY));
            }
            else if (pDimBlock->GetName() == "RelDimension")
            {
                Warning(pDimBlock->GetFile()+":"+pDimBlock->GetLineNbr(),
                    "\"RelDimension\" for ResizeBounds:Min is not yet supported. Skipped."
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

                SetMaxResize(s_uint(iX), s_uint(iY));
            }
            else if (pDimBlock->GetName() == "RelDimension")
            {
                Warning(pDimBlock->GetFile()+":"+pDimBlock->GetLineNbr(),
                    "\"RelDimension\" for ResizeBounds:Max is not yet supported. Skipped."
                );
            }
        }
    }
}

void Frame::ParseTitleRegionBlock_( s_ptr<XML::Block> pBlock )
{
    s_ptr<XML::Block> pTitleRegionBlock = pBlock->GetBlock("TitleRegion");
    if (pTitleRegionBlock)
    {
        CreateTitleRegion();

        if (pTitleRegion_)
            pTitleRegion_->ParseBlock(pTitleRegionBlock);
    }
}

void Frame::ParseBackdropBlock_( s_ptr<XML::Block> pBlock )
{
    s_ptr<XML::Block> pBackdropBlock = pBlock->GetBlock("Backdrop");
    if (pBackdropBlock)
    {
        s_refptr<GUI::Backdrop> pBackdrop(new GUI::Backdrop(this));

        pBackdrop->SetBackground(GUIManager::GetSingleton()->ParseFileName(
            pBackdropBlock->GetAttribute("bgFile")
        ));
        pBackdrop->SetEdge(GUIManager::GetSingleton()->ParseFileName(
            pBackdropBlock->GetAttribute("edgeFile")
        ));

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
                    "RelInset for Backdrop:BackgroundInsets is not yet supported ("+sName_+")."
                );
            }
        }

        s_ptr<XML::Block> pColorBlock = pBackdropBlock->GetBlock("BackgroundColor");
        if (pColorBlock)
        {
            pBackdrop->SetBackgroundColor(Color(
                s_float(pColorBlock->GetAttribute("a")),
                s_float(pColorBlock->GetAttribute("r")),
                s_float(pColorBlock->GetAttribute("g")),
                s_float(pColorBlock->GetAttribute("b"))
            ));
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
                    "RelValue for Backdrop:EdgeSize is not yet supported ("+sName_+")."
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
                    "RelValue for Backdrop:TileSize is not yet supported ("+sName_+")."
                );
            }
        }

        SetBackdrop(pBackdrop);
    }
}

void Frame::ParseHitRectInsetsBlock_( s_ptr<XML::Block> pBlock )
{
    s_ptr<XML::Block> pHitRectBlock = pBlock->GetBlock("HitRectInsets");
    if (pHitRectBlock)
    {
        s_ptr<XML::Block> pInsetBlock = pHitRectBlock->GetRadioBlock();
        if (pInsetBlock->GetName() == "AbsInset")
        {
            SetAbsHitRectInsets(
                s_int(pInsetBlock->GetAttribute("left")),
                s_int(pInsetBlock->GetAttribute("right")),
                s_int(pInsetBlock->GetAttribute("top")),
                s_int(pInsetBlock->GetAttribute("bottom"))
            );
        }
        else if (pInsetBlock->GetName() == "RelInset")
        {
            SetRelHitRectInsets(
                s_float(pInsetBlock->GetAttribute("left")),
                s_float(pInsetBlock->GetAttribute("right")),
                s_float(pInsetBlock->GetAttribute("top")),
                s_float(pInsetBlock->GetAttribute("bottom"))
            );
        }
    }
}

void Frame::ParseLayersBlock_( s_ptr<XML::Block> pBlock )
{
    s_ptr<XML::Block> pLayersBlock = pBlock->GetBlock("Layers");
    if (pLayersBlock)
    {
        s_ptr<XML::Block> pLayerBlock;
        foreach_block (pLayerBlock, pLayersBlock)
        {
            s_str sLevel = pLayerBlock->GetAttribute("level");
            s_ptr<XML::Block> pRegionBlock;
            foreach_block (pRegionBlock, pLayerBlock)
            {
                s_ptr<GUI::LayeredRegion> pRegion = GUIManager::GetSingleton()->CreateLayeredRegion(pRegionBlock->GetName());
                if (pRegion)
                {
                    try
                    {
                        pRegion->SetDrawLayer(sLevel);
                        pRegion->SetParent(this);
                        pRegion->ParseBlock(pRegionBlock);
                    }
                    catch (const GUIException& e)
                    {
                        pRegion.Delete();
                        Error("", e.GetDescription());
                    }
                    catch (...)
                    {
                        pRegion.Delete();
                        throw;
                    }
                }
            }
        }
    }
}

void Frame::ParseFramesBlock_( s_ptr<XML::Block> pBlock )
{
    s_ptr<XML::Block> pFramesBlock = pBlock->GetBlock("Frames");
    if (pFramesBlock)
    {
        s_ptr<XML::Block> pElemBlock;
        foreach_block (pElemBlock, pFramesBlock)
        {
            s_ptr<Frame> pFrame = GUIManager::GetSingleton()->CreateFrame(pElemBlock->GetName());
            if (pFrame)
            {
                try
                {
                    pFrame->SetAddOn(GUIManager::GetSingleton()->GetCurrentAddOn());
                    pFrame->SetParent(this);
                    pFrame->ParseBlock(pElemBlock);
                    if (!pFrame->IsVirtual())
                        pFrame->On("Load");
                }
                catch (const GUIException& e)
                {
                    pFrame.Delete();
                    Error("", e.GetDescription());
                }
                catch (...)
                {
                    pFrame.Delete();
                    throw;
                }
            }
        }
    }
}

void Frame::ParseScriptsBlock_( s_ptr<XML::Block> pBlock )
{
    s_ptr<XML::Block> pScriptsBlock = pBlock->GetBlock("Scripts");
    if (pScriptsBlock)
    {
        s_ptr<XML::Block> pScriptBlock;
        foreach_block (pScriptBlock, pScriptsBlock)
        {
            DefineScript(pScriptBlock->GetName(), pScriptBlock->GetValue());
        }
    }
}
