/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_int.h"
#include "frost_utils_uint.h"
#include "frost_utils_float.h"
#include "frost_utils_double.h"
#include "frost_utils_str.h"
#include "frost_utils_bool.h"
#include "frost_utils_array.h"
#include "frost_utils.h"

#include <math.h>
#include <float.h>
#include <OgreMath.h>

using namespace std;

namespace Frost
{
    const s_str s_float::CLASS_NAME = "s_float";

    const s_float s_float::NaN = s_float(FLOAT_NAN);
    const s_float s_float::INFPLUS = s_float(FLOAT_INF_PLUS);
    const s_float s_float::INFMINUS = s_float(FLOAT_INF_MINUS);
    float s_float::fDummy = 0.0f;

    s_float::s_float()
    {
        mType_ = FLOAT;
        fValue_ = 0.0f;
    }

    s_float::s_float(const s_float& fValue)
    {
        mType_ = fValue.mType_;
        fValue_ = fValue.fValue_;
    }

    s_float::s_float( const float& fValue )
    {
        mType_ = FLOAT;
        fValue_ = fValue;
    }

    s_float::s_float( const s_double& mValue )
    {
        if (mValue.GetType() == s_double::DOUBLE_NAN)
        {
            mType_ = FLOAT_NAN;
            fValue_ = 0.0f;
        }
        else if (mValue.GetType() == s_double::DOUBLE_INF_PLUS)
        {
            mType_ = FLOAT_INF_PLUS;
            fValue_ = 0.0f;
        }
        else if (mValue.GetType() == s_double::DOUBLE_INF_MINUS)
        {
            mType_ = FLOAT_INF_MINUS;
            fValue_ = 0.0f;
        }
        else
        {
            mType_ = FLOAT;
            fValue_ = static_cast<float>(mValue.Get());
        }
    }

    s_float::s_float( const s_int& mValue )
    {
        if (mValue.GetType() == s_int::INTEGER_NAN)
        {
            mType_ = FLOAT_NAN;
            fValue_ = 0.0f;
        }
        else if (mValue.GetType() == s_int::INTEGER_INF_PLUS)
        {
            mType_ = FLOAT_INF_PLUS;
            fValue_ = 0.0f;
        }
        else if (mValue.GetType() == s_int::INTEGER_INF_MINUS)
        {
            mType_ = FLOAT_INF_MINUS;
            fValue_ = 0.0f;
        }
        else
        {
            mType_ = FLOAT;
            fValue_ = static_cast<float>(mValue.Get());
        }
    }

    s_float::s_float( const s_uint& mValue )
    {
        if (mValue.GetType() == s_uint::INTEGER_NAN)
        {
            mType_ = FLOAT_NAN;
            fValue_ = 0.0f;
        }
        else if (mValue.GetType() == s_uint::INTEGER_INF)
        {
            mType_ = FLOAT_INF_PLUS;
            fValue_ = 0.0f;
        }
        else
        {
            mType_ = FLOAT;
            fValue_ = static_cast<float>(mValue.Get());
        }
    }

    s_float::s_float( const s_bool& mValue )
    {
        mType_ = FLOAT;
        if (mValue)
            fValue_ = 1.0f;
        else
            fValue_ = 0.0f;
    }

    s_float::s_float( const FloatType& mType )
    {
        mType_ = mType;
        fValue_ = 0.0f;
    }

    s_float::s_float( const char* sValue )
    {
        mType_ = FLOAT;
        fValue_ = atof(sValue);
    }

    s_float::s_float( const string& sValue )
    {
        mType_ = FLOAT;
        fValue_ = atof(sValue.c_str());
    }

    s_float::s_float( const s_str& sValue )
    {
        mType_ = FLOAT;
        fValue_ = atof(sValue.c_str());
    }

    s_float::operator MemberFn() const
    {
        if (mType_ == FLOAT)
            return &s_float::SetNaN;
        else
            return NULL;
    }

    s_float::FloatType s_float::GetType() const
    {
        return mType_;
    }

