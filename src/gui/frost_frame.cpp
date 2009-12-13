/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_frame.h"

#include "gui/frost_layeredregion.h"
#include "gui/frost_uiobject.h"
#include "gui/frost_region.h"
#include "gui/frost_button.h"
#include "gui/frost_checkbutton.h"
#include "gui/frost_colorselect.h"
#include "gui/frost_cooldown.h"
#include "gui/frost_editbox.h"
#include "gui/frost_messageframe.h"
#include "gui/frost_scrollframe.h"
#include "gui/frost_scrollingmessageframe.h"
#include "gui/frost_slider.h"
#include "gui/frost_statusbar.h"
#include "gui/frost_texture.h"
#include "gui/frost_fontstring.h"
#include "gui/frost_guimanager.h"
#include "gui/frost_backdrop.h"
#include "frost_inputmanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str Frame::CLASS_NAME  = "GUI::Frame";
const s_str Layer::CLASS_NAME  = "GUI::Layer";

Frame::Frame() : Region(), lAbsHitRectInsetList_(0), lRelHitRectInsetList_(0.0f)
{
    lType_.PushBack("Frame");

    mStrata_ = STRATA_MEDIUM;

    uiMinWidth_ = 0u;
    uiMaxWidth_ = s_uint(s_uint::INTEGER_INF);
    uiMinHeight_ = 0u;
    uiMaxHeight_ = s_uint(s_uint::INTEGER_INF);

    fScale_ = 1.0f;

    uiLevel_ = 0u;
}

Frame::~Frame()
{
    s_map< s_uint, s_ptr<LayeredRegion> >::iterator iterRegion;
    foreach (iterRegion, lRegionList_)
    {
        iterRegion->second.Delete();
    }

    s_map< s_uint, s_ptr<Frame> >::iterator iterChild;
    foreach (iterChild, lChildList_)
    {
        iterChild->second.Delete();
    }

    pTitleRegion_.Delete();
}

void Frame::Render()
{
    if (IsVisible())
    {
        if (pBackdrop_)
        {
            pBackdrop_->Render();
        }

        // Render child regions
        s_map<LayerType, Layer>::iterator iterLayer;
        foreach (iterLayer, lLayerList_)
        {
            Layer& mLayer = iterLayer->second;
            if (!mLayer.bDisabled)
            {
                s_map< s_uint, s_ptr<LayeredRegion> >::iterator iterRegion;
                foreach (iterRegion, mLayer.lRegionList)
                {
                    s_ptr<LayeredRegion> pRegion = iterRegion->second;
                    if (pRegion->IsShown())
                        pRegion->Render();
                }
            }
        }
    }
}

void Frame::CreateGlue()
{
    if (bVirtual_)
    {
        s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
        pLua->PushNumber(uiID_);
        lGlueList_.PushBack(
            pLua->Push<LuaVirtualGlue>(new LuaVirtualGlue(pLua->GetState()))
        );
        pLua->SetGlobal(sLuaName_);
    }
    else
    {
        s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
        pLua->PushString(sName_);
        lGlueList_.PushBack(
            pLua->Push<LuaFrame>(new LuaFrame(pLua->GetState()))
        );
        pLua->SetGlobal(sLuaName_);
    }
}

