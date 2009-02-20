/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_TEXTURE_H
#define FROST_GUI_TEXTURE_H

#include "frost.h"
#include "gui/frost_gui_layeredregion.h"
#include "gui/frost_gui_gradient.h"

namespace Frost
{
    namespace GUI
    {
        enum TextureType
        {
            TTYPE_FILE,
            TTYPE_COLOR
        };

        enum TextureBlendMode
        {
            BLEND_NONE,
            BLEND_BLEND,
            BLEND_KEY,
            BLEND_ADD,
            BLEND_MOD
        };

        /// The base of the GUI's appearence
        /** This object contains either a texture taken from a file,
        *   or a plain color.
        */
        class Texture : public LayeredRegion
        {
        public :

            /// Constructor.
            Texture();

            /// Returns this Textures's blending mode.
            /** \return This Textures's blending mode
            */
            TextureBlendMode          GetBlendMode() const;

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
            /** \return This Textures's texture file
            *   \note If this Texture is a plain color, this function returns
            *         "Solid Texture".
            */
            const s_str&              GetTexture() const;

            /// Returns this Textures's vertex color.
            /** \return This Textures's vertex color
            *   \note This color is used to filter the Texture's colors :
            *         for each pixel, the original color is multiplied
            *         by this vertex color.
            */
            const Color&              GetVertexColor() const;

            /// Checks if this Texture is desaturated.
            /** \return 'true' if the Texture is desaturated
            *   \note Only available on certain graphic cards (most of modern ones
            *         are capable of this).
            */
            const s_bool&             IsDesaturated() const;

            /// Sets this Texture's blending mode.
            /** \param mBlendMode The new blending mode
            */
            void                      SetBlendMode(TextureBlendMode mBlendMode);

            /// Makes this Texture appear without any color.
            /** \param bIsDesaturated 'true' if you want to remove colors
            *   \note Only available on certain graphic cards (most of modern ones
            *         are capable of this).
            */
            void                      SetDesaturated(const s_bool& bIsDesaturated) const;

            /// Adds a Gradient effect to this Texture.
            /** \param mGradient The Gradient to add
            *   \note To remove a Gradient, call SetGradient(Gradient::EMPTY_GRADIENT).
            */
            void                      SetGradient(const Gradient& mGradient);

            /// Sets this Texture's texture coordinates.
            /** \param lCoordinates This Texture's texture coordinates
            *   \note The texture coordinates are arranged as a rectangle, which is made
            *         of four points : 1 (top left), 2 (top right), 3 (bottom right) and
            *         4 (bottom left).<br>
            *         The array must be arranged like this : (x1, y1, x3, y3). Other
            *         corners are calculated using these coordinates.
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
            *   \note To disable this texture file, call SetTexture("").
            *   \note This function is not compatible with SetColor() : only the latest
            *         you have called will apply.
            */
            void                      SetTexture(const s_str& sFile);

            /// Sets this Texture's color.
            /** \param mColor The color to use
            *   \note This function is not compatible with SetTexture() : only the latest
            *         you have called will apply.
            */
            void                      SetColor(const Color& mColor);

            /// Sets this Textures's vertex color.
            /** \param mColor This Textures's new vertex color
            *   \note This color is used to filter the Texture's colors :
            *         for each pixel, the original color is multiplied
            *         by this vertex color.
            */
            void                      SetVertexColor(const Color& mColor);

            /// Returns this widget's Lua glue.
            virtual void              CreateGlue();

            static const s_str CLASS_NAME;

        protected :

            TextureType        mType_;
            TextureBlendMode   mBlendMode_;
            s_bool             bIsDesaturated_;
            Gradient           mGradient_;

            s_str              sTextureFile_;
            s_array<s_float,8> lTexCoord_;
            s_bool             bTexCoordModifiesRect_;

            Color              mColor_;

        };

        class LuaTexture : public LuaLayeredRegion
        {
        public :

            LuaTexture(lua_State* pLua);

            // Glues
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
            int _SetVertexColor(lua_State*);

            static const char className[];
            static Lunar<LuaTexture>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_ptr<Texture> pTextureParent_;

        };
    }
}

#endif
