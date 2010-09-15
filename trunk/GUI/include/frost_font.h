#ifndef FROST_GUI_FONT_H
#define FROST_GUI_FONT_H

#include <frost_utils.h>

namespace Frost
{
    typedef Point<s_float> Vector2D;

namespace GUI
{
    class Material;
    class FontImpl;

    struct CharacterInfo
    {
        uint uiCodePoint;

        float fU1, fV1;
        float fU2, fV2;

        s_map<s_uint, Vector2D> lKerningInfo;
    };

    /// Manages font creation
    class Font
    {
    public :

        Font(const s_str& sFontFile, const s_uint& uiSize);

        ~Font();

        s_array<s_float,4> GetCharacterUVs(const s_uint& uiChar) const;

        s_float GetCharacterWidth(const s_uint& uiChar) const;

        s_float GetCharacterKerning(const s_uint& uiChar1, const s_uint& uiChar2) const;

        Material GetTexture();

        const s_float& GetTextureWidth() const;

        const s_float& GetTextureHeight() const;

        static const s_str CLASS_NAME;

    private :

        s_refptr<FontImpl> pImpl_;

    };
}
}

#endif
