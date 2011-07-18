/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Sprite header             */
/*                                        */
/*                                        */


// # Note # :
// This class is inspired by HGE's hgeSprite class.
// HGE is a simple and fast 2D drawing library. I have
// been using it before switching to Ogre3D, and I
// really liked the synthax.
//
// More infos about HGE :
// http://hge.relishgames.com/

#ifndef FROST_GUI_SPRITEIMPL_H
#define FROST_GUI_SPRITEIMPL_H

#include <frost_utils.h>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Quad.hpp>

namespace sf
{
    class Image;
}

namespace Frost
{
    void RenderQuad(const sf::Quad& mQuad);

namespace GUI
{
    class RenderTarget;
    class Material;
    struct Vertex;

    /// Use to draw a texture on the screen
    /** This class is meant to simplify rendering of
    *   2D elements. Everything can be done with the Quad
    *   struct, but it's really not simple to use.<br>
    *   Thanks to this class, the only thing you have to do
    *   to acheive interesting effects is calling 2 or 3
    *   functions and let the magic do the rest.
    */
    class SpriteImpl
    {
    public :

        /// Default constructor.
        /** \param pManager The GUIManager this sprite has been created for.
        */
        SpriteImpl(s_ptr<GUIManager> pManager);

        /// Constructor.
        /** \param pManager The GUIManager this sprite has been created for.
        *   \param pMat The Material to use
        *   \note Using this constructor, you don't have to provide
        *         any width or height : they are calculated from the
        *         Material's texture.<br>
        *         So, your sprite will be as large as the texture and
        *         contain it all.<br>
        *         <b>Note</b> : you can't call that constructor with a
        *         blank Material.
        */
        SpriteImpl(s_ptr<GUIManager> pManager, const s_str& sTextureFile);

        /// Constructor.
        /** \param pManager The GUIManager this sprite has been created for.
        *   \param pMat    The Material to use
        *   \param fWidth  The width of the Sprite
        *   \param fHeight The height of the Sprite
        *   \note If the width and height you provide are smaller than
        *         the texture's ones, the texture will be cut on the right
        *         and bottom edges.<br>
        *         However, if they are larger than the texture's one, the
        *         texture will be tiled.
        */
        SpriteImpl(s_ptr<GUIManager> pManager, const s_str& sTextureFile, const s_float& fWidth, const s_float& fHeight);

        /// Constructor.
        /** \param pManager The GUIManager this sprite has been created for.
        *   \param mColor  The color of the Sprite
        *   \param fWidth  The width of the Sprite
        *   \param fHeight The height of the Sprite
        *   \note This constructor allow you to use a simple color instead
        *         of a real texture.
        */
        SpriteImpl(s_ptr<GUIManager> pManager, const Color& mColor, const s_float& fWidth, const s_float& fHeight);

        /// Constructor.
        /** \param pRenderTarget The render target to read data from
        */
        SpriteImpl(s_ptr<GUIManager> pManager, s_ptr<RenderTarget> pRenderTarget);

        /// Constructor.
        /** \param pManager The GUIManager this sprite has been created for.
        *   \param pRenderTarget The render target to read data from
        *   \param fWidth  The width of the Sprite
        *   \param fHeight The height of the Sprite
        */
        SpriteImpl(s_ptr<GUIManager> pManager, s_ptr<RenderTarget> pRenderTarget, const s_float& fWidth, const s_float& fHeight);

        /// Constructor.
        SpriteImpl(s_ptr<GUIManager> pManager, const Material& mMat);

        /// Constructor.
        SpriteImpl(s_ptr<GUIManager> pManager, const Material& mMat, const s_float& fWidth, const s_float& fHeight);

        /// Constructor.
        SpriteImpl(s_ptr<GUIManager> pManager, const Material& mMat, const s_float& fU, const s_float& fV, const s_float& fWidth, const s_float& fHeight);

        /// Destructor.
        ~SpriteImpl();

        /// Renders this Sprite on the current render target.
        /** \param fX The horizontal position
        *   \param fY The vertical position
        *   \note Must be called between SpriteManager::Begin() and
        *         SpriteManager::End().
        */
        void Render(const s_float& fX, const s_float& fY) const;

