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

using namespace std;

namespace Frost
{
    const s_str s_double::CLASS_NAME  = "s_double";

    const s_double s_double::NaN      = s_double(DOUBLE_NAN);
    const s_double s_double::INFPLUS  = s_double(DOUBLE_INF_PLUS);
    const s_double s_double::INFMINUS = s_double(DOUBLE_INF_MINUS);
    double s_double::dDummy           = 0.0;

    s_double::s_double()
    {
        mType_ = DOUBLE;
        dValue_ = 0.0;
    }

    s_double::s_double( const double& dValue )
    {
        mType_ = DOUBLE;
        dValue_ = dValue;
    }

    s_double::s_double( const s_float& mValue )
    {
        if (mValue.GetType() == s_float::FLOAT_NAN)
        {
            mType_ = DOUBLE_NAN;
            dValue_ = 0.0;
        }
        else if (mValue.GetType() == s_float::FLOAT_INF_PLUS)
        {
            mType_ = DOUBLE_INF_PLUS;
            dValue_ = 0.0;
        }
        else if (mValue.GetType() == s_float::FLOAT_INF_MINUS)
        {
            mType_ = DOUBLE_INF_MINUS;
            dValue_ = 0.0;
        }
        else
        {
            mType_ = DOUBLE;
            dValue_ = static_cast<double>(mValue.Get());
        }
    }

    s_double::s_double( const s_int& mValue )
    {
        if (mValue.GetType() == s_int::INTEGER_NAN)
        {
            mType_ = DOUBLE_NAN;
            dValue_ = 0.0;
        }
        else if (mValue.GetType() == s_int::INTEGER_INF_PLUS)
        {
            mType_ = DOUBLE_INF_PLUS;
            dValue_ = 0.0;
        }
        else if (mValue.GetType() == s_int::INTEGER_INF_MINUS)
        {
            mType_ = DOUBLE_INF_MINUS;
            dValue_ = 0.0;
        }
        else
        {
            mType_ = DOUBLE;
            dValue_ = static_cast<double>(mValue.Get());
        }
    }

    s_double::s_double( const s_uint& mValue )
    {
        if (mValue.GetType() == s_uint::INTEGER_NAN)
        {
            mType_ = DOUBLE_NAN;
            dValue_ = 0.0;
        }
        else if (mValue.GetType() == s_uint::INTEGER_INF)
        {
            mType_ = DOUBLE_INF_PLUS;
            dValue_ = 0.0;
        }
        else
        {
            mType_ = DOUBLE;
            dValue_ = static_cast<double>(mValue.Get());
        }
    }

    s_double::s_double( const s_bool& mValue )
    {
        mType_ = DOUBLE;
        if (mValue)
            dValue_ = 1.0;
        else
            dValue_ = 0.0;
    }

    s_double::s_double( const DoubleType& mType )
    {
        mType_ = mType;
        dValue_ = 0.0;
    }

    s_double::s_double( const char* sValue )
    {
        mType_ = DOUBLE;
        dValue_ = atof(sValue);
    }

    s_double::s_double( const string& sValue )
    {
        mType_ = DOUBLE;
        dValue_ = atof(sValue.c_str());
    }

    s_double::s_double( const s_str& sValue )
    {
        mType_ = DOUBLE;
        dValue_ = atof(sValue.c_str());
    }

    s_double::DoubleType s_double::GetType() const
    {
        return mType_;
    }

    void s_double::SetInfinitePlus()
    {
        mType_ = DOUBLE_INF_PLUS;
        dValue_ = 0.0;
    }

    void s_double::SetInfiniteMinus()
    {
        mType_ = DOUBLE_INF_MINUS;
        dValue_ = 0.0;
    }

    void s_double::SetNaN()
    {
        mType_ = DOUBLE_NAN;
        dValue_ = 0.0;
    }

    s_bool s_double::IsInfinitePlus() const
    {
        return (mType_ == DOUBLE_INF_PLUS);
    }

    s_bool s_double::IsInfiniteMinus() const
    {
        return (mType_ == DOUBLE_INF_MINUS);
    }

    s_bool s_double::IsNaN() const
    {
        return (mType_ == DOUBLE_NAN);
    }

    s_bool s_double::IsNull() const
    {
        return ( IsValid() && (fabs(dValue_) < DBL_MIN) );
    }

    s_double s_double::operator+ ( const s_double& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return s_double(DOUBLE_NAN);

        if (mValue.IsInfinitePlus())
        {
            if (IsInfiniteMinus())
                return s_double(DOUBLE_NAN);
            else
                return s_double(DOUBLE_INF_PLUS);
        }
        else if (mValue.IsInfiniteMinus())
        {
            if (IsInfinitePlus())
                return s_double(DOUBLE_NAN);
            else
                return s_double(DOUBLE_INF_MINUS);
        }
        else
        {
            return s_double(dValue_ + mValue.dValue_);
        }
    }

