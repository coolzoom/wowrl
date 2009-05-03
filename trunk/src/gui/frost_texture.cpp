/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_texture.h"

#include "gui/frost_gui_layeredregion.h"
#include "gui/frost_guimanager.h"
#include "frost_materialmanager.h"
#include "frost_material.h"
#include "frost_spritemanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str Texture::CLASS_NAME = "GUI::Texture";

Texture::Texture() : LayeredRegion()
{
}

void Texture::Render()
{
    pSprite_->Render2V(
        s_float(lBorderList_[BORDER_LEFT]), s_float(lBorderList_[BORDER_TOP]),
        s_float(lBorderList_[BORDER_RIGHT]), s_float(lBorderList_[BORDER_BOTTOM])
    );
}

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
    return mBlendMode_;
}

const s_array<s_float,8>& Texture::GetTexCoord() const
{
    return lTexCoord_;
}

const s_bool& Texture::GetTexCoordModifiesRect() const
{
    return bTexCoordModifiesRect_;
}

const s_str& Texture::GetTexture() const
{
    return sTextureFile_;
}

const Color& Texture::GetVertexColor() const
{
    return pSprite_->GetColor();
}

const s_bool& Texture::IsDesaturated() const
{
    return bIsDesaturated_;
}

void Texture::SetBlendMode( TextureBlendMode mBlendMode )
{
    mBlendMode_ = mBlendMode;
}

void Texture::SetBlendMode( const s_str& sBlendMode )
{
    if (sBlendMode == "BLEND")
        mBlendMode_ = BLEND_BLEND;
    else if (sBlendMode == "ADD")
        mBlendMode_ = BLEND_ADD;
    else if (sBlendMode == "MOD")
        mBlendMode_ = BLEND_MOD;
    else if (sBlendMode == "KEY")
        mBlendMode_ = BLEND_KEY;
    else if (sBlendMode == "NONE")
        mBlendMode_ = BLEND_NONE;
    else
    {
        Warning(lType_.back(),
            "Unknown blending : \""+sBlendMode+"\". Using \"BLEND\"."
        );
        mBlendMode_ = BLEND_BLEND;
    }
}

void Texture::SetDesaturated( const s_bool& bIsDesaturated )
{
    bIsDesaturated_ = bIsDesaturated;
}

void Texture::SetGradient( const Gradient& mGradient )
{
}

void Texture::SetTexCoord( const s_array<s_float,4>& lCoordinates )
{
    pSprite_->SetTextureRect(lCoordinates);
    lTexCoord_ = pSprite_->GetTextureCoords();
}

void Texture::SetTexCoord( const s_array<s_float,8>& lCoordinates )
{
    pSprite_->SetTextureCoords(lCoordinates);
    lTexCoord_ = pSprite_->GetTextureCoords();
}

void Texture::SetTexCoordModifiesRect( const s_bool& bTexCoordModifiesRect )
{
    bTexCoordModifiesRect_ = bTexCoordModifiesRect;
}

void Texture::SetTexture( const s_str& sFile )
{
    sTextureFile_ = sFile;
    s_refptr<Material> pMat;

    pSprite_.SetNull(); // Deletes the old sprite and its material

    if (File::Exists(sTextureFile_))
    {
        pMat = MaterialManager::GetSingleton()->CreateMaterial2D(sTextureFile_);
        pSprite_ = s_refptr<Sprite>(new Sprite(pMat));
    }
    else
    {
        Error(CLASS_NAME,
            "Cannot find file \""+sFile+"\". Using white texture instead."
        );
        pMat = MaterialManager::GetSingleton()->CreateMaterial2D(sName_+"_texture", 255, 255, 255);
        pSprite_ = s_refptr<Sprite>(new Sprite(pMat, 256, 256));
    }
}

void Texture::SetColor( const Color& mColor )
{
    pSprite_.SetNull(); // Deletes the old sprite and its material

    s_refptr<Material> pMat = MaterialManager::GetSingleton()->CreateMaterial2D(sName_+"_texture", mColor);
    pSprite_ = s_refptr<Sprite>(new Sprite(pMat));
}

void Texture::SetVertexColor( const Color& mColor )
{
    pSprite_->SetColor(mColor);
}