s_str Frame::Serialize( const s_str& sTab ) const
{
    s_str sStr = Region::Serialize(sTab);
    sStr << sTab << "  # Strata      : ";
    switch (mStrata_)
    {
        case STRATA_PARENT :            sStr << "PARENT\n"; break;
        case STRATA_BACKGROUND :        sStr << "BACKGROUND\n"; break;
        case STRATA_LOW :               sStr << "LOW\n"; break;
        case STRATA_MEDIUM :            sStr << "MEDIUM\n"; break;
        case STRATA_HIGH :              sStr << "BACKGROUND\n"; break;
        case STRATA_DIALOG :            sStr << "DIALOG\n"; break;
        case STRATA_FULLSCREEN :        sStr << "FULLSCREEN\n"; break;
        case STRATA_FULLSCREEN_DIALOG : sStr << "FULLSCREEN_DIALOG\n"; break;
        case STRATA_TOOLTIP :           sStr << "TOOLTIP\n"; break;
    }
    sStr << sTab << "  # Level       : " << uiLevel_ << "\n";
    if (!bIsMouseEnabled_ && !bIsKeyboardEnabled_ && !bIsMouseWheelEnabled_)
        sStr << sTab << "  # Inputs      : none\n";
    else
    {
        sStr << sTab << "  # Inputs      :\n";
        sStr << sTab << "  |-###\n";
        if (bIsMouseEnabled_)
            sStr << sTab << "  |   # mouse\n";
        if (bIsKeyboardEnabled_)
            sStr << sTab << "  |   # keyboard\n";
        if (bIsMouseWheelEnabled_)
            sStr << sTab << "  |   # mouse wheel\n";
        sStr << sTab << "  |-###\n";
    }
    sStr << sTab << "  # Movable     : " << bIsMovable_ << "\n";
    sStr << sTab << "  # Resizable   : " << bIsResizable_ << "\n";
    sStr << sTab << "  # Clamped     : " << bIsClampedToScreen_ << "\n";
    sStr << sTab << "  # HRect inset :\n";
    sStr << sTab << "  |-###\n";
    sStr << sTab << "  |   # left   : " << lAbsHitRectInsetList_[BORDER_LEFT] << "\n";
    sStr << sTab << "  |   # right  : " << lAbsHitRectInsetList_[BORDER_RIGHT] << "\n";
    sStr << sTab << "  |   # top    : " << lAbsHitRectInsetList_[BORDER_TOP] << "\n";
    sStr << sTab << "  |   # bottom : " << lAbsHitRectInsetList_[BORDER_BOTTOM] << "\n";
    sStr << sTab << "  |-###\n";
    sStr << sTab << "  # Min width   : " << uiMinWidth_ << "\n";
    sStr << sTab << "  # Max width   : " << uiMaxWidth_ << "\n";
    sStr << sTab << "  # Min height  : " << uiMinHeight_ << "\n";
    sStr << sTab << "  # Max height  : " << uiMaxHeight_ << "\n";
    sStr << sTab << "  # Scale       : " << fScale_ << "\n";
    if (pTitleRegion_)
    {
        sStr << sTab << "  # Title reg.  :\n";
        sStr << sTab << "  |-###\n";
        sStr << pTitleRegion_->Serialize(sTab+"  | ");
        sStr << sTab << "  |-###\n";
    }
    if (pBackdrop_)
    {
        const s_array<s_int,4>& lInsets = pBackdrop_->GetBackgroundInsets();

        sStr << sTab << "  # Backdrop    :\n";
        sStr << sTab << "  |-###\n";
        sStr << sTab << "  |   # Background : " << pBackdrop_->GetBackgroundFile() << "\n";
        sStr << sTab << "  |   # Tilling    : " << pBackdrop_->IsBackgroundTilling() << "\n";
        if (pBackdrop_->IsBackgroundTilling())
            sStr << sTab << "  |   # Tile size  : " << pBackdrop_->GetTileSize() << "\n";
        sStr << sTab << "  |   # BG Insets  :\n";
        sStr << sTab << "  |   |-###\n";
        sStr << sTab << "  |   |   # left   : " << lInsets[BORDER_LEFT] << "\n";
        sStr << sTab << "  |   |   # right  : " << lInsets[BORDER_RIGHT] << "\n";
        sStr << sTab << "  |   |   # top    : " << lInsets[BORDER_TOP] << "\n";
        sStr << sTab << "  |   |   # bottom : " << lInsets[BORDER_BOTTOM] << "\n";
        sStr << sTab << "  |   |-###\n";
        sStr << sTab << "  |   # Edge       : " << pBackdrop_->GetEdgeFile() << "\n";
        sStr << sTab << "  |   # Edge size  : " << pBackdrop_->GetEdgeSize() << "\n";
        sStr << sTab << "  |-###\n";
    }

    if (!lRegionList_.IsEmpty())
    {
        if (lChildList_.GetSize() == 1)
            sStr << sTab << "  # Region : \n";
        else
            sStr << sTab << "  # Regions     : " << lRegionList_.GetSize() << "\n";
        sStr << sTab << "  |-###\n";

        s_map< s_uint, s_ptr<LayeredRegion> >::const_iterator iterRegion;
        foreach (iterRegion, lRegionList_)
        {
            sStr << iterRegion->second->Serialize(sTab+"  | ");
            sStr << sTab << "  |-###\n";
        }
    }

    if (!lChildList_.IsEmpty())
    {
        if (lChildList_.GetSize() == 1)
            sStr << sTab << "  # Child : \n";
        else
            sStr << sTab << "  # Children    : " << lChildList_.GetSize() << "\n";
        sStr << sTab << "  |-###\n";

        s_map< s_uint, s_ptr<Frame> >::const_iterator iterChild;
        foreach (iterChild, lChildList_)
        {
            sStr << iterChild->second->Serialize(sTab+"  | ");
            sStr << sTab << "  |-###\n";
        }
    }

    return sStr;
}

s_bool Frame::CanUseScript( const s_str& sScriptName ) const
{
    if ((sScriptName == "OnDragStart") ||
        (sScriptName == "OnDragStop") ||
        (sScriptName == "OnEnter") ||
        (sScriptName == "OnEvent") ||
        (sScriptName == "OnHide") ||
        (sScriptName == "OnKeyDown") ||
        (sScriptName == "OnKeyUp") ||
        (sScriptName == "OnLeave") ||
        (sScriptName == "OnLoad") ||
        (sScriptName == "OnMouseDown") ||
        (sScriptName == "OnMouseUp") ||
        (sScriptName == "OnMouseWheel") ||
        (sScriptName == "OnReceiveDrag") ||
        (sScriptName == "OnShow") ||
        (sScriptName == "OnSizeChanged") ||
        (sScriptName == "OnUpdate"))
        return true;
    else
        return false;
}

