/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_texture.h"

#include "gui/frost_layeredregion.h"
#include "gui/frost_guimanager.h"
#include "material/frost_materialmanager.h"
#include "material/frost_material.h"
#include "gui/frost_spritemanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str Texture::CLASS_NAME = "GUI::Texture";

Texture::Texture() : LayeredRegion()
{
    lTexCoord_[0] = lTexCoord_[1] = lTexCoord_[3] = lTexCoord_[6] = 0.0f;
    lTexCoord_[2] = lTexCoord_[4] = lTexCoord_[5] = lTexCoord_[7] = 1.0f;
    mObjectType_ = OJBECT_TYPE_TEXTURE;
    lType_.PushBack("Texture");
}

Texture::~Texture()
{
}

s_str Texture::Serialize( const s_str& sTab ) const
{
    s_str sStr = UIObject::Serialize(sTab);

    return sStr;
}

void Texture::Render()
{
    if (pSprite_ && IsVisible())
    {
        pSprite_->Render2V(
            s_float(lBorderList_[BORDER_LEFT]),  s_float(lBorderList_[BORDER_TOP]),
            s_float(lBorderList_[BORDER_RIGHT]), s_float(lBorderList_[BORDER_BOTTOM])
        );
    }
}

void Texture::CreateGlue()
{
    s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
    pLua->PushString(sName_);
    lGlueList_.PushBack(
        pLua->Push<LuaTexture>(new LuaTexture(pLua->GetState()))
    );
    pLua->SetGlobal(sName_);
}

void Texture::CopyFrom( s_ptr<UIObject> pObj )
{
    UIObject::CopyFrom(pObj);

    s_ptr<Texture> pTexture = s_ptr<Texture>::DynamicCast(pObj);

    if (pTexture)
    {
        s_str sTexture = pTexture->GetTexture();
        if (!sTexture.IsEmpty())
        {
            this->SetTexture(sTexture);
        }
        else
        {
            const Gradient& mGradient = pTexture->GetGradient();
            if (!mGradient.IsEmpty())
            {
                this->SetGradient(mGradient);
            }
            else
            {
                this->SetColor(pTexture->GetColor());
            }
        }

        this->SetBlendMode(pTexture->GetBlendMode());
        this->SetTexCoord(pTexture->GetTexCoord());
        this->SetTexCoordModifiesRect(pTexture->GetTexCoordModifiesRect());
        this->SetVertexColor(pTexture->GetVertexColor());
        this->SetDesaturated(pTexture->IsDesaturated());
    }
    else
    {
        Error(lType_.Back(),
            "Trying to derive \""+sName_+"\" from \""+pObj->GetName()+"\", but they are not of the same type "
            "(respectively "+this->GetObjectType()+" and "+pObj->GetObjectType()+")."
        );
    }
}

TextureBlendMode Texture::GetBlendMode() const
{
    return mBlendMode_;
}

const Color& Texture::GetColor() const
{
    return mColor_;
}

const Gradient& Texture::GetGradient() const
{
    return mGradient_;
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
    if (pSprite_)
        return pSprite_->GetColor();
    else
    {
        Error(lType_.Back(),
            "Trying to call GetVertexColor on an uninitialized Texture : "+sName_+"."
        );
        return Color::NaN;
    }
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
    Warning(lType_.Back(),
        "Texture::SetBlendMode is not yet implemented."
    );
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
        Warning(lType_.Back(),
            "Unknown blending : \""+sBlendMode+"\". Using \"BLEND\"."
        );
        mBlendMode_ = BLEND_BLEND;
    }
}

void Texture::SetDesaturated( const s_bool& bIsDesaturated )
{
    if (pSprite_)
    {
        bIsDesaturated_ = bIsDesaturated;
        pSprite_->GetMaterial()->SetDesaturated(bIsDesaturated_);
    }
    else
    {
        Error(lType_.Back(),
            "Trying to desaturate an uninitialized Texture : "+sName_+"."
        );
    }
}

void Texture::SetGradient( const Gradient& mGradient )
{
    mColor_ = Color::NaN;
    sTextureFile_ = "";
    mGradient_ = mGradient;
    s_refptr<Material> pMat = MaterialManager::GetSingleton()->CreateMaterial2D(sName_+"_texture", 255, 255, 255);
    pSprite_ = s_refptr<Sprite>(new Sprite(pMat, 256, 256));

    if (mGradient_.GetOrientation() == ORIENTATION_HORIZONTAL)
    {
        pSprite_->SetColor(mGradient_.GetMinColor(), 0);
        pSprite_->SetColor(mGradient_.GetMinColor(), 3);
        pSprite_->SetColor(mGradient_.GetMaxColor(), 1);
        pSprite_->SetColor(mGradient_.GetMaxColor(), 2);
    }
    else
    {
        pSprite_->SetColor(mGradient_.GetMinColor(), 0);
        pSprite_->SetColor(mGradient_.GetMinColor(), 1);
        pSprite_->SetColor(mGradient_.GetMaxColor(), 2);
        pSprite_->SetColor(mGradient_.GetMaxColor(), 3);
    }
}

void Texture::SetTexCoord( const s_array<s_float,4>& lCoordinates )
{
    if (pSprite_)
    {
        s_array<s_float,4> lSortedCoordinates;
        lSortedCoordinates[0] = lCoordinates[0];
        lSortedCoordinates[1] = lCoordinates[2];
        lSortedCoordinates[2] = lCoordinates[1];
        lSortedCoordinates[3] = lCoordinates[3];
        pSprite_->SetTextureRect(lSortedCoordinates, true);
        lTexCoord_ = pSprite_->GetTextureCoords(true);
    }
    else
    {
        Error(lType_.Back(),
            "Trying to call SetTexCoord on an uninitialized Texture : "+sName_+"."
        );
    }
}

void Texture::SetTexCoord( const s_array<s_float,8>& lCoordinates )
{
    if (pSprite_)
    {
        pSprite_->SetTextureCoords(lCoordinates, true);
        lTexCoord_ = lCoordinates;
    }
    else
    {
        Error(lType_.Back(),
            "Trying to call SetTexCoord on an uninitialized Texture : "+sName_+"."
        );
    }
}

void Texture::SetTexCoordModifiesRect( const s_bool& bTexCoordModifiesRect )
{
    bTexCoordModifiesRect_ = bTexCoordModifiesRect;
}

void Texture::SetTexture( const s_str& sFile )
{
    mGradient_ = Gradient();
    mColor_ = Color::NaN;
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
        Error(lType_.Back(),
            "Cannot find file \""+sFile+"\". Using white texture instead."
        );
        pMat = MaterialManager::GetSingleton()->CreateMaterial2D(sName_+"_texture", 255, 255, 255);
        pSprite_ = s_refptr<Sprite>(new Sprite(pMat, 256, 256));
    }
}

void Texture::SetColor( const Color& mColor )
{
    mGradient_ = Gradient();
    sTextureFile_ = "";
    pSprite_.SetNull(); // Deletes the old sprite and its material

    s_refptr<Material> pMat = MaterialManager::GetSingleton()->CreateMaterial2D(sName_+"_texture", mColor);
    pSprite_ = s_refptr<Sprite>(new Sprite(pMat, 256, 256));
    mColor_ = mColor;
}

void Texture::SetVertexColor( const Color& mColor )
{
    if (pSprite_)
    {
        pSprite_->SetColor(mColor);
    }
    else
    {
        Error(lType_.Back(),
            "Trying to set vertex color of an uninitialized Texture : "+
            sName_+"."
        );
    }
}
