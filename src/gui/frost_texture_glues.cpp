/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_texture.h"

#include "gui/frost_layeredregion.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str LuaTexture::CLASS_NAME = "GUI::LuaTexture";

LuaTexture::LuaTexture(lua_State* pLua) : LuaLayeredRegion(pLua)
{
    pTextureParent_ = s_ptr<Texture>::DynamicCast(pParent_);
    if (pParent_ && !pTextureParent_)
    {
        throw Exception(CLASS_NAME, "Dynamic cast failed !");
    }
}

int LuaTexture::_GetBlendMode( lua_State* pLua )
{
    Lua::Function mFunc("Texture:GetBlendMode", pLua, 1);

    Texture::BlendMode mBlend = pTextureParent_->GetBlendMode();

    s_str sBlend;
    switch (mBlend)
    {
        case Texture::BLEND_NONE : sBlend = "NONE"; break;
        case Texture::BLEND_BLEND : sBlend = "BLEND"; break;
        case Texture::BLEND_KEY : sBlend = "KEY"; break;
        case Texture::BLEND_ADD : sBlend = "ADD"; break;
        case Texture::BLEND_MOD : sBlend = "MOD"; break;
    }

    mFunc.Push(sBlend);

    return mFunc.Return();
}

int LuaTexture::_GetTexCoord( lua_State* pLua )
{
    Lua::Function mFunc("Texture:GetTexCoord", pLua, 8);

    const s_array<s_float,8>& lCoords = pTextureParent_->GetTexCoord();

    for (uint i = 0; i < 8; ++i)
        mFunc.Push(lCoords.Get(i));

    return mFunc.Return();
}

int LuaTexture::_GetTexCoordModifiesRect( lua_State* pLua )
{
    Lua::Function mFunc("Texture:GetTexCoordModifiesRect", pLua, 1);

    mFunc.Push(pTextureParent_->GetTexCoordModifiesRect());

    return mFunc.Return();
}

int LuaTexture::_GetTexture( lua_State* pLua )
{
    Lua::Function mFunc("Texture:GetTexture", pLua, 1);

    mFunc.Push(pTextureParent_->GetTexture());

    return mFunc.Return();
}

int LuaTexture::_GetVertexColor( lua_State* pLua )
{
    Lua::Function mFunc("Texture:GetVertexColor", pLua, 4);

    Color mColor = pTextureParent_->GetVertexColor();

    mFunc.Push(mColor.GetR());
    mFunc.Push(mColor.GetG());
    mFunc.Push(mColor.GetB());
    mFunc.Push(mColor.GetA());

    return mFunc.Return();
}

int LuaTexture::_IsDesaturated( lua_State* pLua )
{
    Lua::Function mFunc("Texture:IsDesaturated", pLua, 1);

    mFunc.Push(pTextureParent_->IsDesaturated());

    return mFunc.Return();
}

