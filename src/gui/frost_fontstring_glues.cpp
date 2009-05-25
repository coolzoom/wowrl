/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_fontstring.h"

#include "gui/frost_gui_layeredregion.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaFontString::CLASS_NAME = "GUI::LuaFontString";

LuaFontString::LuaFontString(lua_State* pLua) : LuaLayeredRegion(pLua)
{
    pFontStringParent_ = (FontString*)(pParent_.Get());
}

int LuaFontString::_GetFont( lua_State* pLua )
{
    Lua::Function mFunc("FontString:GetFont", pLua, 1);

    mFunc.Push(pFontStringParent_->GetFontName());

    return mFunc.Return();
}

int LuaFontString::_GetJustifyH( lua_State* pLua )
{
    Lua::Function mFunc("FontString:GetJustifyH", pLua, 1);

    Text::Alignment mAlignment = pFontStringParent_->GetJustifyH();
    s_str sAligment;

    switch (mAlignment)
    {
        case Text::ALIGN_LEFT : sAligment = "LEFT"; break;
        case Text::ALIGN_CENTER : sAligment = "CENTER"; break;
        case Text::ALIGN_RIGHT : sAligment = "RIGHT"; break;
    }

    mFunc.Push(sAligment);

    return mFunc.Return();
}

int LuaFontString::_GetShadowColor( lua_State* pLua )
{
    Lua::Function mFunc("FontString:GetShadowColor", pLua, 4);

    const Color& mShadowColor = pFontStringParent_->GetShadowColor();

    mFunc.Push(s_float(mShadowColor.GetR())/255);
    mFunc.Push(s_float(mShadowColor.GetG())/255);
    mFunc.Push(s_float(mShadowColor.GetB())/255);
    mFunc.Push(s_float(mShadowColor.GetA())/255);

    return mFunc.Return();
}

int LuaFontString::_GetShadowOffset( lua_State* pLua )
{
    Lua::Function mFunc("FontString:GetShadowOffset", pLua, 2);

    mFunc.Push(pFontStringParent_->GetShadowXOffset());
    mFunc.Push(pFontStringParent_->GetShadowYOffset());

    return mFunc.Return();
}

int LuaFontString::_GetSpacing( lua_State* pLua )
{
    Lua::Function mFunc("FontString:GetSpacing", pLua, 1);

    mFunc.Push(pFontStringParent_->GetSpacing());

    return mFunc.Return();
}

int LuaFontString::_GetTextColor( lua_State* pLua )
{
    Lua::Function mFunc("FontString:GetTextColor", pLua, 4);

    const Color& mTextColor = pFontStringParent_->GetTextColor();

    mFunc.Push(s_float(mTextColor.GetR())/255);
    mFunc.Push(s_float(mTextColor.GetG())/255);
    mFunc.Push(s_float(mTextColor.GetB())/255);
    mFunc.Push(s_float(mTextColor.GetA())/255);

    return mFunc.Return();
}

