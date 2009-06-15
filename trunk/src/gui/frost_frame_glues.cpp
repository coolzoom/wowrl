/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_frame.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaFrame::CLASS_NAME  = "GUI::LuaFrame";

LuaFrame::LuaFrame(lua_State* pLua) : LuaUIObject(pLua)
{
    pFrameParent_ = (Frame*)(pParent_.Get());
}

int LuaFrame::_DisableDrawLayer(lua_State* pLua)
{
    Lua::Function mFunc("Frame:DisableDrawLayer", pLua);
    mFunc.Add(0, "layer", Lua::TYPE_STRING, VALUE_STRING);
    if (mFunc.Check())
    {
        s_str sLayer = mFunc.Get(0)->GetS();
        if (sLayer == "BACKGROUND")
        {
            pFrameParent_->DisableDrawLayer(LAYER_BACKGROUND);
        }
        else if (sLayer == "BORDER")
        {
            pFrameParent_->DisableDrawLayer(LAYER_BORDER);
        }
        else if (sLayer == "ARTWORK")
        {
            pFrameParent_->DisableDrawLayer(LAYER_ARTWORK);
        }
        else if (sLayer == "OVERLAY")
        {
            pFrameParent_->DisableDrawLayer(LAYER_OVERLAY);
        }
        else if (sLayer == "HIGHLIGHT")
        {
            pFrameParent_->DisableDrawLayer(LAYER_HIGHLIGHT);
        }
    }

    return mFunc.Return();
}

int LuaFrame::_EnableDrawLayer(lua_State* pLua)
{
    Lua::Function mFunc("Frame:EnableDrawLayer", pLua);
    mFunc.Add(0, "layer", Lua::TYPE_STRING, VALUE_STRING);
    if (mFunc.Check())
    {
        s_str sLayer = mFunc.Get(0)->GetS();
        if (sLayer == "BACKGROUND")
        {
            pFrameParent_->EnableDrawLayer(LAYER_BACKGROUND);
        }
        else if (sLayer == "BORDER")
        {
            pFrameParent_->EnableDrawLayer(LAYER_BORDER);
        }
        else if (sLayer == "ARTWORK")
        {
            pFrameParent_->EnableDrawLayer(LAYER_ARTWORK);
        }
        else if (sLayer == "OVERLAY")
        {
            pFrameParent_->EnableDrawLayer(LAYER_OVERLAY);
        }
        else if (sLayer == "HIGHLIGHT")
        {
            pFrameParent_->EnableDrawLayer(LAYER_HIGHLIGHT);
        }
    }

    return mFunc.Return();
}

int LuaFrame::_EnableKeyboard(lua_State* pLua)
{
    Lua::Function mFunc("Frame:EnableKeyboard", pLua);
    mFunc.Add(0, "is keyboard enable", Lua::TYPE_BOOLEAN, VALUE_BOOL);
    if (mFunc.Check())
    {
        pFrameParent_->EnableKeyboard(mFunc.Get(0)->GetB());
    }

    return mFunc.Return();
}

int LuaFrame::_EnableMouse(lua_State* pLua)
{
    Lua::Function mFunc("Frame:EnableMouse", pLua);
    mFunc.Add(0, "is mouse enabled", Lua::TYPE_BOOLEAN, VALUE_BOOL);
    if (mFunc.Check())
    {
        pFrameParent_->EnableMouse(mFunc.Get(0)->GetB());
    }

    return mFunc.Return();
}

int LuaFrame::_EnableMouseWheel(lua_State* pLua)
{
    Lua::Function mFunc("Frame:EnableMouseWheel", pLua);
    mFunc.Add(0, "is mouse wheel enabled", Lua::TYPE_BOOLEAN, VALUE_BOOL);
    if (mFunc.Check())
    {
        pFrameParent_->EnableMouseWheel(mFunc.Get(0)->GetB());
    }

    return mFunc.Return();
}

int LuaFrame::_GetChildren(lua_State* pLua)
{
    s_ctnr< s_ptr<Frame> > lChildList = pFrameParent_->GetChildren();
    Lua::Function mFunc("Frame:GetChildren", pLua, lChildList.GetSize());

    s_ctnr< s_ptr<Frame> >::iterator iterChild;
    foreach (iterChild, lChildList)
    {
        Lunar<LuaUIObject>::push(pLua, (*iterChild)->GetGlue().Get());
        mFunc.Push(Lua::ReturnValue(Lua::RETURN_OBJECT));
    }

    return mFunc.Return();
}

