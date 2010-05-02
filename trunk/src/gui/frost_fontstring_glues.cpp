/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_fontstring.h"

#include "gui/frost_layeredregion.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaFontString::CLASS_NAME = "GUI::LuaFontString";

LuaFontString::LuaFontString(lua_State* pLua) : LuaLayeredRegion(pLua)
{
    pFontStringParent_ = s_ptr<FontString>::DynamicCast(pParent_);
    if (pParent_ && !pFontStringParent_)
    {
        throw Exception(CLASS_NAME, "Dynamic cast failed !");
    }
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

    mFunc.Push(mShadowColor.GetR());
    mFunc.Push(mShadowColor.GetG());
    mFunc.Push(mShadowColor.GetB());
    mFunc.Push(mShadowColor.GetA());

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

    mFunc.Push(mTextColor.GetR());
    mFunc.Push(mTextColor.GetG());
    mFunc.Push(mTextColor.GetB());
    mFunc.Push(mTextColor.GetA());

    return mFunc.Return();
}

int LuaFontString::_SetFont( lua_State* pLua )
{
    Lua::Function mFunc("FontString:SetFont", pLua);
    mFunc.Add(0, "file", Lua::TYPE_STRING);
    mFunc.Add(1, "height", Lua::TYPE_NUMBER);
    mFunc.Add(3, "flags", Lua::TYPE_STRING, true);

    if (mFunc.Check())
    {
        pFontStringParent_->SetFont(mFunc.Get(0)->GetString(), s_uint(mFunc.Get(1)->GetNumber()));
        if (mFunc.IsProvided(2))
        {
            s_str sFlags = mFunc.Get(2)->GetString();
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
    mFunc.Add(0, "justify horizontal", Lua::TYPE_STRING);

    if (mFunc.Check())
    {
        s_str sJustifyH = mFunc.Get(0)->GetString();
        if (sJustifyH == "LEFT")
            pFontStringParent_->SetJustifyH(Text::ALIGN_LEFT);
        else if (sJustifyH == "CENTER")
            pFontStringParent_->SetJustifyH(Text::ALIGN_CENTER);
        else if (sJustifyH == "RIGHT")
            pFontStringParent_->SetJustifyH(Text::ALIGN_RIGHT);
        else
        {
            Warning(mFunc.GetName(),
                "Unknown justify behavior : \""+sJustifyH+"\"."
            );
        }
    }

    return mFunc.Return();
}

int LuaFontString::_SetShadowColor( lua_State* pLua )
{
    Lua::Function mFunc("FontString:SetShadowColor", pLua);
    mFunc.Add(0, "red", Lua::TYPE_NUMBER);
    mFunc.Add(1, "green", Lua::TYPE_NUMBER);
    mFunc.Add(2, "blue", Lua::TYPE_NUMBER);
    mFunc.Add(3, "alpha", Lua::TYPE_NUMBER, true);

    if (mFunc.Check())
    {
        Color mColor;
        if (mFunc.IsProvided(3))
        {
            mColor = Color(
                mFunc.Get(3)->GetNumber(),
                mFunc.Get(0)->GetNumber(),
                mFunc.Get(1)->GetNumber(),
                mFunc.Get(2)->GetNumber()
            );
        }
        else
        {
            mColor = Color(
                mFunc.Get(0)->GetNumber(),
                mFunc.Get(1)->GetNumber(),
                mFunc.Get(2)->GetNumber()
            );
        }

        pFontStringParent_->SetShadowColor(mColor);
    }

    return mFunc.Return();
}

int LuaFontString::_SetShadowOffset( lua_State* pLua )
{
    Lua::Function mFunc("FontString:SetShadowOffset", pLua);
    mFunc.Add(0, "x offset", Lua::TYPE_NUMBER);
    mFunc.Add(1, "y offset", Lua::TYPE_NUMBER);

    if (mFunc.Check())
    {
        pFontStringParent_->SetShadowOffsets(
            s_int(mFunc.Get(0)->GetNumber()),
            s_int(mFunc.Get(1)->GetNumber())
        );
    }

    return mFunc.Return();
}

int LuaFontString::_SetSpacing( lua_State* pLua )
{
    Lua::Function mFunc("FontString:SetSpacing", pLua);
    mFunc.Add(0, "spacing", Lua::TYPE_NUMBER);

    if (mFunc.Check())
    {
        pFontStringParent_->SetSpacing(mFunc.Get(0)->GetNumber());
    }

    return mFunc.Return();
}

int LuaFontString::_SetTextColor( lua_State* pLua )
{
    Lua::Function mFunc("FontString:SetTextColor", pLua);
    mFunc.Add(0, "red", Lua::TYPE_NUMBER);
    mFunc.Add(1, "green", Lua::TYPE_NUMBER);
    mFunc.Add(2, "blue", Lua::TYPE_NUMBER);
    mFunc.Add(3, "alpha", Lua::TYPE_NUMBER, true);

    if (mFunc.Check())
    {
        Color mColor;
        if (mFunc.IsProvided(3))
        {
            mColor = Color(
                mFunc.Get(3)->GetNumber(),
                mFunc.Get(0)->GetNumber(),
                mFunc.Get(1)->GetNumber(),
                mFunc.Get(2)->GetNumber()
            );
        }
        else
        {
            mColor = Color(
                mFunc.Get(0)->GetNumber(),
                mFunc.Get(1)->GetNumber(),
                mFunc.Get(2)->GetNumber()
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
    mFunc.Add(0, "can non space wrap", Lua::TYPE_BOOLEAN);

    if (mFunc.Check())
    {
        pFontStringParent_->SetNonSpaceWrap(mFunc.Get(0)->GetBool());
    }

    return mFunc.Return();
}

int LuaFontString::_SetText( lua_State* pLua )
{
    Lua::Function mFunc("FontString:SetText", pLua);
    mFunc.Add(0, "text", Lua::TYPE_STRING);
    mFunc.Add(0, "number", Lua::TYPE_NUMBER);
    mFunc.Add(0, "bool", Lua::TYPE_BOOLEAN);

    if (mFunc.Check())
    {
        s_str sText;
        if (mFunc.Get(0)->GetType() == Lua::TYPE_STRING)
            sText = mFunc.Get(0)->GetString();
        else if (mFunc.Get(0)->GetType() == Lua::TYPE_NUMBER)
            sText = s_str(mFunc.Get(0)->GetNumber());
        else if (mFunc.Get(0)->GetType() == Lua::TYPE_BOOLEAN)
            sText = s_str(mFunc.Get(0)->GetBool());

        pFontStringParent_->SetText(sText);
    }

    return mFunc.Return();
}