int LuaFontString::_SetFont( lua_State* pLua )
{
    Lua::Function mFunc("FontString:SetFont", pLua);
    mFunc.Add(0, "file", Lua::TYPE_STRING, VALUE_STRING);
    mFunc.Add(1, "height", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(3, "flags", Lua::TYPE_STRING, VALUE_STRING, true);

    if (mFunc.Check())
    {
        pFontStringParent_->SetFont(mFunc.Get(0)->GetS(), mFunc.Get(1)->GetUI());
        if (mFunc.IsProvided(2))
        {
            s_str sFlags = mFunc.Get(2)->GetS();
            if (sFlags.Find("OUTLINE") || sFlags.Find("THICKOUTLINE"))
                pFontStringParent_->SetOutlined(true);
            else if (sFlags.IsEmpty())
                pFontStringParent_->SetOutlined(false);
            else
            {
                Warning(mFunc.GetName(),
                    "Unknown flag list : \""+sFlags+"\"."
                );
            }
        }
        else
        {
            pFontStringParent_->SetOutlined(false);
        }
    }

    return mFunc.Return();
}

int LuaFontString::_SetJustifyH( lua_State* pLua )
{
    Lua::Function mFunc("FontString:SetJustifyH", pLua);
    mFunc.Add(0, "justify horizontal", Lua::TYPE_STRING, VALUE_STRING);

    if (mFunc.Check())
    {
        s_str sJustifyH = mFunc.Get(0)->GetS();
        if (sJustifyH == "LEFT")
            pFontStringParent_->SetJustifyH(Text::ALIGN_LEFT);
        else if (sJustifyH == "CENTER")
            pFontStringParent_->SetJustifyH(Text::ALIGN_CENTER);
        else if (sJustifyH == "RIGHT")
            pFontStringParent_->SetJustifyH(Text::ALIGN_CENTER);
        else
        {
            Error(mFunc.GetName(),
                "Unknown justify behavior : \""+sJustifyH+"\"."
            );
        }
    }

    return mFunc.Return();
}

int LuaFontString::_SetShadowColor( lua_State* pLua )
{
    Lua::Function mFunc("FontString:SetShadowColor", pLua);
    mFunc.Add(0, "red", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(1, "green", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(2, "blue", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(3, "alpha", Lua::TYPE_NUMBER, VALUE_FLOAT, true);

    if (mFunc.Check())
    {
        Color mColor;
        if (mFunc.IsProvided(3))
        {
            mColor = Color(
                s_uint(255*mFunc.Get(3)->GetF()),
                s_uint(255*mFunc.Get(0)->GetF()),
                s_uint(255*mFunc.Get(1)->GetF()),
                s_uint(255*mFunc.Get(2)->GetF())
            );
        }
        else
        {
            mColor = Color(
                s_uint(255*mFunc.Get(0)->GetF()),
                s_uint(255*mFunc.Get(1)->GetF()),
                s_uint(255*mFunc.Get(2)->GetF())
            );
        }

        pFontStringParent_->SetShadowColor(mColor);
    }

    return mFunc.Return();
}

int LuaFontString::_SetShadowOffset( lua_State* pLua )
{
    Lua::Function mFunc("FontString:SetShadowOffset", pLua);
    mFunc.Add(0, "x offset", Lua::TYPE_NUMBER, VALUE_INT);
    mFunc.Add(1, "y offset", Lua::TYPE_NUMBER, VALUE_INT);

    if (mFunc.Check())
    {
        pFontStringParent_->SetShadowOffsets(mFunc.Get(0)->GetI(), mFunc.Get(1)->GetI());
    }

    return mFunc.Return();
}

int LuaFontString::_SetSpacing( lua_State* pLua )
{
    Lua::Function mFunc("FontString:SetSpacing", pLua);
    mFunc.Add(0, "spacing", Lua::TYPE_NUMBER, VALUE_FLOAT);

    if (mFunc.Check())
    {
        pFontStringParent_->SetSpacing(mFunc.Get(0)->GetF());
    }

    return mFunc.Return();
}

int LuaFontString::_SetTextColor( lua_State* pLua )
{
    Lua::Function mFunc("FontString:SetTextColor", pLua);
    mFunc.Add(0, "red", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(1, "green", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(2, "blue", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(3, "alpha", Lua::TYPE_NUMBER, VALUE_FLOAT, true);

    if (mFunc.Check())
    {
        Color mColor;
        if (mFunc.IsProvided(3))
        {
            mColor = Color(
                s_uint(255*mFunc.Get(3)->GetF()),
                s_uint(255*mFunc.Get(0)->GetF()),
                s_uint(255*mFunc.Get(1)->GetF()),
                s_uint(255*mFunc.Get(2)->GetF())
            );
        }
        else
        {
            mColor = Color(
                s_uint(255*mFunc.Get(0)->GetF()),
                s_uint(255*mFunc.Get(1)->GetF()),
                s_uint(255*mFunc.Get(2)->GetF())
            );
        }

        pFontStringParent_->SetTextColor(mColor);
    }

    return mFunc.Return();
}

int LuaFontString::_CanNonSpaceWrap( lua_State* pLua )
{
    Lua::Function mFunc("FontString:CanNonSpaceWrap", pLua, 1);

    mFunc.Push(pFontStringParent_->CanNonSpaceWrap());

    return mFunc.Return();
}

int LuaFontString::_GetStringHeight( lua_State* pLua )
{
    Lua::Function mFunc("FontString:GetStringHeight", pLua, 1);

    mFunc.Push(pFontStringParent_->GetStringHeight());

    return mFunc.Return();
}

int LuaFontString::_GetStringWidth( lua_State* pLua )
{
    Lua::Function mFunc("FontString:GetStringWidth", pLua, 1);

    mFunc.Push(pFontStringParent_->GetStringWidth());

    return mFunc.Return();
}

int LuaFontString::_GetText( lua_State* pLua )
{
    Lua::Function mFunc("FontString:GetText", pLua, 1);

    mFunc.Push(pFontStringParent_->GetText());

    return mFunc.Return();
}

int LuaFontString::_SetNonSpaceWrap( lua_State* pLua )
{
    Lua::Function mFunc("FontString:SetNonSpaceWrap", pLua);
    mFunc.Add(0, "can non space wrap", Lua::TYPE_BOOLEAN, VALUE_BOOL);

    if (mFunc.Check())
    {
        pFontStringParent_->SetNonSpaceWrap(mFunc.Get(0)->GetB());
    }

    return mFunc.Return();
}

int LuaFontString::_SetText( lua_State* pLua )
{
    Lua::Function mFunc("FontString:SetText", pLua);
    mFunc.Add(0, "text", Lua::TYPE_STRING, VALUE_STRING);

    if (mFunc.Check())
    {
        pFontStringParent_->SetText(mFunc.Get(0)->GetS());
    }

    return mFunc.Return();
}
