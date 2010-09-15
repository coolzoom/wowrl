/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           Font manager source          */
/*                                        */
/*                                        */

#include "frost_font.h"
#include "frost_material.h"

#if FRONT_END_OGRE
    #include "impl/ogre/ogre_font.h"
#elif FRONT_END_SFML
    #include "impl/sfml/sfml_font.h"
#else
    #error "No implementation defined for Font !"
#endif

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str Font::CLASS_NAME = "GUI::Font";

Font::Font( const s_str& sFontFile, const s_uint& uiSize )
{
    pImpl_ = s_refptr<FontImpl>(new FontImpl(sFontFile, uiSize));
}

Font::~Font()
{
}

s_array<s_float,4> Font::GetCharacterUVs( const s_uint& uiChar ) const
{
    return pImpl_->GetCharacterUVs(uiChar);
}

s_float Font::GetCharacterWidth( const s_uint& uiChar ) const
{
    return pImpl_->GetCharacterWidth(uiChar);
}

s_float Font::GetCharacterKerning( const s_uint& uiChar1, const s_uint& uiChar2 ) const
{
    return pImpl_->GetCharacterKerning(uiChar1, uiChar2);
}

const s_float& Font::GetTextureWidth() const
{
    return pImpl_->GetTextureWidth();
}

const s_float& Font::GetTextureHeight() const
{
    return pImpl_->GetTextureHeight();
}

Material Font::GetTexture()
{
    return pImpl_->GetTexture();
}
