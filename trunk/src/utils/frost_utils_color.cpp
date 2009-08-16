/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_color.h"

#include <OgreColourValue.h>

using namespace std;

namespace Frost
{
    const s_str Color::CLASS_NAME = "Color";

    const Color Color::BLACK = Color(0  , 0  ,0   );
    const Color Color::WHITE = Color(255, 255, 255);
    const Color Color::RED   = Color(255, 0  , 0  );
    const Color Color::GREEN = Color(0  , 255, 0  );
    const Color Color::BLUE  = Color(0  , 0  , 255);
    const Color Color::VOID  = Color(0);
    const Color Color::NaN   = Color(s_uint::NaN);

    Color::Color()
    {
        ucA_ = 255;
        BuildUIColor_();
    }

    Color::Color( const s_uchar& ucR, const s_uchar& ucG, const s_uchar& ucB )
    {
        ucA_ = 255;
        ucR_ = ucR;
        ucG_ = ucG;
        ucB_ = ucB;
        BuildUIColor_();
    }

    Color::Color( const s_uchar& ucA, const s_uchar& ucR, const s_uchar& ucG, const s_uchar& ucB )
    {
        ucA_ = ucA;
        ucR_ = ucR;
        ucG_ = ucG;
        ucB_ = ucB;
        BuildUIColor_();
    }

    Color::Color( const s_uint& uiColor )
    {
        uiColor_ = uiColor;
        BuildABGRColor_();
    }

    const s_uchar& Color::GetA() const
    {
        return ucA_;
    }

    const s_uchar& Color::GetR() const
    {
        return ucR_;
    }

    const s_uchar& Color::GetG() const
    {
        return ucG_;
    }

    const s_uchar& Color::GetB() const
    {
        return ucB_;
    }

    const s_uint& Color::GetPacked() const
    {
        return uiColor_;
    }

    void Color::SetA( const s_uchar& ucA )
    {
        ucA_ = ucA;
        BuildUIColor_();
    }

    void Color::SetR( const s_uchar& ucR )
    {
        ucR_ = ucR;
        BuildUIColor_();
    }

    void Color::SetG( const s_uchar& ucG )
    {
        ucG_ = ucG;
        BuildUIColor_();
    }

    void Color::SetB( const s_uchar& ucB )
    {
        ucB_ = ucB;
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
        if (!ucA_.IsNaN() && !ucB_.IsNaN() && !ucG_.IsNaN() && !ucR_.IsNaN())
        {
            uiColor_  = ucA_.Get() << 24;
            uiColor_ += ucB_.Get() << 16;
            uiColor_ += ucG_.Get() << 8;
            uiColor_ += ucR_.Get();
        }
        else
            uiColor_ = s_uint::NaN;
    }

    void Color::BuildABGRColor_()
    {
        if (!uiColor_.IsNaN())
        {
            ucA_ = (uiColor_.Get() >> 24) & 0xFF;
            ucB_ = (uiColor_.Get() >> 16) & 0xFF;
            ucG_ = (uiColor_.Get() >> 8)  & 0xFF;
            ucR_ =  uiColor_.Get()        & 0xFF;
        }
        else
        {
            ucA_ = ucB_ = ucG_ = ucR_ = s_uchar::NaN;
        }
    }

    Color Color::Random( const s_bool& bRandomAlpha )
    {
        if (bRandomAlpha)
            return Color(
                s_uchar::Random(0, 255),
                s_uchar::Random(0, 255),
                s_uchar::Random(0, 255),
                s_uchar::Random(0, 255)
            );
        else
            return Color(
                s_uchar::Random(0, 255),
                s_uchar::Random(0, 255),
                s_uchar::Random(0, 255)
            );
    }

    Ogre::ColourValue Color::FrostToOgre( const Color& mColor )
    {
        Ogre::ColourValue mOgreColor;
        mOgreColor.setAsARGB(mColor.GetPacked().Get());
        return mOgreColor;
    }

    Color Color::OgreToFrost( const Ogre::ColourValue& mOgreColor )
    {
        Color mColor;
        mColor.SetPacked(mOgreColor.getAsARGB());
        return mColor;
    }

    s_bool Color::operator == ( const Color& mColor ) const
    {
        return (ucA_ == mColor.ucA_) && (ucR_ == mColor.ucR_) && (ucG_ == mColor.ucG_) && (ucB_ == mColor.ucB_);
    }

    s_bool Color::operator != ( const Color& mColor ) const
    {
        return (ucA_ != mColor.ucA_) || (ucR_ != mColor.ucR_) || (ucG_ != mColor.ucG_) || (ucB_ != mColor.ucB_);
    }

    s_str operator+ ( const string_element* sLeft, const Color& mRight )
    {
        return s_str(sLeft) + mRight;
    }

    s_str operator+ (const s_str& sLeft, const Color& mRight)
    {
        s_str sTemp;
        sTemp += "(a:"+s_uint(mRight.GetA());
        sTemp += ", r:"+s_uint(mRight.GetR());
        sTemp += ", g:"+s_uint(mRight.GetG());
        sTemp += ", b:"+s_uint(mRight.GetB());
        sTemp += ", packed:"+mRight.GetPacked()+")";
        return sLeft + sTemp;
    }
}



