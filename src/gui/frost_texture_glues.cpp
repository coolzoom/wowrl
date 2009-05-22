/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_texture.h"

#include "gui/frost_gui_layeredregion.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaTexture::CLASS_NAME = "GUI::LuaTexture";

LuaTexture::LuaTexture(lua_State* pLua) : LuaLayeredRegion(pLua)
{
    pTextureParent_ = (Texture*)(pParent_.Get());
}

int LuaTexture::_GetBlendMode( lua_State* pLua )
{
    Lua::Function mFunc("Texture:GetBlendMode", pLua, 1);

    TextureBlendMode mBlend = pTextureParent_->GetBlendMode();

    s_str sBlend;
    switch (mBlend)
    {
        case BLEND_NONE : sBlend = "NONE"; break;
        case BLEND_BLEND : sBlend = "BLEND"; break;
        case BLEND_KEY : sBlend = "KEY"; break;
        case BLEND_ADD : sBlend = "ADD"; break;
        case BLEND_MOD : sBlend = "MOD"; break;
    }

    mFunc.Push(Lua::ReturnValue(sBlend));

    return mFunc.Return();
}

int LuaTexture::_GetTexCoord( lua_State* pLua )
{
    Lua::Function mFunc("Texture:GetTexCoord", pLua, 8);

    const s_array<s_float,8>& lCoords = pTextureParent_->GetTexCoord();

    for (int i = 0; i < 8; i++)
        mFunc.Push(Lua::ReturnValue(lCoords.Get(i)));

    return mFunc.Return();
}

int LuaTexture::_GetTexCoordModifiesRect( lua_State* pLua )
{
    Lua::Function mFunc("Texture:GetTexCoordModifiesRect", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pTextureParent_->GetTexCoordModifiesRect()));

    return mFunc.Return();
}

int LuaTexture::_GetTexture( lua_State* pLua )
{
    Lua::Function mFunc("Texture:GetTexture", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pTextureParent_->GetTexture()));

    return mFunc.Return();
}

int LuaTexture::_GetVertexColor( lua_State* pLua )
{
    Lua::Function mFunc("Texture:GetVertexColor", pLua, 0);

    Color mColor = pTextureParent_->GetVertexColor();

    mFunc.Push(Lua::ReturnValue(mColor.GetR()));
    mFunc.Push(Lua::ReturnValue(mColor.GetG()));
    mFunc.Push(Lua::ReturnValue(mColor.GetB()));
    mFunc.Push(Lua::ReturnValue(mColor.GetA()));

    return mFunc.Return();
}

int LuaTexture::_IsDesaturated( lua_State* pLua )
{
    Lua::Function mFunc("Texture:IsDesaturated", pLua, 1);

    mFunc.Push(Lua::ReturnValue(pTextureParent_->IsDesaturated()));

    return mFunc.Return();
}

int LuaTexture::_SetBlendMode( lua_State* pLua )
{
    Lua::Function mFunc("Texture:SetBlendMode", pLua);
    mFunc.Add(0, "blend mode", Lua::TYPE_STRING, VALUE_STRING);
    if (mFunc.Check())
    {
        s_str sBlend = mFunc.Get(0)->GetS();
        TextureBlendMode mBlend;
        if (sBlend == "NONE")
            mBlend = BLEND_NONE;
        else if (sBlend == "BLEND")
            mBlend = BLEND_BLEND;
        else if (sBlend == "KEY")
            mBlend = BLEND_KEY;
        else if (sBlend == "ADD")
            mBlend = BLEND_ADD;
        else if (sBlend == "MOD")
            mBlend = BLEND_MOD;
        else
        {
            Error(mFunc.GetName(),
                "Unknown blending mode : \""+sBlend+"\"."
            );
            return mFunc.Return();
        }

        pTextureParent_->SetBlendMode(mBlend);
    }

    return mFunc.Return();
}

int LuaTexture::_SetDesaturated( lua_State* pLua )
{
    Lua::Function mFunc("Texture:SetDesaturated", pLua, 1);
    mFunc.Add(0, "is desaturated", Lua::TYPE_BOOLEAN, VALUE_BOOL);
    if (mFunc.Check())
    {
        pTextureParent_->SetDesaturated(mFunc.Get(0)->GetB());
    }

    mFunc.Push(Lua::ReturnValue(s_bool(true)));

    return mFunc.Return();
}

