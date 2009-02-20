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
    mFunc.Add(0, "blendMode", Lua::TYPE_STRING, VALUE_STRING);
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
    mFunc.Add(0, "isDesaturated", Lua::TYPE_BOOLEAN, VALUE_BOOL);
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
    mFunc.Add(1, "minR", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(2, "minG", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(3, "minB", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(4, "maxR", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(5, "maxG", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(6, "maxB", Lua::TYPE_NUMBER, VALUE_UINT);
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
            Color(mFunc.Get(1)->GetUI(), mFunc.Get(2)->GetUI(), mFunc.Get(3)->GetUI()),
            Color(mFunc.Get(4)->GetUI(), mFunc.Get(5)->GetUI(), mFunc.Get(6)->GetUI())
        ));
    }

    return mFunc.Return();
}

int LuaTexture::_SetGradientAlpha( lua_State* pLua )
{
    Lua::Function mFunc("Texture:SetGradientAlpha", pLua);
    mFunc.Add(0, "orientation", Lua::TYPE_STRING, VALUE_STRING);
    mFunc.Add(1, "minR", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(2, "minG", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(3, "minB", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(4, "minA", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(5, "maxR", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(6, "maxG", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(7, "maxB", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(8, "maxA", Lua::TYPE_NUMBER, VALUE_UINT);
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
            Color(mFunc.Get(4)->GetUI(), mFunc.Get(1)->GetUI(), mFunc.Get(2)->GetUI(), mFunc.Get(3)->GetUI()),
            Color(mFunc.Get(8)->GetUI(), mFunc.Get(5)->GetUI(), mFunc.Get(6)->GetUI(), mFunc.Get(7)->GetUI())
        ));
    }

    return mFunc.Return();
}

int LuaTexture::_SetTexCoord( lua_State* pLua )
{
    Lua::Function mFunc("Texture:SetTexCoord", pLua);
    mFunc.Add(0, "x1", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(1, "y1", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(2, "x2", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(3, "y2", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.NewParamSet();
    mFunc.Add(0, "x1", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(1, "y1", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(2, "x2", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(3, "y2", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(4, "x3", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(5, "y3", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(6, "x4", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(7, "y4", Lua::TYPE_NUMBER, VALUE_UINT);

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
    mFunc.Add(0, "modifies", Lua::TYPE_BOOLEAN, VALUE_BOOL);

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
    mFunc.Add(0, "r", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(1, "g", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(2, "b", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(3, "a", Lua::TYPE_NUMBER, VALUE_UINT, true);

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
                    mFunc.Get(3)->GetUI(),
                    mFunc.Get(0)->GetUI(),
                    mFunc.Get(1)->GetUI(),
                    mFunc.Get(2)->GetUI()
                );
            }
            else
            {
                mColor = Color(
                    mFunc.Get(0)->GetUI(),
                    mFunc.Get(1)->GetUI(),
                    mFunc.Get(2)->GetUI()
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
    mFunc.Add(0, "r", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(1, "g", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(2, "b", Lua::TYPE_NUMBER, VALUE_UINT);
    mFunc.Add(3, "a", Lua::TYPE_NUMBER, VALUE_UINT, true);

    Color mColor;
    if (mFunc.IsProvided(3))
    {
        mColor = Color(
            mFunc.Get(3)->GetUI(),
            mFunc.Get(0)->GetUI(),
            mFunc.Get(1)->GetUI(),
            mFunc.Get(2)->GetUI()
        );
    }
    else
    {
        mColor = Color(
            mFunc.Get(0)->GetUI(),
            mFunc.Get(1)->GetUI(),
            mFunc.Get(2)->GetUI()
        );
    }
    pTextureParent_->SetColor(mColor);

    return mFunc.Return();
}
