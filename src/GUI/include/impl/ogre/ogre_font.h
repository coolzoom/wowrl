#ifndef FROST_GUI_FONTIMPL_H
#define FROST_GUI_FONTIMPL_H

#include <frost_utils.h>

namespace Ogre
{
    class Texture;
}

namespace Frost
{
namespace GUI
{
    class Material;
    class MaterialImpl;
    struct CharacterInfo;

    /// Manages font creation
    class FontImpl
    {
    public :

        FontImpl(const s_str& sFontFile, const s_uint& uiSize);

        ~FontImpl();

        s_array<s_float,4> GetCharacterUVs(const s_uint& uiChar) const;

        s_float GetCharacterWidth(const s_uint& uiChar) const;

        s_float GetCharacterKerning(const s_uint& uiChar1, const s_uint& uiChar2) const;

        Material GetTexture() const;

        const s_float& GetTextureWidth() const;

        const s_float& GetTextureHeight() const;

        static const s_str CLASS_NAME;

    private :

        s_map<s_uint, CharacterInfo> lCharacterList_;

        s_str                  sID_;
        s_ptr<Ogre::Texture>   pTex_;
        s_refptr<MaterialImpl> pMatImpl_;

        s_float fTextureWidth_;
        s_float fTextureHeight_;

    };
}
}

#endif
