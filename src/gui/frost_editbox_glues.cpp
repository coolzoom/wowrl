/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_editbox.h"

#include "gui/frost_frame.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaEditBox::CLASS_NAME = "GUI::LuaEditBox";

LuaEditBox::LuaEditBox(lua_State* pLua) : LuaFrame(pLua)
{
    pEditBoxParent_ = s_ptr<EditBox>::DynamicCast(pParent_);
    if (pParent_ && !pEditBoxParent_)
    {
        throw Exception(CLASS_NAME, "Dynamic cast failed !");
    }
}

int LuaEditBox::_AddHistoryLine( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:AddHistoryLine", pLua);
    mFunc.Add(0, "line", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        pEditBoxParent_->AddHistoryLine(mFunc.Get(0)->GetString());
    }

    return mFunc.Return();
}

int LuaEditBox::_ClearFocus( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:ClearFocus", pLua);

    pEditBoxParent_->SetFocus(false);

    return mFunc.Return();
}

int LuaEditBox::_GetBlinkSpeed( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:GetBlinkSpeed", pLua, 1);

    mFunc.Push(pEditBoxParent_->GetBlinkSpeed());

    return mFunc.Return();
}

int LuaEditBox::_GetHistoryLines( lua_State* pLua )
{
    const s_ctnr<s_str>& lHistoryLine = pEditBoxParent_->GetHistoryLines();
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:GetHistoryLines", pLua, lHistoryLine.GetSize());

    s_ctnr<s_str>::const_iterator iterLine;
    foreach (iterLine, lHistoryLine)
    {
        mFunc.Push(*iterLine);
    }

    return mFunc.Return();
}

int LuaEditBox::_GetMaxLetters( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:GetMaxLetters", pLua, 1);

    mFunc.Push(pEditBoxParent_->GetMaxLetters());

    return mFunc.Return();
}

int LuaEditBox::_GetNumLetters( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:GetNumLetters", pLua, 1);

    mFunc.Push(pEditBoxParent_->GetNumLetters());

    return mFunc.Return();
}

int LuaEditBox::_GetNumber( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:GetNumber", pLua, 1);

    mFunc.Push(s_float(pEditBoxParent_->GetText()));

    return mFunc.Return();
}

int LuaEditBox::_GetText( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:GetText", pLua, 1);

    mFunc.Push(pEditBoxParent_->GetText());

    return mFunc.Return();
}

int LuaEditBox::_GetTextInsets( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:GetTextInsets", pLua, 4);

    const s_array<s_int,4>& lInsets = pEditBoxParent_->GetTextInsets();

    for (uint i = 0; i < 4; ++i)
        mFunc.Push(lInsets[i]);

    return mFunc.Return();
}

int LuaEditBox::_HighlithtText( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:HighlithtText", pLua);
    mFunc.Add(0, "start", Lua::TYPE_NUMBER, true);
    mFunc.Add(1, "end", Lua::TYPE_NUMBER, true);
    if (mFunc.Check())
    {
        s_uint uiStart = 0;
        s_uint uiEnd = s_uint::INF;

        if (mFunc.IsProvided(0))
            uiStart = s_uint(mFunc.Get(0)->GetNumber());
        if (mFunc.IsProvided(1))
            uiEnd = s_uint(mFunc.Get(1)->GetNumber());

        pEditBoxParent_->HighlightText(uiStart, uiEnd);
    }

    return mFunc.Return();
}

int LuaEditBox::_Insert( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:Insert", pLua);
    mFunc.Add(0, "text", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        pEditBoxParent_->InsertAfterCursor(mFunc.Get(0)->GetString());
    }

    return mFunc.Return();
}

int LuaEditBox::_IsAutoFocus( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:IsAutoFocus", pLua);

    mFunc.Push(pEditBoxParent_->IsAutoFocusEnabled());

    return mFunc.Return();
}

