/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_color.h"

#ifdef USING_OGRE
#include <OgreColourValue.h>
#endif

using namespace std;

namespace Frost
{
    const s_str Color::CLASS_NAME = "Color";

    const Color Color::BLACK = Color(0.0f, 0.0f, 0.0f);
    const Color Color::WHITE = Color(1.0f, 1.0f, 1.0f);
    const Color Color::RED   = Color(1.0f, 0.0f, 0.0f);
    const Color Color::GREEN = Color(0.0f, 1.0f, 0.0f);
    const Color Color::BLUE  = Color(0.0f, 0.0f, 1.0f);
    const Color Color::VOID  = Color(0);
    const Color Color::NaN   = Color(s_uint::NaN);

    Color::Color()
    {
        a = 1.0f;
    }

    Color::Color( const s_float& fR, const s_float& fG, const s_float& fB )
    {
        a = 1.0f;
        r = fR;
        g = fG;
        b = fB;
    }

    Color::Color( const s_float& fA, const s_float& fR, const s_float& fG, const s_float& fB )
    {
        a = fA;
        r = fR;
        g = fG;
        b = fB;
    }

    Color::Color( const s_uint& uiColor )
    {
        BuildABGRColor_(uiColor);
    }

    const s_float& Color::GetA() const
    {
        return a;
    }

    const s_float& Color::GetR() const
    {
        return r;
    }

    const s_float& Color::GetG() const
    {
        return g;
    }

    const s_float& Color::GetB() const
    {
        return b;
    }

    s_uint Color::GetPacked() const
    {
        return BuildUIColor_();
    }

    void Color::SetA( const s_float& fA )
    {
        a = fA;
    }

    void Color::SetR( const s_float& fR )
    {
        r = fR;
    }

    void Color::SetG( const s_float& fG )
    {
        g = fG;
    }

    void Color::SetB( const s_float& fB )
    {
        b = fB;
    }

    void Color::SetPacked( const s_uint& uiColor )
    {
        BuildABGRColor_(uiColor);
    }

    s_bool Color::IsNaN() const
    {
        return a.IsNaN();
    }

    void Color::Saturate()
    {
        a.Clamp(0.0f, 1.0f);
        r.Clamp(0.0f, 1.0f);
        g.Clamp(0.0f, 1.0f);
        b.Clamp(0.0f, 1.0f);
    }

    Color Color::Saturate( const Color& mColor )
    {
        Color mSat = mColor; mSat.Saturate();
        return mSat;
    }

    s_uint Color::BuildUIColor_() const
    {
        s_uint uiColor;

        if (!a.IsNaN() && !b.IsNaN() && !g.IsNaN() && !r.IsNaN())
        {
            uiColor  = uint(a.Get()*255.0f) << 24;
            uiColor += uint(b.Get()*255.0f) << 16;
            uiColor += uint(g.Get()*255.0f) << 8;
            uiColor += uint(r.Get());
        }
        else
            uiColor = s_uint::NaN;

        return uiColor;
    }

    void Color::BuildABGRColor_(const s_uint& uiColor)
    {
        if (!uiColor.IsNaN())
        {
            a = ((uiColor.Get() >> 24) & 0xFF)/255.0f;
            b = ((uiColor.Get() >> 16) & 0xFF)/255.0f;
            g = ((uiColor.Get() >> 8)  & 0xFF)/255.0f;
            r = ( uiColor.Get()        & 0xFF)/255.0f;
        }
        else
            a = b = g = r = s_float::NaN;
    }

    Color Color::Random( const s_bool& bRandomAlpha )
    {
        if (bRandomAlpha)
            return Color(
                s_float::Random(0.0f, 1.0f),
                s_float::Random(0.0f, 1.0f),
                s_float::Random(0.0f, 1.0f),
                s_float::Random(0.0f, 1.0f)
            );
        else
            return Color(
                s_float::Random(0.0f, 1.0f),
                s_float::Random(0.0f, 1.0f),
                s_float::Random(0.0f, 1.0f)
            );
    }

