/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Text header               */
/*                                        */
/*                                        */

#ifndef FROST_TEXT_H
#define FROST_TEXT_H

#include "frost.h"

namespace Frost
{
    /// Used to draw some text on the screen
    /**
    */
    class Text
    {
    public :

        /// Contains a string that will be drawn on a line
        struct Line
        {
            s_str   sCaption;
            s_float fWidth;
        };

        enum ColorAction
        {
            COLOR_ACTION_NONE,
            COLOR_ACTION_SET,
            COLOR_ACTION_RESET
        };

        /// Contains information about the text at a given position
        struct Format
        {
            Format() : mColorAction(COLOR_ACTION_NONE)
            {}

            Color       mColor;
            ColorAction mColorAction;
        };

        enum Alignment
        {
            ALIGN_LEFT,
            ALIGN_CENTER,
            ALIGN_RIGHT
        };

        enum VerticalAlignment
        {
            ALIGN_TOP,
            ALIGN_MIDDLE,
            ALIGN_BOTTOM
        };

        /// Holds the position, tex. coordinates and color of a character.
        struct Letter
        {
            s_float fX1, fY1;
            s_float fX2, fY2;
            s_float fU1, fV1;
            s_float fU2, fV2;
            Color   mColor;
        };

        /// Constructor.
        /** \param sFileName The path to the .ttf file to use
        *   \param fSize    The size of the font (in point)
        */
        Text(const s_str& sFileName, const s_float& fSize);

        /// Destructor.
        ~Text();

        /// Returns the path to the .ttf file.
        /** \return The path to the .ttf file
        */
        const s_str&   GetFontName() const;

        /// Returns the size of the font.
        /** \return The size of the font
        */
        const s_float& GetFontSize() const;

        /// Returns the height of one line (constant).
        /** \return The height of one line (constant)
        */
        s_float        GetLineHeight() const;

        /// Set the text to render.
        /** \param sText The text to render
        *   \note This text can be formated :<br>
        *         - "|cAARRGGBB" : sets text color (hexadecimal).<br>
        *         - "|r" : sets text color to default.<br>
        *         - "||" : writes "|".
        */
        void           SetText(const s_str& sText);

        /// Returns the text that will be rendered.
        /** \return The text that will be rendered
        *   \note This string contains format tags.
        */
        const s_str&   GetText() const;

        /// Sets this text's default color.
        /** \param mColor      The default color
        *   \param bForceColor 'true' to ignore color tags
        */
        void           SetColor(const Color& mColor, const s_bool& bForceColor = false);

        /// Returns this text's default color.
        /** \return This text's default color
        */
        const Color&   GetColor() const;

        /// Sets the dimensions of the text box.
        /** \param fW The new witdh
        *   \param fH The new height
        *   \note To remove the text box, use s_float::NaN.
        */
        void           SetDimensions(const s_float& fW, const s_float& fH);

        /// Sets the width of the text box.
        /** \param fBoxW The new witdh
        *   \note To remove it, use s_float::NaN.
        */
        void           SetBoxWidth(const s_float& fBoxW);

        /// Sets the height of the text box.
        /** \param fBoxH The new height
        *   \note To remove it, use s_float::NaN.
        */
        void           SetBoxHeight(const s_float& fBoxH);

        /// Returns the width of the rendered text.
        /** \return The width of the rendered text
        *   \note Takes the text box into account if any.
        */
        const s_float& GetWidth();

        /// Returns the height of the rendered text.
        /** \return The height of the rendered text
        *   \note Takes the text box into account if any.
        */
        const s_float& GetHeight();

        /// Returns the width of the text box.
        /** \return The width of the text box
        */
        const s_float& GetBoxWidth() const;

        /// Returns the height of the text box.
        /** \return The height of the text box
        */
        const s_float& GetBoxHeight() const;

        /// Returns the length of the text.
        /** \return The length of the text
        *   \note Ignores the text box, but not manual line jumps.
        */
        s_float        GetTextWidth() const;

        /// Returns the lenght of a provided string.
        /** \param sString The string to measure
        *   \return The lenght of the provided string
        */
        s_float        GetStringWidth(const s_str& sString) const;

        /// Returns the length of a single character.
        /** \param uiChar The character to measure
        *   \return The lenght of this character
        *   \note Thanks to <windows.h>, I can't name this function
        *         "GetCharWidth"... Bloody macros !
        */
        s_float        GetCharacterWidth(const s_uint& uiChar) const;

