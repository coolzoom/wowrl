/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_frame.h"

#include "gui/frost_layeredregion.h"
#include "gui/frost_uiobject.h"
#include "gui/frost_button.h"
#include "gui/frost_editbox.h"
#include "gui/frost_scrollingmessageframe.h"
#include "gui/frost_slider.h"
#include "gui/frost_statusbar.h"
#include "gui/frost_guimanager.h"
#include "gui/frost_backdrop.h"
#include "frost_inputmanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str Frame::CLASS_NAME  = "GUI::Frame";
const s_str Layer::CLASS_NAME  = "GUI::Layer";
const s_str Level::CLASS_NAME  = "GUI::Level";
const s_str Strata::CLASS_NAME = "GUI::Strata";

Frame::Frame() : UIObject(), lAbsHitRectInsetList_(0), lRelHitRectInsetList_(0.0f)
{
    mObjectType_ = OJBECT_TYPE_FRAME;
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

    pBackdrop_.Delete();
}

void Frame::Render()
{
    if (IsVisible())
    {
        if (pBackdrop_)
            pBackdrop_->Render();

        // Render child regions
        s_map<LayerType, Layer>::iterator iterLayer;
        foreach (iterLayer, lLayerList_)
        {
            Layer& mLayer = iterLayer->second;
            if (!mLayer.bDisabled)
            {
                map< s_uint, s_ptr<LayeredRegion> >::iterator iterRegion;
                foreach (iterRegion, mLayer.lRegionList)
                {
                    s_ptr<LayeredRegion> pRegion = iterRegion->second;
                    if (pRegion->IsShown())
                        pRegion->Render();
                }
            }
        }

        // Render child frames
        s_map<FrameStrata, Strata>::iterator iterStrata;
        foreach (iterStrata, lStrataList_)
        {
            Strata& mStrata = iterStrata->second;

            map<s_uint, Level>::iterator iterLevel;
            foreach (iterLevel, mStrata.lLevelList)
            {
                Level& mLevel = iterLevel->second;

                map< s_uint, s_ptr<Frame> >::iterator iterFrame;
                foreach (iterFrame, mLevel.lFrameList)
                {
                    s_ptr<Frame> pFrame = iterFrame->second;
                    if ( (pFrame != mLevel.pTopLevel) && (pFrame != mStrata.pTopStrata) )
                    {
                        if (pFrame->IsShown())
                            pFrame->Render();
                    }
                }

                if (mLevel.pTopLevel)
                {
                    if (mLevel.pTopLevel->IsShown())
                        mLevel.pTopLevel->Render();
                }
            }

            if (mStrata.pTopStrata)
            {
                if (mStrata.pTopStrata->IsShown())
                    mStrata.pTopStrata->Render();
            }
        }
    }
}

void Frame::CreateGlue()
{
    s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
    pLua->PushString(sName_);
    lGlueList_.PushBack(
        pLua->Push<LuaFrame>(new LuaFrame(pLua->GetState()))
    );
    pLua->SetGlobal(sName_);
}

s_str Frame::Serialize( const s_str& sTab ) const
{
    s_str sStr = UIObject::Serialize(sTab);

    if (!lRegionList_.IsEmpty())
    {
        if (lChildList_.GetSize() == 1)
            sStr << sTab << "  # Region : \n";
        else
            sStr << sTab << "  # Regions     : " << lRegionList_.GetSize() << "\n";
        sStr << sTab << "  |-###\n";

        map< s_uint, s_ptr<LayeredRegion> >::const_iterator iterRegion;
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

        map< s_uint, s_ptr<Frame> >::const_iterator iterChild;
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
        s_map<s_str, s_bool>::const_iterator iterScript;
        foreach (iterScript, pFrame->lDefinedScriptList_)
        {
            if (iterScript->second)
                this->NotifyScriptDefined("On"+iterScript->first);
        }

        this->SetFrameStrata(pFrame->GetFrameStrata());

        // ?
        //bIsParentStrata_ = pFrame->bIsParentStrata_;

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
            s_ptr<Frame> pNewChild;
            if (pChild->GetObjectType() == "Frame")
                pNewChild = new Frame();
            else if (pChild->GetObjectType() == "Button")
                pNewChild = new Button();
            else if (pChild->GetObjectType() == "EditBox")
                pNewChild = new EditBox();
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
                pNewChild->SetName(pChild->GetName());
                if (!GUIManager::GetSingleton()->AddUIObject(pNewChild))
                {
                    Warning(lType_.Back(),
                        "Trying to add \""+pChild->GetName()+"\" to \""+sName_+"\", but its name was already taken : \""
                        +pNewChild->GetName()+"\". Skipped."
                    );
                    pNewChild.Delete();
                    continue;
                }
                this->AddChild(pNewChild);
                pNewChild->CopyFrom(pChild);
            }
        }

        if (pFrame->pBackdrop_)
        {
            pBackdrop_ = new Backdrop(this);
            pBackdrop_->CopyFrom(pFrame->pBackdrop_);
        }

        s_map< s_uint, s_ptr<LayeredRegion> >::const_iterator iterRegion;
        foreach (iterRegion, pFrame->lRegionList_)
        {
            // TODO : Copier les regions
        }

        bBuildStrataList_ = true;
        bBuildLayerList_ = true;
    }
    else
    {
        Error(lType_.Back(),
            "Trying to derive \""+sName_+"\" from \""+pObj->GetName()+"\", but they are not of the same type "
            "(respectively "+this->GetObjectType()+" and "+pObj->GetObjectType()+")."
        );
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
    bIsKeyboardEnabled_ = bIsKeyboardEnabled;
}