int LuaTexture::_SetGradient( lua_State* pLua )
{
    Lua::Function mFunc("Texture:SetGradient", pLua);
    mFunc.Add(0, "orientation", Lua::TYPE_STRING, VALUE_STRING);
    mFunc.Add(1, "min red", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(2, "min green", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(3, "min blue", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(4, "max red", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(5, "max green", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(6, "max blue", Lua::TYPE_NUMBER, VALUE_FLOAT);
    if (mFunc.Check())
    {
        s_str sOrientation = mFunc.Get(0)->GetS();
        GradientOrientation mOrientation;
        if (sOrientation == "HORIZONTAL")
            mOrientation = ORIENTATION_HORIZONTAL;
        else if (sOrientation == "VERTICAL")
            mOrientation = ORIENTATION_VERTICAL;
        else
        {
            Error(mFunc.GetName(),
                "Unknown gradient orientation : \""+sOrientation+"\"."
            );
            return mFunc.Return();
        }

        pTextureParent_->SetGradient(Gradient(
            mOrientation,
            Color(
                s_uint(255*mFunc.Get(1)->GetF()),
                s_uint(255*mFunc.Get(2)->GetF()),
                s_uint(255*mFunc.Get(3)->GetF())
            ),
            Color(
                s_uint(255*mFunc.Get(4)->GetF()),
                s_uint(255*mFunc.Get(5)->GetF()),
                s_uint(255*mFunc.Get(6)->GetF())
            )
        ));
    }

    return mFunc.Return();
}

int LuaTexture::_SetGradientAlpha( lua_State* pLua )
{
    Lua::Function mFunc("Texture:SetGradientAlpha", pLua);
    mFunc.Add(0, "orientation", Lua::TYPE_STRING, VALUE_STRING);
    mFunc.Add(1, "min red", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(2, "min green", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(3, "min blue", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(4, "min alpha", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(5, "max red", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(6, "max green", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(7, "max blue", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(8, "max alpha", Lua::TYPE_NUMBER, VALUE_FLOAT);
    if (mFunc.Check())
    {
        s_str sOrientation = mFunc.Get(0)->GetS();
        GradientOrientation mOrientation;
        if (sOrientation == "HORIZONTAL")
            mOrientation = ORIENTATION_HORIZONTAL;
        else if (sOrientation == "VERTICAL")
            mOrientation = ORIENTATION_VERTICAL;
        else
        {
            Error(mFunc.GetName(),
                "Unknown gradient orientation : \""+sOrientation+"\"."
            );
            return mFunc.Return();
        }

        pTextureParent_->SetGradient(Gradient(
            mOrientation,
            Color(
                s_uint(255*mFunc.Get(4)->GetF()),
                s_uint(255*mFunc.Get(1)->GetF()),
                s_uint(255*mFunc.Get(2)->GetF()),
                s_uint(255*mFunc.Get(3)->GetF())
            ),
            Color(
                s_uint(255*mFunc.Get(8)->GetF()),
                s_uint(255*mFunc.Get(5)->GetF()),
                s_uint(255*mFunc.Get(6)->GetF()),
                s_uint(255*mFunc.Get(7)->GetF())
            )
        ));
    }

    return mFunc.Return();
}

int LuaTexture::_SetTexCoord( lua_State* pLua )
{
    Lua::Function mFunc("Texture:SetTexCoord", pLua);
    mFunc.Add(0, "top-left X", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(1, "top-left Y", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(2, "bottom-right X", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(3, "bottom-right Y", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.NewParamSet();
    mFunc.Add(0, "top-left X", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(1, "top-left Y", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(2, "top-right X", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(3, "top-right Y", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(4, "bottom-right X", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(5, "bottom-right Y", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(6, "bottom-left X", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(7, "bottom-left Y", Lua::TYPE_NUMBER, VALUE_FLOAT);

    if (mFunc.Check())
    {
        if (mFunc.GetParamSetRank() == 0)
        {
            // Only 4 coordinates provided
            pTextureParent_->SetTexCoord(s_array<s_float,4>((
                mFunc.Get(0)->GetF(), mFunc.Get(1)->GetF(),
                mFunc.Get(2)->GetF(), mFunc.Get(3)->GetF()
            )));
        }
        else
        {
            // Or 8
            pTextureParent_->SetTexCoord(s_array<s_float,8>((
                mFunc.Get(0)->GetF(), mFunc.Get(1)->GetF(),
                mFunc.Get(2)->GetF(), mFunc.Get(3)->GetF(),
                mFunc.Get(4)->GetF(), mFunc.Get(5)->GetF(),
                mFunc.Get(6)->GetF(), mFunc.Get(7)->GetF()
            )));
        }
    }

    return mFunc.Return();
}

int LuaTexture::_SetTexCoordModifiesRect( lua_State* pLua )
{
    Lua::Function mFunc("Texture:SetTexCoordModifiesRect", pLua);
    mFunc.Add(0, "does SetTexCoord modifies size", Lua::TYPE_BOOLEAN, VALUE_BOOL);

    if (mFunc.Check())
    {
        pTextureParent_->SetTexCoordModifiesRect(mFunc.Get(0)->GetB());
    }

    return mFunc.Return();
}

int LuaTexture::_SetTexture( lua_State* pLua )
{
    Lua::Function mFunc("Texture:SetTexture", pLua);
    mFunc.Add(0, "texture", Lua::TYPE_STRING, VALUE_STRING);
    mFunc.NewParamSet();
    mFunc.Add(0, "red", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(1, "green", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(2, "blue", Lua::TYPE_NUMBER, VALUE_FLOAT);
    mFunc.Add(3, "alpha", Lua::TYPE_NUMBER, VALUE_FLOAT, true);

    if (mFunc.Check())
    {
        if (mFunc.GetParamSetRank() == 0)
        {
            // Texture name
            pTextureParent_->SetTexture(mFunc.Get(0)->GetS());
        }
        else
        {
            // Texture color
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
            pTextureParent_->SetColor(mColor);
        }
    }

    return mFunc.Return();
}

int LuaTexture::_SetVertexColor( lua_State* pLua )
{
    Lua::Function mFunc("Texture:SetVertexColor", pLua);
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

        pTextureParent_->SetVertexColor(mColor);
    }

    return mFunc.Return();
}
