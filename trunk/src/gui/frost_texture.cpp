/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_texture.h"

#include "gui/frost_gui_layeredregion.h"
#include "gui/frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str Texture::CLASS_NAME = "GUI::Texture";

Texture::Texture() : LayeredRegion()
{
}

// TODO : implémenter

void Texture::CreateGlue()
{
    s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
    pLua->PushString(sName_);
    LuaTexture* pNewGlue;
    pGlue_ = pNewGlue = new LuaTexture(pLua->GetState());
    Lunar<LuaTexture>::push(pLua->GetState(), pNewGlue);
    pLua->SetGlobal(sName_);
}

TextureBlendMode Texture::GetBlendMode() const
{
}

const s_array<s_float,8>& Texture::GetTexCoord() const
{
}

const s_bool& Texture::GetTexCoordModifiesRect() const
{
}

const s_str& Texture::GetTexture() const
{
}

const Color& Texture::GetVertexColor() const
{
}

const s_bool& Texture::IsDesaturated() const
{
}

void Texture::SetBlendMode( TextureBlendMode mBlendMode )
{
}

void Texture::SetDesaturated( const s_bool& bIsDesaturated ) const
{
}

void Texture::SetGradient( const Gradient& mGradient )
{
}

void Texture::SetTexCoord( const s_array<s_float,4>& lCoordinates )
{
}

void Texture::SetTexCoord( const s_array<s_float,8>& lCoordinates )
{
}

void Texture::SetTexCoordModifiesRect( const s_bool& bTexCoordModifiesRect )
{
}

void Texture::SetTexture( const s_str& sFile )
{
}

void Texture::SetColor( const Color& mColor )
{
}

void Texture::SetVertexColor( const Color& mColor )
{
}
