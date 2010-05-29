/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_button.h"

#include "gui/frost_frame.h"
#include "gui/frost_fontstring.h"
#include "gui/frost_texture.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaButton::CLASS_NAME = "GUI::LuaButton";

LuaButton::LuaButton(lua_State* pLua) : LuaFrame(pLua)
{
    pButtonParent_ = s_ptr<Button>::DynamicCast(pParent_);
    if (pParent_ && !pButtonParent_)
    {
        throw Exception(CLASS_NAME, "Dynamic cast failed !");
    }
}

int LuaButton::_Click( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:Click", pLua);

    pButtonParent_->On("Click");

    return mFunc.Return();
}

int LuaButton::_Disable( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:Disable", pLua);

    pButtonParent_->Disable();

    return mFunc.Return();
}

int LuaButton::_Enable( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:Enable", pLua);

    pButtonParent_->Enable();

    return mFunc.Return();
}

int LuaButton::_GetButtonState( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:GetButtonState", pLua, 1);

    switch (pButtonParent_->GetButtonState())
    {
        case Button::STATE_UP :       mFunc.Push(s_str("NORMAL"));   break;
        case Button::STATE_DOWN :     mFunc.Push(s_str("PUSHED"));   break;
        case Button::STATE_DISABLED : mFunc.Push(s_str("DISABLED")); break;
        default : mFunc.PushNil(); break;
    }

    return mFunc.Return();
}

int LuaButton::_GetDisabledFontObject( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:GetDisabledFontObject", pLua, 1);

    s_ptr<FontString> pFontString = pButtonParent_->GetDisabledText();
    if (pFontString)
    {
        pFontString->PushOnLua(mFunc.GetState());
        mFunc.NotifyPushed();
    }
    else
        mFunc.PushNil();

    return mFunc.Return();
}

int LuaButton::_GetDisabledTextColor( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:GetDisabledTextColor", pLua, 4);

    s_ptr<FontString> pFontString = pButtonParent_->GetDisabledText();
    if (pFontString)
    {
        const Color& mColor = pFontString->GetTextColor();
        mFunc.Push(mColor.GetR());
        mFunc.Push(mColor.GetG());
        mFunc.Push(mColor.GetB());
        mFunc.Push(mColor.GetA());
    }
    else
        mFunc.PushNil(4);

    return mFunc.Return();
}

int LuaButton::_GetDisabledTexture( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:GetDisabledTexture", pLua, 1);

    s_ptr<Texture> pTexture = pButtonParent_->GetDisabledTexture();
    if (pTexture)
    {
        pTexture->PushOnLua(mFunc.GetState());
        mFunc.NotifyPushed();
    }
    else
        mFunc.PushNil();

    return mFunc.Return();
}

int LuaButton::_GetHighlightFontObject( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:GetHighlightFontObject", pLua, 1);

    s_ptr<FontString> pFontString = pButtonParent_->GetHighlightText();
    if (pFontString)
    {
        pFontString->PushOnLua(mFunc.GetState());
        mFunc.NotifyPushed();
    }
    else
        mFunc.PushNil();

    return mFunc.Return();
}

int LuaButton::_GetHighlightTextColor( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:GetHighlightTextColor", pLua, 4);

    s_ptr<FontString> pFontString = pButtonParent_->GetHighlightText();
    if (pFontString)
    {
        const Color& mColor = pFontString->GetTextColor();
        mFunc.Push(mColor.GetR());
        mFunc.Push(mColor.GetG());
        mFunc.Push(mColor.GetB());
        mFunc.Push(mColor.GetA());
    }
    else
        mFunc.PushNil(4);

    return mFunc.Return();
}

int LuaButton::_GetHighlightTexture( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:GetHighlightTexture", pLua, 1);

    s_ptr<Texture> pTexture = pButtonParent_->GetHighlightTexture();
    if (pTexture)
    {
        pTexture->PushOnLua(mFunc.GetState());
        mFunc.NotifyPushed();
    }
    else
        mFunc.PushNil();

    return mFunc.Return();
}

