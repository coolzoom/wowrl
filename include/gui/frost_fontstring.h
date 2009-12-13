/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_FONTSTRING_H
#define FROST_GUI_FONTSTRING_H

#include "frost.h"
#include "gui/frost_layeredregion.h"
#include "gui/frost_text.h"

namespace Frost
{
    namespace GUI
    {
        /// The GUI class used to draw text on the screen
        /** This object contains formated text, managed
        *   by the Text class.
        */
        class FontString : public LayeredRegion
        {
        public :

            /// Constructor.
            FontString();

            /// Destructor.
            ~FontString();

            /// Prints all relevant information about this widget in a string.
            /** \param sTab The offset to give to all lines
            *   \return All relevant information about this widget
            */
            s_str            Serialize(const s_str& sTab) const;

            /// Renders this widget on the current render target.
            void             Render();

            /// Copies an UIObject's parameters into this FontString (inheritance).
            /** \param pObj The UIObject to copy
            */
            void             CopyFrom(s_ptr<UIObject> pObj);

            /// Updates this widget's logic.
            void             Update();

            /// Returns the name of the font file.
            /** \return The name of the font file
            */
            const s_str&     GetFontName() const;

            /// Returns the heigh of the font.
            /** \return The heigh of the font
            */
            const s_uint&    GetFontHeight() const;

            /// Adds or remove the outline around the text.
            /** \param bIsOutlined 'true' to enable the outline
            *   \note The thickness of this outline is constant and
            *         does not depend on the font's size.
            */
            void             SetOutlined(const s_bool& bIsOutlined);

            /// Check if this FontString is outlined.
            /** \return 'true' if this FontString is outlined
            */
            const s_bool&    IsOutlined() const;

            /// Returns the horizontal alignment behavior.
            /** \return The horizontal alignment behavior
            */
            Text::Alignment  GetJustifyH() const;

            /// Returns the vertical alignment behavior.
            /** \return The vertical alignment behavior
            */
            Text::VerticalAlignment GetJustifyV() const;

            /// Returns this FontString's shadow color.
            /** \return This FontString's shadow color
            */
            const Color&     GetShadowColor() const;

            /// Returns this FontString's shadow offsets.
            /** \return This FontString's shadow offsets
            *   \note Contains (X, Y) offsets.
            */
            s_array<s_int,2> GetShadowOffsets() const;

            /// Returns this FontString's shadow X offset.
            /** \return This FontString's shadow X offset
            */
            const s_int&     GetShadowXOffset() const;

            /// Returns this FontString's shadow Y offset.
            /** \return This FontString's shadow Y offset
            */
            const s_int&     GetShadowYOffset() const;

            /// Returns this FontString's offsets.
            /** \return This FontString's offsets
            *   \note Contains (X, Y) offsets.
            */
            s_array<s_int,2> GetOffsets() const;

            /// Returns the space between each letter.
            /** \return The space between each letter
            */
            const s_float&   GetSpacing() const;

            /// Returns the text color.
            /** \return The text color
            */
            const Color&     GetTextColor() const;

            /// Sets this FontString's font (file and size).
            /** \param sFontName   The file path to the .ttf file
            *   \param uiHeight    The font height
            */
            void             SetFont(const s_str& sFontName, const s_uint& uiHeight);

            /// Sets this FontString's horizontal aligment behavior.
            /** \param mJustifyH The horizontal alignment behavior
            */
            void             SetJustifyH(Text::Alignment mJustifyH);

            /// Sets this FontString's vertical aligment behavior.
            /** \param mJustifyV The vertical alignment behavior
            */
            void             SetJustifyV(Text::VerticalAlignment mJustifyV);

            /// Sets this FontString's shadow color.
            /** \param mShadowColor The shadow color
            */
            void             SetShadowColor(const Color& mShadowColor);

            /// Sets this FontString's shadow offsets.
            /** \param iShadowXOffset The horizontal offset
            *   \param iShadowYOffset The vertical offset
            */
            void             SetShadowOffsets(const s_int& iShadowXOffset, const s_int& iShadowYOffset);

            /// Sets this FontString's shadow offsets.
            /** \param lShadowOffsets Offsets
            *   \note Contains (X, Y) offsets.
            */
            void             SetShadowOffsets(const s_array<s_int,2>& lShadowOffsets);

            /// Sets this FontString's offsets.
            /** \param iXOffset The horizontal offset
            *   \param iYOffset The vertical offset
            */
            void             SetOffsets(const s_int& iXOffset, const s_int& iYOffset);

            /// Sets this FontString's offsets.
            /** \param lOffsets Offsets
            *   \note Contains (X, Y) offsets.
            */
            void             SetOffsets(const s_array<s_int,2>& lOffsets);

