/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_statusbar.h"
#include "gui/frost_texture.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaStatusBar::CLASS_NAME = "GUI::LuaStatusBar";

LuaStatusBar::LuaStatusBar(lua_State* pLua) : LuaFrame(pLua)
{
    pStatusBarParent_ = s_ptr<StatusBar>::DynamicCast(pParent_);
    if (pParent_ && !pStatusBarParent_)
    {
        throw Exception(CLASS_NAME, "Dynamic cast failed !");
    }
}

int LuaStatusBar::_GetMinMaxValues( lua_State* pLua )
{
    Lua::Function mFunc("StatusBar:GetMinMaxValues", pLua, 2);

    mFunc.Push(pStatusBarParent_->GetMinValue());
    mFunc.Push(pStatusBarParent_->GetMaxValue());

    return mFunc.Return();
}

int LuaStatusBar::_GetOrientation( lua_State* pLua )
{
    Lua::Function mFunc("StatusBar:GetOrientation", pLua, 1);

    // TODO : Implement StatusBar::GetOrientation
    Warning(CLASS_NAME, "StatusBar:GetOrientation is not yet implemented !");

    mFunc.PushNil();

    return mFunc.Return();
}

int LuaStatusBar::_GetStatusBarColor( lua_State* pLua )
{
    Lua::Function mFunc("StatusBar:GetStatusBarColor", pLua, 4);

    const Color& mColor = pStatusBarParent_->GetBarColor();

    mFunc.Push(s_float(mColor.GetR())/255.0f);
    mFunc.Push(s_float(mColor.GetG())/255.0f);
    mFunc.Push(s_float(mColor.GetB())/255.0f);
    mFunc.Push(s_float(mColor.GetA())/255.0f);

    return mFunc.Return();
}

int LuaStatusBar::_GetStatusBarTexture( lua_State* pLua )
{
    Lua::Function mFunc("StatusBar:GetStatusBarTexture", pLua, 1);

    s_ptr<Texture> pTexture = pStatusBarParent_->GetBarTexture();
    if (pTexture)
    {
        pTexture->PushOnLua(mFunc.GetState());
        mFunc.NotifyPushed();
    }
    else
        mFunc.PushNil();

    return mFunc.Return();
}

int LuaStatusBar::_GetValue( lua_State* pLua )
{
    Lua::Function mFunc("StatusBar:GetValue", pLua, 1);

    mFunc.Push(pStatusBarParent_->GetValue());

    return mFunc.Return();
}

int LuaStatusBar::_SetMinMaxValues( lua_State* pLua )
{
    Lua::Function mFunc("StatusBar:SetMinMaxValues", pLua);
    mFunc.Add(0, "min", Lua::TYPE_NUMBER);
    mFunc.Add(1, "max", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pStatusBarParent_->SetMinMaxValues(
            mFunc.Get(0)->GetNumber(),
            mFunc.Get(1)->GetNumber()
        );
    }

    return mFunc.Return();
}

int LuaStatusBar::_SetOrientation( lua_State* pLua )
{
    Lua::Function mFunc("StatusBar:SetOrientation", pLua);

    // TODO : Implement StatusBar::SetOrientation
    Warning(CLASS_NAME, "StatusBar:SetOrientation is not yet implemented !");

    mFunc.PushNil();

    return mFunc.Return();
}

int LuaStatusBar::_SetStatusBarColor( lua_State* pLua )
{
    Lua::Function mFunc("StatusBar:SetStatusBarColor", pLua);
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

        pStatusBarParent_->SetBarColor(mColor);
    }

    return mFunc.Return();
}

int LuaStatusBar::_SetStatusBarTexture( lua_State* pLua )
{
    Lua::Function mFunc("StatusBar:SetStatusBarTexture", pLua);
    mFunc.Add(0, "texture", Lua::TYPE_USERDATA);
    if (mFunc.Check())
    {
        s_ptr<LuaTexture> pLuaTexture = mFunc.GetState()->Get<LuaTexture>();
        if (pLuaTexture)
        {
            s_ptr<Texture> pTexture = s_ptr<Texture>::DynamicCast(pLuaTexture->GetParent());
            pStatusBarParent_->SetBarTexture(pTexture);
        }
    }

    return mFunc.Return();
}

int LuaStatusBar::_SetValue( lua_State* pLua )
{
    Lua::Function mFunc("StatusBar:SetValue", pLua);
    mFunc.Add(0, "value", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pStatusBarParent_->SetValue(mFunc.Get(0)->GetNumber());
    }

    return mFunc.Return();
}
