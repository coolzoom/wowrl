/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_slider.h"

#include "frost_texture.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaSlider::CLASS_NAME = "GUI::LuaSlider";

LuaSlider::LuaSlider(lua_State* pLua) : LuaFrame(pLua)
{
    pSliderParent_ = s_ptr<Slider>::DynamicCast(pParent_);
    if (pParent_ && !pSliderParent_)
    {
        throw Exception(CLASS_NAME, "Dynamic cast failed !");
    }
}

int LuaSlider::_AllowClicksOutsideThumb(lua_State* pLua)
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("Slider:AllowClicksOutsideThumb", pLua);
    mFunc.Add(0, "allow", Lua::TYPE_BOOLEAN);
    if (mFunc.Check())
    {
        pSliderParent_->SetAllowClicksOutsideThumb(mFunc.Get(0)->GetBool());
    }

    return mFunc.Return();
}

int LuaSlider::_GetMaxValue(lua_State* pLua)
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("Slider:GetMaxValue", pLua, 1);

    mFunc.Push(pSliderParent_->GetMaxValue());

    return mFunc.Return();
}

int LuaSlider::_GetMinValue(lua_State* pLua)
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("Slider:GetMinValue", pLua, 1);

    mFunc.Push(pSliderParent_->GetMinValue());

    return mFunc.Return();
}

int LuaSlider::_GetMinMaxValues(lua_State* pLua)
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("Slider:GetMinMaxValues", pLua, 2);

    mFunc.Push(pSliderParent_->GetMinValue());
    mFunc.Push(pSliderParent_->GetMaxValue());

    return mFunc.Return();
}

int LuaSlider::_GetOrientation(lua_State* pLua)
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("Slider:GetOrientation", pLua, 1);

    switch (pSliderParent_->GetOrientation())
    {
        case Slider::ORIENT_VERTICAL   : mFunc.Push(s_str("VERTICAL"));
        case Slider::ORIENT_HORIZONTAL : mFunc.Push(s_str("HORIZONTAL"));
    }

    return mFunc.Return();
}

int LuaSlider::_GetThumbTexture(lua_State* pLua)
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("Slider:GetThumbTexture", pLua, 1);

    s_ptr<Texture> pTexture = pSliderParent_->GetThumbTexture();
    if (pTexture)
    {
        pTexture->PushOnLua(mFunc.GetState());
        mFunc.NotifyPushed();
    }

    return mFunc.Return();
}

int LuaSlider::_GetValue(lua_State* pLua)
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("Slider:GetValue", pLua, 1);

    mFunc.Push(pSliderParent_->GetValue());

    return mFunc.Return();
}

int LuaSlider::_GetValueStep(lua_State* pLua)
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("Slider:GetValueStep", pLua, 1);

    mFunc.Push(pSliderParent_->GetValueStep());

    return mFunc.Return();
}

int LuaSlider::_SetMaxValue(lua_State* pLua)
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("Slider:SetMaxValue", pLua);
    mFunc.Add(0, "max", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pSliderParent_->SetMaxValue(mFunc.Get(0)->GetNumber());
    }

    return mFunc.Return();
}

int LuaSlider::_SetMinValue(lua_State* pLua)
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("Slider:SetMinValue", pLua);
    mFunc.Add(0, "min", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pSliderParent_->SetMinValue(mFunc.Get(0)->GetNumber());
    }

    return mFunc.Return();
}

int LuaSlider::_SetMinMaxValues(lua_State* pLua)
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("Slider:SetMinMaxValues", pLua);
    mFunc.Add(0, "min", Lua::TYPE_NUMBER);
    mFunc.Add(1, "max", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pSliderParent_->SetMinMaxValues(
            mFunc.Get(0)->GetNumber(),
            mFunc.Get(1)->GetNumber()
        );
    }

    return mFunc.Return();
}

int LuaSlider::_SetOrientation(lua_State* pLua)
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("Slider:SetOrientation", pLua);
    mFunc.Add(0, "value", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pSliderParent_->SetValue(mFunc.Get(0)->GetNumber());
    }

    return mFunc.Return();
}

int LuaSlider::_SetThumbTexture(lua_State* pLua)
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("Slider:SetThumbTexture", pLua);
    mFunc.Add(0, "texture", Lua::TYPE_USERDATA);
    if (mFunc.Check())
    {
        s_ptr<LuaTexture> pLuaTexture = mFunc.GetState()->Get<LuaTexture>();
        if (pLuaTexture)
        {
            s_ptr<Texture> pTexture = s_ptr<Texture>::DynamicCast(pLuaTexture->GetParent());
            pSliderParent_->SetThumbTexture(pTexture);
        }
    }

    return mFunc.Return();
}

int LuaSlider::_SetValueStep(lua_State* pLua)
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("Slider:SetValueStep", pLua);
    mFunc.Add(0, "value step", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        pSliderParent_->SetValueStep(mFunc.Get(0)->GetNumber());
    }

    return mFunc.Return();
}

int LuaSlider::_SetValue(lua_State* pLua)
{
    if (!CheckParent_())
        return 0;

    Lua::Function mFunc("Slider:SetValue", pLua);
    mFunc.Add(0, "value", Lua::TYPE_NUMBER);
    mFunc.Add(1, "silent", Lua::TYPE_BOOLEAN, true);
    if (mFunc.Check())
    {
        if (mFunc.IsProvided(1))
            pSliderParent_->SetValue(mFunc.Get(0)->GetNumber(), mFunc.Get(1)->GetBool());
        else
            pSliderParent_->SetValue(mFunc.Get(0)->GetNumber());
    }

    return mFunc.Return();
}