int LuaFrame::_GetEffectiveAlpha(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetEffectiveAlpha", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pFrameParent_->GetEffectiveAlpha()));

    return mFunc.Return();
}

int LuaFrame::_GetEffectiveScale(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetEffectiveScale", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pFrameParent_->GetEffectiveScale()));

    return mFunc.Return();
}

int LuaFrame::_GetFrameLevel(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetFrameLevel", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pFrameParent_->GetFrameLevel()));

    return mFunc.Return();
}

int LuaFrame::_GetFrameStrata(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetFrameStrata", pLua, 1);

    FrameStrata mStrata = pFrameParent_->GetFrameStrata();
    s_str sStrata;

    if (mStrata == STRATA_BACKGROUND)
        sStrata = "BACKGROUND";
    else if (mStrata == STRATA_LOW)
        sStrata = "LOW";
    else if (mStrata == STRATA_MEDIUM)
        sStrata = "MEDIUM";
    else if (mStrata == STRATA_HIGH)
        sStrata = "HIGH";
    else if (mStrata == STRATA_DIALOG)
        sStrata = "DIALOG";
    else if (mStrata == STRATA_FULLSCREEN)
        sStrata = "FULLSCREEN";
    else if (mStrata == STRATA_FULLSCREEN_DIALOG)
        sStrata = "FULLSCREEN_DIALOG";
    else if (mStrata == STRATA_TOOLTIP)
        sStrata = "TOOLTIP";

    mFunc.Push(Lua::ReturnValue(sStrata));

    return mFunc.Return();
}

int LuaFrame::_GetFrameType(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetFrameType", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pFrameParent_->GetFrameType()));

    return mFunc.Return();
}

int LuaFrame::_GetHitRectInsets(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetHitRectInsets", pLua, 4);

    s_array<s_int,4> lInsets = pFrameParent_->GetAbsHitRectInsets();

    for (s_uint i; i<4; i++)
        mFunc.Push(Lua::ReturnValue(lInsets[i]));

    return mFunc.Return();
}

int LuaFrame::_GetID(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetID", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pFrameParent_->GetID()));

    return mFunc.Return();
}

int LuaFrame::_GetMaxResize(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetMaxResize", pLua, 2);

    s_array<s_uint,2> lMax = pFrameParent_->GetMaxResize();

    mFunc.Push(Lua::ReturnValue(lMax[0]));
    mFunc.Push(Lua::ReturnValue(lMax[1]));

    return mFunc.Return();
}

int LuaFrame::_GetMinResize(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetMinResize", pLua, 2);

    s_array<s_uint,2> lMin = pFrameParent_->GetMinResize();

    mFunc.Push(Lua::ReturnValue(lMin[0]));
    mFunc.Push(Lua::ReturnValue(lMin[1]));

    return mFunc.Return();
}

int LuaFrame::_GetNumChildren(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetNumChildren", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pFrameParent_->GetNumChildren()));

    return mFunc.Return();
}

int LuaFrame::_GetNumRegions(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetNumRegions", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pFrameParent_->GetNumRegions()));

    return mFunc.Return();
}

int LuaFrame::_GetScale(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetScale", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pFrameParent_->GetScale()));

    return mFunc.Return();
}

int LuaFrame::_GetScript(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetScript", pLua, 1);
    mFunc.Add(0, "script name", Lua::TYPE_STRING, VALUE_STRING);
    if (mFunc.Check())
    {
        s_str sScriptName = mFunc.Get(0)->GetS();
        if (pFrameParent_->HasScript(sScriptName))
        {
            lua_getglobal(pLua, (pFrameParent_->GetName() + ":" + sScriptName).c_str());
            mFunc.Push(Lua::ReturnValue(Lua::RETURN_OBJECT));
        }
    }

    return mFunc.Return();
}

int LuaFrame::_HasScript(lua_State* pLua)
{
    Lua::Function mFunc("Frame:HasScript", pLua, 1);
    mFunc.Add(0, "script name", Lua::TYPE_STRING, VALUE_STRING);
    if (mFunc.Check())
    {
        mFunc.Push(Lua::ReturnValue(pFrameParent_->CanUseScript(mFunc.Get(0)->GetS())));
    }

    return mFunc.Return();
}

int LuaFrame::_IsClampedToScreen(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsClampedToScreen", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pFrameParent_->IsClampedToScreen()));

    return mFunc.Return();
}

