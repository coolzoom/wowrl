/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_frame.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

int Frame::_DisableDrawLayer(lua_State* pLua)
{
    Lua::Function mFunc("Frame:DisableDrawLayer", pLua);
    mFunc.Add(0, "layer", LUA_TSTRING, VALUE_STRING);
    if (mFunc.Check())
    {
        s_str sLayer = mFunc.Get(0)->GetS();
        if (sLayer == "BACKGROUND")
        {
            DisableDrawLayer(LAYER_BACKGROUND);
        }
        else if (sLayer == "BORDER")
        {
            DisableDrawLayer(LAYER_BORDER);
        }
        else if (sLayer == "ARTWORK")
        {
            DisableDrawLayer(LAYER_ARTWORK);
        }
        else if (sLayer == "OVERLAY")
        {
            DisableDrawLayer(LAYER_OVERLAY);
        }
        else if (sLayer == "HIGHLIGHT")
        {
            DisableDrawLayer(LAYER_HIGHLIGHT);
        }
    }

    return mFunc.Return();
}

int Frame::_EnableDrawLayer(lua_State* pLua)
{
    Lua::Function mFunc("Frame:EnableDrawLayer", pLua);
    mFunc.Add(0, "layer", LUA_TSTRING, VALUE_STRING);
    if (mFunc.Check())
    {
        s_str sLayer = mFunc.Get(0)->GetS();
        if (sLayer == "BACKGROUND")
        {
            EnableDrawLayer(LAYER_BACKGROUND);
        }
        else if (sLayer == "BORDER")
        {
            EnableDrawLayer(LAYER_BORDER);
        }
        else if (sLayer == "ARTWORK")
        {
            EnableDrawLayer(LAYER_ARTWORK);
        }
        else if (sLayer == "OVERLAY")
        {
            EnableDrawLayer(LAYER_OVERLAY);
        }
        else if (sLayer == "HIGHLIGHT")
        {
            EnableDrawLayer(LAYER_HIGHLIGHT);
        }
    }

    return mFunc.Return();
}

int Frame::_EnableKeyboard(lua_State* pLua)
{
    Lua::Function mFunc("Frame:EnableKeyboard", pLua);
    mFunc.Add(0, "enableKeyboard", LUA_TBOOLEAN, VALUE_BOOL);
    if (mFunc.Check())
    {
        EnableKeyboard(mFunc.Get(0)->GetB());
    }

    return mFunc.Return();
}

int Frame::_EnableMouse(lua_State* pLua)
{
    Lua::Function mFunc("Frame:EnableMouse", pLua);
    mFunc.Add(0, "enableMouse", LUA_TBOOLEAN, VALUE_BOOL);
    if (mFunc.Check())
    {
        EnableMouse(mFunc.Get(0)->GetB());
    }

    return mFunc.Return();
}

int Frame::_EnableMouseWheel(lua_State* pLua)
{
    Lua::Function mFunc("Frame:EnableMouseWheel", pLua);
    mFunc.Add(0, "enableMouseWheel", LUA_TBOOLEAN, VALUE_BOOL);
    if (mFunc.Check())
    {
        EnableMouseWheel(mFunc.Get(0)->GetB());
    }

    return mFunc.Return();
}

int Frame::_GetChildren(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetChildren", pLua, lChildList_.size());

    map< s_uint, s_ptr<Frame> >::iterator iterChild;
    foreach (iterChild, lChildList_)
    {
        Lunar<UIObject>::push(pLua, iterChild->second.Get());
        mFunc.Push(Lua::ReturnValue(Lua::RETURN_OBJECT));
    }

    return mFunc.Return();
}

int Frame::_GetEffectiveAlpha(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetEffectiveAlpha", pLua, 1);

    mFunc.Push(Lua::ReturnValue(GetEffectiveAlpha()));

    return mFunc.Return();
}

int Frame::_GetEffectiveScale(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetEffectiveScale", pLua, 1);

    mFunc.Push(Lua::ReturnValue(GetEffectiveScale()));

    return mFunc.Return();
}

int Frame::_GetFrameLevel(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetFrameLevel", pLua, 1);

    mFunc.Push(Lua::ReturnValue(GetFrameLevel()));

    return mFunc.Return();
}