        /// Deforms this Sprite and render it on the current render target.
        /** \param fX      The horizontal position
        *   \param fY      The vertical position
        *   \param fRot    The rotation to apply (angle in radian)
        *   \param fHScale The horizontal scale to apply
        *   \param fVScale The vertical scale to apply
        *   \note This function doesn't store the deformation.<br>
        *         Must be called between SpriteManager::Begin() and
        *         SpriteManager::End().
        */
        void RenderEx(const s_float& fX, const s_float& fY,
                      const s_float& fRot,
                      const s_float& fHScale = 1.0f, const s_float& fVScale = 1.0f) const;

        /// Stretches this this Sprite and render it on the current render target.
        /** \param fX1 The top-left corner horizontal position
        *   \param fY1 The top-left corner vertical position
        *   \param fX3 The bottom-right corner horizontal position
        *   \param fY3 The bottom-right corner vertical position
        *   \note This function doesn't store the deformation.<br>
        *         Must be called between SpriteManager::Begin() and
        *         SpriteManager::End().
        */
        void Render2V(const s_float& fX1, const s_float& fY1,
                      const s_float& fX3, const s_float& fY3);

        /// Stretches this this Sprite and render it on the current render target.
        /** \param fX1 The top-left corner horizontal position
        *   \param fY1 The top-left corner vertical position
        *   \param fX2 The top-right corner horizontal position
        *   \param fY2 The top-right corner vertical position
        *   \param fX3 The bottom-right corner horizontal position
        *   \param fY3 The bottom-right corner vertical position
        *   \param fX4 The bottom-left corner horizontal position
        *   \param fY4 The bottom-left corner vertical position
        *   \note This function doesn't store the deformation.<br>
        *         Must be called between SpriteManager::Begin() and
        *         SpriteManager::End().
        */
        void Render4V(const s_float& fX1, const s_float& fY1,
                      const s_float& fX2, const s_float& fY2,
                      const s_float& fX3, const s_float& fY3,
                      const s_float& fX4, const s_float& fY4);

        /// Render this Sprite with the same parameter than the last render call.
        /** \note This function is here for performance, when you want to set the
        *         internal quad once and for all.
        */
        void RenderStatic() const;

        /// Sets this Sprite's internal quad.
        /** \param lVertexArray
        *   \note This quad will be overwritten by any RenderXXX() call.
        *         If you want to render it, call RenderStatic().
        */
        void SetQuad(const s_array<Vertex,4>& lVertexArray);

        /// Changes the color of this sprite.
        /** \param mColor The new color
        *   \param uiIndex The index of the vertice to change
        *   \note If you provide an index, this function will only change
        *         a single vertex's color.<br>
        *         Index 0 is for top left, index 1 is for top right, ...
        */
        void SetColor(const Color& mColor, const s_uint& uiIndex = s_uint::INF);

        /// Makes this sprite colorless.
        /** \param bDesaturated 'true' to desaturate the texture/color
        *   \note Depending on the implementation, this method may resort to
        *         using pixel shaders.
        */
        void SetDesaturated(const s_bool& bDesaturated);

        /// Sets the blending mode of this sprite.
        /** \param mBlendMode The new blending mode
        *   \note See BlendMode.
        */
        void SetBlendMode(BlendMode mBlendMode);

        /// Changes this Sprite's center.
        /** \param mHotSpot A 2D point containing the new center's position
        *   \note HotSpot is used to rotate and scale your sprite with RenderEx().<br>
        *         It is also considered as the reference point when you call Render()
        *         (same goes for RenderEx()).
        */
        void SetHotSpot(const Point<s_float>& mHotSpot);

        /// Changes this Sprite's texture rectangle.
        /** \param lTextureRect The new texture rect
        *   \param bNormalized  'true' if the coords are already clamped to [0, 1]
        *   \note Texture rectangle is the zone of the texture you want to display.<br>
        *         Note that it doesn't need to be adjusted to this Sprite's dimensions.
        *         The texture will then be stretched to fit the Sprite's dimensions.
        */
        void SetTextureRect(const s_array<s_float,4>& lTextureRect, const s_bool& bNormalized = false);