int LuaFrame::_IsFrameType(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsFrameType", pLua, 1);
    mFunc.Add(0, "frame type", Lua::TYPE_STRING, VALUE_STRING);
    if (mFunc.Check())
    {
        if (pFrameParent_->GetFrameType() == mFunc.Get(0)->GetS())
            mFunc.Push(Lua::ReturnValue(s_bool(true)));
        else
            mFunc.Push(Lua::ReturnValue(s_bool(false)));
    }

    return mFunc.Return();
}

int LuaFrame::_IsKeyboardEnabled(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsKeyboardEnabled", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pFrameParent_->IsKeyboardEnabled()));

    return mFunc.Return();
}

int LuaFrame::_IsMouseEnabled(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsMouseEnabled", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pFrameParent_->IsMouseEnabled()));

    return mFunc.Return();
}

int LuaFrame::_IsMouseWheelEnabled(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsMouseWheelEnabled", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pFrameParent_->IsMouseWheelEnabled()));

    return mFunc.Return();
}

int LuaFrame::_IsMovable(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsMovable", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pFrameParent_->IsMovable()));

    return mFunc.Return();
}

int LuaFrame::_IsResizable(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsResizable", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pFrameParent_->IsResizable()));

    return mFunc.Return();
}

int LuaFrame::_IsTopLevel(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsTopLevel", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pFrameParent_->IsTopLevel()));

    return mFunc.Return();
}

int LuaFrame::_IsUserPlaced(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsUserPlaced", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pFrameParent_->IsUserPlaced()));

    return mFunc.Return();
}

int LuaFrame::_RegisterAllEvents(lua_State* pLua)
{
    Lua::Function mFunc("Frame:RegisterAllEvents", pLua);

    pFrameParent_->RegisterAllEvents();

    return mFunc.Return();
}

int LuaFrame::_RegisterEvent(lua_State* pLua)
{
    Lua::Function mFunc("Frame:RegisterEvent", pLua);
    mFunc.Add(0, "event name", Lua::TYPE_STRING, VALUE_STRING);
    if (mFunc.Check())
    {
        pFrameParent_->RegisterEvent(mFunc.Get(0)->GetS());
    }

    return mFunc.Return();
}

int LuaFrame::_RegisterForDrag(lua_State* pLua)
{
    Lua::Function mFunc("Frame:RegisterForDrag", pLua);
    mFunc.Add(0, "button 1", Lua::TYPE_STRING, VALUE_STRING, true);
    mFunc.Add(1, "button 2", Lua::TYPE_STRING, VALUE_STRING, true);
    mFunc.Add(2, "button 3", Lua::TYPE_STRING, VALUE_STRING, true);
    if (mFunc.Check())
    {
        s_ctnr<s_str> lButtonList;
        for (s_uint i; i<3; i++)
        {
            if (mFunc.IsProvided(i))
                lButtonList.PushBack(mFunc.Get(i)->GetS());
            else
                break;
        }
        pFrameParent_->RegisterForDrag(lButtonList);
    }

    return mFunc.Return();
}

int LuaFrame::_SetClampedToScreen(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetClampedToScreen", pLua);
    mFunc.Add(0, "is clamped to screen", Lua::TYPE_BOOLEAN, VALUE_BOOL);
    if (mFunc.Check())
    {
        pFrameParent_->SetClampedToScreen(mFunc.Get(0)->GetB());
    }

    return mFunc.Return();
}

int LuaFrame::_SetFrameStrata(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetFrameStrata", pLua);
    mFunc.Add(0, "strata", Lua::TYPE_STRING, VALUE_STRING);
    if (mFunc.Check())
    {
        pFrameParent_->SetFrameStrata(mFunc.Get(0)->GetS());
    }

    return mFunc.Return();
}

int LuaFrame::_SetHitRectInsets(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetHitRectInsets", pLua);
    mFunc.Add(0, "left", Lua::TYPE_NUMBER, VALUE_INT);
    mFunc.Add(1, "right", Lua::TYPE_NUMBER, VALUE_INT);
    mFunc.Add(2, "top", Lua::TYPE_NUMBER, VALUE_INT);
    mFunc.Add(3, "bottom", Lua::TYPE_NUMBER, VALUE_INT);
    if (mFunc.Check())
    {
        pFrameParent_->SetAbsHitRectInsets(
            mFunc.Get(0)->GetI(),
            mFunc.Get(1)->GetI(),
            mFunc.Get(2)->GetI(),
            mFunc.Get(3)->GetI()
        );
    }

    return mFunc.Return();
}

