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

#include <utils/frost_utils_file.h>

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str Texture::CLASS_NAME = "GUI::Texture";

Texture::Texture() : LayeredRegion()
{
    lTexCoord_[0] = lTexCoord_[1] = lTexCoord_[3] = lTexCoord_[6] = 0.0f;
    lTexCoord_[2] = lTexCoord_[4] = lTexCoord_[5] = lTexCoord_[7] = 1.0f;
    lType_.PushBack("Texture");
}

Texture::~Texture()
{
}

s_str Texture::Serialize( const s_str& sTab ) const
{
    s_str sStr = LayeredRegion::Serialize(sTab);

    if (!sTextureFile_.IsEmpty())
    {
        sStr << sTab << "  # File        : " << sTextureFile_ << "\n";
        sStr << sTab << "  # Tex. coord. :\n";
        sStr << sTab << "  #-###\n";
        sStr << sTab << "  |   # top-left     : " + (lTexCoord_[0], lTexCoord_[1])  << "\n";
        sStr << sTab << "  |   # top-right    : " + (lTexCoord_[2], lTexCoord_[3]) << "\n";
        sStr << sTab << "  |   # bottom-right : " + (lTexCoord_[4], lTexCoord_[5]) << "\n";
        sStr << sTab << "  |   # bottom-left  : " + (lTexCoord_[6], lTexCoord_[7]) << "\n";
        sStr << sTab << "  #-###\n";
        sStr << sTab << "  # TexCModRect : " << bTexCoordModifiesRect_ << "\n";
    }
    else if (!mGradient_.IsEmpty())
    {
        sStr << sTab << "  # Gradient    :\n";
        sStr << sTab << "  #-###\n";
        sStr << sTab << "  |   # min color   : " + mGradient_.GetMinColor() << "\n";
        sStr << sTab << "  |   # max color   : " + mGradient_.GetMaxColor() << "\n";
        sStr << sTab << "  |   # orientation : ";
        switch (mGradient_.GetOrientation())
        {
            case Gradient::HORIZONTAL : sStr << "HORIZONTAL\n"; break;
            case Gradient::VERTICAL :   sStr << "VERTICAL\n"; break;
            default : sStr << "<error>\n"; break;
        }
        sStr << sTab << "  #-###\n";
    }
    else
    {
        sStr << sTab << "  # Color       : " + mColor_ << "\n";
    }

    sStr << sTab << "  # Blend mode  : ";
    switch (mBlendMode_)
    {
        case BLEND_NONE : sStr << "NONE\n"; break;
        case BLEND_BLEND : sStr << "BLEND\n"; break;
        case BLEND_KEY : sStr << "KEY\n"; break;
        case BLEND_ADD : sStr << "ADD\n"; break;
        case BLEND_MOD : sStr << "MOD\n"; break;
        default : sStr << "<error>\n"; break;
    }

    sStr << sTab << "  # Desaturated : " << bIsDesaturated_ << "\n";

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
    lGlueList_.PushBack(pLua->PushNew<LuaTexture>());
    pLua->SetGlobal(sLuaName_);
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
        this->SetDesaturated(pTexture->IsDesaturated());
    }
}

Texture::BlendMode Texture::GetBlendMode() const
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

void Texture::SetBlendMode( BlendMode mBlendMode )
{
    if (mBlendMode_ != mBlendMode)
    {
        mBlendMode_ = mBlendMode;
        NotifyRendererNeedRedraw();
    }
}

void Texture::SetBlendMode( const s_str& sBlendMode )
{
    BlendMode mOldBlendMode = mBlendMode_;

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

    if (mOldBlendMode != mBlendMode_)
    {
        NotifyRendererNeedRedraw();
    }
}

void Texture::SetDesaturated( const s_bool& bIsDesaturated )
{
    if (!pSprite_)
    {
        Error(lType_.Back(),
            "Trying to desaturate an uninitialized Texture : "+sName_+"."
        );
        return;
    }

    if (bIsDesaturated_ != bIsDesaturated)
    {
        bIsDesaturated_ = bIsDesaturated;
        if (bIsDesaturated_)
            pSprite_->GetMaterial()->SetPixelShader("GUI_Desaturation");
        else
            pSprite_->GetMaterial()->RemovePixelShader();

        NotifyRendererNeedRedraw();
    }
}