void Frame::CopyFrom( s_ptr<UIObject> pObj )
{
    UIObject::CopyFrom(pObj);

    s_ptr<Frame> pFrame = s_ptr<Frame>::DynamicCast(pObj);

    if (pFrame)
    {
        s_map<s_str, s_str>::const_iterator iterScript;
        foreach (iterScript, pFrame->lDefinedScriptList_)
        {
            if (!iterScript->second.IsEmpty())
                this->DefineScript("On"+iterScript->first, iterScript->second);
        }

        this->SetFrameStrata(pFrame->GetFrameStrata());

        // ?
        //bIsParentStrata_ = pFrame->bIsParentStrata_;

        if (pFrame->GetFrameLevel() != 0)
        {
            s_ptr<UIObject> pHighParent = this;
            for (s_uint i = 0; i < pFrame->GetFrameLevel(); ++i)
            {
                pHighParent = pHighParent->GetParent();
            }

            this->SetLevel(
                s_ptr<Frame>::DynamicCast(pHighParent)->GetFrameLevel()+
                pFrame->GetFrameLevel()
            );
        }


        this->SetTopStrata(pFrame->IsTopStrata());
        this->SetTopLevel(pFrame->IsTopLevel());

        this->EnableKeyboard(pFrame->IsKeyboardEnabled());
        this->EnableMouse(pFrame->IsMouseEnabled());
        this->EnableMouseWheel(pFrame->IsMouseWheelEnabled());

        this->SetMovable(pFrame->IsMovable());
        this->SetClampedToScreen(pFrame->IsClampedToScreen());
        this->SetResizable(pFrame->IsResizable());

        s_array<s_int,4> lAbsInsets = pFrame->GetAbsHitRectInsets();
        this->SetAbsHitRectInsets(lAbsInsets[0], lAbsInsets[1], lAbsInsets[2], lAbsInsets[3]);
        s_array<s_float,4> lRelInsets = pFrame->GetRelHitRectInsets();
        this->SetRelHitRectInsets(lRelInsets[0], lRelInsets[1], lRelInsets[2], lRelInsets[3]);

        s_array<s_uint,2> lMax = pFrame->GetMaxResize();
        this->SetMaxResize(lMax[0], lMax[1]);
        s_array<s_uint,2> lMin = pFrame->GetMinResize();
        this->SetMinResize(lMin[0], lMin[1]);

        this->SetScale(pFrame->GetScale());

        s_map< s_uint, s_ptr<Frame> >::const_iterator iterChild;
        foreach (iterChild, pFrame->lChildList_)
        {
            s_ptr<Frame> pChild = iterChild->second;
            if (!pChild->IsSpecial())
            {
                s_ptr<Frame> pNewChild;
                if (pChild->GetObjectType() == "Frame")
                    pNewChild = new Frame();
                else if (pChild->GetObjectType() == "Button")
                    pNewChild = new Button();
                else if (pChild->GetObjectType() == "CheckButton")
                    pNewChild = new CheckButton();
                else if (pChild->GetObjectType() == "ColorSelect")
                    pNewChild = new ColorSelect();
                else if (pChild->GetObjectType() == "Cooldown")
                    pNewChild = new Cooldown();
                else if (pChild->GetObjectType() == "EditBox")
                    pNewChild = new EditBox();
                else if (pChild->GetObjectType() == "MessageFrame")
                    pNewChild = new MessageFrame();
                else if (pChild->GetObjectType() == "ScrollFrame")
                    pNewChild = new ScrollFrame();
                else if (pChild->GetObjectType() == "ScrollingMessageFrame")
                    pNewChild = new ScrollingMessageFrame();
                else if (pChild->GetObjectType() == "Slider")
                    pNewChild = new Slider();
                else if (pChild->GetObjectType() == "StatusBar")
                    pNewChild = new StatusBar();
                else
                {
                    Warning(lType_.Back(),
                        "Trying to add \""+pChild->GetName()+"\" (type : "+pChild->GetObjectType()+") to \""+sName_+"\", "
                        "but no copying code was available. Skipped."
                    );
                }

                if (pNewChild)
                {
                    pNewChild->SetParent(this);
                    if (this->IsVirtual())
                        pNewChild->SetVirtual();
                    pNewChild->SetName(pChild->GetRawName());
                    if (!GUIManager::GetSingleton()->AddUIObject(pNewChild))
                    {
                        Warning(lType_.Back(),
                            "Trying to add \""+pChild->GetName()+"\" to \""+sName_+"\", but its name was already taken : \""
                            +pNewChild->GetName()+"\". Skipped."
                        );
                        pNewChild.Delete();
                        continue;
                    }
                    pNewChild->CreateGlue();
                    this->AddChild(pNewChild);
                    pNewChild->CopyFrom(pChild);
                    if (!bVirtual_)
                        pNewChild->On("Load");
                }
            }
        }

        if (pFrame->pBackdrop_)
        {
            pBackdrop_ = s_refptr<Backdrop>(new Backdrop(this));
            pBackdrop_->CopyFrom(*pFrame->pBackdrop_);
        }

        if (pFrame->pTitleRegion_)
        {
            this->CreateTitleRegion();
            pTitleRegion_->CopyFrom(pFrame->pTitleRegion_);
        }

        s_map< s_uint, s_ptr<LayeredRegion> >::const_iterator iterRegion;
        foreach (iterRegion, pFrame->lRegionList_)
        {
            s_ptr<LayeredRegion> pArt = iterRegion->second;
            if (!pArt->IsSpecial())
            {
                s_ptr<LayeredRegion> pNewArt;
                if (pArt->GetObjectType() == "Texture")
                    pNewArt = new Texture();
                else if (pArt->GetObjectType() == "FontString")
                    pNewArt = new FontString();
                else
                {
                    Warning(lType_.Back(),
                        "Trying to add \""+pArt->GetName()+"\" (type : "+pArt->GetObjectType()+") to \""+sName_+"\", "
                        "but no copying code was available. Skipped."
                    );
                }

                if (pNewArt)
                {
                    pNewArt->SetParent(this);
                    if (this->IsVirtual())
                        pNewArt->SetVirtual();
                    pNewArt->SetName(pArt->GetRawName());
                    if (!GUIManager::GetSingleton()->AddUIObject(pNewArt))
                    {
                        Warning(lType_.Back(),
                            "Trying to add \""+pArt->GetName()+"\" to \""+sName_+"\", but its name was already taken : \""
                            +pNewArt->GetName()+"\". Skipped."
                        );
                        pNewArt.Delete();
                        continue;
                    }
                    if (!pNewArt->IsVirtual())
                        pNewArt->CreateGlue();

                    pNewArt->SetDrawLayer(pArt->GetDrawLayer());

                    this->AddRegion(pNewArt);
                    pNewArt->CopyFrom(pArt);
                }
            }
        }

        bBuildStrataList_ = true;
        bBuildLayerList_ = true;
    }
}

void Frame::CreateTitleRegion()
{
    if (!pTitleRegion_)
    {
        pTitleRegion_ = new Region();
        if (this->IsVirtual())
            pTitleRegion_->SetVirtual();
        pTitleRegion_->SetSpecial();
        pTitleRegion_->SetParent(this);
        pTitleRegion_->SetName(sName_+"TitleRegion");

        if (!GUIManager::GetSingleton()->AddUIObject(pTitleRegion_))
        {
            Warning(lType_.Back(),
                "Couldn't create \""+sName_+"\"'s title region because another UIObject "
                "already took its name : \""+pTitleRegion_->GetName()+"\"."
            );
            pTitleRegion_.Delete();
            return;
        }

        if (!pTitleRegion_->IsVirtual())
            pTitleRegion_->CreateGlue();
    }
    else
    {
        Warning(lType_.Back(),
            "\""+sName_+"\" already has a title region."
        );
    }
}

