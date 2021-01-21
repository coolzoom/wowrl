/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_TEXTURE_H
#define FROST_GUI_TEXTURE_H

#include <frost_utils.h>
#include "frost_layeredregion.h"
#include "frost_gradient.h"
#include "frost_sprite.h"

namespace Frost
{
    namespace GUI
    {
        /// The base of the GUI's appearence
        /** This object contains either a texture taken from a file,
        *   or a plain color.
        */
        class Texture : public LayeredRegion
        {
        public :

            enum BlendMode
            {
                BLEND_NONE,
                BLEND_BLEND,
                BLEND_KEY,
                BLEND_ADD,
                BLEND_MOD
            };

            /// Constructor.
            Texture(s_ptr<GUIManager> pManager);

            /// Destructor.
            ~Texture();

            /// Prints all relevant information about this widget in a string.
            /** \param sTab The offset to give to all lines
            *   \return All relevant information about this widget
            */
            s_str                     Serialize(const s_str& sTab) const;

            /// Renders this widget on the current render target.
            void                      Render();

            /// Copies an UIObject's parameters into this Texture (inheritance).
            /** \param pObj The UIObject to copy
            */
            void                      CopyFrom(s_ptr<UIObject> pObj);

            /// Returns this Texture's blending mode.
            /** \return This Texture's blending mode
            */
            BlendMode                 GetBlendMode() const;

            /// Returns this Texture's color.
            /** \return This Texture's color (Color::NaN if none)
            */
            const Color&              GetColor() const;

            /// Returns this Texture's gradient.
            /** \return This Texture's gradient (Gradient::NONE if none)
            */
            const Gradient&           GetGradient() const;

            /// Returns this Texture's texture coordinates.
            /** \return This Texture's texture coordinates
            *   \note The texture coordinates are arranged as a rectangle, which is made
            *         of four points : 1 (top left), 2 (top right), 3 (bottom right) and
            *         4 (bottom left).<br>
            *         The returned array is composed like this :
            *         (x1, y1, x2, y2, x3, y3, x4, y4).
            */
            const s_array<s_float,8>& GetTexCoord() const;

            /// Checks if this Texture's dimensions are affected by texture coordinates.
            /** \return 'true' if this Texture's dimensions are affected by texture
            *           coordinates
            */
            const s_bool&             GetTexCoordModifiesRect() const;

            /// Returns this Textures's texture file.
            /** \return This Textures's texture file (empty string if none).
            */
            const s_str&              GetTexture() const;

            /// Returns this Textures's vertex color.
            /** \return This Textures's vertex color
            *   \note This color is used to filter the Texture's colors :
            *         for each pixel, the original color is multiplied
            *         by this vertex color.
            */
            Color                     GetVertexColor() const;

            /// Checks if this Texture is desaturated.
            /** \return 'true' if the Texture is desaturated
            *   \note Only available on certain graphic cards (most of modern ones
            *         are capable of this).
            */
            const s_bool&             IsDesaturated() const;

            /// Sets this Texture's blending mode.
            /** \param mBlendMode The new blending mode
            */
            void                      SetBlendMode(BlendMode mBlendMode);

            /// Sets this Texture's blending mode.
            /** \param sBlendMode The new blending mode
            */
            void                      SetBlendMode(const s_str& sBlendMode);

            /// Makes this Texture appear without any color.
            /** \param bIsDesaturated 'true' if you want to remove colors
            */
            void                      SetDesaturated(const s_bool& bIsDesaturated);

            /// Adds a Gradient effect to this Texture.
            /** \param mGradient The Gradient to add
            *   \note To remove a Gradient, call SetGradient(Gradient::NONE).
            */
            void                      SetGradient(const Gradient& mGradient);

            /// Sets this Texture's texture coordinates.
            /** \param lCoordinates This Texture's texture coordinates
            *   \note The texture coordinates are arranged as a rectangle, which is made
            *         of four points : 1 (top left), 2 (top right), 3 (bottom right) and
            *         4 (bottom left).<br>
            *         The array must be arranged like this : (x1, x3, y1, y3), or (left,
            *         right, top, bottom). Other corners are calculated using these coordinates.
            *   \note This function only allows horizontal/rectangle texture coordinates.
            */
            void                      SetTexCoord(const s_array<s_float,4>& lCoordinates);