    s_double s_double::operator- ( const s_double& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return s_double(DOUBLE_NAN);

        if (mValue.IsInfinitePlus())
        {
            if (IsInfinitePlus())
                return s_double(DOUBLE_NAN);
            else
                return s_double(DOUBLE_INF_MINUS);
        }
        else if (mValue.IsInfiniteMinus())
        {
            if (IsInfiniteMinus())
                return s_double(DOUBLE_NAN);
            else
                return s_double(DOUBLE_INF_PLUS);
        }
        else
        {
            return s_double(dValue_ - mValue.dValue_);
        }
    }

    s_double s_double::operator* ( const s_double& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return s_double(DOUBLE_NAN);

        if (mValue.IsInfinitePlus())
        {
            if (IsInfiniteMinus())
                return s_double(DOUBLE_NAN);
            else
            {
                if (dValue_ < 0.0)
                    return s_double(DOUBLE_INF_MINUS);
                else
                    return s_double(DOUBLE_INF_PLUS);
            }
        }
        else if (mValue.IsInfiniteMinus())
        {
            if (IsInfinitePlus())
                return s_double(DOUBLE_NAN);
            else
            {
                if (dValue_ < 0.0)
                    return s_double(DOUBLE_INF_PLUS);
                else
                    return s_double(DOUBLE_INF_MINUS);
            }
        }
        else
        {
            return s_double(dValue_ * mValue.dValue_);
        }
    }

    s_double s_double::operator/ ( const s_double& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN() || mValue.dValue_ == 0.0)
            return s_double(DOUBLE_NAN);