    void s_float::SetInfinitePlus()
    {
        mType_ = FLOAT_INF_PLUS;
        fValue_ = 0.0f;
    }

    void s_float::SetInfiniteMinus()
    {
        mType_ = FLOAT_INF_MINUS;
        fValue_ = 0.0f;
    }

    void s_float::SetNaN()
    {
        mType_ = FLOAT_NAN;
        fValue_ = 0.0f;
    }

    s_bool s_float::IsInfinitePlus() const
    {
        return (mType_ == FLOAT_INF_PLUS);
    }

    s_bool s_float::IsInfiniteMinus() const
    {
        return (mType_ == FLOAT_INF_MINUS);
    }

    s_bool s_float::IsNaN() const
    {
        return (mType_ == FLOAT_NAN);
    }

    s_bool s_float::IsNull() const
    {
        return ( IsValid() && (fValue_ < FLT_MIN) && (fValue_ > -FLT_MIN) );
    }

    s_float s_float::operator+ ( const s_float& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return s_float(FLOAT_NAN);

        if (mValue.IsInfinitePlus())
        {
            if (IsInfiniteMinus())
                return s_float(FLOAT_NAN);
            else
                return s_float(FLOAT_INF_PLUS);
        }
        else if (mValue.IsInfiniteMinus())
        {
            if (IsInfinitePlus())
                return s_float(FLOAT_NAN);
            else
                return s_float(FLOAT_INF_MINUS);
        }
        else
        {
            return s_float(fValue_ + mValue.fValue_);
        }
    }

    s_float s_float::operator- ( const s_float& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return s_float(FLOAT_NAN);

        if (mValue.IsInfinitePlus())
        {
            if (IsInfinitePlus())
                return s_float(FLOAT_NAN);
            else
                return s_float(FLOAT_INF_MINUS);
        }
        else if (mValue.IsInfiniteMinus())
        {
            if (IsInfiniteMinus())
                return s_float(FLOAT_NAN);
            else
                return s_float(FLOAT_INF_PLUS);
        }
        else
        {
            return s_float(fValue_ - mValue.fValue_);
        }
    }

    s_float s_float::operator* ( const s_float& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return s_float(FLOAT_NAN);

        if (mValue.IsInfinitePlus())
        {
            if (IsInfiniteMinus())
                return s_float(FLOAT_NAN);
            else
            {
                if (fValue_ < 0.0f)
                    return s_float(FLOAT_INF_MINUS);
                else
                    return s_float(FLOAT_INF_PLUS);
            }
        }
        else if (mValue.IsInfiniteMinus())
        {
            if (IsInfinitePlus())
                return s_float(FLOAT_NAN);
            else
            {
                if (fValue_ < 0.0f)
                    return s_float(FLOAT_INF_PLUS);
                else
                    return s_float(FLOAT_INF_MINUS);
            }
        }
        else
        {
            return s_float(fValue_ * mValue.fValue_);
        }
    }

    s_float s_float::operator/ ( const s_float& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN() || mValue.fValue_ == 0.0f)
            return s_float(FLOAT_NAN);

