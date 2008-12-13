/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_color.h"
#include "frost_utils_str.h"

using namespace std;

namespace Frost
{
    const s_str Color::CLASS_NAME = "Color";

    Color::Color()
    {
        uiA_ = 255u;
    }

    Color::Color( const s_uint& uiR, const s_uint& uiG, const s_uint& uiB )
    {
        uiA_ = 255u;
        uiR_ = uiR;
        uiG_ = uiG;
        uiB_ = uiB;
        BuildUIColor_();
    }

    Color::Color( const s_uint& uiA, const s_uint& uiR, const s_uint& uiG, const s_uint& uiB )
    {
        uiA_ = uiA;
        uiR_ = uiR;
        uiG_ = uiG;
        uiB_ = uiB;
        BuildUIColor_();
    }

    Color::Color( const s_uint& uiColor )
    {
        uiColor_ = uiColor;
        BuildABGRColor_();
    }

    const s_uint& Color::GetA() const
    {
        return uiA_;
    }

    const s_uint& Color::GetR() const
    {
        return uiR_;
    }

    const s_uint& Color::GetG() const
    {
        return uiG_;
    }

    const s_uint& Color::GetB() const
    {
        return uiB_;
    }

    const s_uint& Color::Get() const
    {
        return uiColor_;
    }

    void Color::SetA( const s_uint& uiA )
    {
        uiA_ = uiA;
        BuildUIColor_();
    }

    void Color::SetR( const s_uint& uiR )
    {
        uiR_ = uiR;
        BuildUIColor_();
    }

    void Color::SetG( const s_uint& uiG )
    {
        uiG_ = uiG;
        BuildUIColor_();
    }

    void Color::SetB( const s_uint& uiB )
    {
        uiB_ = uiB;
        BuildUIColor_();
    }

    void Color::Set( const s_uint& uiColor )
    {
        uiColor_ = uiColor;
        BuildABGRColor_();
    }

    void Color::BuildUIColor_()
    {
        uiColor_ =  ((uchar)uiA_.Get()) << 24;
        uiColor_ += ((uchar)uiB_.Get()) << 16;
        uiColor_ += ((uchar)uiG_.Get()) << 8;
        uiColor_ += ((uchar)uiR_.Get());
    }

    void Color::BuildABGRColor_()
    {
        uiA_ = ((((uchar)uiColor_.Get()) >> 24) & 0xFF);
        uiB_ = ((((uchar)uiColor_.Get()) >> 16) & 0xFF);
        uiG_ = ((((uchar)uiColor_.Get()) >> 8) & 0xFF);
        uiR_ =  (((uchar)uiColor_.Get()) & 0xFF);
    }

    s_str operator + (const char* sLeft, const Color& mRight)
    {
        return s_str(sLeft) + mRight;
    }

    s_str operator + (const s_str& sLeft, const Color& mRight)
    {
        s_str sTemp;
        sTemp += "(a:"+s_str(mRight.GetA());
        sTemp += ", r:"+s_str(mRight.GetR());
        sTemp += ", g:"+s_str(mRight.GetG());
        sTemp += ", b:"+s_str(mRight.GetB());
        sTemp += ")";
        return sLeft + sTemp;
    }
}



