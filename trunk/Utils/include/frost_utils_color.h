/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */


#ifndef FROST_UTILS_COLOR_H
#define FROST_UTILS_COLOR_H

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
        /** \param fA The alpha component
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
        s_uint         GetPacked() const;

        /// Sets the alpha component.
        /** \param fA The alpha component
        */
        void           SetA(const s_float& fA);

        /// Sets the red component.
        /** \param fR The red component
        */
        void           SetR(const s_float& fR);

        /// Sets the green component.
        /** \param fG The green component
        */
        void           SetG(const s_float& fG);

        /// Sets the blue component.
        /** \param fB The blue component
        */
        void           SetB(const s_float& fB);

        /// Sets the packed ARGB component.
        /** \param uiColor packed ARGB component.
        */
        void           SetPacked(const s_uint& uiColor);

        /// Clamps color components to [0,1].
        void           Saturate();

        /// Sets if this Color has a NaN chanel.
        /** \return 'true' if this Color has a NaN chanel
        */
        s_bool         IsNaN() const;

        s_float&       operator [] (const s_uint& uiIndex);
        const s_float& operator [] (const s_uint& uiIndex) const;

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

        Color   operator *  (const s_float& fValue) const;
        Color   operator /  (const s_float& fValue) const;
        void    operator *= (const s_float& fValue);
        void    operator /= (const s_float& fValue);

        /// Creates a random color.
        /** \param bRandomAlpha 'true' to randomize the alpha chanel too,
        *                       'false' to make it opaque (alpha = 255)
        *   \return A random color
        */
        static Color  Random(const s_bool& bRandomAlpha = false);

        /// Clamps color components to [0,1].
        /** \param mColor The color to clamp
        *   \return The clamped color
        */
        static Color  Saturate(const Color& mColor);

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

        s_float a;
        s_float r;
        s_float g;
        s_float b;

    private :

        s_uint BuildUIColor_() const;
        void BuildABGRColor_(const s_uint& uiColor);
    };

    Color operator * (const s_float& fLeft, const Color& mRight);
    Color operator / (const s_float& fLeft, const Color& mRight);

    /** \cond NOT_REMOVE_FROM_DOC
    */
    template<> class StringConverter<string_element, Color>
    {
    public :

        typedef s_str::string string;

        static string Convert(const Color& mColor);
    };
    /** \endcond
    */
}

#endif