        if (mValue.IsInfinitePlus())
        {
            if (IsInfiniteMinus())
                return s_float(FLOAT_NAN);
            else
                return s_float(0.0f);
        }
        else if (mValue.IsInfiniteMinus())
        {
            if (IsInfinitePlus())
                return s_float(FLOAT_NAN);
            else
                return s_float(0.0f);
        }
        else
        {
            return s_float(fValue_ / mValue.fValue_);
        }
    }

    s_float operator+ ( const float& fLeft, const s_float& fRight )
    {
        return s_float(fLeft) + fRight;
    }

    s_float operator- ( const float& fLeft, const s_float& fRight )
    {
        return s_float(fLeft) - fRight;
    }

    s_float operator* ( const float& fLeft, const s_float& fRight )
    {
        return s_float(fLeft) * fRight;
    }

    s_float operator/ ( const float& fLeft, const s_float& fRight )
    {
        return s_float(fLeft) / fRight;
    }

    void s_float::operator+= ( const s_float& mValue )
    {
        *this = *this + mValue;
    }

    void s_float::operator-= ( const s_float& mValue )
    {
        *this = *this - mValue;
    }

    void s_float::operator*= ( const s_float& mValue )
    {
        *this = *this * mValue;
    }

    void s_float::operator/= ( const s_float& mValue )
    {
        *this = *this / mValue;
    }

    bool s_float::operator== ( const s_float& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return false;
        else
        {
            if ( (mType_ == mValue.mType_) && (mType_ != FLOAT) )
                return true;

            return (fValue_ == mValue.fValue_);
        }
    }

    bool s_float::operator!= ( const s_float& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return false;
        else
        {
            if (mType_ != mValue.mType_)
                return true;

            return (fValue_ != mValue.fValue_);
        }
    }

    bool s_float::operator< ( const s_float& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return false;
        else
        {
            if ( (mType_ == mValue.mType_) && (mType_ != FLOAT) )
                return false;

            if (mType_ == FLOAT_INF_MINUS)
                return true;
            if (mType_ == FLOAT_INF_PLUS)
                return false;

            if (mValue.mType_ == FLOAT_INF_MINUS)
                return false;
            if (mValue.mType_ == FLOAT_INF_PLUS)
                return true;

            return (fValue_ < mValue.fValue_);
        }
    }

    bool s_float::operator> ( const s_float& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return false;
        else
        {
            if ( (mType_ == mValue.mType_) && (mType_ != FLOAT) )
                return false;

            if (mType_ == FLOAT_INF_MINUS)
                return false;
            if (mType_ == FLOAT_INF_PLUS)
                return true;

            if (mValue.mType_ == FLOAT_INF_MINUS)
                return true;
            if (mValue.mType_ == FLOAT_INF_PLUS)
                return false;

            return (fValue_ > mValue.fValue_);
        }
    }

    bool s_float::operator<= ( const s_float& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return false;
        else
        {
            if ( (mType_ == mValue.mType_) && (mType_ != FLOAT) )
                return true;

            if (mType_ == FLOAT_INF_MINUS)
                return true;
            if (mType_ == FLOAT_INF_PLUS)
                return false;

            if (mValue.mType_ == FLOAT_INF_MINUS)
                return false;
            if (mValue.mType_ == FLOAT_INF_PLUS)
                return true;

            return (fValue_ <= mValue.fValue_);
        }
    }

    bool s_float::operator>= ( const s_float& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return false;
        else
        {
            if ( (mType_ == mValue.mType_) && (mType_ != FLOAT) )
                return true;

            if (mType_ == FLOAT_INF_MINUS)
                return false;
            if (mType_ == FLOAT_INF_PLUS)
                return true;

            if (mValue.mType_ == FLOAT_INF_MINUS)
                return true;
            if (mValue.mType_ == FLOAT_INF_PLUS)
                return false;

            return (fValue_ >= mValue.fValue_);
        }
    }

    s_float& s_float::operator= ( const s_float& mValue )
    {
        fValue_ = mValue.fValue_;
        mType_ = mValue.mType_;
        return *this;
    }

    bool s_float::operator! () const
    {
        return (mType_ != FLOAT);
    }

    s_bool s_float::IsValid() const
    {
        return (mType_ == FLOAT);
    }

    s_float s_float::operator- () const
    {
        if (!IsNaN())
        {
            if (IsInfinitePlus())
                return s_float(FLOAT_INF_MINUS);
            else if (IsInfiniteMinus())
                return s_float(FLOAT_INF_PLUS);
            else
                return s_float(-fValue_);
        }
        else
            return s_float(FLOAT_NAN);
    }

    void s_float::DegToRad()
    {
        if (IsValid())
        {
            fValue_ *=  2*M_PI/360.0f;
        }
    }

    void s_float::ToRad()
    {
        if (IsValid())
        {
            fValue_ *= 2*M_PI;
        }
    }

    s_float s_float::GetRad() const
    {
        if (IsValid())
        {
            return s_float(fValue_*2*M_PI);
        }
        else
            return 0.0f;
    }

    s_uint s_float::GetNearestPowerOfTwo() const
    {
        if (IsValid())
        {
            s_uint i = 1;
            while (fValue_ > i.Get())
                i = i << 1;

            return i;
        }
        else
            return s_uint(s_uint::INTEGER_NAN);
    }

    void s_float::Pow( const s_float& fPower )
    {
        if (IsValid() && fPower.IsValid())
        {
            fValue_ = pow(fValue_, fPower.Get());
        }
    }

    void s_float::RadToDeg( const s_bool& bNegativeAllowed )
    {
        if (IsValid())
        {
            fValue_ *= 360.0f/(2*M_PI);
            if ( (!bNegativeAllowed) && (fValue_ < 0.0f) )
                fValue_ += 360.0f;
        }
    }

    void s_float::ToDeg( const s_bool& bNegativeAllowed )
    {
        if (IsValid())
        {
            fValue_ *= 360.0f;
            if ( (!bNegativeAllowed) && (fValue_ < 0.0f) )
                fValue_ += 360.0f;
        }
    }

    s_float s_float::GetDeg() const
    {
        if (IsValid())
        {
            return s_float(fValue_*360.0f);
        }
        else
            return 0.0f;
    }

    void s_float::Random( const s_float& fMin, const s_float& fMax )
    {
        if (fMin.IsValid() && fMax.IsValid())
        {
            mType_ = FLOAT;
            if (fMax < fMin)
                fValue_ = fMin.Get();
            else
                fValue_ = Ogre::Math::RangeRandom(fMin.Get(), fMax.Get());
        }
    }

    void s_float::Round( const RoundType& mRoundType )
    {
        switch (mRoundType)
        {
            case ROUND_CEIL :
                fValue_ = ceil(fValue_);
                break;
            case ROUND_MID :
                fValue_ = round(fValue_);
                break;
            case ROUND_FLOOR :
                fValue_ = floor(fValue_);
                break;
        }
    }

    void s_float::Saturate( const s_float& fMin, const s_float& fMax )
    {
        if (IsValid() && fMin.IsValid() && fMax.IsValid())
        {
            if (fValue_ < fMin.Get())
                fValue_ = fMin.Get();
            if (fValue_ > fMax.Get())
                fValue_ = fMax.Get();
        }
    }

    s_int s_float::Sign()
    {
        if (IsValid())
        {
            if (fValue_ == 0.0f)
                return s_int(1);
            else
                return s_int(static_cast<int>(fValue_/fabs(fValue_)));
        }
        else
            return s_int(s_int::INTEGER_NAN);
    }

    s_float s_float::Min( const s_float& fLeft, const s_float& fRight )
    {
        if (fLeft >= fRight)
            return fRight;
        else
            return fLeft;
    }

    s_float s_float::Max( const s_float& fLeft, const s_float& fRight )
    {
        if (fLeft <= fRight)
            return fRight;
        else
            return fLeft;
    }

    s_float s_float::Rand( const s_float& fMin, const s_float& fMax )
    {
        if (fMin.IsValid() && fMax.IsValid())
        {
            if (fMax < fMin)
            {
                return s_float(fMin);
            }
            else
            {
                s_float f(static_cast<float>(Ogre::Math::RangeRandom(fMin.Get(), fMax.Get())));
                return f;
            }
        }
        else
            return s_float(FLOAT_NAN);
    }

    s_str operator+ ( const char* sLeft, const s_float& fRight )
    {
        return s_str(sLeft) << fRight;
    }

    s_str s_float::operator+ ( const char* sValue ) const
    {
        return s_str(*this) + sValue;
    }

    s_str s_float::operator+ ( const s_str& sValue ) const
    {
        return s_str(*this) + sValue;
    }

    ctnr<s_float> s_float::operator, ( const s_float& fValue ) const
    {
        ctnr<s_float> mContainer;
        mContainer.Push(*this);
        mContainer.Push(fValue);
        return mContainer;
    }
}
