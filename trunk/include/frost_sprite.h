/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Sprite header             */
/*                                        */
/*                                        */


#ifndef FROST_SPRITE_H
#define FROST_SPRITE_H

#include "frost.h"
#include "frost_gfx.h"

namespace Frost
{
    /// Use to draw a texture on the screen
    /** This class is meant to simplify rendering of
    *   2D elements. Everything can be done with the Quad
    *   struct, but it's really not simple to use.<br>
    *   Thanks to this class, the only thing you have to do
    *   to acheive interesting effects is calling 2 or 3
    *   functions and let the magic do the rest.
    */
    class Sprite
    {
    public :

        /// Constructor.
        /** \param pMat The Material to use
        *   \note Using this constructor, you don't have to provide
        *         any width or height : they are calculated from the
        *         Material's texture.<br>
        *         So, your sprite will be as large as the texture and
        *         contain it all.<br>
        *         <b>Note</b> : you can't call that constructor with a
        *         blank Material.
        */
        Sprite(s_refptr<Material> pMat);

        /// Constructor.
        /** \param pMat    The Material to use
        *   \param fWidth  The width of the Sprite
        *   \param fHeight The height of the Sprite
        *   \note If the width and height you provide are smaller than
        *         the texture's ones, the texture will be cut on the right
        *         and bottom edges.<br>
        *         However, if they are larger than the texture's one, the
        *         texture will be tiled.
        */
        Sprite(s_refptr<Material> pMat, const s_float& fWidth, const s_float& fHeight);

        /// Constructor.
        /** \param pMat    The Material to use
        *   \param fU      The position of the left border on the texture
        *   \param fV      The position of the top border on the texture
        *   \param fWidth  The width of the Sprite
        *   \param fHeight The height of the Sprite
        *   \note If the width and height you provide are smaller than
        *         the texture's ones, the texture will be cut on the right
        *         and bottom edges.<br>
        *         However, if they are larger than the texture's one, the
        *         texture will be tiled.<br>
        *         The U and V parameters, in combination with width and height,
        *         allow you to select a small part of your texture :<br>
        *         Sprite(mMat, 52, 28, 32, 32);<br>
        *         ... will create a 32x32 Sprite which takes its texture from
        *         the pixel located by (52, 28) on the texture.
        */
        Sprite(
            s_refptr<Material> pMat,
            const s_float& fU,
            const s_float& fV,
            const s_float& fWidth,
            const s_float& fHeight
        );

        /// Constructor.
        /** \param fWidth  The width of the Sprite
        *   \param fHeight The height of the Sprite
        *   \param mColor  The color of the Sprite
        *   \note This constructor allow you to use a simple color instead
        *         of a real texture.
        */
        Sprite(const s_float& fWidth, const s_float& fHeight, const Color& mColor);

        /// Renders this Sprite on the screen.
        /** \param fX The horizontal position
        *   \param fY The vertical position
        */
        void Render(const s_float& fX, const s_float& fY);

        /// Deforms this Sprite and render it on the screen.
        /** \param fX      The horizontal position
        *   \param fY      The vertical position
        *   \param fRot    The rotation to apply (angle in radian)
        *   \param fHScale The horizontal scale to apply
        *   \param fVScale The vertical scale to apply
        *   \note This function doesn't store the deformation.
        */
        void RenderEx(
            const s_float& fX,
            const s_float& fY,
            const s_float& fRot,
            const s_float& fHScale = 1.0f,
            const s_float& fVScale = 1.0f
        );

        /// Changes the color of this sprite.
        /** \param mColor The new color
        *   \param uiIndex The index of the vertice to change
        *   \note If you provide an index, this function will only change
        *         a single vertex's color.<br>
        *         Index 0 is for top left, index 1 is for top right, ...
        */
        void SetColor(const Color& mColor, const s_uint& uiIndex = s_uint::INF);

        /// Changes this Sprite's center.
        /** \param mHotSpot A 2D point containing the new center's position
        *   \note HotSpot is used to rotate and scale your sprite with RenderEx().<br>
        *         It is also considered as the reference point when you call Render()
        *         (same goes for RenderEx()).
        */
        void SetHotSpot(const Point<s_float>& mHotSpot);