s_ptr<Frame> Frame::GetChild( const s_str& sName ) const
{
    s_ptr<UIObject> pObj = GUIManager::GetSingleton()->GetUIObjectByName(sName);

    if (pObj && lChildList_.Find(pObj->GetID()))
        return lChildList_.Get(pObj->GetID())->second;
    else
        return nullptr;
}

s_ptr<LayeredRegion> Frame::GetRegion( const s_str& sName ) const
{
    s_ptr<UIObject> pObj = GUIManager::GetSingleton()->GetUIObjectByName(sName);

    if (pObj && lRegionList_.Find(pObj->GetID()))
        return lRegionList_.Get(pObj->GetID())->second;
    else
        return nullptr;
}

void Frame::SetAbsWidth( const s_uint& uiAbsWidth )
{
    UIObject::SetAbsWidth(s_uint::Clamp(uiAbsWidth, uiMinWidth_, uiMaxWidth_));
}

void Frame::SetAbsHeight( const s_uint& uiAbsHeight )
{
    UIObject::SetAbsHeight(s_uint::Clamp(uiAbsHeight, uiMinHeight_, uiMaxHeight_));
}

void Frame::CheckPosition()
{
    if (lBorderList_[BORDER_RIGHT] - lBorderList_[BORDER_LEFT] < s_int(uiMinWidth_))
    {
        lBorderList_[BORDER_RIGHT] = lBorderList_[BORDER_LEFT] + s_int(uiMinWidth_);
        uiAbsWidth_ = uiMinWidth_;
    }
    else if (lBorderList_[BORDER_RIGHT] - lBorderList_[BORDER_LEFT] > s_int(uiMaxWidth_))
    {
        lBorderList_[BORDER_RIGHT] = lBorderList_[BORDER_LEFT] + s_int(uiMaxWidth_);
        uiAbsWidth_ = uiMaxWidth_;
    }

    if (lBorderList_[BORDER_BOTTOM] - lBorderList_[BORDER_TOP] < s_int(uiMinHeight_))
    {
        lBorderList_[BORDER_BOTTOM] = lBorderList_[BORDER_TOP] + s_int(uiMinHeight_);
        uiAbsHeight_ = uiMinHeight_;
    }
    else if (lBorderList_[BORDER_BOTTOM] - lBorderList_[BORDER_TOP] > s_int(uiMaxHeight_))
    {
        lBorderList_[BORDER_BOTTOM] = lBorderList_[BORDER_TOP] + s_int(uiMaxHeight_);
        uiAbsHeight_ = uiMaxHeight_;
    }

    if (bIsClampedToScreen_)
    {
        s_int iScreenW = s_int(Engine::GetSingleton()->GetScreenWidth());
        s_int iScreenH = s_int(Engine::GetSingleton()->GetScreenHeight());

        if (lBorderList_[BORDER_RIGHT] > iScreenW)
        {
            if (lBorderList_[BORDER_RIGHT]-lBorderList_[BORDER_LEFT] > iScreenW)
            {
                lBorderList_[BORDER_LEFT] = 0;
                lBorderList_[BORDER_RIGHT] = iScreenW;
            }
            else
            {
                lBorderList_[BORDER_RIGHT] = iScreenW;
                lBorderList_[BORDER_LEFT] = iScreenW - s_int(uiAbsWidth_);
            }
        }
        if (lBorderList_[BORDER_LEFT] < 0)
        {
            if (lBorderList_[BORDER_RIGHT]-lBorderList_[BORDER_LEFT] > iScreenW)
            {
                lBorderList_[BORDER_LEFT] = 0;
                lBorderList_[BORDER_RIGHT] = iScreenW;
            }
            else
            {
                lBorderList_[BORDER_LEFT] = 0;
                lBorderList_[BORDER_RIGHT] = s_int(uiAbsWidth_);
            }
        }

        if (lBorderList_[BORDER_BOTTOM] > iScreenH)
        {
            if (lBorderList_[BORDER_BOTTOM]-lBorderList_[BORDER_TOP] > iScreenH)
            {
                lBorderList_[BORDER_TOP] = 0;
                lBorderList_[BORDER_BOTTOM] = iScreenH;
            }
            else
            {
                lBorderList_[BORDER_BOTTOM] = iScreenH;
                lBorderList_[BORDER_TOP] = iScreenH - s_int(uiAbsHeight_);
            }
        }
        if (lBorderList_[BORDER_TOP] < 0)
        {
            if (lBorderList_[BORDER_BOTTOM]-lBorderList_[BORDER_TOP] > iScreenH)
            {
                lBorderList_[BORDER_TOP] = 0;
                lBorderList_[BORDER_BOTTOM] = iScreenH;
            }
            else
            {
                lBorderList_[BORDER_TOP] = 0;
                lBorderList_[BORDER_BOTTOM] = s_int(uiAbsHeight_);
            }
        }
    }
}

void Frame::DisableDrawLayer( LayerType mLayer )
{
    lLayerList_[mLayer].bDisabled = true;
}

void Frame::EnableDrawLayer( LayerType mLayer )
{
    lLayerList_[mLayer].bDisabled = false;
}

void Frame::EnableKeyboard( const s_bool& bIsKeyboardEnabled )
{
    if (!bVirtual_)
    {
        if (bIsKeyboardEnabled && !bIsKeyboardEnabled_)
        {
            EventReceiver::RegisterEvent("KEY_PRESSED");
            EventReceiver::RegisterEvent("KEY_RELEASED");
        }
        else if (!bIsKeyboardEnabled && bIsKeyboardEnabled_)
        {
            EventReceiver::UnregisterEvent("KEY_PRESSED");
            EventReceiver::UnregisterEvent("KEY_RELEASED");
        }
    }

    bIsKeyboardEnabled_ = bIsKeyboardEnabled;
}