int LuaButton::_GetNormalFontObject( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:GetNormalFontObject", pLua);

    s_ptr<FontString> pFontString = pButtonParent_->GetNormalText();
    if (pFontString)
    {
        pFontString->PushOnLua(mFunc.GetState());
        mFunc.NotifyPushed();
    }
    else
        mFunc.PushNil();

    return mFunc.Return();
}

int LuaButton::_GetNormalTexture( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:GetNormalTexture", pLua, 1);

    s_ptr<Texture> pTexture = pButtonParent_->GetNormalTexture();
    if (pTexture)
    {
        pTexture->PushOnLua(mFunc.GetState());
        mFunc.NotifyPushed();
    }
    else
        mFunc.PushNil();

    return mFunc.Return();
}

int LuaButton::_GetPushedTextOffset( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:GetPushedTextOffset", pLua, 2);

    s_array<s_int,2> lOffset = pButtonParent_->GetPushedTextOffset();

    mFunc.Push(lOffset[0]);
    mFunc.Push(lOffset[1]);

    return mFunc.Return();
}

int LuaButton::_GetPushedTexture( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:GetPushedTexture", pLua, 1);

    s_ptr<Texture> pTexture = pButtonParent_->GetPushedTexture();
    if (pTexture)
    {
        pTexture->PushOnLua(mFunc.GetState());
        mFunc.NotifyPushed();
    }
    else
        mFunc.PushNil();

    return mFunc.Return();
}

int LuaButton::_GetText( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:GetText", pLua, 1);

    mFunc.Push(pButtonParent_->GetText());

    return mFunc.Return();
}

int LuaButton::_GetTextHeight( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:GetTextHeight", pLua, 1);

    s_ptr<FontString> pCurrentFont = pButtonParent_->GetCurrentFontString();
    if (pCurrentFont)
        mFunc.Push(pCurrentFont->GetStringHeight());
    else
        mFunc.PushNil();

    return mFunc.Return();
}

int LuaButton::_GetTextWidth( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:GetTextWidth", pLua, 1);

    s_ptr<FontString> pCurrentFont = pButtonParent_->GetCurrentFontString();
    if (pCurrentFont)
        mFunc.Push(pCurrentFont->GetStringWidth());
    else
        mFunc.PushNil();

    return mFunc.Return();
}

int LuaButton::_IsEnabled( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:IsEnabled", pLua, 1);

    mFunc.Push(pButtonParent_->IsEnabled());

    return mFunc.Return();
}

int LuaButton::_LockHighlight( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:LockHighlight", pLua);

    pButtonParent_->LockHighlight();

    return mFunc.Return();
}

int LuaButton::_RegisterForClicks( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:RegisterForClicks", pLua);

    // TODO : Implement Button:RegisterForClicks
    Log("Lua : not implemented : \""+mFunc.GetName()+"\".");

    return mFunc.Return();
}

int LuaButton::_SetButtonState( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:SetButtonState", pLua);
    mFunc.Add(0, "button state", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        s_str sState = mFunc.Get(0)->GetString();
        if (sState == "NORMAL")
        {
            pButtonParent_->Enable();
            pButtonParent_->Release();
        }
        else if (sState == "PUSHED")
        {
            pButtonParent_->Enable();
            pButtonParent_->Push();
        }
        else if (sState == "DISABLED")
        {
            pButtonParent_->Disable();
            pButtonParent_->Release();
        }
        else
        {
            Warning(mFunc.GetName(), "Unkown button state : \""+sState+"\".");
        }
    }

    return mFunc.Return();
}

int LuaButton::_SetDisabledFontObject( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:SetDisabledFontObject", pLua);
    mFunc.Add(0, "font object", Lua::TYPE_USERDATA);
    if (mFunc.Check())
    {
        s_ptr<LuaFontString> pLuaFontString = mFunc.GetState()->Get<LuaFontString>();
        if (pLuaFontString)
        {
            s_ptr<FontString> pFontString = s_ptr<FontString>::DynamicCast(pLuaFontString->GetParent());
            pButtonParent_->SetDisabledText(pFontString);
        }
    }

    return mFunc.Return();
}

