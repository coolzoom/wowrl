/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */


#ifndef FROST_UTILS_COLOR_H
#define FROST_UTILS_COLOR_H

#include "frost_utils_config.h"
#include "frost_utils_types.h"

#ifdef USING_OGRE
namespace Ogre
{
    class ColourValue;
}
#endif

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
        /** \param fR The red component
        *   \param fG The green component
        *   \param fB The blue component
        */
        Color(const s_float& fR, const s_float& fG, const s_float& fB);

        /// Constructor.
        /** \param ucA The alpha component
        *   \param fR The green component
        *   \param fG The green component
        *   \param fB The blue component
        */
        Color(const s_float& fA, const s_float& fR, const s_float& fG, const s_float& fB);

        /// Constructor.
        /** \param uiColor The ARGB packed color
        */
        Color(const s_uint& uiColor);

        /// Returns the alpha component.
        /** \return The alpha component
        */
        const s_float& GetA() const;

        /// Returns the red component.
        /** \return The red component
        */
        const s_float& GetR() const;

        /// Returns the green component.
        /** \return The green component
        */
        const s_float& GetG() const;

        /// Returns the blue component.
        /** \return The blue component
        */
        const s_float& GetB() const;

        /// Returns the packed ARGB value.
        /** \return The packed ARGB value
        */
        const s_uint& GetPacked() const;

        /// Sets the alpha component.
        /** \param ucA The alpha component
        */
        void          SetA(const s_float& fA);

        /// Sets the red component.
        /** \param fR The red component
        */
        void          SetR(const s_float& fR);

        /// Sets the green component.
        /** \param fG The green component
        */
        void          SetG(const s_float& fG);

        /// Sets the blue component.
        /** \param fB The blue component
        */
        void          SetB(const s_float& fB);

        /// Sets the packed ARGB component.
        /** \param uiColor packed ARGB component.
        */
        void          SetPacked(const s_uint& uiColor);

        /// Sets if this Color has a NaN chanel.
        /** \return 'true' if this Color has a NaN chanel
        */
        s_bool        IsNaN() const;

        s_bool operator == (const Color& mColor) const;
        s_bool operator != (const Color& mColor) const;

        Color  operator * (const Color& mColor) const;
        Color  operator / (const Color& mColor) const;

        Color  operator + (const Color& mColor) const;
        Color  operator - (const Color& mColor) const;

        Color&  operator *= (const Color& mColor);
        Color&  operator /= (const Color& mColor);

        Color&  operator += (const Color& mColor);
        Color&  operator -= (const Color& mColor);

        /// Creates a random color.
        /** \param bRandomAlpha 'true' to randomize the alpha chanel too,
        *                       'false' to make it opaque (alpha = 255)
        *   \return A random color
        */
        static Color  Random(const s_bool& bRandomAlpha = false);

        #ifdef USING_OGRE
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
        #endif

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

        s_float fA_;
        s_float fR_;
        s_float fG_;
        s_float fB_;

        s_uint uiColor_;
    };

    s_str operator + (const char* sLeft, const Color& mRight);
    s_str operator + (const s_str& sLeft, const Color& mRight);
}

#endif