            /// Sets the space between each letter.
            /** \param fSpacing The space between each letter
            */
            void             SetSpacing(const s_float& fSpacing);

            /// Sets the text color.
            /** \param mTextColor The text color
            */
            void             SetTextColor(const Color& mTextColor);

            /// Checks is large text is truncated or wrapped.
            /** \return 'true' if larget text is truncated
            *   \note See SetNonSpaceWrap for more infos.
            */
            const s_bool&    CanNonSpaceWrap() const;

            /// Returns the height of the string if not format is applied.
            /** \return The height of the string if not format is applied
            */
            s_float          GetStringHeight() const;

            /// Returns the width of the string if not format is applied.
            /** \return The width of the string if not format is applied
            */
            s_float          GetStringWidth() const;

            /// Returns the rendered text (with format tags).
            /** \return The rendered text (with format tags)
            */
            const s_str&     GetText() const;

            /// Sets whether large text is truncated or wrapped.
            /** \param bCanNonSpaceWrap 'true' to truncate the text
            *   \note This applies to large chunks of text with no
            *         spaces. When truncated, "..." is appended at
            *         the line's end. Else, the "word" is cut and
            *         continues on the next line.
            */
            void             SetNonSpaceWrap(const s_bool& bCanNonSpaceWrap);

            /// Checks if this FontString draws a shadow under its text.
            /** \return 'true' if this FontString draws a shadow under its text
            */
            const s_bool&    HasShadow() const;

            /// Sets whether this FontString should draw a shadow under its text.
            /** \param bHasShadow 'true' to enable shadow
            */
            void             SetShadow(const s_bool& bHasShadow);

            /// Sets the rendered text.
            /** \param sText The rendered text
            *   \note See Text::SetText for more infos about formatting.
            */
            void             SetText(const s_str& sText);

            /// Creates the associated Lua glue.
            void             CreateGlue();

            /// Parses data from an XML::Block.
            /** \param pBlock The FontString's XML::Block
            */
            void             ParseBlock(s_ptr<XML::Block> pBlock);

            static const s_str CLASS_NAME;

        private :

            void ParseAttributes_(s_ptr<XML::Block> pBlock);
            void ParseColorBlock_(s_ptr<XML::Block> pBlock);
            void ParseShadowBlock_(s_ptr<XML::Block> pBlock);

            void UpdateBorders_();

            s_refptr<Text>  pText_;
            s_str           sText_;
            s_str           sFontName_;
            s_uint          uiHeight_;
            s_float         fSpacing_;
            Text::Alignment mJustifyH_;
            Text::VerticalAlignment mJustifyV_;
            s_bool          bIsOutlined_;
            s_bool          bHasShadow_;
            s_bool          bCanNonSpaceWrap_;
            Color           mTextColor_;
            Color           mShadowColor_;
            s_int           iShadowXOffset_;
            s_int           iShadowYOffset_;
            s_int           iXOffset_;
            s_int           iYOffset_;

        };

        /** \cond NOT_REMOVE_FROM_DOC
        */

        class LuaFontString : public LuaLayeredRegion
        {
        public :

            LuaFontString(lua_State* pLua);

            // FontInstance
            int _GetFont(lua_State*);
            int _GetFontObject(lua_State*) { return 0; } // WBI
            int _GetJustifyH(lua_State*);
            /**/ int _GetJustifyV(lua_State*) { return 0; }
            int _GetShadowColor(lua_State*);
            int _GetShadowOffset(lua_State*);
            int _GetSpacing(lua_State*);
            int _GetTextColor(lua_State*);
            int _SetFont(lua_State*);
            int _SetFontObject(lua_State*) { return 0; } // WBI
            int _SetJustifyH(lua_State*);
            /**/ int _SetJustifyV(lua_State*) { return 0; }
            int _SetShadowColor(lua_State*);
            int _SetShadowOffset(lua_State*);
            int _SetSpacing(lua_State*);
            int _SetTextColor(lua_State*);
            // FontString
            int _CanNonSpaceWrap(lua_State*);
            int _GetStringHeight(lua_State*);
            int _GetStringWidth(lua_State*);
            int _GetText(lua_State*);
            /**/ int _SetAlphaGradient(lua_State*) { return 0; }
            int _SetNonSpaceWrap(lua_State*);
            int _SetText(lua_State*);
            /**/ int _SetTextHeight(lua_State*) { return 0; }

            static const char className[];
            static const char* classList[];
            static Lunar<LuaFontString>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_ptr<FontString> pFontStringParent_;

        };

        /** \endcond
        */
    }
}

#endif