int LuaButton::_SetDisabledTextColor( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:SetDisabledTextColor", pLua);
    mFunc.Add(0, "r", Lua::TYPE_NUMBER);
    mFunc.Add(1, "g", Lua::TYPE_NUMBER);
    mFunc.Add(2, "b", Lua::TYPE_NUMBER);
    mFunc.Add(3, "a", Lua::TYPE_NUMBER, true);
    if (mFunc.Check())
    {
        s_ptr<FontString> pFontString = pButtonParent_->GetDisabledText();
        if (pFontString)
        {
            if (mFunc.IsProvided(3))
            {
                pFontString->SetTextColor(Color(
                    mFunc.Get(3)->GetNumber(),
                    mFunc.Get(0)->GetNumber(),
                    mFunc.Get(1)->GetNumber(),
                    mFunc.Get(2)->GetNumber()
                ));
            }
            else
            {
                pFontString->SetTextColor(Color(
                    mFunc.Get(0)->GetNumber(),
                    mFunc.Get(1)->GetNumber(),
                    mFunc.Get(2)->GetNumber()
                ));
            }
        }
    }

    return mFunc.Return();
}

int LuaButton::_SetDisabledTexture( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:SetDisabledTexture", pLua);
    mFunc.Add(0, "texture", Lua::TYPE_USERDATA);
    if (mFunc.Check())
    {
        s_ptr<LuaTexture> pLuaTexture = mFunc.GetState()->Get<LuaTexture>();
        if (pLuaTexture)
        {
            s_ptr<Texture> pTexture = s_ptr<Texture>::DynamicCast(pLuaTexture->GetParent());
            pButtonParent_->SetDisabledTexture(pTexture);
        }
    }

    return mFunc.Return();
}

int LuaButton::_SetHighlightFontObject( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:SetHighlightFontObject", pLua);
    mFunc.Add(0, "font object", Lua::TYPE_USERDATA);
    if (mFunc.Check())
    {
        s_ptr<LuaFontString> pLuaFontString = mFunc.GetState()->Get<LuaFontString>();
        if (pLuaFontString)
        {
            s_ptr<FontString> pFontString = s_ptr<FontString>::DynamicCast(pLuaFontString->GetParent());
            pButtonParent_->SetHighlightText(pFontString);
        }
    }

    return mFunc.Return();
}

int LuaButton::_SetHighlightTextColor( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:SetHighlightTextColor", pLua);
    mFunc.Add(0, "r", Lua::TYPE_NUMBER);
    mFunc.Add(1, "g", Lua::TYPE_NUMBER);
    mFunc.Add(2, "b", Lua::TYPE_NUMBER);
    mFunc.Add(3, "a", Lua::TYPE_NUMBER, true);
    if (mFunc.Check())
    {
        s_ptr<FontString> pFontString = pButtonParent_->GetHighlightText();
        if (pFontString)
        {
            if (mFunc.IsProvided(3))
            {
                pFontString->SetTextColor(Color(
                    mFunc.Get(3)->GetNumber(),
                    mFunc.Get(0)->GetNumber(),
                    mFunc.Get(1)->GetNumber(),
                    mFunc.Get(2)->GetNumber()
                ));
            }
            else
            {
                pFontString->SetTextColor(Color(
                    mFunc.Get(0)->GetNumber(),
                    mFunc.Get(1)->GetNumber(),
                    mFunc.Get(2)->GetNumber()
                ));
            }
        }
    }

    return mFunc.Return();
}

int LuaButton::_SetHighlightTexture( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:SetHighlightTexture", pLua);
    mFunc.Add(0, "texture", Lua::TYPE_USERDATA);
    if (mFunc.Check())
    {
        s_ptr<LuaTexture> pLuaTexture = mFunc.GetState()->Get<LuaTexture>();
        if (pLuaTexture)
        {
            s_ptr<Texture> pTexture = s_ptr<Texture>::DynamicCast(pLuaTexture->GetParent());
            pButtonParent_->SetHighlightTexture(pTexture);
        }
    }

    return mFunc.Return();
}