void Frame::EnableMouse( const s_bool& bIsMouseEnabled )
{
    if (!bVirtual_)
    {
        if (bIsMouseEnabled && !bIsMouseEnabled_)
        {
            EventReceiver::RegisterEvent("MOUSE_MOVED_RAW");
            EventReceiver::RegisterEvent("MOUSE_PRESSED");
            EventReceiver::RegisterEvent("MOUSE_DOUBLE_CLICKED");
            EventReceiver::RegisterEvent("MOUSE_RELEASED");
        }
        else if (!bIsMouseEnabled && bIsMouseEnabled_)
        {
            EventReceiver::UnregisterEvent("MOUSE_MOVED_RAW");
            EventReceiver::UnregisterEvent("MOUSE_PRESSED");
            EventReceiver::UnregisterEvent("MOUSE_DOUBLE_CLICKED");
            EventReceiver::UnregisterEvent("MOUSE_RELEASED");
        }
    }

    bIsMouseEnabled_ = bIsMouseEnabled;
}

void Frame::EnableMouseWheel( const s_bool& bIsMouseWheelEnabled )
{
    if (!bVirtual_)
    {
        if (bIsMouseWheelEnabled && !bIsMouseWheelEnabled_)
            EventReceiver::RegisterEvent("MOUSE_WHEEL");
        else if (!bIsMouseWheelEnabled && bIsMouseWheelEnabled_)
            EventReceiver::UnregisterEvent("MOUSE_WHEEL");
    }

    bIsMouseWheelEnabled_ = bIsMouseWheelEnabled;
}

void Frame::FireBuildLayerList_()
{
    bBuildLayerList_ = true;
}

void Frame::FireUpdateBorders()
{
    bUpdateBorders_ = true;

    if (pTitleRegion_)
        pTitleRegion_->FireUpdateBorders();

    s_map< s_uint, s_ptr<LayeredRegion> >::iterator iterRegion;
    foreach (iterRegion, lRegionList_)
    {
        iterRegion->second->FireUpdateBorders();
    }

    s_map< s_uint, s_ptr<Frame> >::iterator iterChild;
    foreach (iterChild, lChildList_)
    {
        iterChild->second->FireUpdateBorders();
    }
}

s_bool Frame::HasScript( const s_str& sScriptName ) const
{
    return lDefinedScriptList_.Find(sScriptName);
}

void Frame::AddRegion( s_ptr<LayeredRegion> pRegion )
{
    if (pRegion)
    {
        if (!lRegionList_.Find(pRegion->GetID()))
        {
            lRegionList_[pRegion->GetID()] = pRegion;
            FireBuildLayerList_();

            if (!bVirtual_)
            {
                s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
                const s_str& sRawName = pRegion->GetRawName();
                if (sRawName.StartsWith("$parent"))
                {
                    s_str sTempName = pRegion->GetName();
                    sTempName.EraseFromStart(sName_.GetSize());
                    pLua->GetGlobal(pRegion->GetName());
                    pLua->SetGlobal(sName_+"."+sTempName);
                }
            }
        }
        else
        {
            Warning(lType_.Back(),
                "Trying to add \""+pRegion->GetName()+"\" to \""+sName_+"\"'s children, "
                "but it was already one of this Frame's children."
            );
        }
    }
}

void Frame::RemoveRegion( s_ptr<LayeredRegion> pRegion )
{
    if (pRegion)
    {
        s_map< s_uint, s_ptr<LayeredRegion> >::iterator iter = lRegionList_.Get(pRegion->GetID());
        if (iter != lRegionList_.End())
        {
            lRegionList_.Erase(iter);
            FireBuildLayerList_();
        }
        else
        {
            Warning(lType_.Back(),
                "Trying to remove \""+pRegion->GetName()+"\" from \""+sName_+"\"'s children, "
                "but it was not one of this Frame's children."
            );
        }
    }
}

void Frame::AddChild( s_ptr<Frame> pChild )
{
    if (pChild)
    {
        if (!lChildList_.Find(pChild->GetID()))
        {
            lChildList_[pChild->GetID()] = pChild;
            GUIManager::GetSingleton()->FireBuildStrataList();

            if (!bVirtual_)
            {
                s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
                s_str sRawName = pChild->GetRawName();
                if (sRawName.StartsWith("$parent"))
                {
                    sRawName.EraseFromStart(7);
                    pLua->GetGlobal(pChild->GetLuaName());
                    pLua->SetGlobal(sLuaName_+"."+sRawName);
                }
            }
        }
        else
        {
            Warning(lType_.Back(),
                "Trying to add \""+pChild->GetName()+"\" to \""+sName_+"\"'s children, "
                "but it was already one of this Frame's children."
            );
        }
    }
}

void Frame::RemoveChild( s_ptr<Frame> pChild )
{
    if (pChild)
    {
        s_map< s_uint, s_ptr<Frame> >::iterator iter = lChildList_.Get(pChild->GetID());
        if (iter != lChildList_.End())
        {
            lChildList_.Erase(iter);
            GUIManager::GetSingleton()->FireBuildStrataList();
        }
        else
        {
            Warning(lType_.Back(),
                "Trying to remove \""+pChild->GetName()+"\" from \""+sName_+"\"'s children, "
                "but it was not one of this Frame's children."
            );
        }
    }
}

s_ctnr< s_ptr<Frame> > Frame::GetChildren()
{
    s_ctnr< s_ptr<Frame> > lChilds;
    s_map<s_uint, s_ptr<Frame> >::iterator iterChild;
    foreach (iterChild, lChildList_)
    {
        lChilds.PushBack(iterChild->second);
    }

    return lChilds;
}

s_float Frame::GetEffectiveAlpha() const
{
    if (pParent_)
    {
        return fAlpha_ * pParentFrame_->GetEffectiveAlpha();
    }
    else
    {
        return fAlpha_;
    }
}

s_float Frame::GetEffectiveScale() const
{
    if (pParent_)
    {
        return fScale_ * pParentFrame_->GetEffectiveScale();
    }
    else
    {
        return fScale_;
    }
}

const s_uint& Frame::GetFrameLevel() const
{
    return uiLevel_;
}

FrameStrata Frame::GetFrameStrata() const
{
    return mStrata_;
}

s_wptr<Backdrop> Frame::GetBackdrop() const
{
    return pBackdrop_;
}

const s_str& Frame::GetFrameType() const
{
    return lType_.Back();
}