            /// Sets this Texture's texture coordinates.
            /** \param lCoordinates This Texture's texture coordinates
            *   \note The texture coordinates are arranged as a rectangle, which is made
            *         of four points : 1 (top left), 2 (top right), 3 (bottom right) and
            *         4 (bottom left).<br>
            *         The array must be arranged like this :
            *         (x1, y1, x2, y2, x3, y3, x4, y4).
            *   \note This function allows rotated/deformed texture coordinates.
            */
            void                      SetTexCoord(const s_array<s_float,8>& lCoordinates);

            /// Sets whether this Texture's dimensions are affected by texture coordinates.
            /** \param bTexCoordModifiesRect 'true' to make dimensions change with tex coords
            */
            void                      SetTexCoordModifiesRect(const s_bool& bTexCoordModifiesRect);

            /// Sets this Texture's texture file.
            /** \param sFile The file from which to read data
            *   \note This function takes care of checking that the file can be opened.
            *   \note This function is not compatible with SetColor() : only the latest
            *         you have called will apply.
            */
            void                      SetTexture(const s_str& sFile);

            /// Reads texture data from a RenderTarget.
            /** \param pRenderTarget The RenderTarget from which to read the data
            *   \note This function is only meant for internal use and is not available
            *         to the Lua API.
            *   \note This function is not compatible with SetColor() : only the latest
            *         you have called will apply.
            */
            void                      SetTexture(s_ptr<RenderTarget> pRenderTarget);

            /// Sets this Texture's color.
            /** \param mColor The color to use
            *   \note This function is not compatible with SetTexture() : only the latest
            *         you have called will apply.
            */
            void                      SetColor(const Color& mColor);

            /// Directly sets this Texture's underlying sprite.
            /** \param pSprite The new sprite to use
            *   \note The Texture's dimensions will be adjusted to fit those
            *         of the provided sprite, and same goes for texture
            *         coordinates.
            *   \note Be sure to know what you're doing when you call this
            *         function.
            */
            void                      SetSprite(s_refptr<Sprite> pSprite);

            /// Sets this Texture's vertex color.
            /** \param mColor This Textures's new vertex color
            *   \note This color is used to filter the Texture's colors :
            *         for each pixel, the original color is multiplied
            *         by this vertex color.
            */
            void                      SetVertexColor(const Color& mColor);

            /// Creates the associated Lua glue.
            void                      CreateGlue();

            /// Parses data from an XML::Block.
            /** \param pBlock The Texture's XML::Block
            */
            void                      ParseBlock(s_ptr<XML::Block> pBlock);

            static const s_str CLASS_NAME;

        private :

            void ParseAttributes_(s_ptr<XML::Block> pBlock);
            void ParseTexCoordsBlock_(s_ptr<XML::Block> pBlock);
            void ParseColorBlock_(s_ptr<XML::Block> pBlock);
            void ParseGradientBlock_(s_ptr<XML::Block> pBlock);

            s_refptr<Sprite> pSprite_;
            s_str            sTextureFile_;

            BlendMode mBlendMode_;
            s_bool    bIsDesaturated_;
            Gradient  mGradient_;
            Color     mColor_;

            s_array<s_float,8> lTexCoord_;
            s_bool             bTexCoordModifiesRect_;

        };

        /** \cond NOT_REMOVE_FROM_DOC
        */

        class LuaTexture : public LuaLayeredRegion
        {
        public :

            LuaTexture(lua_State* pLua);

            // LayeredRegion
            int _SetVertexColor(lua_State*);
            // Texture
            int _GetBlendMode(lua_State*);
            int _GetTexCoord(lua_State*);
            int _GetTexCoordModifiesRect(lua_State*);
            int _GetTexture(lua_State*);
            int _GetVertexColor(lua_State*);
            int _IsDesaturated(lua_State*);
            int _SetBlendMode(lua_State*);
            int _SetDesaturated(lua_State*);
            int _SetGradient(lua_State*);
            int _SetGradientAlpha(lua_State*);
            int _SetTexCoord(lua_State*);
            int _SetTexCoordModifiesRect(lua_State*);
            int _SetTexture(lua_State*);

            static const char className[];
            static const char* classList[];
            static Lunar<LuaTexture>::RegType methods[];
            static const s_str CLASS_NAME;
        protected :

            s_ptr<Texture> pTextureParent_;

        };

        /** \endcond
        */
    }
}

#endif