int LuaButton::_SetNormalFontObject( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:SetNormalFontObject", pLua);
    mFunc.Add(0, "font object", Lua::TYPE_USERDATA);
    if (mFunc.Check())
    {
        s_ptr<LuaFontString> pLuaFontString = mFunc.GetState()->Get<LuaFontString>();
        if (pLuaFontString)
        {
            s_ptr<FontString> pFontString = s_ptr<FontString>::DynamicCast(pLuaFontString->GetParent());
            pButtonParent_->SetNormalText(pFontString);
        }
    }

    return mFunc.Return();
}

int LuaButton::_SetNormalTextColor( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:SetNormalTextColor", pLua);
    mFunc.Add(0, "r", Lua::TYPE_NUMBER);
    mFunc.Add(1, "g", Lua::TYPE_NUMBER);
    mFunc.Add(2, "b", Lua::TYPE_NUMBER);
    mFunc.Add(3, "a", Lua::TYPE_NUMBER, true);
    if (mFunc.Check())
    {
        s_ptr<FontString> pFontString = pButtonParent_->GetNormalText();
        if (pFontString)
        {
            if (mFunc.IsProvided(3))
            {
                pFontString->SetTextColor(Color(
                    mFunc.Get(3)->GetNumber(),
                    mFunc.Get(0)->GetNumber(),
                    mFunc.Get(1)->GetNumber(),
                    mFunc.Get(2)->GetNumber()
                ));
            }
            else
            {
                pFontString->SetTextColor(Color(
                    mFunc.Get(0)->GetNumber(),
                    mFunc.Get(1)->GetNumber(),
                    mFunc.Get(2)->GetNumber()
                ));
            }
        }
    }

    return mFunc.Return();
}

int LuaButton::_SetNormalTexture( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:SetNormalTexture", pLua);
    mFunc.Add(0, "texture", Lua::TYPE_USERDATA);
    if (mFunc.Check())
    {
        s_ptr<LuaTexture> pLuaTexture = mFunc.GetState()->Get<LuaTexture>();
        if (pLuaTexture)
        {
            s_ptr<Texture> pTexture = s_ptr<Texture>::DynamicCast(pLuaTexture->GetParent());
            pButtonParent_->SetNormalTexture(pTexture);
        }
    }

    return mFunc.Return();
}

int LuaButton::_SetPushedTextOffset( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:SetPushedTextOffset", pLua);
    mFunc.Add(0, "x offset", Lua::TYPE_NUMBER);
    mFunc.Add(1, "y offset", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pButtonParent_->SetPushedTextOffset((
            s_int(mFunc.Get(0)->GetNumber()), s_int(mFunc.Get(1)->GetNumber())
        ));
    }

    return mFunc.Return();
}

int LuaButton::_SetPushedTexture( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:SetPushedTexture", pLua);
    mFunc.Add(0, "texture", Lua::TYPE_USERDATA);
    if (mFunc.Check())
    {
        s_ptr<LuaTexture> pLuaTexture = mFunc.GetState()->Get<LuaTexture>();
        if (pLuaTexture)
        {
            s_ptr<Texture> pTexture = s_ptr<Texture>::DynamicCast(pTexture->GetParent());
            pButtonParent_->SetPushedTexture(pTexture);
        }
    }

    return mFunc.Return();
}

int LuaButton::_SetText( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:SetText", pLua);
    mFunc.Add(0, "text", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        pButtonParent_->SetText(mFunc.Get(0)->GetString());
    }

    return mFunc.Return();
}

int LuaButton::_UnlockHighlight( lua_State* pLua )
{
    if (!CheckParent_())
        return 0;
        
    Lua::Function mFunc("Button:UnlockHighlight", pLua);

    pButtonParent_->UnlockHighlight();

    return mFunc.Return();
}