s_array<s_int,4> Frame::GetAbsHitRectInsets() const
{
    return lAbsHitRectInsetList_;
}

s_array<s_float,4> Frame::GetRelHitRectInsets() const
{
    return lRelHitRectInsetList_;
}

s_array<s_uint,2> Frame::GetMaxResize() const
{
    return (uiMaxWidth_, uiMaxHeight_);
}

s_array<s_uint,2> Frame::GetMinResize() const
{
    return (uiMinWidth_, uiMinHeight_);
}

s_uint Frame::GetNumChildren() const
{
    return lChildList_.GetSize();
}

s_uint Frame::GetNumRegions() const
{
    return lRegionList_.GetSize();
}

const s_float& Frame::GetScale() const
{
    return fScale_;
}

s_ptr<Region> Frame::GetTitleRegion() const
{
    return pTitleRegion_;
}

const s_bool& Frame::IsClampedToScreen() const
{
    return bIsClampedToScreen_;
}

s_bool Frame::IsInFrame( const s_int& iX, const s_int& iY ) const
{
    s_bool bInFrame = (
        iX.IsInRange(
            lBorderList_[BORDER_LEFT]  + lAbsHitRectInsetList_[BORDER_LEFT],
            lBorderList_[BORDER_RIGHT] - lAbsHitRectInsetList_[BORDER_RIGHT] - 1
        ) &&
        iY.IsInRange(
            lBorderList_[BORDER_TOP]    + lAbsHitRectInsetList_[BORDER_TOP],
            lBorderList_[BORDER_BOTTOM] - lAbsHitRectInsetList_[BORDER_BOTTOM] - 1
        )
    );

    if (pTitleRegion_ && pTitleRegion_->IsInRegion(iX, iY))
        return true;
    else
        return bInFrame;
}

const s_bool& Frame::IsKeyboardEnabled() const
{
    return bIsKeyboardEnabled_;
}

const s_bool& Frame::IsMouseEnabled() const
{
    return bIsMouseEnabled_;
}

const s_bool& Frame::IsMouseWheelEnabled() const
{
    return bIsMouseWheelEnabled_;
}

const s_bool& Frame::IsMovable() const
{
    return bIsMovable_;
}

const s_bool& Frame::IsResizable() const
{
    return bIsResizable_;
}

const s_bool& Frame::IsTopLevel() const
{
    return bIsTopLevel_;
}

const s_bool& Frame::IsTopStrata() const
{
    return bIsTopStrata_;
}

const s_bool& Frame::IsUserPlaced() const
{
    return bIsUserPlaced_;
}

void Frame::DefineScript( const s_str& sScriptName, const s_str& sContent )
{
    s_str sCutScriptName = sScriptName;
    sCutScriptName.EraseFromStart(2);
    lDefinedScriptList_[sCutScriptName] = sContent;

    s_str sStr;
    sStr += "function " + sLuaName_ + ":" + sScriptName + "()\n";
    sStr += sContent + "\n";
    sStr += "end";
    GUIManager::GetSingleton()->GetLua()->DoString(sStr);
}

void Frame::NotifyScriptDefined( const s_str& sScriptName )
{
    s_str sCutScriptName = sScriptName;
    sCutScriptName.EraseFromStart(2);
    lDefinedScriptList_[sCutScriptName] = "";
}

void Frame::OnEvent( const Event& mEvent )
{
    if (lDefinedScriptList_.Find("Event") && (lRegEventList_.Find(mEvent.GetName()) || bHasAllEventsRegistred_))
    {
        s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();

        // ADDON_LOADED should only be fired if it's this Frame's addon
        if (mEvent.GetName() == "ADDON_LOADED")
        {
            if (!pAddOn_ || pAddOn_->sName != mEvent.Get(0)->Get<s_str>())
                return;
        }

        Event mTemp = mEvent;
        On("Event", &mTemp);
    }

    if (bIsMouseEnabled_)
    {
        if (mEvent.GetName() == "MOUSE_MOVED_RAW")
        {
            if (!lMouseButtonList_.IsEmpty() && !bMouseDragged_)
            {
                s_ctnr<s_str>::iterator iterButton;
                foreach (iterButton, lMouseButtonList_)
                {
                    if (lRegDragList_.Find(*iterButton))
                    {
                        On("DragStart");
                        bMouseDragged_ = true;
                        break;
                    }
                }
            }
        }
        else if (mEvent.GetName() == "MOUSE_PRESSED")
        {
            if (bMouseInTitleRegion_)
                StartMoving();

            if (bMouseInFrame_)
            {
                s_str sMouseButton = InputManager::GetSingleton()->GetMouseButtonString(
                    (MouseButton)mEvent[0].Get<s_uint>().Get()
                );
                Event mEvent;
                mEvent.Add(sMouseButton);
                On("MouseDown", &mEvent);
                lMouseButtonList_.PushBack(sMouseButton);
            }
        }
        else if (mEvent.GetName() == "MOUSE_RELEASED")
        {
            if (bIsMovable_)
                StopMoving();

            if (bMouseInFrame_)
            {
                s_str sMouseButton = InputManager::GetSingleton()->GetMouseButtonString(
                    (MouseButton)mEvent[0].Get<s_uint>().Get()
                );
                Event mEvent;
                mEvent.Add(sMouseButton);
                On("MouseUp", &mEvent);
            }

            lMouseButtonList_.Clear();

            if (bMouseDragged_)
                On("DragStop");

            bMouseDragged_ = false;
        }
        else if (mEvent.GetName() == "MOUSE_WHEEL")
        {
            if (bMouseInFrame_)
                On("MouseWheel");
        }
    }
}