int LuaTexture::_SetBlendMode( lua_State* pLua )
{
    Lua::Function mFunc("Texture:SetBlendMode", pLua);
    mFunc.Add(0, "blend mode", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        s_str sBlend = mFunc.Get(0)->GetString();
        Texture::BlendMode mBlend;
        if (sBlend == "NONE")
            mBlend = Texture::BLEND_NONE;
        else if (sBlend == "BLEND")
            mBlend = Texture::BLEND_BLEND;
        else if (sBlend == "KEY")
            mBlend = Texture::BLEND_KEY;
        else if (sBlend == "ADD")
            mBlend = Texture::BLEND_ADD;
        else if (sBlend == "MOD")
            mBlend = Texture::BLEND_MOD;
        else
        {
            Warning(mFunc.GetName(),
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
    mFunc.Add(0, "is desaturated", Lua::TYPE_BOOLEAN);
    if (mFunc.Check())
    {
        pTextureParent_->SetDesaturated(mFunc.Get(0)->GetBool());
    }

    mFunc.Push(s_bool(true));

    return mFunc.Return();
}

int LuaTexture::_SetGradient( lua_State* pLua )
{
    Lua::Function mFunc("Texture:SetGradient", pLua);
    mFunc.Add(0, "orientation", Lua::TYPE_STRING);
    mFunc.Add(1, "min red", Lua::TYPE_NUMBER);
    mFunc.Add(2, "min green", Lua::TYPE_NUMBER);
    mFunc.Add(3, "min blue", Lua::TYPE_NUMBER);
    mFunc.Add(4, "max red", Lua::TYPE_NUMBER);
    mFunc.Add(5, "max green", Lua::TYPE_NUMBER);
    mFunc.Add(6, "max blue", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        s_str sOrientation = mFunc.Get(0)->GetString();
        Gradient::Orientation mOrientation;
        if (sOrientation == "HORIZONTAL")
            mOrientation = Gradient::HORIZONTAL;
        else if (sOrientation == "VERTICAL")
            mOrientation = Gradient::VERTICAL;
        else
        {
            Warning(mFunc.GetName(),
                "Unknown gradient orientation : \""+sOrientation+"\"."
            );
            return mFunc.Return();
        }

        pTextureParent_->SetGradient(Gradient(
            mOrientation,
            Color(
                mFunc.Get(1)->GetNumber(),
                mFunc.Get(2)->GetNumber(),
                mFunc.Get(3)->GetNumber()
            ),
            Color(
                mFunc.Get(4)->GetNumber(),
                mFunc.Get(5)->GetNumber(),
                mFunc.Get(6)->GetNumber()
            )
        ));
    }

    return mFunc.Return();
}

int LuaTexture::_SetGradientAlpha( lua_State* pLua )
{
    Lua::Function mFunc("Texture:SetGradientAlpha", pLua);
    mFunc.Add(0, "orientation", Lua::TYPE_STRING);
    mFunc.Add(1, "min red", Lua::TYPE_NUMBER);
    mFunc.Add(2, "min green", Lua::TYPE_NUMBER);
    mFunc.Add(3, "min blue", Lua::TYPE_NUMBER);
    mFunc.Add(4, "min alpha", Lua::TYPE_NUMBER);
    mFunc.Add(5, "max red", Lua::TYPE_NUMBER);
    mFunc.Add(6, "max green", Lua::TYPE_NUMBER);
    mFunc.Add(7, "max blue", Lua::TYPE_NUMBER);
    mFunc.Add(8, "max alpha", Lua::TYPE_NUMBER);
    if (mFunc.Check())
    {
        s_str sOrientation = mFunc.Get(0)->GetString();
        Gradient::Orientation mOrientation;
        if (sOrientation == "HORIZONTAL")
            mOrientation = Gradient::HORIZONTAL;
        else if (sOrientation == "VERTICAL")
            mOrientation = Gradient::VERTICAL;
        else
        {
            Warning(mFunc.GetName(),
                "Unknown gradient orientation : \""+sOrientation+"\"."
            );
            return mFunc.Return();
        }

        pTextureParent_->SetGradient(Gradient(
            mOrientation,
            Color(
                mFunc.Get(4)->GetNumber(),
                mFunc.Get(1)->GetNumber(),
                mFunc.Get(2)->GetNumber(),
                mFunc.Get(3)->GetNumber()
            ),
            Color(
                mFunc.Get(8)->GetNumber(),
                mFunc.Get(5)->GetNumber(),
                mFunc.Get(6)->GetNumber(),
                mFunc.Get(7)->GetNumber()
            )
        ));
    }

    return mFunc.Return();
}

int LuaTexture::_SetTexCoord( lua_State* pLua )
{
    Lua::Function mFunc("Texture:SetTexCoord", pLua);
    mFunc.Add(0, "top-left X", Lua::TYPE_NUMBER);
    mFunc.Add(1, "top-left Y", Lua::TYPE_NUMBER);
    mFunc.Add(2, "bottom-right X", Lua::TYPE_NUMBER);
    mFunc.Add(3, "bottom-right Y", Lua::TYPE_NUMBER);
    mFunc.NewParamSet();
    mFunc.Add(0, "top-left X", Lua::TYPE_NUMBER);
    mFunc.Add(1, "top-left Y", Lua::TYPE_NUMBER);
    mFunc.Add(2, "top-right X", Lua::TYPE_NUMBER);
    mFunc.Add(3, "top-right Y", Lua::TYPE_NUMBER);
    mFunc.Add(4, "bottom-right X", Lua::TYPE_NUMBER);
    mFunc.Add(5, "bottom-right Y", Lua::TYPE_NUMBER);
    mFunc.Add(6, "bottom-left X", Lua::TYPE_NUMBER);
    mFunc.Add(7, "bottom-left Y", Lua::TYPE_NUMBER);

    if (mFunc.Check())
    {
        if (mFunc.GetParamSetRank() == 0)
        {
            // Only 4 coordinates provided
            pTextureParent_->SetTexCoord(s_array<s_float,4>((
                mFunc.Get(0)->GetNumber(), mFunc.Get(1)->GetNumber(),
                mFunc.Get(2)->GetNumber(), mFunc.Get(3)->GetNumber()
            )));
        }
        else
        {
            // Or 8
            pTextureParent_->SetTexCoord(s_array<s_float,8>((
                mFunc.Get(0)->GetNumber(), mFunc.Get(1)->GetNumber(),
                mFunc.Get(2)->GetNumber(), mFunc.Get(3)->GetNumber(),
                mFunc.Get(4)->GetNumber(), mFunc.Get(5)->GetNumber(),
                mFunc.Get(6)->GetNumber(), mFunc.Get(7)->GetNumber()
            )));
        }
    }

    return mFunc.Return();
}

int LuaTexture::_SetTexCoordModifiesRect( lua_State* pLua )
{
    Lua::Function mFunc("Texture:SetTexCoordModifiesRect", pLua);
    mFunc.Add(0, "does SetTexCoord modifies size", Lua::TYPE_BOOLEAN);

    if (mFunc.Check())
    {
        pTextureParent_->SetTexCoordModifiesRect(mFunc.Get(0)->GetBool());
    }

    return mFunc.Return();
}

int LuaTexture::_SetTexture( lua_State* pLua )
{
    Lua::Function mFunc("Texture:SetTexture", pLua);
    mFunc.Add(0, "texture", Lua::TYPE_STRING);
    mFunc.NewParamSet();
    mFunc.Add(0, "red", Lua::TYPE_NUMBER);
    mFunc.Add(1, "green", Lua::TYPE_NUMBER);
    mFunc.Add(2, "blue", Lua::TYPE_NUMBER);
    mFunc.Add(3, "alpha", Lua::TYPE_NUMBER, true);

    if (mFunc.Check())
    {
        if (mFunc.GetParamSetRank() == 0)
        {
            // Texture name
            pTextureParent_->SetTexture(mFunc.Get(0)->GetString());
        }
        else
        {
            // Texture color
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
            pTextureParent_->SetColor(mColor);
        }
    }

    return mFunc.Return();
}

int LuaTexture::_SetVertexColor( lua_State* pLua )
{
    Lua::Function mFunc("Texture:SetVertexColor", pLua);
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

        pTextureParent_->SetVertexColor(mColor);
    }

    return mFunc.Return();
}