        if (mValue.IsInfinitePlus())
        {
            if (IsInfiniteMinus())
                return s_double(DOUBLE_NAN);
            else
                return s_double(0.0);
        }
        else if (mValue.IsInfiniteMinus())
        {
            if (IsInfinitePlus())
                return s_double(DOUBLE_NAN);
            else
                return s_double(0.0);
        }
        else
        {
            return s_double(dValue_ / mValue.dValue_);
        }
    }

    s_double operator+ ( const double& dLeft, const s_double& fRight )
    {
        return s_double(dLeft) + fRight;
    }

    s_double operator- ( const double& dLeft, const s_double& fRight )
    {
        return s_double(dLeft) - fRight;
    }

    s_double operator* ( const double& dLeft, const s_double& fRight )
    {
        return s_double(dLeft) * fRight;
    }

    s_double operator/ ( const double& dLeft, const s_double& fRight )
    {
        return s_double(dLeft) / fRight;
    }

    void s_double::operator+= ( const s_double& mValue )
    {
        *this = *this + mValue;
    }

    void s_double::operator-= ( const s_double& mValue )
    {
        *this = *this - mValue;
    }

    void s_double::operator*= ( const s_double& mValue )
    {
        *this = *this * mValue;
    }

    void s_double::operator/= ( const s_double& mValue )
    {
        *this = *this / mValue;
    }

    bool s_double::operator== ( const s_double& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return false;
        else
        {
            if ( (mType_ == mValue.mType_) && (mType_ != DOUBLE) )
                return true;

            return (dValue_ == mValue.dValue_);
        }
    }

    bool s_double::operator!= ( const s_double& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return false;
        else
        {
            if (mType_ != mValue.mType_)
                return true;

            return (dValue_ != mValue.dValue_);
        }
    }

    bool s_double::operator< ( const s_double& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return false;
        else
        {
            if ( (mType_ == mValue.mType_) && (mType_ != DOUBLE) )
                return false;

            if (mType_ == DOUBLE_INF_MINUS)
                return true;
            if (mType_ == DOUBLE_INF_PLUS)
                return false;

            if (mValue.mType_ == DOUBLE_INF_MINUS)
                return false;
            if (mValue.mType_ == DOUBLE_INF_PLUS)
                return true;

            return (dValue_ < mValue.dValue_);
        }
    }

    bool s_double::operator> ( const s_double& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return false;
        else
        {
            if ( (mType_ == mValue.mType_) && (mType_ != DOUBLE) )
                return false;

            if (mType_ == DOUBLE_INF_MINUS)
                return false;
            if (mType_ == DOUBLE_INF_PLUS)
                return true;

            if (mValue.mType_ == DOUBLE_INF_MINUS)
                return true;
            if (mValue.mType_ == DOUBLE_INF_PLUS)
                return false;

            return (dValue_ > mValue.dValue_);
        }
    }

    bool s_double::operator<= ( const s_double& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return false;
        else
        {
            if ( (mType_ == mValue.mType_) && (mType_ != DOUBLE) )
                return true;

            if (mType_ == DOUBLE_INF_MINUS)
                return true;
            if (mType_ == DOUBLE_INF_PLUS)
                return false;

            if (mValue.mType_ == DOUBLE_INF_MINUS)
                return false;
            if (mValue.mType_ == DOUBLE_INF_PLUS)
                return true;

            return (dValue_ <= mValue.dValue_);
        }
    }

    bool s_double::operator>= ( const s_double& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return false;
        else
        {
            if ( (mType_ == mValue.mType_) && (mType_ != DOUBLE) )
                return true;

            if (mType_ == DOUBLE_INF_MINUS)
                return false;
            if (mType_ == DOUBLE_INF_PLUS)
                return true;

            if (mValue.mType_ == DOUBLE_INF_MINUS)
                return true;
            if (mValue.mType_ == DOUBLE_INF_PLUS)
                return false;

            return (dValue_ >= mValue.dValue_);
        }
    }

    s_double& s_double::operator= ( const s_double& mValue )
    {
        dValue_ = mValue.dValue_;
        mType_ = mValue.mType_;
        return *this;
    }

    s_bool s_double::IsValid() const
    {
        return (mType_ == DOUBLE);
    }

    s_double s_double::operator- () const
    {
        if (!IsNaN())
        {
            if (IsInfinitePlus())
                return s_double(DOUBLE_INF_MINUS);
            else if (IsInfiniteMinus())
                return s_double(DOUBLE_INF_PLUS);
            else
                return s_double(-dValue_);
        }
        else
            return s_double(DOUBLE_NAN);
    }

    void s_double::DegToRad()
    {
        if (IsValid())
        {
            dValue_ *= 2*M_PI/360.0;
        }
    }

    void s_double::ToRad()
    {
        if (IsValid())
        {
            dValue_ *= 2*M_PI;
        }
    }

    s_double s_double::GetRad() const
    {
        if (IsValid())
        {
            return s_double(dValue_*2*M_PI);
        }
        else
            return 0.0;
    }

    s_uint s_double::GetNearestPowerOfTwo() const
    {
        if (IsValid())
        {
            s_uint i = 1;
            while (dValue_ > i.Get())
                i = i << 1;

            return i;
        }
        else
            return s_uint(s_uint::INTEGER_NAN);
    }

    void s_double::Pow( const s_double& dPower )
    {
        if (IsValid() && dPower.IsValid())
        {
            dValue_ = pow(dValue_, dPower.Get());
        }
    }

    void s_double::RadToDeg( const s_bool& bNegativeAllowed )
    {
        if (IsValid())
        {
            dValue_ *= 360.0/(2*M_PI);
            if ( (!bNegativeAllowed) && (dValue_ < 0.0) )
                dValue_ += 360.0;
        }
    }

    void s_double::ToDeg( const s_bool& bNegativeAllowed )
    {
        if (IsValid())
        {
            dValue_ *= 360.0;
            if ( (!bNegativeAllowed) && (dValue_ < 0.0) )
                dValue_ += 360.0;
        }
    }

    s_double s_double::GetDeg() const
    {
        if (IsValid())
        {
            return s_double(dValue_*360.0);
        }
        else
            return 0.0;
    }

    void s_double::Randomize( const s_double& dMin, const s_double& dMax )
    {
        if (dMin.IsValid() && dMax.IsValid())
        {
            mType_ = DOUBLE;
            if (dMax < dMin)
                dValue_ = dMin.Get();
            else
                dValue_ = ((dMax-dMin)*s_double(rand()/float(RAND_MAX))).Get() + dMin.Get();
        }
    }

    void s_double::Round( const RoundType& mRoundType )
    {
        switch (mRoundType)
        {
            case ROUND_CEIL :
                dValue_ = ceil(dValue_);
                break;
            case ROUND_MID :
                dValue_ = round(dValue_);
                break;
            case ROUND_FLOOR :
                dValue_ = floor(dValue_);
                break;
        }
    }

    void s_double::Saturate( const s_double& dMin, const s_double& dMax )
    {
        if (IsValid() && dMin.IsValid() && dMax.IsValid())
        {
            if (dValue_ < dMin.Get())
                dValue_ = dMin.Get();
            if (dValue_ > dMax.Get())
                dValue_ = dMax.Get();
        }
    }

    s_int s_double::Sign()
    {
        if (IsValid())
        {
            if (dValue_ == 0.0)
                return s_int(1);
            else
                return s_int(static_cast<int>(dValue_/fabs(dValue_)));
        }
        else
            return s_int(s_int::INTEGER_NAN);
    }

    s_double s_double::Min( const s_double& dLeft, const s_double& dRight )
    {
        if (dLeft >= dRight)
            return dRight;
        else
            return dLeft;
    }

    s_double s_double::Max( const s_double& dLeft, const s_double& dRight )
    {
        if (dLeft <= dRight)
            return dRight;
        else
            return dLeft;
    }

    s_double s_double::Random( const s_double& dMin, const s_double& dMax )
    {
        if (dMin.IsValid() && dMax.IsValid())
        {
            if (dMax < dMin)
            {
                return s_double(dMin);
            }
            else
            {
                return (dMax-dMin)*s_double(rand()/float(RAND_MAX)) + dMin;
            }
        }
        else
            return s_double(DOUBLE_NAN);
    }

    s_str operator+ ( const char* sLeft, const s_double& fRight )
    {
        return s_str(sLeft) << fRight;
    }

    s_str s_double::operator+ ( const char* sValue ) const
    {
        return s_str((*this)) + sValue;
    }

    s_str s_double::operator+ ( const s_str& sValue ) const
    {
        return s_str((*this)) + sValue;
    }

    s_ctnr<s_double> s_double::operator, ( const s_double& dValue ) const
    {
        s_ctnr<s_double> mContainer;
        mContainer.Push(*this);
        mContainer.Push(dValue);
        return mContainer;
    }
}