void Frame::On( const s_str& sScriptName, s_ptr<Event> pEvent )
{
    if (lDefinedScriptList_.Find(sScriptName))
    {
        s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();

        this->PushOnLua(pLua);
        pLua->SetGlobal("this");

        if (pInheritance_)
        {
            pInheritance_->PushOnLua(pLua);
            pLua->SetGlobal("base");
        }
        else
        {
            pLua->PushNil();
            pLua->SetGlobal("base");
        }

        if ((sScriptName == "KeyDown") ||
            (sScriptName == "KeyUp"))
        {
            // Set key name
            if (pEvent)
            {
                pLua->PushString(pEvent->Get(0)->Get<s_str>());
                pLua->SetGlobal("arg1");
            }
        }
        else if (sScriptName == "MouseDown")
        {
            // Set mouse button
            pLua->PushString(pEvent->Get(0)->Get<s_str>());
            pLua->SetGlobal("arg1");
        }
        else if (sScriptName == "MouseUp")
        {
            // Set mouse button
            pLua->PushString(pEvent->Get(0)->Get<s_str>());
            pLua->SetGlobal("arg1");
        }
        else if (sScriptName == "MouseWheel")
        {
            pLua->PushNumber(InputManager::GetSingleton()->GetMouseWheel());
            pLua->SetGlobal("arg1");
        }
        else if (sScriptName == "Update")
        {
            // Set delta time
            pLua->PushNumber(TimeManager::GetSingleton()->GetDelta());
            pLua->SetGlobal("arg1");
        }
        else if (sScriptName == "Event")
        {
            // Set event name
            pLua->PushString(pEvent->GetName());
            pLua->SetGlobal("event");

            // Set arguments
            for (s_uint i; i < pEvent->GetNumParam(); ++i)
            {
                s_ptr<const s_var> pArg = pEvent->Get(i);
                pLua->PushVar(*pArg);
                pLua->SetGlobal("arg"+(i+1));
            }
        }

        pLua->CallFunction(sName_+":On"+sScriptName);
    }
}

void Frame::RegisterAllEvents()
{
    bHasAllEventsRegistred_ = true;
    lRegEventList_.Clear();
}

void Frame::RegisterEvent( const s_str& sEvent )
{
    lRegEventList_[sEvent] = true;
    EventReceiver::RegisterEvent(sEvent);
}

void Frame::RegisterForDrag( const s_ctnr<s_str>& lButtonList )
{
    lRegDragList_.Clear();
    s_ctnr<s_str>::const_iterator iterButton;
    foreach (iterButton, lButtonList)
    {
        lRegDragList_[*iterButton] = true;
    }
}

void Frame::SetClampedToScreen( const s_bool& bIsClampedToScreen )
{
    bIsClampedToScreen_ = bIsClampedToScreen;
}

void Frame::SetFrameStrata( FrameStrata mStrata )
{
    if (mStrata == STRATA_PARENT)
    {
        if (!bVirtual_)
        {
            if (pParentFrame_)
                mStrata = pParentFrame_->GetFrameStrata();
            else
                mStrata = STRATA_MEDIUM;
        }
    }

    if ( (mStrata_ != mStrata) && !bVirtual_ )
        GUIManager::GetSingleton()->FireBuildStrataList();

    mStrata_ = mStrata;
}

void Frame::SetFrameStrata( const s_str& sStrata )
{
    FrameStrata mStrata;

    if (sStrata == "BACKGROUND")
        mStrata = STRATA_BACKGROUND;
    else if (sStrata == "LOW")
        mStrata = STRATA_LOW;
    else if (sStrata == "MEDIUM")
        mStrata = STRATA_MEDIUM;
    else if (sStrata == "HIGH")
        mStrata = STRATA_HIGH;
    else if (sStrata == "DIALOG")
        mStrata = STRATA_DIALOG;
    else if (sStrata == "FULLSCREEN")
        mStrata = STRATA_FULLSCREEN;
    else if (sStrata == "FULLSCREEN_DIALOG")
        mStrata = STRATA_FULLSCREEN_DIALOG;
    else if (sStrata == "TOOLTIP")
        mStrata = STRATA_TOOLTIP;
    else if (sStrata == "PARENT")
    {
        if (bVirtual_)
            mStrata = STRATA_PARENT;
        else
        {
            if (pParentFrame_)
                mStrata = pParentFrame_->GetFrameStrata();
            else
                mStrata = STRATA_MEDIUM;
        }
    }
    else
    {
        Warning(lType_.Back(),
            "Unknown strata : \""+sStrata+"\"."
        );
        return;
    }

    SetFrameStrata(mStrata);
}

void Frame::SetBackdrop( s_refptr<Backdrop> pBackdrop )
{
    pBackdrop_ = pBackdrop;
}

void Frame::SetAbsHitRectInsets( const s_int& iLeft, const s_int& iRight, const s_int& iTop, const s_int& iBottom )
{
    lAbsHitRectInsetList_ = (iLeft, iRight, iTop, iBottom);
}

void Frame::SetRelHitRectInsets( const s_float& fLeft, const s_float& fRight, const s_float& fTop, const s_float& fBottom )
{
    lRelHitRectInsetList_ = (fLeft, fRight, fTop, fBottom);
}

void Frame::SetLevel( const s_uint& uiLevel )
{
    if (uiLevel_ == 0)
    {
        uiLevel_ = uiLevel;
    }
    else
    {
        Warning(lType_.Back(), "SetLevel() can't be called more than once.");
    }
}

void Frame::SetMaxResize( const s_uint& uiMaxWidth, const s_uint& uiMaxHeight )
{
    if (uiMaxWidth >= uiMinWidth_)
        uiMaxWidth_ = uiMaxWidth;
    if (uiMaxHeight >= uiMinHeight_)
        uiMaxHeight_ = uiMaxHeight;
}

void Frame::SetMinResize( const s_uint& uiMinWidth, const s_uint& uiMinHeight )
{
    if (uiMinWidth <= uiMaxWidth_)
        uiMinWidth_ = uiMinWidth;
    if (uiMinHeight <= uiMaxHeight_)
        uiMinHeight_ = uiMinHeight;
}

void Frame::SetMovable( const s_bool& bIsMovable )
{
    bIsMovable_ = bIsMovable;
}