    #ifdef USING_OGRE
    Ogre::ColourValue Color::FrostToOgre( const Color& mColor )
    {
        return Ogre::ColourValue(
            mColor.GetR().Get(),
            mColor.GetG().Get(),
            mColor.GetB().Get(),
            mColor.GetA().Get()
        );
    }

    Color Color::OgreToFrost( const Ogre::ColourValue& mOgreColor )
    {
        return Color(
            mOgreColor.a,
            mOgreColor.r,
            mOgreColor.g,
            mOgreColor.b
        );
    }
    #endif

    s_float& Color::operator [] ( const s_uint& uiIndex )
    {
        return (&r)[uiIndex.Get()];
    }

    const s_float& Color::operator [] ( const s_uint& uiIndex ) const
    {
        return (&r)[uiIndex.Get()];
    }

    s_bool Color::operator == ( const Color& mColor ) const
    {
        return (a == mColor.a) && (r == mColor.r) && (g == mColor.g) && (b == mColor.b);
    }

    s_bool Color::operator != ( const Color& mColor ) const
    {
        return (a != mColor.a) || (r != mColor.r) || (g != mColor.g) || (b != mColor.b);
    }

    Color Color::operator * (const Color& mColor) const
    {
        return Color(
            a*mColor.a, r*mColor.r, g*mColor.g, b*mColor.b
        );
    }

    Color Color::operator / (const Color& mColor) const
    {
        return Color(
            s_float::Min(a/mColor.a, 1.0f),
            s_float::Min(r/mColor.r, 1.0f),
            s_float::Min(g/mColor.g, 1.0f),
            s_float::Min(b/mColor.b, 1.0f)
        );
    }

    Color Color::operator + (const Color& mColor) const
    {
        return Color(
            a + mColor.a,
            r + mColor.r,
            g + mColor.g,
            b + mColor.b
        );
    }

    Color Color::operator - (const Color& mColor) const
    {
        return Color(
            a - mColor.a,
            r - mColor.r,
            g - mColor.g,
            b - mColor.b
        );
    }

    Color& Color::operator *= (const Color& mColor)
    {
        a *= mColor.a;
        r *= mColor.r;
        g *= mColor.g;
        b *= mColor.b;

        return *this;
    }

    Color& Color::operator /= (const Color& mColor)
    {
        a /= mColor.a;
        r /= mColor.r;
        g /= mColor.g;
        b /= mColor.b;

        return *this;
    }

    Color& Color::operator += (const Color& mColor)
    {
        a += mColor.a;
        r += mColor.r;
        g += mColor.g;
        b += mColor.b;

        return *this;
    }

    Color& Color::operator -= (const Color& mColor)
    {
        a -= mColor.a;
        r -= mColor.r;
        g -= mColor.g;
        b -= mColor.b;

        return *this;
    }

    Color Color::operator * ( const s_float& fValue ) const
    {
        return Color(r*fValue, g*fValue, b*fValue);
    }

    Color Color::operator / ( const s_float& fValue ) const
    {
        return Color(r/fValue, g/fValue, b/fValue);
    }

    void Color::operator *= ( const s_float& fValue )
    {
        r *= fValue; g *= fValue; b *= fValue;
    }

    void Color::operator /= ( const s_float& fValue )
    {
        r /= fValue; g /= fValue; b /= fValue;
    }

    Color operator * (const s_float& fLeft, const Color& mRight)
    {
        return mRight*fLeft;
    }

    Color operator / (const s_float& fLeft, const Color& mRight)
    {
        return mRight/fLeft;
    }

    s_str::string StringConverter<string_element, Color>::Convert( const Color& mColor )
    {
        return  "(a:"+StringConverter<string_element, s_uint>::Convert(s_uint(mColor.GetA()*255.0f))+
               ", r:"+StringConverter<string_element, s_uint>::Convert(s_uint(mColor.GetR()*255.0f))+
               ", g:"+StringConverter<string_element, s_uint>::Convert(s_uint(mColor.GetG()*255.0f))+
               ", b:"+StringConverter<string_element, s_uint>::Convert(s_uint(mColor.GetB()*255.0f))+")";
    }
}
