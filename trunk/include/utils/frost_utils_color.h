/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */


#ifndef FROST_UTILS_COLOR_H
#define FROST_UTILS_COLOR_H

#include "frost_utils.h"
#include "frost_utils_uint.h"

namespace Ogre
{
    class ColourValue;
}

namespace Frost
{
    /// Color container
    /** Automatically builds ARGB packed
    *   color out of separated A, R, G and B
    *   values and vice versa.
    */
    class Color
    {
    public :

        /// Default constructor.
        Color();

        /// Constructor.
        /** \param uiR The red component
        *   \param uiG The green component
        *   \param uiB The blue component
        */
        Color(const s_uint& uiR, const s_uint& uiG, const s_uint& uiB);

        /// Constructor.
        /** \param uiA The alpha component
        *   \param uiR The green component
        *   \param uiG The green component
        *   \param uiB The blue component
        */
        Color(const s_uint& uiA, const s_uint& uiR, const s_uint& uiG, const s_uint& uiB);

        /// Constructor.
        /** \param uiColor The ARGB packed color
        */
        Color(const s_uint& uiColor);

        /// Returns the alpha component.
        /** \return The alpha component
        */
        const s_uint& GetA() const;

        /// Returns the red component.
        /** \return The red component
        */
        const s_uint& GetR() const;

        /// Returns the green component.
        /** \return The green component
        */
        const s_uint& GetG() const;

        /// Returns the blue component.
        /** \return The blue component
        */
        const s_uint& GetB() const;

        /// Returns the packed ARGB value.
        /** \return The packed ARGB value
        */
        const s_uint& GetPacked() const;

        /// Sets the alpha component.
        /** \param uiA The alpha component
        */
        void          SetA(const s_uint& uiA);

        /// Sets the red component.
        /** \param uiR The red component
        */
        void          SetR(const s_uint& uiR);

        /// Sets the green component.
        /** \param uiG The green component
        */
        void          SetG(const s_uint& uiG);

        /// Sets the blue component.
        /** \param uiB The blue component
        */
        void          SetB(const s_uint& uiB);

        /// Sets the packed ARGB component.
        /** \param uiColor packed ARGB component.
        */
        void          SetPacked(const s_uint& uiColor);

        /// Sets if this Color is valid.
        /** \return 'true' if this Color is valid
        *   \note The only Color which is not valid is Color::NaN.
        */
        s_bool        IsValid() const;

        s_bool operator == (const Color& mColor) const;
        s_bool operator != (const Color& mColor) const;

        /// Creates a random color.
        /** \param bRandomAlpha 'true' to randomize the alpha chanel too,
        *                       'false' to make it opaque (alpha = 255)
        *   \return A random color
        */
        static Color  Random(const s_bool& bRandomAlpha = false);

        /// Converts a Frost color to an Ogre one.
        /** \param mColor The color to convert
        *   \return The Ogre color
        */
        static Ogre::ColourValue FrostToOgre(const Color& mColor);

        /// Converts an Ogre color to a Frost one.
        /** \param mColor The color to convert
        *   \return The Frost color
        */
        static Color OgreToFrost(const Ogre::ColourValue& mColor);

        static const Color BLACK;
        static const Color WHITE;
        static const Color RED;
        static const Color GREEN;
        static const Color BLUE;
        static const Color VOID;
        static const Color NaN;

        static const s_str CLASS_NAME;

    private :

        void BuildUIColor_();
        void BuildABGRColor_();

        s_uint uiA_;
        s_uint uiR_;
        s_uint uiG_;
        s_uint uiB_;

        s_uint uiColor_;
    };

    s_str operator + (const char* sLeft, const Color& mRight);
    s_str operator + (const s_str& sLeft, const Color& mRight);
}

#endif
