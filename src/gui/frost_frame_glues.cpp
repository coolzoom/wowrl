/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_frame.h"
#include "gui/frost_backdrop.h"
#include "gui/frost_titleregion.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaFrame::CLASS_NAME  = "GUI::LuaFrame";

LuaFrame::LuaFrame(lua_State* pLua) : LuaUIObject(pLua)
{
    pFrameParent_ = s_ptr<Frame>::DynamicCast(pParent_);
    if (pParent_ && !pFrameParent_)
    {
        throw Exception(CLASS_NAME, "Dynamic cast failed !");
    }
}

int LuaFrame::_CreateTitleRegion(lua_State* pLua)
{
    Lua::Function mFunc("Frame::CreateTitleRegion", pLua);

    pFrameParent_->CreateTitleRegion();

    return mFunc.Return();
}

int LuaFrame::_DisableDrawLayer(lua_State* pLua)
{
    Lua::Function mFunc("Frame:DisableDrawLayer", pLua);
    mFunc.Add(0, "layer", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        s_str sLayer = mFunc.Get(0)->GetString();
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
    mFunc.Add(0, "layer", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        s_str sLayer = mFunc.Get(0)->GetString();
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
    mFunc.Add(0, "is keyboard enable", Lua::TYPE_BOOLEAN);
    if (mFunc.Check())
    {
        pFrameParent_->EnableKeyboard(mFunc.Get(0)->GetBool());
    }

    return mFunc.Return();
}

int LuaFrame::_EnableMouse(lua_State* pLua)
{
    Lua::Function mFunc("Frame:EnableMouse", pLua);
    mFunc.Add(0, "is mouse enabled", Lua::TYPE_BOOLEAN);
    if (mFunc.Check())
    {
        pFrameParent_->EnableMouse(mFunc.Get(0)->GetBool());
    }

    return mFunc.Return();
}

int LuaFrame::_EnableMouseWheel(lua_State* pLua)
{
    Lua::Function mFunc("Frame:EnableMouseWheel", pLua);
    mFunc.Add(0, "is mouse wheel enabled", Lua::TYPE_BOOLEAN);
    if (mFunc.Check())
    {
        pFrameParent_->EnableMouseWheel(mFunc.Get(0)->GetBool());
    }

    return mFunc.Return();
}

int LuaFrame::_GetBackdrop(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetBackdrop", pLua, 1);

    if (s_refptr<Backdrop> pBackdrop = pFrameParent_->GetBackdrop().Lock())
    {
        s_ptr<Lua::State> pState = mFunc.GetState();

        pState->NewTable();
        pState->SetFieldString("bgFile", pBackdrop->GetBackgroundFile());
        pState->SetFieldString("edgeFile", pBackdrop->GetEdgeFile());
        pState->SetFieldBool("tile", pBackdrop->IsBackgroundTilling());

        pState->SetFieldInt("tileSize", s_int(pBackdrop->GetTileSize()));
        pState->SetFieldInt("edgeSize", s_int(pBackdrop->GetEdgeSize()));

        pState->NewTable();
        pState->SetField("insets");
        pState->GetField("insets");

        const s_array<s_int,4>& lInsets = pBackdrop->GetBackgroundInsets();
        pState->SetFieldInt("left", lInsets[BORDER_LEFT]);
        pState->SetFieldInt("right", lInsets[BORDER_RIGHT]);
        pState->SetFieldInt("top", lInsets[BORDER_TOP]);
        pState->SetFieldInt("bottom", lInsets[BORDER_BOTTOM]);

        pState->Pop();

        mFunc.NotifyPushed();
    }
    else
    {
        mFunc.PushNil();
    }

    return mFunc.Return();
}

int LuaFrame::_GetBackdropBorderColor(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetBackdropBorderColor", pLua, 4);

    if (pFrameParent_->GetBackdrop())
    {
        const Color& mColor = pFrameParent_->GetBackdrop()->GetEdgeColor();
        mFunc.Push(s_float(mColor.GetR())/255.0f);
        mFunc.Push(s_float(mColor.GetG())/255.0f);
        mFunc.Push(s_float(mColor.GetB())/255.0f);
        mFunc.Push(s_float(mColor.GetA())/255.0f);
    }
    else
    {
        mFunc.PushNil(4);
    }

    return mFunc.Return();
}

int LuaFrame::_GetBackdropColor(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetBackdropColor", pLua, 4);

    if (pFrameParent_->GetBackdrop())
    {
        const Color& mColor = pFrameParent_->GetBackdrop()->GetBackgroundColor();
        mFunc.Push(s_float(mColor.GetR())/255.0f);
        mFunc.Push(s_float(mColor.GetG())/255.0f);
        mFunc.Push(s_float(mColor.GetB())/255.0f);
        mFunc.Push(s_float(mColor.GetA())/255.0f);
    }
    else
    {
        mFunc.PushNil(4);
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
        (*iterChild)->PushOnLua(mFunc.GetState());
        mFunc.NotifyPushed();
    }

    return mFunc.Return();
}

int LuaFrame::_GetEffectiveAlpha(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetEffectiveAlpha", pLua, 1);

    mFunc.Push(pFrameParent_->GetEffectiveAlpha());

    return mFunc.Return();
}

int LuaFrame::_GetEffectiveScale(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetEffectiveScale", pLua, 1);

    mFunc.Push(pFrameParent_->GetEffectiveScale());

    return mFunc.Return();
}

int LuaFrame::_GetFrameLevel(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetFrameLevel", pLua, 1);

    mFunc.Push(pFrameParent_->GetFrameLevel());

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

    mFunc.Push(sStrata);

    return mFunc.Return();
}

int LuaFrame::_GetFrameType(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetFrameType", pLua, 1);

    mFunc.Push(pFrameParent_->GetFrameType());

    return mFunc.Return();
}

int LuaFrame::_GetHitRectInsets(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetHitRectInsets", pLua, 4);

    s_array<s_int,4> lInsets = pFrameParent_->GetAbsHitRectInsets();

    for (uint i = 0; i < 4; ++i)
        mFunc.Push(lInsets[i]);

    return mFunc.Return();
}

int LuaFrame::_GetID(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetID", pLua, 1);

    mFunc.Push(pFrameParent_->GetID());

    return mFunc.Return();
}

int LuaFrame::_GetMaxResize(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetMaxResize", pLua, 2);

    s_array<s_uint,2> lMax = pFrameParent_->GetMaxResize();

    mFunc.Push(lMax[0]);
    mFunc.Push(lMax[1]);

    return mFunc.Return();
}

int LuaFrame::_GetMinResize(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetMinResize", pLua, 2);

    s_array<s_uint,2> lMin = pFrameParent_->GetMinResize();

    mFunc.Push(lMin[0]);
    mFunc.Push(lMin[1]);

    return mFunc.Return();
}

int LuaFrame::_GetNumChildren(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetNumChildren", pLua, 1);

    mFunc.Push(pFrameParent_->GetNumChildren());

    return mFunc.Return();
}

int LuaFrame::_GetNumRegions(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetNumRegions", pLua, 1);

    mFunc.Push(pFrameParent_->GetNumRegions());

    return mFunc.Return();
}

int LuaFrame::_GetScale(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetScale", pLua, 1);

    mFunc.Push(pFrameParent_->GetScale());

    return mFunc.Return();
}

int LuaFrame::_GetScript(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetScript", pLua, 1);
    mFunc.Add(0, "script name", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        s_str sScriptName = mFunc.Get(0)->GetString();
        if (pFrameParent_->HasScript(sScriptName))
        {
            lua_getglobal(pLua, (pFrameParent_->GetName() + ":" + sScriptName).c_str());
            mFunc.NotifyPushed();
        }
    }

    return mFunc.Return();
}

int LuaFrame::_GetTitleRegion(lua_State* pLua)
{
    Lua::Function mFunc("Frame:GetTitleRegion", pLua, 1);

    if (pFrameParent_->GetTitleRegion())
    {
        pFrameParent_->GetTitleRegion()->PushOnLua(mFunc.GetState());
        mFunc.NotifyPushed();
    }
    else
    {
        mFunc.PushNil();
    }

    return mFunc.Return();
}

int LuaFrame::_HasScript(lua_State* pLua)
{
    Lua::Function mFunc("Frame:HasScript", pLua, 1);
    mFunc.Add(0, "script name", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        mFunc.Push(pFrameParent_->CanUseScript(mFunc.Get(0)->GetString()));
    }

    return mFunc.Return();
}

int LuaFrame::_IsClampedToScreen(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsClampedToScreen", pLua, 1);

    mFunc.Push(pFrameParent_->IsClampedToScreen());

    return mFunc.Return();
}

int LuaFrame::_IsFrameType(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsFrameType", pLua, 1);
    mFunc.Add(0, "frame type", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        if (pFrameParent_->GetFrameType() == mFunc.Get(0)->GetString())
            mFunc.Push(s_bool(true));
        else
            mFunc.Push(s_bool(false));
    }

    return mFunc.Return();
}

int LuaFrame::_IsKeyboardEnabled(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsKeyboardEnabled", pLua, 1);

    mFunc.Push(pFrameParent_->IsKeyboardEnabled());

    return mFunc.Return();
}

int LuaFrame::_IsMouseEnabled(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsMouseEnabled", pLua, 1);

    mFunc.Push(pFrameParent_->IsMouseEnabled());

    return mFunc.Return();
}

int LuaFrame::_IsMouseWheelEnabled(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsMouseWheelEnabled", pLua, 1);

    mFunc.Push(pFrameParent_->IsMouseWheelEnabled());

    return mFunc.Return();
}

int LuaFrame::_IsMovable(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsMovable", pLua, 1);

    mFunc.Push(pFrameParent_->IsMovable());

    return mFunc.Return();
}

int LuaFrame::_IsResizable(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsResizable", pLua, 1);

    mFunc.Push(pFrameParent_->IsResizable());

    return mFunc.Return();
}

int LuaFrame::_IsTopLevel(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsTopLevel", pLua, 1);

    mFunc.Push(pFrameParent_->IsTopLevel());

    return mFunc.Return();
}

int LuaFrame::_IsUserPlaced(lua_State* pLua)
{
    Lua::Function mFunc("Frame:IsUserPlaced", pLua, 1);

    mFunc.Push(pFrameParent_->IsUserPlaced());

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
    mFunc.Add(0, "event name", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        pFrameParent_->RegisterEvent(mFunc.Get(0)->GetString());
    }

    return mFunc.Return();
}

int LuaFrame::_RegisterForDrag(lua_State* pLua)
{
    Lua::Function mFunc("Frame:RegisterForDrag", pLua);
    mFunc.Add(0, "button 1", Lua::TYPE_STRING, true);
    mFunc.Add(1, "button 2", Lua::TYPE_STRING, true);
    mFunc.Add(2, "button 3", Lua::TYPE_STRING, true);
    if (mFunc.Check())
    {
        s_ctnr<s_str> lButtonList;
        for (s_uint i; i < 3; ++i)
        {
            if (mFunc.IsProvided(i))
                lButtonList.PushBack(mFunc.Get(i)->GetString());
            else
                break;
        }
        pFrameParent_->RegisterForDrag(lButtonList);
    }

    return mFunc.Return();
}

int LuaFrame::_SetBackdrop(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetBackdrop", pLua);
    mFunc.Add(0, "backdrop table", Lua::TYPE_TABLE);
    mFunc.Add(0, "nil", Lua::TYPE_NIL);
    if (mFunc.Check())
    {
        if (mFunc.Get(0)->GetType() == Lua::TYPE_NIL)
        {
            pFrameParent_->SetBackdrop(s_refptr<Backdrop>(NULL));
        }
        else
        {
            s_refptr<Backdrop> pBackdrop(new Backdrop(pFrameParent_));

            s_ptr<Lua::State> pState = mFunc.GetState();

            pBackdrop->SetBackground(pState->GetFieldString("bgFile", false, ""));
            pBackdrop->SetEdge(pState->GetFieldString("edgeFile", false, ""));
            pBackdrop->SetBackgroundTilling(pState->GetFieldBool("tile", false, false));

            s_uint uiTileSize = s_uint(pState->GetFieldInt("tileSize", false, s_int::NaN));
            if (uiTileSize.IsValid())
                pBackdrop->SetTileSize(uiTileSize);

            s_uint uiEdgeSize = s_uint(pState->GetFieldInt("edgeSize", false, s_int::NaN));
            if (uiEdgeSize.IsValid())
                pBackdrop->SetEdgeSize(uiEdgeSize);

            pState->GetField("insets");

            if (pState->GetType() == Lua::TYPE_TABLE)
            {
                pBackdrop->SetBackgroundInsets((
                    pState->GetFieldInt("left",   false, 0),
                    pState->GetFieldInt("right",  false, 0),
                    pState->GetFieldInt("top",    false, 0),
                    pState->GetFieldInt("bottom", false, 0)
                ));
            }

            pFrameParent_->SetBackdrop(pBackdrop);
        }
    }

    return mFunc.Return();
}

int LuaFrame::_SetBackdropBorderColor(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetBackdropBorderColor", pLua);
    mFunc.Add(0, "red", Lua::TYPE_NUMBER);
    mFunc.Add(1, "green", Lua::TYPE_NUMBER);
    mFunc.Add(2, "blue", Lua::TYPE_NUMBER);
    mFunc.Add(3, "alpha", Lua::TYPE_NUMBER, true);
    if (mFunc.Check())
    {
        if (s_refptr<Backdrop> pBackdrop = pFrameParent_->GetBackdrop().Lock())
        {
            Color mColor;
            if (mFunc.IsProvided(3))
            {
                mColor = Color(
                    s_uchar(255.0f*mFunc.Get(3)->GetNumber()),
                    s_uchar(255.0f*mFunc.Get(0)->GetNumber()),
                    s_uchar(255.0f*mFunc.Get(1)->GetNumber()),
                    s_uchar(255.0f*mFunc.Get(2)->GetNumber())
                );
            }
            else
            {
                mColor = Color(
                    s_uchar(255.0f*mFunc.Get(0)->GetNumber()),
                    s_uchar(255.0f*mFunc.Get(1)->GetNumber()),
                    s_uchar(255.0f*mFunc.Get(2)->GetNumber())
                );
            }

            pBackdrop->SetEdgeColor(mColor);
        }
    }

    return mFunc.Return();
}

int LuaFrame::_SetBackdropColor(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetBackdropColor", pLua);
    mFunc.Add(0, "red", Lua::TYPE_NUMBER);
    mFunc.Add(1, "green", Lua::TYPE_NUMBER);
    mFunc.Add(2, "blue", Lua::TYPE_NUMBER);
    mFunc.Add(3, "alpha", Lua::TYPE_NUMBER, true);
    if (mFunc.Check())
    {
        if (s_refptr<Backdrop> pBackdrop = pFrameParent_->GetBackdrop().Lock())
        {
            Color mColor;
            if (mFunc.IsProvided(3))
            {
                mColor = Color(
                    s_uchar(255.0f*mFunc.Get(3)->GetNumber()),
                    s_uchar(255.0f*mFunc.Get(0)->GetNumber()),
                    s_uchar(255.0f*mFunc.Get(1)->GetNumber()),
                    s_uchar(255.0f*mFunc.Get(2)->GetNumber())
                );
            }
            else
            {
                mColor = Color(
                    s_uchar(255.0f*mFunc.Get(0)->GetNumber()),
                    s_uchar(255.0f*mFunc.Get(1)->GetNumber()),
                    s_uchar(255.0f*mFunc.Get(2)->GetNumber())
                );
            }

            pBackdrop->SetBackgroundColor(mColor);
        }
    }

    return mFunc.Return();
}


int LuaFrame::_SetClampedToScreen(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetClampedToScreen", pLua);
    mFunc.Add(0, "is clamped to screen", Lua::TYPE_BOOLEAN);
    if (mFunc.Check())
    {
        pFrameParent_->SetClampedToScreen(mFunc.Get(0)->GetBool());
    }

    return mFunc.Return();
}

int LuaFrame::_SetFrameStrata(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetFrameStrata", pLua);
    mFunc.Add(0, "strata", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        pFrameParent_->SetFrameStrata(mFunc.Get(0)->GetString());
    }

    return mFunc.Return();
}

int LuaFrame::_SetHitRectInsets(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetHitRectInsets", pLua);
    mFunc.Add(0, "left", Lua::TYPE_NUMBER);
    mFunc.Add(1, "right", Lua::TYPE_NUMBER);
    mFunc.Add(2, "top", Lua::TYPE_NUMBER);
    mFunc.Add(3, "bottom", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pFrameParent_->SetAbsHitRectInsets(
            s_int(mFunc.Get(0)->GetNumber()),
            s_int(mFunc.Get(1)->GetNumber()),
            s_int(mFunc.Get(2)->GetNumber()),
            s_int(mFunc.Get(3)->GetNumber())
        );
    }

    return mFunc.Return();
}

int LuaFrame::_SetMaxResize(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetMaxResize", pLua);
    mFunc.Add(0, "width", Lua::TYPE_NUMBER);
    mFunc.Add(1, "height", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pFrameParent_->SetMaxResize(
            s_uint(mFunc.Get(0)->GetNumber()),
            s_uint(mFunc.Get(1)->GetNumber())
        );
    }

    return mFunc.Return();
}

int LuaFrame::_SetMinResize(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetMinResize", pLua);
    mFunc.Add(0, "width", Lua::TYPE_NUMBER);
    mFunc.Add(1, "height", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pFrameParent_->SetMinResize(
            s_uint(mFunc.Get(0)->GetNumber()),
            s_uint(mFunc.Get(1)->GetNumber())
        );
    }

    return mFunc.Return();
}

int LuaFrame::_SetMovable(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetMovable", pLua);
    mFunc.Add(0, "is movable", Lua::TYPE_BOOLEAN);
    if (mFunc.Check())
    {
        pFrameParent_->SetMovable(mFunc.Get(0)->GetBool());
    }

    return mFunc.Return();
}

int LuaFrame::_SetResizable(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetResizable", pLua);
    mFunc.Add(0, "is resizable", Lua::TYPE_BOOLEAN);
    if (mFunc.Check())
    {
        pFrameParent_->SetResizable(mFunc.Get(0)->GetBool());
    }

    return mFunc.Return();
}

int LuaFrame::_SetScale(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetScale", pLua);
    mFunc.Add(0, "scale", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pFrameParent_->SetScale(mFunc.Get(0)->GetNumber());
    }

    return mFunc.Return();
}

int LuaFrame::_SetScript(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetScript", pLua);
    mFunc.Add(0, "script name", Lua::TYPE_STRING);
    mFunc.Add(1, "function", Lua::TYPE_FUNCTION);
    mFunc.Add(1, "nil", Lua::TYPE_NIL);
    if (mFunc.Check())
    {
        s_str sScriptName = mFunc.Get(0)->GetString();
        if (pFrameParent_->CanUseScript(sScriptName))
        {
            s_ptr<Lua::State> pLua = mFunc.GetState();
            s_ptr<Lua::Argument> pArg = mFunc.Get(1);
            if (pArg->GetType() == Lua::TYPE_FUNCTION)
            {
                pLua->PushValue(pArg->GetIndex());
                pLua->SetGlobal(pFrameParent_->GetName() + ":" + sScriptName);
                pFrameParent_->NotifyScriptDefined(sScriptName);
            }
            else
            {
                pLua->PushNil();
                pLua->SetGlobal(pFrameParent_->GetName() + ":" + sScriptName);
                pFrameParent_->NotifyScriptDefined(sScriptName);
            }
        }
        else
            Error(pFrameParent_->GetFrameType(),
                "\""+pFrameParent_->GetName()+"\" can't use script \""+sScriptName+"\"."
            );
    }

    return mFunc.Return();
}

int LuaFrame::_SetTopLevel(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetTopLevel", pLua);
    mFunc.Add(0, "is top level", Lua::TYPE_BOOLEAN);
    if (mFunc.Check())
    {
        pFrameParent_->SetTopLevel(mFunc.Get(0)->GetBool());
    }

    return mFunc.Return();
}

int LuaFrame::_SetUserPlaced(lua_State* pLua)
{
    Lua::Function mFunc("Frame:SetUserPlaced", pLua);
    mFunc.Add(0, "is user placed", Lua::TYPE_BOOLEAN);
    if (mFunc.Check())
    {
        pFrameParent_->SetUserPlaced(mFunc.Get(0)->GetBool());
    }

    return mFunc.Return();
}

int LuaFrame::_StartMoving(lua_State* pLua)
{
    Lua::Function mFunc("Frame:StartMoving", pLua);

    pFrameParent_->StartMoving();

    return mFunc.Return();
}

int LuaFrame::_StartSizing(lua_State* pLua)
{
    Lua::Function mFunc("Frame:StartSizing", pLua);
    mFunc.Add(0, "point", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        pFrameParent_->StartSizing(Anchor::GetAnchorPoint(mFunc.Get(0)->GetString()));
    }

    return mFunc.Return();
}

int LuaFrame::_StopMovingOrSizing(lua_State* pLua)
{
    Lua::Function mFunc("Frame:StopMovingOrSizing", pLua);

    pFrameParent_->StopMoving();
    pFrameParent_->StopSizing();

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
    mFunc.Add(0, "event name", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        pFrameParent_->UnregisterEvent(mFunc.Get(0)->GetString());
    }

    return mFunc.Return();
}