        /// Returns the kerning between two characters.
        /** \param uiChar1 The first character
        *   \param uiChar2 The second character
        *   \return The kerning between two characters
        *   \note Kerning is a letter spacing adjustment that makes the
        *         text look more condensed : is you stick an A near a V,
        *         you can reduce the space between the two letters, but not
        *         if you put two Vs side to side.
        */
        s_float        GetCharacterKerning(const s_uint& uiChar1, const s_uint& uiChar2) const;

        /// Returns the height of the text.
        /** \return The height of one text
        *   \note Ignores the text box, but not manual line jumps.
        */
        s_float        GetTextHeight() const;

        /// Sets text alignment.
        /** \param mAlign The new alignment
        */
        void           SetAlignment(const Alignment& mAlign);

        /// Sets text vertical alignment.
        /** \param mVertAlign The new vertical alignment
        */
        void           SetVerticalAlignment(const VerticalAlignment& mVertAlign);

        /// Returns the text aligment.
        /** \return The text alignment
        */
        const Alignment& GetAlignment() const;

        /// Returns the text aligment.
        /** \return The text alignment
        */
        const VerticalAlignment& GetVerticalAlignment() const;

        /// Sets this Text's tracking.
        /** \param fTracking The new tracking
        *   \note Tracking is the space between each character. Default
        *         is 0.
        */
        void           SetTracking(const s_float& fTracking);

        /// Returns this Text's tracking.
        /** \return This Text's tracking
        */
        const s_float& GetTracking() const;

        /// Sets this Text's line spacing.
        /** \param fLineSpacing The new line spacing
        *   \note Line spacing is a coefficient that, multiplied by the
        *         height of a line, gives the space between two lines.
        *         Default is 1.5f.
        */
        void           SetLineSpacing(const s_float& fLineSpacing);

        /// Returns this Text's line spacing.
        /** \return This Text's line spacing
        */
        const s_float& GetLineSpacing() const;

        /// Allows removal of a line's starting spaces.
        /** \param bRemoveStartingSpaces 'true' to remove them
        *   \note The text box does word wrapping : it cuts too long
        *         lines only between words. But sometimes, the rendered
        *         text must be cut between several spaces. By default,
        *         the algorithm puts cuted spaces at the beginning of
        *         the next line. You can change this behavior by setting
        *         this function to 'true'.
        */
        void           SetRemoveStartingSpaces(const s_bool& bRemoveStartingSpaces);

        /// Checks if starting spaces removing is active.
        /** \return 'true' if starting spaces removing is active
        */
        const s_bool&  GetRemoveStartingSpaces() const;

        /// Returns this Text's font.
        /** \return This Text's font
        */
        s_ptr<Ogre::Font> GetOgreFont();

        /// Returns this Text's material.
        /** \return This Text's material
        */
        s_refptr<Material> GetMaterial();

        /// Renders this Text at the given position.
        /** \param fX The horizontal position of the top left corner
        *   \param fY The vertical position of the top left corner
        *   \note Must be called between SpriteManager::Begin() and
        *         SpriteManager::End().
        */
        void           Render(const s_float& fX, const s_float& fY);

        /// Updates this Text's cache.
        /** \note Automatically done by Render().<br>
        *         Only use this method if you need it to
        *         be updated sooner.
        */
        void           Update();

        static const s_str CLASS_NAME;

    private :

        void UpdateLines_();
        void UpdateCache_();

        s_str     sFileName_;
        s_bool    bReady_;
        s_float   fSize_;
        s_float   fTracking_;
        s_float   fLineSpacing_;
        s_float   fSpaceWidth_;
        s_bool    bRemoveStartingSpaces_;
        Color     mColor_;
        s_bool    bForceColor_;
        s_float   fW_, fH_;
        s_float   fX_, fY_;
        s_float   fBoxW_, fBoxH_;
        s_str     sText_;
        Alignment mAlign_;
        VerticalAlignment mVertAlign_;

        s_ctnr<Line>          lLineList_;
        s_map<s_uint, Format> lFormatList_;

        s_refptr<Quad> pQuad_;

        s_ptr<Ogre::Font>  pOgreFont_;
        s_refptr<Material> pFontMat_;

        s_bool         bUpdateCache_;
        s_ctnr<Letter> lLetterCache_;

        s_bool         bUpdateQuads_;
        s_ctnr<Quad>   lQuadList_;

    };
}


#endif