void Texture::SetGradient( const Gradient& mGradient )
{
    mColor_ = Color::NaN;
    sTextureFile_ = "";
    mGradient_ = mGradient;
    s_refptr<Material> pMat = MaterialManager::GetSingleton()->CreateMaterial2D(sName_+"_texture", 255, 255, 255);
    pSprite_ = s_refptr<Sprite>(new Sprite(pMat, 256, 256));

    if (mGradient_.GetOrientation() == Gradient::HORIZONTAL)
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

    NotifyRendererNeedRedraw();
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
        NotifyRendererNeedRedraw();
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
        NotifyRendererNeedRedraw();
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
    if (bTexCoordModifiesRect_ != bTexCoordModifiesRect)
    {
        bTexCoordModifiesRect_ = bTexCoordModifiesRect;
        NotifyRendererNeedRedraw();
    }
}

void Texture::SetTexture( const s_str& sFile )
{
    mGradient_ = Gradient();
    mColor_ = Color::NaN;
    sTextureFile_ = sFile;

    pSprite_ = nullptr; // Deletes the old sprite and its material

    if (File::Exists(sTextureFile_))
    {
        s_refptr<Material> pMat = MaterialManager::GetSingleton()->CreateMaterial2D(sTextureFile_);
        pSprite_ = s_refptr<Sprite>(new Sprite(pMat));
        pSprite_->SetTextureCoords(lTexCoord_, true);
    }
    else
    {
        Error(lType_.Back(),
            "Cannot find file \""+sFile+"\" for \""+sName_+"\".\nUsing white texture instead."
        );
        s_refptr<Material> pMat = MaterialManager::GetSingleton()->CreateMaterial2D(sName_+"_texture", 255, 255, 255);
        pSprite_ = s_refptr<Sprite>(new Sprite(pMat, 256, 256));
    }

    NotifyRendererNeedRedraw();
}

void Texture::SetTexture( s_ptr<RenderTarget> pRenderTarget )
{
    mGradient_ = Gradient();
    mColor_ = Color::NaN;
    sTextureFile_ = "";

    pSprite_ = nullptr; // Deletes the old sprite and its material

    if (pRenderTarget)
    {
        s_refptr<Material> pMat = MaterialManager::GetSingleton()->CreateMaterial2DFromRT(pRenderTarget);
        pSprite_ = s_refptr<Sprite>(new Sprite(
            pMat, s_float(pRenderTarget->GetWidth()), s_float(pRenderTarget->GetHeight())
        ));
    }
    else
    {
        Error(lType_.Back(),
            "Cannot create a Texture with a NULL RenterTarget.\nUsing white texture instead."
        );
        s_refptr<Material> pMat = MaterialManager::GetSingleton()->CreateMaterial2D(sName_+"_texture", 255, 255, 255);
        pSprite_ = s_refptr<Sprite>(new Sprite(pMat, 256, 256));
    }

    NotifyRendererNeedRedraw();
}

void Texture::SetColor( const Color& mColor )
{
    mGradient_ = Gradient();
    sTextureFile_ = "";

    pSprite_ = nullptr; // Deletes the old sprite and its material

    s_refptr<Material> pMat = MaterialManager::GetSingleton()->CreateMaterial2D(sName_+"_texture", 255, 255, 255);
    pSprite_ = s_refptr<Sprite>(new Sprite(pMat, 256, 256));
    pSprite_->SetColor(mColor);
    mColor_ = mColor;

    NotifyRendererNeedRedraw();
}

void Texture::SetMaterial( s_refptr<Material> pMat )
{
    mGradient_ = Gradient();
    sTextureFile_ = "";

    pSprite_ = nullptr; // Deletes the old sprite and its material

    pSprite_ = s_refptr<Sprite>(new Sprite(pMat));
    pSprite_->SetTextureCoords(lTexCoord_, true);

    NotifyRendererNeedRedraw();
}

void Texture::SetVertexColor( const Color& mColor )
{
    if (pSprite_)
    {
        pSprite_->SetColor(mColor);
        NotifyRendererNeedRedraw();
    }
    else
    {
        Error(lType_.Back(),
            "Trying to set vertex color of an uninitialized Texture : "+
            sName_+"."
        );
    }
}