int Frame::_GetFrameStrata(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetFrameStrata", pLua, 1);

    FrameStrata mStrata = GetFrameStrata();
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

int Frame::_GetFrameType(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetFrameType", pLua, 1);

    mFunc.Push(Lua::ReturnValue(GetFrameType()));

    return mFunc.Return();
}

int Frame::_GetHitRectInsets(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetHitRectInsets", pLua, 4);

    s_array<s_int,4> lInsets = GetHitRectInsets();

    for (s_uint i; i<4; i++)
        mFunc.Push(Lua::ReturnValue(lInsets[i]));

    return mFunc.Return();
}

int Frame::_GetID(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetID", pLua, 1);

    mFunc.Push(Lua::ReturnValue(GetID()));

    return mFunc.Return();
}

int Frame::_GetMaxResize(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetMaxResize", pLua, 2);

    s_array<s_uint,2> lMax = GetMaxResize();

    mFunc.Push(Lua::ReturnValue(lMax[0]));
    mFunc.Push(Lua::ReturnValue(lMax[1]));

    return mFunc.Return();
}

int Frame::_GetMinResize(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetMinResize", pLua, 2);

    s_array<s_uint,2> lMin = GetMinResize();

    mFunc.Push(Lua::ReturnValue(lMin[0]));
    mFunc.Push(Lua::ReturnValue(lMin[1]));

    return mFunc.Return();
}

int Frame::_GetNumChildren(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetNumChildren", pLua, 1);

    mFunc.Push(Lua::ReturnValue(GetNumChildren()));

    return mFunc.Return();
}

int Frame::_GetNumRegions(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetNumRegions", pLua, 1);

    mFunc.Push(Lua::ReturnValue(GetNumRegions()));

    return mFunc.Return();
}

int Frame::_GetScale(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetScale", pLua, 1);

    mFunc.Push(Lua::ReturnValue(GetScale()));

    return mFunc.Return();
}

int Frame::_GetScript(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetScript", pLua, 1);
    mFunc.Add(0, "scriptName", LUA_TSTRING, VALUE_STRING);
    if (mFunc.Check())
    {
        s_str sScriptName = mFunc.Get(0)->GetS();
        if (HasScript(sScriptName))
        {
            lua_getglobal(pLua, (sName_ + ":" + sScriptName).c_str());
            mFunc.Push(Lua::ReturnValue(Lua::RETURN_OBJECT));
        }
    }

    return mFunc.Return();
}

int Frame::_HasScript(lua_State* pLua)
{
    Lua::Function mFunc("Frame:HasScript", pLua, 1);
    mFunc.Add(0, "scriptName", LUA_TSTRING, VALUE_STRING);
    if (mFunc.Check())
    {
        mFunc.Push(Lua::ReturnValue(CanUseScript(mFunc.Get(0)->GetS())));
    }

    return mFunc.Return();
}

int Frame::_IsClampedToScreen(lua_State* pLua)
{
    Lua::Function mFunc("Frame:Func", pLua, 1);

    mFunc.Push(Lua::ReturnValue(IsClampedToScreen()));

    return mFunc.Return();
}

int Frame::_IsFrameType(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsFrameType", pLua, 1);
    mFunc.Add(0, "frameType", LUA_TSTRING, VALUE_STRING);
    if (mFunc.Check())
    {
        if (GetFrameType() == mFunc.Get(0)->GetS())
            mFunc.Push(Lua::ReturnValue(s_bool(true)));
        else
            mFunc.Push(Lua::ReturnValue(s_bool(false)));
    }

    return mFunc.Return();
}

int Frame::_IsKeyboardEnabled(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsKeyboardEnabled", pLua, 1);

    mFunc.Push(Lua::ReturnValue(IsKeyboardEnabled()));

    return mFunc.Return();
}

int Frame::_IsMouseEnabled(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsMouseEnabled", pLua, 1);

    mFunc.Push(Lua::ReturnValue(IsMouseEnabled()));

    return mFunc.Return();
}

int Frame::_IsMouseWheelEnabled(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsMouseWheelEnabled", pLua, 1);

    mFunc.Push(Lua::ReturnValue(IsMouseWheelEnabled()));

    return mFunc.Return();
}

int Frame::_IsMovable(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsMovable", pLua, 1);

    mFunc.Push(Lua::ReturnValue(IsMovable()));

    return mFunc.Return();
}

int Frame::_IsResizable(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsResizable", pLua, 1);

    mFunc.Push(Lua::ReturnValue(IsResizable()));

    return mFunc.Return();
}

int Frame::_IsTopLevel(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsTopLevel", pLua, 1);

    mFunc.Push(Lua::ReturnValue(IsTopLevel()));

    return mFunc.Return();
}

int Frame::_IsUserPlaced(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsUserPlaced", pLua, 1);

    mFunc.Push(Lua::ReturnValue(IsUserPlaced()));

    return mFunc.Return();
}

int Frame::_RegisterAllEvents(lua_State* pLua)
{
    Lua::Function mFunc("Frame:RegisterAllEvents", pLua);

    RegisterAllEvents();

    return mFunc.Return();
}

int Frame::_RegisterEvent(lua_State* pLua)
{
    Lua::Function mFunc("Frame:RegisterEvent", pLua);
    mFunc.Add(0, "eventName", LUA_TSTRING, VALUE_STRING);
    if (mFunc.Check())
    {
        RegisterEvent(mFunc.Get(0)->GetS());
    }

    return mFunc.Return();
}

int Frame::_RegisterForDrag(lua_State* pLua)
{
    Lua::Function mFunc("Frame:RegisterForDrag", pLua);
    mFunc.Add(0, "button1", LUA_TSTRING, VALUE_STRING, true);
    mFunc.Add(1, "button2", LUA_TSTRING, VALUE_STRING, true);
    mFunc.Add(2, "button3", LUA_TSTRING, VALUE_STRING, true);
    if (mFunc.Check())
    {
        vector<s_str> lButtonList;
        for (s_uint i; i<3; i++)
        {
            if (mFunc.IsProvided(i))
                lButtonList.push_back(mFunc.Get(i)->GetS());
            else
                break;
        }
        RegisterForDrag(lButtonList);
    }

    return mFunc.Return();
}

int Frame::_SetClampedToScreen(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetClampedToScreen", pLua);
    mFunc.Add(0, "clampedToScreen", LUA_TBOOLEAN, VALUE_BOOL);
    if (mFunc.Check())
    {
        SetClampedToScreen(mFunc.Get(0)->GetB());
    }

    return mFunc.Return();
}

int Frame::_SetFrameStrata(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetFrameStrata", pLua);
    mFunc.Add(0, "strata", LUA_TSTRING, VALUE_STRING);
    if (mFunc.Check())
    {
        s_str sStrata = mFunc.Get(0)->GetS();
        if (sStrata == "BACKGROUND")
            SetFrameStrata(STRATA_BACKGROUND);
        else if (sStrata == "LOW")
            SetFrameStrata(STRATA_LOW);
        else if (sStrata == "MEDIUM")
            SetFrameStrata(STRATA_MEDIUM);
        else if (sStrata == "HIGH")
            SetFrameStrata(STRATA_HIGH);
        else if (sStrata == "DIALOG")
            SetFrameStrata(STRATA_DIALOG);
        else if (sStrata == "FULLSCREEN")
            SetFrameStrata(STRATA_FULLSCREEN);
        else if (sStrata == "FULLSCREEN_DIALOG")
            SetFrameStrata(STRATA_FULLSCREEN_DIALOG);
        else if (sStrata == "TOOLTIP")
            SetFrameStrata(STRATA_TOOLTIP);
        else
            Warning(lType_.back(), "Unknown strata : \""+sStrata+"\".");
    }

    return mFunc.Return();
}

int Frame::_SetHitRectInsets(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetHitRectInsets", pLua);
    mFunc.Add(0, "left", LUA_TNUMBER, VALUE_INT);
    mFunc.Add(1, "right", LUA_TNUMBER, VALUE_INT);
    mFunc.Add(2, "top", LUA_TNUMBER, VALUE_INT);
    mFunc.Add(3, "bottom", LUA_TNUMBER, VALUE_INT);
    if (mFunc.Check())
    {
        SetHitRectInsets(
            mFunc.Get(0)->GetI(),
            mFunc.Get(1)->GetI(),
            mFunc.Get(2)->GetI(),
            mFunc.Get(3)->GetI()
        );
    }

    return mFunc.Return();
}

int Frame::_SetMaxResize(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetMaxResize", pLua);
    mFunc.Add(0, "width", LUA_TNUMBER, VALUE_UINT);
    mFunc.Add(1, "height", LUA_TNUMBER, VALUE_UINT);
    if (mFunc.Check())
    {
        SetMaxResize(mFunc.Get(0)->GetUI(), mFunc.Get(1)->GetUI());
    }

    return mFunc.Return();
}

int Frame::_SetMinResize(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetMinResize", pLua);
    mFunc.Add(0, "width", LUA_TNUMBER, VALUE_UINT);
    mFunc.Add(1, "height", LUA_TNUMBER, VALUE_UINT);
    if (mFunc.Check())
    {
        SetMinResize(mFunc.Get(0)->GetUI(), mFunc.Get(1)->GetUI());
    }

    return mFunc.Return();
}

int Frame::_SetMovable(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetMovable", pLua);
    mFunc.Add(0, "movable", LUA_TBOOLEAN, VALUE_BOOL);
    if (mFunc.Check())
    {
        SetMovable(mFunc.Get(0)->GetB());
    }

    return mFunc.Return();
}

int Frame::_SetResizable(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetResizable", pLua);
    mFunc.Add(0, "resizable", LUA_TBOOLEAN, VALUE_BOOL);
    if (mFunc.Check())
    {
        SetResizable(mFunc.Get(0)->GetB());
    }

    return mFunc.Return();
}

int Frame::_SetScale(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetScale", pLua);
    mFunc.Add(0, "scale", LUA_TNUMBER, VALUE_FLOAT);
    if (mFunc.Check())
    {
        SetScale(mFunc.Get(0)->GetF());
    }

    return mFunc.Return();
}

int Frame::_SetScript(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetScript", pLua);
    mFunc.Add(0, "scriptName", LUA_TSTRING, VALUE_STRING);
    mFunc.Add(1, "function", LUA_TFUNCTION, VALUE_NONE);
    mFunc.Add(1, "nil", LUA_TNIL, VALUE_NONE);
    if (mFunc.Check())
    {
        s_str sScriptName = mFunc.Get(0)->GetS();
        if (CanUseScript(sScriptName))
        {
            s_ptr<Lua::Argument> pSecond = mFunc.Get(1);
            if (pSecond->GetType() == LUA_TFUNCTION)
            {
                lua_pushvalue(pLua, pSecond->GetUI().Get());
                lua_setglobal(pLua, (sName_ + ":" + sScriptName).c_str());
            }
            else
            {
                lua_pushnil(pLua);
                lua_setglobal(pLua, (sName_ + ":" + sScriptName).c_str());
            }
        }
        else
            Error(lType_.back(), "Can't use script \""+sScriptName+"\".");
    }

    return mFunc.Return();
}

int Frame::_SetTopLevel(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetTopLevel", pLua);
    mFunc.Add(0, "topLevel", LUA_TBOOLEAN, VALUE_BOOL);
    if (mFunc.Check())
    {
        SetTopLevel(mFunc.Get(0)->GetB());
    }

    return mFunc.Return();
}

int Frame::_SetUserPlaced(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetUserPlaced", pLua);
    mFunc.Add(0, "userPlaced", LUA_TBOOLEAN, VALUE_BOOL);
    if (mFunc.Check())
    {
        SetUserPlaced(mFunc.Get(0)->GetB());
    }

    return mFunc.Return();
}

int Frame::_UnregisterAllEvents(lua_State* pLua)
{
    Lua::Function mFunc("Frame:UnregisterAllEvents", pLua);

    UnregisterAllEvents();

    return mFunc.Return();
}

int Frame::_UnregisterEvent(lua_State* pLua)
{
    Lua::Function mFunc("Frame:UnregisterEvent", pLua);
    mFunc.Add(0, "eventName", LUA_TSTRING, VALUE_STRING);
    if (mFunc.Check())
    {
        UnregisterEvent(mFunc.Get(0)->GetS());
    }

    return mFunc.Return();
}

