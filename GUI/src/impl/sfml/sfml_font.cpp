/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Font source               */
/*                                        */
/*                                        */

#include "frost_font.h"
#include "impl/sfml/sfml_font.h"
#include "frost_material.h"
#include "impl/sfml/sfml_material.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include <SFML/Graphics.hpp>

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str FontImpl::CLASS_NAME = "GUI::FontImpl";

FontImpl::FontImpl( const s_str& sFontFile, const s_uint& uiSize )
{
    // NOTE : Code inspired from Ogre::Font, from the OGRE3D graphics engine
    // http://www.ogre3d.org

    FT_Library mFT;
    if (FT_Init_FreeType(&mFT))
        throw Exception(CLASS_NAME, "Error initializing FreeType !");

    FT_Face mFace;

    // Add some space between letters to prevent artifacts
    uint uiSpacing = 5;

    if (FT_New_Face( mFT, sFontFile.c_str(), 0, &mFace))
    {
        throw Exception(CLASS_NAME, "Error loading font : \""+sFontFile+"\".\n"
            "Couldn't load face."
        );
    }

    if (FT_Set_Pixel_Sizes(mFace, uiSize.Get(), 0))
    {
        throw Exception(CLASS_NAME, "Error loading font : \""+sFontFile+"\".\n"
            "Couldn't set font size."
        );
    }

    int iMaxHeight = 0, iMaxWidth = 0, iMaxBearingY = 0;

    // Calculate maximum width, height and bearing
    for (uint cp = 32; cp <= 255; ++cp)
    {
        FT_Load_Char(mFace, cp, FT_LOAD_RENDER);

        int iCharHeight = 2*(mFace->glyph->bitmap.rows << 6) - mFace->glyph->metrics.horiBearingY;
        if (iCharHeight > iMaxHeight)
            iMaxHeight = iCharHeight;

        if (mFace->glyph->metrics.horiBearingY > iMaxBearingY)
            iMaxBearingY = mFace->glyph->metrics.horiBearingY;

        int iCharWidth = (mFace->glyph->advance.x >> 6) + (mFace->glyph->metrics.horiBearingX >> 6);
        if (iCharWidth > iMaxWidth)
            iMaxWidth = iCharWidth;
    }

    iMaxBearingY = iMaxBearingY >> 6;

    // Calculate the size of the texture
    size_t uiTexSize = (iMaxWidth + uiSpacing)*((iMaxHeight >> 6) + uiSpacing)*(255-33);

    uint uiTexSide = static_cast<uint>(::sqrt(uiTexSize));
    uiTexSide += std::max(iMaxWidth, iMaxHeight>>6);

    // Round up to nearest power of two
    uint i = 1;
    while (uiTexSide > i)
        i += i;
    uiTexSide = i;

    size_t uiFinalWidth, uiFinalHeight;
    if (uiTexSide*uiTexSide/2 >= uiTexSize)
    {
        uiFinalHeight = uiTexSide/2;
    }
    else
    {
        uiFinalHeight = uiTexSide;
    }
    uiFinalWidth = uiTexSide;

    fTextureWidth_ = static_cast<float>(uiFinalWidth);
    fTextureHeight_ = static_cast<float>(uiFinalHeight);

    mTexture_.Create(uiFinalWidth, uiFinalHeight, sf::Color(255, 255, 255, 0));

    size_t l = 0, m = 0;
    CharacterInfo mCI;
    for (uint cp = 32; cp <= 255; ++cp)
    {
        mCI.uiCodePoint = cp;

        if (FT_Load_Char(mFace, cp, FT_LOAD_RENDER))
        {
            Warning(CLASS_NAME, "Can't load character "+s_uchar(s_uint(cp))+" in font \""+sFontFile+"\".");
            continue;
        }

        FT_Int iAdvance = (mFace->glyph->advance.x >> 6) + (mFace->glyph->metrics.horiBearingX >> 6);

        uchar* sBuffer = mFace->glyph->bitmap.buffer;
        if (sBuffer)
        {
            int iYBearing = iMaxBearingY - (mFace->glyph->metrics.horiBearingY >> 6);

            for (int j = 0; j < mFace->glyph->bitmap.rows; ++j)
            {
                for (int k = 0; k < mFace->glyph->bitmap.width; ++k, ++sBuffer)
                {
                    mTexture_.SetPixel(l + k, j + m + iYBearing, sf::Color(255, 255, 255, *sBuffer));
                }
            }
        }

        /*if (FT_HAS_KERNING(mFace))
        {
            FT_Vector kern;
            unsigned int prev, next;
            for (uint cp2 = 33; cp2 <= 255; ++cp2)
            {
                prev = FT_Get_Char_Index(mFace, cp);
                next = FT_Get_Char_Index(mFace, cp2);
                if (!FT_Get_Kerning(mFace, prev, next, FT_KERNING_DEFAULT, &kern))
                {
                    mCI.lKerningInfo[cp2] = Point<float>(
                        (kern.x >> 6),
                        (kern.y >> 6)
                    );
                }
            }
        }*/

        mCI.fU1 = l/static_cast<float>(uiFinalWidth);
        mCI.fV1 = m/static_cast<float>(uiFinalHeight);
        mCI.fU2 = (l + (mFace->glyph->advance.x >> 6))/static_cast<float>(uiFinalWidth);
        mCI.fV2 = (m + (iMaxHeight >> 6))/static_cast<float>(uiFinalHeight);

        lCharacterList_[cp] = mCI;

        // Advance a column
        l += (iAdvance + uiSpacing);

        // If at end of row
        if (l + iAdvance > uiFinalWidth - 1)
        {
            m += (iMaxHeight >> 6) + uiSpacing;
            l = 0;
        }
    }

    FT_Done_FreeType(mFT);
}

FontImpl::~FontImpl()
{
}

s_array<s_float,4> FontImpl::GetCharacterUVs( const s_uint& uiChar ) const
{
    s_map<s_uint, CharacterInfo>::const_iterator iter = lCharacterList_.Get(uiChar);
    if (iter != lCharacterList_.End())
    {
        const CharacterInfo& mChar = iter->second;
        s_array<s_float,4> mArray;
        mArray[0] = mChar.fU1;
        mArray[1] = mChar.fV1;
        mArray[2] = mChar.fU2;
        mArray[3] = mChar.fV2;
        return mArray;
    }
    else
    {
        throw Exception(CLASS_NAME, "Character "+uiChar+" is not in the character table.");
    }
}

s_float FontImpl::GetCharacterWidth( const s_uint& uiChar ) const
{
    s_map<s_uint, CharacterInfo>::const_iterator iter = lCharacterList_.Get(uiChar);
    if (iter != lCharacterList_.End())
    {
        const CharacterInfo& mChar = iter->second;
        return (mChar.fU2 - mChar.fU1)*fTextureWidth_;
    }
    else
    {
        throw Exception(CLASS_NAME, "Character "+uiChar+" is not in the character table.");
    }
}

s_float FontImpl::GetCharacterKerning( const s_uint& uiChar1, const s_uint& uiChar2 ) const
{
    //return lCharacterList_.Get(uiChar1)->second.lKerningInfo.Get(uiChar2)->second.X();
    return 0.0f;
}

const s_float& FontImpl::GetTextureWidth() const
{
    return fTextureWidth_;
}

const s_float& FontImpl::GetTextureHeight() const
{
    return fTextureHeight_;
}

Material FontImpl::GetTexture() const
{
    return Material(s_refptr<MaterialImpl>(new MaterialImpl(&mTexture_)));
}