void Frame::SetParent( s_ptr<UIObject> pParent )
{
    if (pParent != (UIObject*)this)
    {
        if (pParentFrame_)
            pParentFrame_->RemoveChild(this);

        pParent_ = pParent;
        pParentFrame_ = s_ptr<Frame>::DynamicCast(pParent);

        if (!pAddOn_ && pParentFrame_)
            pAddOn_ = pParentFrame_->GetAddOn();

        FireUpdateDimensions();
    }
    else
    {
        Error(lType_.Back(), "Can't call SetParent(this).");
    }
}

void Frame::SetResizable( const s_bool& bIsResizable )
{
    bIsResizable_ = bIsResizable;
}

void Frame::SetScale( const s_float& fScale )
{
    fScale_ = fScale;
}

void Frame::SetTopLevel( const s_bool& bIsTopLevel )
{
    if (bIsTopLevel_ != bIsTopLevel)
    {
        bIsTopLevel_ = bIsTopLevel;
        bIsTopStrata_ = false;
        GUIManager::GetSingleton()->FireBuildStrataList();
    }
}

void Frame::SetTopStrata( const s_bool& bIsTopStrata )
{
    if ( pParent_ && (bIsTopStrata_ != bIsTopStrata) )
    {
        bIsTopStrata_ = bIsTopStrata;
        bIsTopLevel_ = false;
        GUIManager::GetSingleton()->FireBuildStrataList();
    }
}

void Frame::SetUserPlaced( const s_bool& bIsUserPlaced )
{
    bIsUserPlaced_ = bIsUserPlaced;
}

void Frame::StartMoving()
{
    if (bIsMovable_)
    {
        GUIManager::GetSingleton()->StartMoving(this);
    }
}

void Frame::StopMoving()
{
    GUIManager::GetSingleton()->StopMoving(this);
}

void Frame::StartSizing( const AnchorPoint& mPoint )
{
    if (bIsResizable_)
    {
        GUIManager::GetSingleton()->StartSizing(this, mPoint);
    }
}

void Frame::StopSizing()
{
    GUIManager::GetSingleton()->StopSizing(this);
}

void Frame::NotifyVisible_()
{
    lQueuedEventList_.PushBack("Show");
    s_map<s_uint, s_ptr<Frame> >::iterator iterChild;
    foreach (iterChild, lChildList_)
    {
        if (iterChild->second->IsShown())
            iterChild->second->NotifyVisible_();
    }
}

void Frame::NotifyInvisible_()
{
    lQueuedEventList_.PushBack("Hide");
    s_map<s_uint, s_ptr<Frame> >::iterator iterChild;
    foreach (iterChild, lChildList_)
    {
        if (iterChild->second->IsShown())
            iterChild->second->NotifyInvisible_();
    }
}

void Frame::Show()
{
    s_bool bWasShown = bIsShown_;

    UIObject::Show();

    if (IsVisible() && !bWasShown)
        NotifyVisible_();
}

void Frame::Hide()
{
    s_bool bWasVisible = IsVisible();

    UIObject::Hide();

    if (bWasVisible)
        NotifyInvisible_();
}

void Frame::UnregisterAllEvents()
{
    bHasAllEventsRegistred_ = false;
    lRegEventList_.Clear();
}

void Frame::UnregisterEvent( const s_str& sEvent )
{
    lRegEventList_[sEvent] = false;
}

void Frame::SetAddOn( s_ptr<AddOn> pAddOn )
{
    if (!pAddOn_)
    {
        pAddOn_ = pAddOn;
    }
    else
    {
        Warning(lType_.Back(), "SetAddOn() can only be called once.");
    }
}

s_ptr<AddOn> Frame::GetAddOn() const
{
    return pAddOn_;
}

void Frame::NotifyMouseInFrame( const s_bool& bMouseInFrame, const s_int& iX, const s_int& iY )
{
    if (bMouseInFrame)
    {
        if (pTitleRegion_ && pTitleRegion_->IsInRegion(iX, iY))
        {
            bMouseInTitleRegion_ = true;
            if (bMouseInFrame_)
                On("Leave");

            bMouseInFrame_ = false;
        }
        else
        {
            if (!bMouseInFrame_)
                On("Enter");

            bMouseInTitleRegion_ = false;
            bMouseInFrame_ = true;
        }
    }
    else
    {
        if (bMouseInFrame_)
            On("Leave");

        bMouseInTitleRegion_ = false;
        bMouseInFrame_ = false;
    }
}

void Frame::Update()
{
    s_bool bPositionUpdated = bUpdateBorders_;
    UIObject::Update();

    if (bPositionUpdated)
        CheckPosition();

    s_ctnr<s_str>::iterator iterEvent;
    foreach (iterEvent, lQueuedEventList_)
    {
        On(*iterEvent);
    }

    lQueuedEventList_.Clear();

    if (bBuildLayerList_)
    {
        // Clear layers' content
        s_map< LayerType, Layer >::iterator iterLayer;
        foreach (iterLayer, lLayerList_)
        {
            iterLayer->second.lRegionList.Clear();
        }

        // Fill layers with regions
        s_map< s_uint, s_ptr<LayeredRegion> >::iterator iterRegion;
        foreach (iterRegion, lRegionList_)
        {
            s_ptr<LayeredRegion> pRegion = iterRegion->second;
            lLayerList_[pRegion->GetDrawLayer()].lRegionList[pRegion->GetID()] = pRegion;
        }

        bBuildLayerList_ = false;
    }

    if (IsVisible())
        On("Update");

    if (pTitleRegion_)
        pTitleRegion_->Update();

    // Update regions
    s_map< s_uint, s_ptr<LayeredRegion> >::iterator iterRegion;
    foreach (iterRegion, lRegionList_)
    {
        iterRegion->second->Update();
    }

    // Update childrens
    s_map< s_uint, s_ptr<Frame> >::iterator iterChild;
    foreach (iterChild, lChildList_)
    {
        iterChild->second->Update();
    }
}

void Frame::UpdateMaterial( const s_bool& bForceUpdate )
{
    UIObject::UpdateMaterial(bForceUpdate);
}