int LuaFrame::_SetMaxResize(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetMaxResize", pLua);
    mFunc.Add(0, "width", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(1, "height", Lua::TYPE_NUMBER, VALUE_UINT);
    if (mFunc.Check())
    {
        pFrameParent_->SetMaxResize(mFunc.Get(0)->GetUI(), mFunc.Get(1)->GetUI());
    }

    return mFunc.Return();
}

int LuaFrame::_SetMinResize(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetMinResize", pLua);
    mFunc.Add(0, "width", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(1, "height", Lua::TYPE_NUMBER, VALUE_UINT);
    if (mFunc.Check())
    {
        pFrameParent_->SetMinResize(mFunc.Get(0)->GetUI(), mFunc.Get(1)->GetUI());
    }

    return mFunc.Return();
}

int LuaFrame::_SetMovable(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetMovable", pLua);
    mFunc.Add(0, "is movable", Lua::TYPE_BOOLEAN, VALUE_BOOL);
    if (mFunc.Check())
    {
        pFrameParent_->SetMovable(mFunc.Get(0)->GetB());
    }

    return mFunc.Return();
}

int LuaFrame::_SetResizable(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetResizable", pLua);
    mFunc.Add(0, "is resizable", Lua::TYPE_BOOLEAN, VALUE_BOOL);
    if (mFunc.Check())
    {
        pFrameParent_->SetResizable(mFunc.Get(0)->GetB());
    }

    return mFunc.Return();
}

int LuaFrame::_SetScale(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetScale", pLua);
    mFunc.Add(0, "scale", Lua::TYPE_NUMBER, VALUE_FLOAT);
    if (mFunc.Check())
    {
        pFrameParent_->SetScale(mFunc.Get(0)->GetF());
    }

    return mFunc.Return();
}

int LuaFrame::_SetScript(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetScript", pLua);
    mFunc.Add(0, "script name", Lua::TYPE_STRING, VALUE_STRING);
    mFunc.Add(1, "function", Lua::TYPE_FUNCTION, VALUE_NONE);
    mFunc.Add(1, "nil", Lua::TYPE_NIL, VALUE_NONE);
    if (mFunc.Check())
    {
        s_str sScriptName = mFunc.Get(0)->GetS();
        if (pFrameParent_->CanUseScript(sScriptName))
        {
            s_ptr<Lua::Argument> pSecond = mFunc.Get(1);
            if (pSecond->GetType() == Lua::TYPE_FUNCTION)
            {
                lua_pushvalue(pLua, pSecond->GetI().Get());
                lua_setglobal(pLua, (pFrameParent_->GetName() + ":" + sScriptName).c_str());
                pFrameParent_->NotifyScriptDefined(sScriptName);
            }
            else
            {
                lua_pushnil(pLua);
                lua_setglobal(pLua, (pFrameParent_->GetName() + ":" + sScriptName).c_str());
                pFrameParent_->NotifyScriptDefined(sScriptName);
            }
        }
        else
            Error(pFrameParent_->GetFrameType(), "Can't use script \""+sScriptName+"\".");
    }

    return mFunc.Return();
}

int LuaFrame::_SetTopLevel(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetTopLevel", pLua);
    mFunc.Add(0, "is top level", Lua::TYPE_BOOLEAN, VALUE_BOOL);
    if (mFunc.Check())
    {
        pFrameParent_->SetTopLevel(mFunc.Get(0)->GetB());
    }

    return mFunc.Return();
}

int LuaFrame::_SetUserPlaced(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetUserPlaced", pLua);
    mFunc.Add(0, "is user placed", Lua::TYPE_BOOLEAN, VALUE_BOOL);
    if (mFunc.Check())
    {
        pFrameParent_->SetUserPlaced(mFunc.Get(0)->GetB());
    }

    return mFunc.Return();
}

int LuaFrame::_UnregisterAllEvents(lua_State* pLua)
{
    Lua::Function mFunc("Frame:UnregisterAllEvents", pLua);

    pFrameParent_->UnregisterAllEvents();

    return mFunc.Return();
}

int LuaFrame::_UnregisterEvent(lua_State* pLua)
{
    Lua::Function mFunc("Frame:UnregisterEvent", pLua);
    mFunc.Add(0, "event name", Lua::TYPE_STRING, VALUE_STRING);
    if (mFunc.Check())
    {
        pFrameParent_->UnregisterEvent(mFunc.Get(0)->GetS());
    }

    return mFunc.Return();
}