int LuaEditBox::_IsMultiLine( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:IsMultiLine", pLua);

    mFunc.Push(pEditBoxParent_->IsMultiLine());

    return mFunc.Return();
}

int LuaEditBox::_IsNumeric( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:IsNumeric", pLua);

    mFunc.Push(pEditBoxParent_->IsNumericOnly());

    return mFunc.Return();
}

int LuaEditBox::_IsPassword( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:IsPassword", pLua);

    mFunc.Push(pEditBoxParent_->IsPasswordModeEnabled());

    return mFunc.Return();
}

int LuaEditBox::_SetAutoFocus( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:SetAutoFocus", pLua);
    mFunc.Add(0, "enabled", Lua::TYPE_BOOLEAN);
    if (mFunc.Check())
    {
        pEditBoxParent_->EnableAutoFocus(mFunc.Get(0)->GetBool());
    }

    return mFunc.Return();
}

int LuaEditBox::_SetBlinkSpeed( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:SetBlinkSpeed", pLua);
    mFunc.Add(0, "blink speed", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pEditBoxParent_->SetBlinkSpeed(s_double(mFunc.Get(0)->GetNumber()));
    }

    return mFunc.Return();
}

int LuaEditBox::_SetFocus( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:SetFocus", pLua);

    pEditBoxParent_->SetFocus(true);

    return mFunc.Return();
}

int LuaEditBox::_SetMaxHistoryLines( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:SetMaxHistoryLines", pLua);
    mFunc.Add(0, "max lines", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pEditBoxParent_->SetMaxHistoryLines(s_uint(mFunc.Get(0)->GetNumber()));
    }

    return mFunc.Return();
}

int LuaEditBox::_SetMaxLetters( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:SetMaxLetters", pLua);
    mFunc.Add(0, "max letters", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pEditBoxParent_->SetMaxLetters(s_uint(mFunc.Get(0)->GetNumber()));
    }

    return mFunc.Return();
}

int LuaEditBox::_SetMultiLine( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:SetMultiLine", pLua);
    mFunc.Add(0, "multiLine", Lua::TYPE_BOOLEAN);
    if (mFunc.Check())
    {
        pEditBoxParent_->SetMultiLine(mFunc.Get(0)->GetBool());
    }

    return mFunc.Return();
}

int LuaEditBox::_SetNumber( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:SetNumber", pLua);
    mFunc.Add(0, "number", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pEditBoxParent_->SetText(s_str::Convert(mFunc.Get(0)->GetNumber()));
    }

    return mFunc.Return();
}

int LuaEditBox::_SetNumeric( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:SetNumeric", pLua);
    mFunc.Add(0, "numeric", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pEditBoxParent_->SetNumericOnly(mFunc.Get(0)->GetBool());
    }

    return mFunc.Return();
}

int LuaEditBox::_SetPassword( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:SetPassword", pLua);
    mFunc.Add(0, "enable", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pEditBoxParent_->EnablePasswordMode(mFunc.Get(0)->GetBool());
    }

    return mFunc.Return();
}

int LuaEditBox::_SetText( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:SetText", pLua);
    mFunc.Add(0, "text", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        pEditBoxParent_->SetText(mFunc.Get(0)->GetString());
    }

    return mFunc.Return();
}

int LuaEditBox::_SetTextInsets( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("EditBox:SetTextInsets", pLua);
    mFunc.Add(0, "left", Lua::TYPE_NUMBER);
    mFunc.Add(1, "right", Lua::TYPE_NUMBER);
    mFunc.Add(2, "top", Lua::TYPE_NUMBER);
    mFunc.Add(3, "bottom", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pEditBoxParent_->SetTextInsets(
            s_int(mFunc.Get(0)->GetNumber()),
            s_int(mFunc.Get(1)->GetNumber()),
            s_int(mFunc.Get(2)->GetNumber()),
            s_int(mFunc.Get(3)->GetNumber())
        );
    }

    return mFunc.Return();
}