        /// Changes this Sprite's texture rectangle.
        /** \param fX1 The rect's top left horizontal position
        *   \param fY1 The rect's top left vertical position
        *   \param fX3 The rect's bottom right horizontal position
        *   \param fY3 The rect's bottom right vertical position
        *   \param bNormalized  'true' if the coords are already clamped to [0, 1]
        *   \note Texture rectangle is the zone of the texture you want to display.<br>
        *         Note that it doesn't need to be adjusted to this Sprite's dimensions.
        *         The texture will then be stretched to fit the Sprite's dimensions.
        */
        void SetTextureRect(const s_float& fX1, const s_float& fY1,
                            const s_float& fX3, const s_float& fY3,
                            const s_bool& bNormalized = false);

        /// Changes this Sprite's texture coordinates.
        /** \param lTextureCoords The new texture coordinates
        *   \param bNormalized  'true' if the coords are already converted to texture space
        *   \note Texture rectangle is the zone of the texture you want to display.<br>
        *         Note that it doesn't need to be adjusted to this Sprite's dimensions.
        *         The texture will then be stretched to fit the Sprite's dimensions.
        */
        void SetTextureCoords(const s_array<s_float,8>& lTextureCoords, const s_bool& bNormalized = false);

        /// Changes this Sprite's texture coordinates.
        /** \param fX1 The sprites's top left horizontal position
        *   \param fY1 The sprites's top left vertical position
        *   \param fX2 The sprites's top right horizontal position
        *   \param fY2 The sprites's top right vertical position
        *   \param fX3 The sprites's bottom right horizontal position
        *   \param fY3 The sprites's bottom right vertical position
        *   \param fX4 The sprites's bottom left horizontal position
        *   \param fY4 The sprites's bottom left vertical position
        *   \param bNormalized  'true' if the coords are already converted to texture space
        *   \note Texture rectangle is the zone of the texture you want to display.<br>
        *         Note that it doesn't need to be adjusted to this Sprite's dimensions.
        *         The texture will then be stretched to fit the Sprite's dimensions.
        */
        void SetTextureCoords(const s_float& fX1, const s_float& fY1,
                              const s_float& fX2, const s_float& fY2,
                              const s_float& fX3, const s_float& fY3,
                              const s_float& fX4, const s_float& fY4,
                              const s_bool& bNormalized = false);

        /// Changes this Sprite's center.
        /** \param fX The new center's horizontal position
        *   \param fY The new center's vertical position
        *   \note HotSpot is used to rotate and scale your sprite with RenderEx().<br>
        *         It is also considered as the reference point when you call Render()
        *         (same goes for RenderEx()).
        */
        void SetHotSpot(const s_float& fX, const s_float& fY);

        /// Changes this Sprite's dimensions.
        /** \param fWidth      The new width
        *   \param fHeight     The new height
        *   \note If you adjust texture coordinates, you texture won't be deformed.
        *         Else, it will be streched to fit the new dimensions.
        */
        void SetDimensions(const s_float& fWidth, const s_float& fHeight);

        /// Returns this Sprite's width.
        /** \return This Sprite's width
        */
        s_float     GetWidth() const;

        /// Returns this Sprite's height.
        /** \return This Sprite's height
        */
        s_float     GetHeight() const;

        /// Returns this Sprite's color.
        /** \return This Sprite's color
        */
        Color       GetColor() const;

        /// Returns this Sprite's blend mode.
        /** \return This Sprite's blend mode
        */
        BlendMode   GetBlendMode() const;

        /// Returns this Sprite's texture rectangle.
        /** \return This Sprite's texture rectangle
        */
        s_array<s_float,4> GetTextureRect() const;

        /// Returns this Sprite's texture coordinates.
        /** \param bNormalized 'true' to get coordinates converted to texture space
        *   \return This Sprite's texture coordinates
        */
        s_array<s_float,8> GetTextureCoords(const s_bool& bNormalized = false) const;

        s_ptr<const sf::Image> GetImage() const;

        /// Loads a texture file for rendering.
        /** \param sFileName The relative file path
        *   \return A Material object wrapping the texture
        */
        static Material LoadTexture(const s_str& sFileName);

        static const s_str CLASS_NAME;

    private :

        mutable s_ptr<GUIManager> pManager_;

        mutable sf::Quad mQuad_;
        s_str sTextureFile_;

        s_bool bDesaturated_;

        s_float fWidth_, fHeight_;
        s_float fTexWidth_, fTexHeight_;
        s_float fHotSpotX_, fHotSpotY_;
    };
}
}

#endif
