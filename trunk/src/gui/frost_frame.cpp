/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_frame.h"

#include "gui/frost_gui_layeredregion.h"
#include "gui/frost_gui_uiobject.h"
#include "frost_guimanager.h"
#include "frost_inputmanager.h"
#include "frost_timemanager.h"
#include "frost_event.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str Frame::CLASS_NAME = "GUI::Frame";
const s_str Layer::CLASS_NAME = "GUI::Layer";
const s_str Level::CLASS_NAME = "GUI::Level";
const s_str Strata::CLASS_NAME = "GUI::Strata";

Frame::Frame(lua_State* luaVM) : UIObject(luaVM), lHitRectInsetList_(0)
{
    mType_ = OJBECT_TYPE_FRAME;
    lType_.push_back("Frame");

    mStrata_ = STRATA_MEDIUM;

    uiMinWidth_ = 0u;
    uiMaxWidth_ = s_uint(s_uint::INTEGER_INF);
    uiMinHeight_ = 0u;
    uiMaxHeight_ = s_uint(s_uint::INTEGER_INF);

    fScale_ = 1.0f;

    uiLevel_ = 0u;
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
    return MAPFIND(sScriptName, lDefinedScriptList_);
}

vector< s_ptr<Frame> > Frame::GetChildren()
{
    vector< s_ptr<Frame> > lChilds;
    map<s_uint, s_ptr<Frame> >::iterator iterChild;
    foreach (iterChild, lChildList_)
    {
        lChilds.push_back(iterChild->second);
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

const s_str& Frame::GetFrameType() const
{
    return lType_.back();
}

s_array<s_int,4> Frame::GetHitRectInsets() const
{
    return lHitRectInsetList_;
}

s_array<s_uint,2> Frame::GetMaxResize() const
{
    return s_array<s_uint,2>((uiMaxWidth_, uiMaxHeight_));
}

s_array<s_uint,2> Frame::GetMinResize() const
{
    return s_array<s_uint,2>((uiMinWidth_, uiMinHeight_));
}

s_uint Frame::GetNumChildren() const
{
    return lChildList_.size();
}

s_uint Frame::GetNumRegions() const
{
    return lRegionList_.size();
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
    lDefinedScriptList_[sScriptName] = true;
}

void Frame::OnEvent( const Event& mEvent )
{
    // Set event name
    lua_pushstring(pLua_, mEvent.GetName().c_str());
    lua_setglobal(pLua_, "event");

    // Set arguments
    for (s_uint i; i < mEvent.GetNumParam(); i++)
    {
        s_ptr<const s_var> pArg = mEvent.Get(i);
        if ((pArg->GetType() == VALUE_INT) ||
            (pArg->GetType() == VALUE_UINT) ||
            (pArg->GetType() == VALUE_FLOAT) ||
            (pArg->GetType() == VALUE_DOUBLE))
            lua_pushnumber(pLua_, pArg->GetF().Get());
        else if (pArg->GetType() == VALUE_STRING)
            lua_pushstring(pLua_, pArg->GetS().c_str());
        else if (pArg->GetType() == VALUE_BOOL)
            lua_pushboolean(pLua_, pArg->GetB().Get());
        else
            break;

        lua_setglobal(pLua_, ("arg" + s_str(i)).c_str());
        i++;
    }

    lua_getglobal(pLua_, (sName_ + ":OnEvent").c_str());
    if (lua_isfunction(pLua_, -1))
    {
        int iError = lua_pcall(pLua_, 0, 0, 0);
        if (iError) l_ThrowError(pLua_);
    }
    lua_pop(pLua_, 1);
}

void Frame::On( const s_str& sScriptName, s_ptr<Event> pEvent )
{
    if (MAPFIND(sScriptName, lDefinedScriptList_))
	{
        lua_getglobal(pLua_, sName_.c_str());
        lua_setglobal(pLua_, "this");

        if ((sScriptName == "KeyDown") ||
            (sScriptName == "KeyUp") )
        {
            // Set key name
            if (pEvent != NULL)
            {
                lua_pushstring(pLua_, pEvent->Get(0)->GetS().c_str());
                lua_setglobal(pLua_, "arg1");
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

            lua_pushstring(pLua_, sMouseState.c_str());
            lua_setglobal(pLua_, "arg1");
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

            lua_pushstring(pLua_, sMouseState.c_str());
            lua_setglobal(pLua_, "arg1");
        }
        else if (sScriptName == "Update")
        {
            // Set delta time
            lua_pushnumber(pLua_, TimeManager::GetSingleton()->GetDelta().Get());
            lua_setglobal(pLua_, "arg1");
        }

        lua_getglobal(pLua_, (sName_ + ":On" + sScriptName).c_str());
        if (lua_isfunction(pLua_, -1))
        {
            int iError = lua_pcall(pLua_, 0, 0, 0);
            if (iError) l_ThrowError(pLua_);
        }

        lua_pop(pLua_, 1);
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
    lRegEventList_.clear();
}

void Frame::RegisterEvent( const s_str& sEvent )
{
    if (!MAPFIND(sEvent, lRegEventList_))
    {
        lRegEventList_[sEvent] = true;
    }
}

void Frame::RegisterForDrag( const vector<s_str>& lButtonList )
{
    lRegDragList_.clear();
    vector<s_str>::const_iterator iterButton;
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

void Frame::SetHitRectInsets( const s_int& iLeft, const s_int& iRight, const s_int& iTop, const s_int& iBottom )
{
    lHitRectInsetList_ = s_array<s_int,4>((iLeft, iRight, iTop, iBottom));
}

void Frame::SetLevel( const s_uint& uiLevel )
{
    if (uiLevel_.IsNull())
    {
        uiLevel_ = uiLevel;
    }
    else
    {
        Warning(lType_.back(), "SetLevel can't be called more than once.");
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
        pParent_ = pParent;
        pParentFrame_ = (Frame*)pParent.Get();
    }
    else
    {
        Error(lType_.back(), "Can't call SetParent(this)");
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
    lRegEventList_.clear();
}

void Frame::UnregisterEvent( const s_str& sEvent )
{
    lRegEventList_[sEvent] = false;
}

void Frame::Update()
{
    ((UIObject*)this)->Update();

    if (bBuildStrataList_)
    {
        lStrataList_.clear();

        map< s_uint, s_ptr<Frame> >::iterator iterChild;
        foreach(iterChild, lChildList_)
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
        lLayerList_.clear();

        map< s_uint, s_ptr<LayeredRegion> >::iterator iterRegion;
        foreach(iterRegion, lRegionList_)
        {
            s_ptr<LayeredRegion> pRegion = iterRegion->second;
            lLayerList_[pRegion->GetDrawLayer()].lRegionList[pRegion->GetID()] = pRegion;
        }

        bBuildLayerList_ = false;
    }
}

void Frame::UpdateMaterial( const s_bool& bForceUpdate )
{
    ((UIObject*)this)->UpdateMaterial(bForceUpdate);
}