        /// Changes this Sprite's center.
        /** \param fX The new center's horizontal position
        *   \param fY The new center's vertical position
        *   \note HotSpot is used to rotate and scale your sprite with RenderEx().<br>
        *         It is also considered as the reference point when you call Render()
        *         (same goes for RenderEx()).
        */
        void SetHotSpot(const s_float& fX, const s_float& fY);

        /// Changes this Sprite's texture rectangle.
        /** \param lTextureRect The new texture rect
        *   \note Texture rectangle is the zone of the texture you want to display.<br>
        *         Note that it doesn't need to be adjusted to this Sprite's dimensions.
        *         The texture will then be stretched to fit the Sprite's dimensions.
        */
        void SetTextureRect(const s_array<s_float,4>& lTextureRect);

        /// Changes this Sprite's texture rectangle.
        /** \param fX1 The rect's top left horizontal position
        *   \param fY1 The rect's top left vertical position
        *   \param fX2 The rect's bottom right horizontal position
        *   \param fY2 The rect's bottom right vertical position
        *   \note Texture rectangle is the zone of the texture you want to display.<br>
        *         Note that it doesn't need to be adjusted to this Sprite's dimensions.
        *         The texture will then be stretched to fit the Sprite's dimensions.
        */
        void SetTextureRect(
            const s_float& fX1,
            const s_float& fY1,
            const s_float& fX2,
            const s_float& fY2
        );

        /// Changes this Sprite's width.
        /** \param fWidth     The new width
        *   \param bAdjustUVs 'true' if you want to adjust texture coordinates
        *   \note If you adjust texture coordinates, you texture won't be deformed.
        *         Else, it will be streched to fit the new dimensions.
        */
        void SetWidth(const s_float& fWidth, const s_bool& bAdjustUVs = false);

        /// Changes this Sprite's height.
        /** \param fHeight     The new height
        *   \param bAdjustUVs 'true' if you want to adjust texture coordinates
        *   \note If you adjust texture coordinates, you texture won't be deformed.
        *         Else, it will be streched to fit the new dimensions.
        */
        void SetHeight(const s_float& fHeight, const s_bool& bAdjustUVs = false);

        /// Changes this Sprite's dimensions.
        /** \param fWidth      The new width
        *   \param fHeight     The new height
        *   \param bAdjustUVs 'true' if you want to adjust texture coordinates
        *   \note If you adjust texture coordinates, you texture won't be deformed.
        *         Else, it will be streched to fit the new dimensions.
        */
        void SetDimensions(
            const s_float& fWidth,
            const s_float& fHeight,
            const s_bool& bAdjustUVs = false
        );

        /// Returns this Sprite's color.
        /** \return This Sprite's color
        */
        const Color&       GetColor() const;

        /// Returns this Sprite's HotSpot.
        /** \return This Sprite's HotSpot
        */
        Point<s_float>     GetHotSpot() const;

        /// Returns this Sprite's texture rectangle.
        /** \return This Sprite's texture rectangle
        */
        s_array<s_float,4> GetTextureRect() const;

        /// Returns this Sprite's width.
        /** \return This Sprite's width
        */
        const s_float&     GetWidth() const ;

        /// Returns this Sprite's height.
        /** \return This Sprite's height
        */
        const s_float&     GetHeight() const;

        /// Returns this Sprite's Material.
        /** \return This Sprite's Material
        *   \note NULL is returned if no Material is used.
        */
        s_refptr<Material> GetMaterial();

        static const s_str CLASS_NAME;

    private :

        /// Updates this Sprite's texture coordinates
        void UpdateUVs_();

        Quad               mQuad_;
        Color              mColor_;
        s_float            fX1_, fY1_;
        s_float            fX2_, fY2_;
        s_float            fWidth_, fHeight_;
        s_float            fHotSpotX_, fHotSpotY_;
        s_uint		       uiTexWidth_, uiTexHeight_;
        s_bool             bUsingMaterial_;
        s_refptr<Material> pMaterial_;
    };
}

#endif // FROST_SPRITE_H
