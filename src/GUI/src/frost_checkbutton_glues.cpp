/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_checkbutton.h"

#include "frost_frame.h"
#include "frost_texture.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaCheckButton::CLASS_NAME = "GUI::LuaCheckButton";

LuaCheckButton::LuaCheckButton(lua_State* pLua) : LuaButton(pLua)
{
    pCheckButtonParent_ = s_ptr<CheckButton>::DynamicCast(pParent_);
    if (pParent_ && !pCheckButtonParent_)
    {
        throw Exception(CLASS_NAME, "Dynamic cast failed !");
    }
}

int LuaCheckButton::_IsChecked(lua_State* pLua)
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("CheckButton:IsChecked", pLua, 1);

    mFunc.Push(pCheckButtonParent_->IsChecked());

    return mFunc.Return();
}

int LuaCheckButton::_GetCheckedTexture(lua_State* pLua)
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("CheckButton:GetCheckedTexture", pLua, 1);

    s_ptr<Texture> pTexture = pCheckButtonParent_->GetCheckedTexture();
    if (pTexture)
    {
        pTexture->PushOnLua(mFunc.GetState());
        mFunc.NotifyPushed();
    }
    else
        mFunc.PushNil();

    return mFunc.Return();
}

int LuaCheckButton::_GetDisabledCheckedTexture(lua_State* pLua)
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("CheckButton:GetDisabledCheckedTexture", pLua, 1);

    s_ptr<Texture> pTexture = pCheckButtonParent_->GetDisabledCheckedTexture();
    if (pTexture)
    {
        pTexture->PushOnLua(mFunc.GetState());
        mFunc.NotifyPushed();
    }
    else
        mFunc.PushNil();

    return mFunc.Return();
}

int LuaCheckButton::_SetChecked(lua_State* pLua)
{
    if (!CheckParent_())
        return 0;

    // This function is a bit trickier, as it doesn't require any particular
    // type for its argument. Whatever it is, Lua can convert it to a boolean
    // value, and we use this value to check/uncheck the button.

    if (lua_gettop(pLua) > 0)
    {
        if (lua_toboolean(pLua, 1) == 0)
            pCheckButtonParent_->UnCheck();
        else
            pCheckButtonParent_->Check();
    }
    else
        pCheckButtonParent_->Check();

    return 0;
}

int LuaCheckButton::_SetCheckedTexture(lua_State* pLua)
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("CheckButton:SetCheckedTexture", pLua);
    mFunc.Add(0, "texture", Lua::TYPE_USERDATA);
    if (mFunc.Check())
    {
        s_ptr<LuaTexture> pLuaTexture = mFunc.GetState()->Get<LuaTexture>();
        if (pLuaTexture)
        {
            s_ptr<Texture> pTexture = s_ptr<Texture>::DynamicCast(pLuaTexture->GetParent());
            pCheckButtonParent_->SetCheckedTexture(pTexture);
        }
    }

    return mFunc.Return();
}

int LuaCheckButton::_SetDisabledCheckedTexture(lua_State* pLua)
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("CheckButton:SetDisabledCheckedTexture", pLua);
    mFunc.Add(0, "texture", Lua::TYPE_USERDATA);
    if (mFunc.Check())
    {
        s_ptr<LuaTexture> pLuaTexture = mFunc.GetState()->Get<LuaTexture>();
        if (pLuaTexture)
        {
            s_ptr<Texture> pTexture = s_ptr<Texture>::DynamicCast(pLuaTexture->GetParent());
            pCheckButtonParent_->SetDisabledCheckedTexture(pTexture);
        }
    }

    return mFunc.Return();
}