void Frame::EnableMouse( const s_bool& bIsMouseEnabled )
{
    bIsMouseEnabled_ = bIsMouseEnabled;
}

void Frame::EnableMouseWheel( const s_bool& bIsMouseWheelEnabled )
{
    bIsMouseWheelEnabled_ = bIsMouseWheelEnabled;
}

void Frame::FireBuildStrataList_()
{
    bBuildStrataList_ = true;
}

void Frame::FireBuildLayerList_()
{
    bBuildLayerList_ = true;
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
            FireBuildStrataList_();
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
            FireBuildStrataList_();
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
    map<s_uint, s_ptr<Frame> >::iterator iterChild;
    foreach (iterChild, lChildList_)
    {
        lChilds.PushBack(iterChild->second);
    }

    return lChilds;
}

s_float Frame::GetEffectiveAlpha() const
{
    if (pParent_ != NULL)
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
    if (pParent_ != NULL)
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

s_ptr<Backdrop> Frame::GetBackdrop() const
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

const s_bool& Frame::IsClampedToScreen() const
{
    return bIsClampedToScreen_;
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

void Frame::NotifyScriptDefined( const s_str& sScriptName )
{
    s_str sCutScriptName = sScriptName;
    sCutScriptName.EraseFromStart(2);
    lDefinedScriptList_[sCutScriptName] = true;
}

void Frame::OnEvent( const Event& mEvent )
{
    s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();

    // Lua handlers do not need direct arguments.
    // Instead, we set the value of some global variables
    // (event : event name, arg1, arg2, ... arg9 : arguments)
    // that the user can use however he wants in his handler.

    // Set event name
    pLua->PushString(mEvent.GetName());
    pLua->SetGlobal("event");

    // Set arguments
    for (s_uint i; i < mEvent.GetNumParam(); ++i)
    {
        s_ptr<const s_var> pArg = mEvent.Get(i);
        pLua->PushVar(*pArg);
        pLua->SetGlobal("arg"+(i+1));
    }

    pLua->CallFunction(sName_+":OnEvent");
}

void Frame::On( const s_str& sScriptName, s_ptr<Event> pEvent )
{
    if (lDefinedScriptList_.Find(sScriptName))
    {
        s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();

        pLua->PushGlobal(sName_);
        pLua->SetGlobal("this");

        if ((sScriptName == "KeyDown") ||
            (sScriptName == "KeyUp"))
        {
            // Set key name
            if (pEvent != NULL)
            {
                pLua->PushString(pEvent->Get(0)->Get<s_str>());
                pLua->SetGlobal("arg1");
            }
        }
        else if (sScriptName == "MouseDown")
        {
            // Set mouse button
            s_str sMouseState;
            MouseState mState = InputManager::GetSingleton()->GetMouseState(MOUSE_MIDDLE);
            if ( (mState == MOUSE_CLICKED) || (mState == MOUSE_LONG) )
                sMouseState = "MiddleButton";
            mState = InputManager::GetSingleton()->GetMouseState(MOUSE_RIGHT);
            if ( (mState == MOUSE_CLICKED) || (mState == MOUSE_LONG) )
                sMouseState = "RightButton";
            mState = InputManager::GetSingleton()->GetMouseState(MOUSE_LEFT);
            if ( (mState == MOUSE_CLICKED) || (mState == MOUSE_LONG) )
                sMouseState = "LeftButton";

            pLua->PushString(sMouseState);
            pLua->SetGlobal("arg1");
        }
        else if (sScriptName == "MouseUp")
        {
            // Set mouse button
            s_str sMouseState;
            MouseState mState = InputManager::GetSingleton()->GetMouseState(MOUSE_MIDDLE);
            if (mState == MOUSE_UP)
                sMouseState = "MiddleButton";
            mState = InputManager::GetSingleton()->GetMouseState(MOUSE_RIGHT);
            if (mState == MOUSE_UP)
                sMouseState = "RightButton";
            mState = InputManager::GetSingleton()->GetMouseState(MOUSE_LEFT);
            if (mState == MOUSE_UP)
                sMouseState = "LeftButton";

            pLua->PushString(sMouseState);
            pLua->SetGlobal("arg1");
        }
        else if (sScriptName == "Update")
        {
            // Set delta time
            pLua->PushNumber(TimeManager::GetSingleton()->GetDelta());
            pLua->SetGlobal("arg1");
        }

        pLua->CallFunction(sName_+":On"+sScriptName);
    }

    if (sScriptName == "Load")
    {
        // OnLoad must be called from parent to childs, because childs often
        // rely on their parent's state
        map<s_uint, s_ptr<Frame> >::iterator iterChild;
        foreach (iterChild, lChildList_)
        {
            iterChild->second->On("Load");
        }
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
    if ( (pParent_ != NULL) && (mStrata_ != mStrata) )
    {
        pParentFrame_->FireBuildStrataList_();
    }

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
        if (pParentFrame_)
            mStrata = pParentFrame_->GetFrameStrata();
        else
            mStrata = STRATA_MEDIUM;
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

void Frame::SetBackdrop( s_ptr<Backdrop> pBackdrop )
{
    pBackdrop_.Delete();
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
        pParentFrame_ = (Frame*)pParent.Get();

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
    if ( (pParent_ != NULL) && (bIsTopLevel_ != bIsTopLevel) )
    {
        pParentFrame_->FireBuildStrataList_();
        if (bIsTopLevel)
        {
            s_ptr<Frame> pTopLevel = pParentFrame_->lStrataList_[mStrata_].lLevelList[uiLevel_].pTopLevel;
            if (pTopLevel != NULL)
                pTopLevel->SetTopLevel(false);
        }
    }

    bIsTopLevel_ = bIsTopLevel;
}

void Frame::SetTopStrata( const s_bool& bIsTopStrata )
{
    if ( (pParent_ != NULL) && (bIsTopStrata_ != bIsTopStrata) )
    {
        pParentFrame_->FireBuildStrataList_();
        if (bIsTopStrata)
        {
            s_ptr<Frame> pTopStrata = pParentFrame_->lStrataList_[mStrata_].pTopStrata;
            if (pTopStrata != NULL)
                pTopStrata->SetTopStrata(false);
        }
    }

    bIsTopStrata_ = bIsTopStrata;
}

void Frame::SetUserPlaced( const s_bool& bIsUserPlaced )
{
    bIsUserPlaced_ = bIsUserPlaced;
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

void Frame::Update()
{
    UIObject::Update();

    if (bBuildStrataList_)
    {
        lStrataList_.Clear();

        // Build the strata map
        s_map< s_uint, s_ptr<Frame> >::iterator iterChild;
        foreach (iterChild, lChildList_)
        {
            s_ptr<Frame> pChild = iterChild->second;
            s_ptr<Strata> pStrata = &lStrataList_[pChild->GetFrameStrata()];
            s_ptr<Level> pLevel = &pStrata->lLevelList[pChild->GetFrameLevel()];
            pLevel->lFrameList[pChild->GetID()] = pChild;

            if (pChild->IsTopStrata())
                pStrata->pTopStrata = pChild;

            if (pChild->IsTopLevel())
                pLevel->pTopLevel = pChild;
        }

        bBuildStrataList_ = false;
    }

    if (bBuildLayerList_)
    {
        // Clear layers' content
        s_map< LayerType, Layer >::iterator iterLayer;
        foreach (iterLayer, lLayerList_)
        {
            iterLayer->second.lRegionList.clear();
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
