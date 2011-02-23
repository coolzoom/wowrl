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
        fA_ = 1.0f;
        BuildUIColor_();
    }

    Color::Color( const s_float& fR, const s_float& fG, const s_float& fB )
    {
        fA_ = 1.0f;
        fR_ = fR;
        fG_ = fG;
        fB_ = fB;
        BuildUIColor_();
    }

    Color::Color( const s_float& fA, const s_float& fR, const s_float& fG, const s_float& fB )
    {
        fA_ = fA;
        fR_ = fR;
        fG_ = fG;
        fB_ = fB;
        BuildUIColor_();
    }

    Color::Color( const s_uint& uiColor )
    {
        uiColor_ = uiColor;
        BuildABGRColor_();
    }

    const s_float& Color::GetA() const
    {
        return fA_;
    }

    const s_float& Color::GetR() const
    {
        return fR_;
    }

    const s_float& Color::GetG() const
    {
        return fG_;
    }

    const s_float& Color::GetB() const
    {
        return fB_;
    }

    const s_uint& Color::GetPacked() const
    {
        return uiColor_;
    }

    void Color::SetA( const s_float& fA )
    {
        fA_ = fA;
        BuildUIColor_();
    }

    void Color::SetR( const s_float& fR )
    {
        fR_ = fR;
        BuildUIColor_();
    }

    void Color::SetG( const s_float& fG )
    {
        fG_ = fG;
        BuildUIColor_();
    }

    void Color::SetB( const s_float& fB )
    {
        fB_ = fB;
        BuildUIColor_();
    }

    void Color::SetPacked( const s_uint& uiColor )
    {
        uiColor_ = uiColor;
        BuildABGRColor_();
    }

    s_bool Color::IsNaN() const
    {
        return uiColor_.IsNaN();
    }

    void Color::BuildUIColor_()
    {
        if (!fA_.IsNaN() && !fB_.IsNaN() && !fG_.IsNaN() && !fR_.IsNaN())
        {
            uiColor_  = uint(fA_.Get()*255.0f) << 24;
            uiColor_ += uint(fB_.Get()*255.0f) << 16;
            uiColor_ += uint(fG_.Get()*255.0f) << 8;
            uiColor_ += uint(fR_.Get());
        }
        else
            uiColor_ = s_uint::NaN;
    }

    void Color::BuildABGRColor_()
    {
        if (!uiColor_.IsNaN())
        {
            fA_ = ((uiColor_.Get() >> 24) & 0xFF)/255.0f;
            fB_ = ((uiColor_.Get() >> 16) & 0xFF)/255.0f;
            fG_ = ((uiColor_.Get() >> 8)  & 0xFF)/255.0f;
            fR_ = ( uiColor_.Get()        & 0xFF)/255.0f;
        }
        else
        {
            fA_ = fB_ = fG_ = fR_ = s_float::NaN;
        }
    }

    Color Color::Random( const s_bool& bRandomAlpha )
    {
        if (bRandomAlpha)
            return Color(
                s_float::Random(0, 1.0f),
                s_float::Random(0, 1.0f),
                s_float::Random(0, 1.0f),
                s_float::Random(0, 1.0f)
            );
        else
            return Color(
                s_float::Random(0, 1.0f),
                s_float::Random(0, 1.0f),
                s_float::Random(0, 1.0f)
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

    s_bool Color::operator == ( const Color& mColor ) const
    {
        return (fA_ == mColor.fA_) && (fR_ == mColor.fR_) && (fG_ == mColor.fG_) && (fB_ == mColor.fB_);
    }

    s_bool Color::operator != ( const Color& mColor ) const
    {
        return (fA_ != mColor.fA_) || (fR_ != mColor.fR_) || (fG_ != mColor.fG_) || (fB_ != mColor.fB_);
    }

    Color Color::operator * (const Color& mColor) const
    {
        return Color(
            fA_*mColor.fA_, fR_*mColor.fR_, fG_*mColor.fG_, fB_*mColor.fB_
        );
    }

    Color Color::operator / (const Color& mColor) const
    {
        return Color(
            s_float::Min(fA_/mColor.fA_, 1.0f),
            s_float::Min(fR_/mColor.fR_, 1.0f),
            s_float::Min(fG_/mColor.fG_, 1.0f),
            s_float::Min(fB_/mColor.fB_, 1.0f)
        );
    }

    Color Color::operator + (const Color& mColor) const
    {
        return Color(
            s_float::Min(fA_ + mColor.fA_, 1.0f),
            s_float::Min(fR_ + mColor.fR_, 1.0f),
            s_float::Min(fG_ + mColor.fG_, 1.0f),
            s_float::Min(fB_ + mColor.fB_, 1.0f)
        );
    }

    Color Color::operator - (const Color& mColor) const
    {
        return Color(
            s_float::Max(fA_ - mColor.fA_, 0.0f),
            s_float::Max(fR_ - mColor.fR_, 0.0f),
            s_float::Max(fG_ - mColor.fG_, 0.0f),
            s_float::Max(fB_ - mColor.fB_, 0.0f)
        );
    }

    Color& Color::operator *= (const Color& mColor)
    {
        fA_ *= mColor.fA_;
        fR_ *= mColor.fR_;
        fG_ *= mColor.fG_;
        fB_ *= mColor.fB_;

        return *this;
    }

    Color& Color::operator /= (const Color& mColor)
    {
        fA_ = s_float::Min(fA_/mColor.fA_, 1.0f);
        fR_ = s_float::Min(fR_/mColor.fR_, 1.0f);
        fG_ = s_float::Min(fG_/mColor.fG_, 1.0f);
        fB_ = s_float::Min(fB_/mColor.fB_, 1.0f);

        return *this;
    }

    Color& Color::operator += (const Color& mColor)
    {
        fA_ = s_float::Min(fA_ + mColor.fA_, 1.0f);
        fR_ = s_float::Min(fR_ + mColor.fR_, 1.0f);
        fG_ = s_float::Min(fG_ + mColor.fG_, 1.0f);
        fB_ = s_float::Min(fB_ + mColor.fB_, 1.0f);

        return *this;
    }

    Color& Color::operator -= (const Color& mColor)
    {
        fA_ = s_float::Max(fA_ - mColor.fA_, 0.0f);
        fR_ = s_float::Max(fR_ - mColor.fR_, 0.0f);
        fG_ = s_float::Max(fG_ - mColor.fG_, 0.0f);
        fB_ = s_float::Max(fB_ - mColor.fB_, 0.0f);

        return *this;
    }

    Color Color::operator * ( const s_float& fValue ) const
    {
        return Color(fR_*fValue, fG_*fValue, fB_*fValue);
    }

    Color Color::operator / ( const s_float& fValue ) const
    {
        return Color(fR_/fValue, fG_/fValue, fB_/fValue);
    }

    void Color::operator *= ( const s_float& fValue )
    {
        fR_ *= fValue; fG_ *= fValue; fB_ *= fValue;
    }

    void Color::operator /= ( const s_float& fValue )
    {
        fR_ /= fValue; fG_ /= fValue; fB_ /= fValue;
    }

    Color operator * (const s_float& fLeft, const Color& mRight)
    {
        return mRight*fLeft;
    }

    Color operator / (const s_float& fLeft, const Color& mRight)
    {
        return mRight/fLeft;
    }

    string_object StringConverter<string_element, Color>::Convert( const Color& mColor )
    {
        return  "(a:"+StringConverter<string_element, s_uint>::Convert(s_uint(mColor.GetA()*255.0f))+
               ", r:"+StringConverter<string_element, s_uint>::Convert(s_uint(mColor.GetR()*255.0f))+
               ", g:"+StringConverter<string_element, s_uint>::Convert(s_uint(mColor.GetG()*255.0f))+
               ", b:"+StringConverter<string_element, s_uint>::Convert(s_uint(mColor.GetB()*255.0f))+")";
    }
}
