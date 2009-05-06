/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              XXXX header               */
/*                                        */
/*                                        */

#ifndef FROST_TEXT_H
#define FROST_TEXT_H

#include "frost.h"
#include "frost_gfx.h"

namespace Frost
{
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

    /// Used to draw some text on the screen
    /**
    */
    class Text
    {
    public :

        /// Constructor.
        /** \param sFileName The path to the .ttf file to use
        *   \param fSize    The size of the font (in point)
        */
        Text(const s_str& sFileName, const s_float& fSize_);

        /// Destructor.
        ~Text();

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
        /** \param fW The new witdh
        *   \note To remove it, use s_float::NaN.
        */
        void           SetWidth(const s_float& fW);

        /// Sets the height of the text box.
        /** \param fH The new height
        *   \note To remove it, use s_float::NaN.
        */
        void           SetHeight(const s_float& fH);

        /// Returns the width of the text box.
        /** \return The width of the text box
        */
        const s_float& GetWidth() const;

        /// Returns the height of the text box.
        /** \return The height of the text box
        */
        const s_float& GetHeight() const;

        /// Returns the length of the text.
        /** \return The length of the text
        *   \note Ignores the text box.
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

        /// Returns the height of one character.
        /** \return The height of one character
        */
        s_float        GetTextHeight() const;

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
        *         Default is 1.0f.
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


        /// Returns this Text's cache.
        /** \return This Text's cache
        *   \note The cache contains the rendered text.
        */
        s_ptr<RenderTarget> GetCache();

        /// Returns this Text's font.
        /** \return This Text's font
        */
        s_ptr<Ogre::Font> GetOgreFont();

        /// Renders this Text at the given position.
        /** \param fX The horizontal position of the top left corner
        *   \param fY The vertical position of the top left corner
        *   \note This function only draws a single sprite on the screen.<br>
        *         Must be called between SpriteManager::Begin() and
        *         SpriteManager::End().
        */
        void          Render(const s_float& fX, const s_float& fY);

        /// Updates this Text's cache.
        /** \note Do not call this function between SpriteManager::Begin()
        *         and SpriteManager::End() : it uses its own render target
        *         so, it has to call Begin/End by itself.
        */
        void          Update();

        static const s_str CLASS_NAME;

    private :

        void UpdateLines_();
        void UpdateCache_();

        s_str sFileName_;
        s_bool bReady_;
        s_float fSize_;
        s_float fTracking_;
        s_float fLineSpacing_;
        s_float fSpaceWidth_;
        s_bool  bRemoveStartingSpaces_;
        Color mColor_;
        s_bool bForceColor_;
        s_float fW_, fH_;
        s_str sText_;
        std::vector<Line> lLineList_;
        std::map<s_uint, Format> lFormatList_;
        s_ptr<Ogre::Font> pOgreFont_;
        s_refptr<Material> pFontMat_;

        s_bool bUpdateCache_;
        s_bool bUpdateCacheContent_;
        s_ptr<RenderTarget> pCache_;
        s_refptr<Material>  pRTMat_;

    };
}


#endif
