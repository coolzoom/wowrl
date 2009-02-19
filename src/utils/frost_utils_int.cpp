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
#include <OgreMath.h>

using namespace std;

namespace Frost
{
    const s_str s_int::CLASS_NAME = "s_int";

    const s_int s_int::NaN = s_int(INTEGER_NAN);
    const s_int s_int::INFPLUS = s_int(INTEGER_INF_PLUS);
    const s_int s_int::INFMINUS = s_int(INTEGER_INF_MINUS);
    int s_int::iDummy = 0;

    s_int::s_int()
    {
        mType_ = INTEGER;
        iValue_ = 0;
    }

    s_int::s_int( const int& iValue )
    {
        mType_ = INTEGER;
        iValue_ = iValue;
    }

    s_int::s_int( const float& fValue )
    {
        mType_ = INTEGER;
        iValue_ = static_cast<int>(round(fValue));
    }

    s_int::s_int( const double& dValue )
    {
        mType_ = INTEGER;
        iValue_ = static_cast<int>(round(dValue));
    }

    s_int::s_int( const s_uint& mValue )
    {
        if (mValue.GetType() == s_uint::INTEGER_NAN)
        {
            mType_ = INTEGER_NAN;
            iValue_ = 0;
        }
        else if (mValue.GetType() == s_uint::INTEGER_INF)
        {
            mType_ = INTEGER_INF_PLUS;
            iValue_ = 0;
        }
        else
        {
            mType_ = INTEGER;
            iValue_ = static_cast<int>(mValue.Get());
        }
    }

    s_int::s_int( const s_float& mValue )
    {
        if (mValue.GetType() == s_float::FLOAT_NAN)
        {
            mType_ = INTEGER_NAN;
            iValue_ = 0;
        }
        else if (mValue.GetType() == s_float::FLOAT_INF_PLUS)
        {
            mType_ = INTEGER_INF_PLUS;
            iValue_ = 0;
        }
        else if (mValue.GetType() == s_float::FLOAT_INF_MINUS)
        {
            mType_ = INTEGER_INF_MINUS;
            iValue_ = 0;
        }
        else
        {
            mType_ = INTEGER;
            iValue_ = static_cast<int>(round(mValue.Get()));
        }
    }

    s_int::s_int( const s_double& mValue )
    {
        if (mValue.GetType() == s_double::DOUBLE_NAN)
        {
            mType_ = INTEGER_NAN;
            iValue_ = 0;
        }
        else if (mValue.GetType() == s_double::DOUBLE_INF_PLUS)
        {
            mType_ = INTEGER_INF_PLUS;
            iValue_ = 0;
        }
        else if (mValue.GetType() == s_double::DOUBLE_INF_MINUS)
        {
            mType_ = INTEGER_INF_MINUS;
            iValue_ = 0;
        }
        else
        {
            mType_ = INTEGER;
            iValue_ = static_cast<int>(round(mValue.Get()));
        }
    }

    s_int::s_int( const s_bool& mValue )
    {
        mType_ = INTEGER;
        if (mValue)
            iValue_ = 1;
        else
            iValue_ = 0;
    }

    s_int::s_int( const IntegerType& mType )
    {
        mType_ = mType;
        iValue_ = 0;
    }

    s_int::s_int( const char* sValue )
    {
        mType_ = INTEGER;
        iValue_ = atoi(sValue);
    }

    s_int::s_int( const string& sValue )
    {
        mType_ = INTEGER;
        iValue_ = atoi(sValue.c_str());
    }

    s_int::s_int( const s_str& sValue )
    {
        mType_ = INTEGER;
        iValue_ = atoi(sValue.c_str());
    }

    s_int::IntegerType s_int::GetType() const
    {
        return mType_;
    }

    void s_int::SetInfinitePlus()
    {
        mType_ = INTEGER_INF_PLUS;
        iValue_ = 0;
    }

    void s_int::SetInfiniteMinus()
    {
        mType_ = INTEGER_INF_MINUS;
        iValue_ = 0;
    }

    void s_int::SetNaN()
    {
        mType_ = INTEGER_NAN;
        iValue_ = 0;
    }

    s_bool s_int::IsInfinitePlus() const
    {
        return (mType_ == INTEGER_INF_PLUS);
    }

    s_bool s_int::IsInfiniteMinus() const
    {
        return (mType_ == INTEGER_INF_MINUS);
    }

    s_bool s_int::IsNaN() const
    {
        return (mType_ == INTEGER_NAN);
    }

    s_bool s_int::IsNull() const
    {
        return ( IsValid() && (iValue_ == 0) );
    }

    s_int s_int::operator+ ( const s_int& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return s_int(INTEGER_NAN);

        if (!mValue.IsValid())
        {
            if (mValue.IsInfinitePlus())
            {
                if (IsInfiniteMinus())
                    return s_int(INTEGER_NAN);
                else
                    return s_int(INTEGER_INF_PLUS);
            }
            else
            {
                if (IsInfinitePlus())
                    return s_int(INTEGER_NAN);
                else
                    return s_int(INTEGER_INF_MINUS);
            }
        }
        else if (!this->IsValid())
        {
            if (IsInfinitePlus())
                return s_int(INTEGER_INF_PLUS);
            else
                return s_int(INTEGER_INF_MINUS);
        }
        else
        {
            return s_int(iValue_ + mValue.iValue_);
        }
    }

    s_int s_int::operator- ( const s_int& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return s_int(INTEGER_NAN);

        if (!mValue.IsValid())
        {
            if (mValue.IsInfinitePlus())
            {
                if (IsInfinitePlus())
                    return s_int(INTEGER_NAN);
                else
                    return s_int(INTEGER_INF_MINUS);
            }
            else
            {
                if (IsInfiniteMinus())
                    return s_int(INTEGER_NAN);
                else
                    return s_int(INTEGER_INF_PLUS);
            }
        }
        else if (!this->IsValid())
        {
            if (IsInfinitePlus())
                return s_int(INTEGER_INF_MINUS);
            else
                return s_int(INTEGER_INF_PLUS);
        }
        else
        {
            return s_int(iValue_ - mValue.iValue_);
        }
    }

    s_int s_int::operator* ( const s_int& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return s_int(INTEGER_NAN);

        if (!mValue.IsValid())
        {
            if (mValue.IsInfinitePlus())
            {
                if (IsInfiniteMinus() || IsNull())
                    return s_int(INTEGER_NAN);
                else if (IsInfinitePlus())
                    return s_int(INTEGER_INF_PLUS);
                else
                {
                    if (iValue_ > 0)
                        return s_int(INTEGER_INF_PLUS);
                    else
                        return s_int(INTEGER_INF_MINUS);
                }
            }
            else
            {
                if (IsInfinitePlus() || IsNull())
                    return s_int(INTEGER_NAN);
                else if (IsInfiniteMinus())
                    return s_int(INTEGER_INF_MINUS);
                else
                {
                    if (iValue_ > 0)
                        return s_int(INTEGER_INF_MINUS);
                    else
                        return s_int(INTEGER_INF_PLUS);
                }
            }
        }
        else if (!this->IsValid())
        {
            if (mValue.IsNull())
                return s_int(INTEGER_NAN);

            if (IsInfinitePlus())
            {
                if (mValue.iValue_ > 0)
                    return s_int(INTEGER_INF_PLUS);
                else
                    return s_int(INTEGER_INF_MINUS);
            }
            else
            {
                if (mValue.iValue_ > 0)
                    return s_int(INTEGER_INF_MINUS);
                else
                    return s_int(INTEGER_INF_PLUS);
            }
        }
        else
        {
            return s_int(iValue_ * mValue.iValue_);
        }
    }

    s_int s_int::operator/ ( const s_int& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN() || mValue.IsNull())
            return s_int(INTEGER_NAN);

        if (!mValue.IsValid())
        {
            if (!this->IsValid())
                return s_int(INTEGER_NAN);
            else
                return s_int(0);
        }
        else if (!this->IsValid())
        {
            if (IsInfinitePlus())
            {
                if (mValue.iValue_ > 0)
                    return s_int(INTEGER_INF_PLUS);
                else
                    return s_int(INTEGER_INF_MINUS);
            }
            else
            {
                if (mValue.iValue_ > 0)
                    return s_int(INTEGER_INF_MINUS);
                else
                    return s_int(INTEGER_INF_PLUS);
            }
        }
        else
        {
            return s_int(iValue_ / mValue.iValue_);
        }
    }

    s_int s_int::operator% ( const s_int& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN() || mValue.IsNull())
            return s_int(INTEGER_NAN);

        if (!mValue.IsValid())
        {
            if (!this->IsValid())
                return s_int(0);
            else
                return *this;
        }
        else if (!this->IsValid())
        {
            return s_int(0);
        }
        else
        {
            return s_int(iValue_ % mValue.iValue_);
        }
    }

    s_int operator+ ( const int& iLeft, const s_int& iRight )
    {
        return s_int(iLeft) + iRight;
    }

    s_int operator- ( const int& iLeft, const s_int& iRight )
    {
        return s_int(iLeft) - iRight;
    }

    s_int operator* ( const int& iLeft, const s_int& iRight )
    {
        return s_int(iLeft) * iRight;
    }

    s_int operator/ ( const int& iLeft, const s_int& iRight )
    {
        return s_int(iLeft) / iRight;
    }

    s_int operator% ( const int& iLeft, const s_int& iRight )
    {
        return s_int(iLeft) % iRight;
    }

    void s_int::operator+= ( const s_int& mValue )
    {
        *this = *this + mValue;
    }

    void s_int::operator-= ( const s_int& mValue )
    {
        *this = *this - mValue;
    }

    void s_int::operator*= ( const s_int& mValue )
    {
        *this = *this * mValue;
    }

    void s_int::operator/= ( const s_int& mValue )
    {
        *this = *this / mValue;
    }

    void s_int::operator%= ( const s_int& mValue )
    {
        *this = *this % mValue;
    }

    bool s_int::operator== ( const s_int& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return false;
        else
        {
            if ( (mType_ == mValue.mType_) && (mType_ != INTEGER) )
                return true;

            return (iValue_ == mValue.iValue_);
        }
    }

    bool s_int::operator!= ( const s_int& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return false;
        else
        {
            if (mType_ != mValue.mType_)
                return true;

            return (iValue_ != mValue.iValue_);
        }
    }

    bool s_int::operator< ( const s_int& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return false;
        else
        {
            if ( (mType_ == mValue.mType_) && (mType_ != INTEGER) )
                return false;

            if (mType_ == INTEGER_INF_MINUS)
                return true;
            if (mType_ == INTEGER_INF_PLUS)
                return false;

            if (mValue.mType_ == INTEGER_INF_MINUS)
                return false;
            if (mValue.mType_ == INTEGER_INF_PLUS)
                return true;

            return (iValue_ < mValue.iValue_);
        }
    }

    bool s_int::operator> ( const s_int& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return false;
        else
        {
            if ( (mType_ == mValue.mType_) && (mType_ != INTEGER) )
                return false;

            if (mType_ == INTEGER_INF_MINUS)
                return false;
            if (mType_ == INTEGER_INF_PLUS)
                return true;

            if (mValue.mType_ == INTEGER_INF_MINUS)
                return true;
            if (mValue.mType_ == INTEGER_INF_PLUS)
                return false;

            return (iValue_ > mValue.iValue_);
        }
    }

    bool s_int::operator<= ( const s_int& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return false;
        else
        {
            if ( (mType_ == mValue.mType_) && (mType_ != INTEGER) )
                return true;

            if (mType_ == INTEGER_INF_MINUS)
                return true;
            if (mType_ == INTEGER_INF_PLUS)
                return false;

            if (mValue.mType_ == INTEGER_INF_MINUS)
                return false;
            if (mValue.mType_ == INTEGER_INF_PLUS)
                return true;

            return (iValue_ <= mValue.iValue_);
        }
    }

    bool s_int::operator>= ( const s_int& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return false;
        else
        {
            if ( (mType_ == mValue.mType_) && (mType_ != INTEGER) )
                return true;

            if (mType_ == INTEGER_INF_MINUS)
                return false;
            if (mType_ == INTEGER_INF_PLUS)
                return true;

            if (mValue.mType_ == INTEGER_INF_MINUS)
                return true;
            if (mValue.mType_ == INTEGER_INF_PLUS)
                return false;

            return (iValue_ >= mValue.iValue_);
        }
    }

    s_int& s_int::operator= ( const s_int& mValue )
    {
        iValue_ = mValue.iValue_;
        mType_ = mValue.mType_;
        return *this;
    }

    s_bool s_int::IsValid() const
    {
        return (mType_ == INTEGER);
    }

    s_int s_int::operator- () const
    {
        if (!IsNaN())
        {
            if (IsInfinitePlus())
                return s_int(INTEGER_INF_MINUS);
            else if (IsInfiniteMinus())
                return s_int(INTEGER_INF_PLUS);
            else
                return s_int(-iValue_);
        }
        else
            return s_int(INTEGER_NAN);
    }

    s_int& s_int::operator++ ()
    {
        if (IsValid())
        {
            ++iValue_;
        }
        return *this;
    }

    s_int s_int::operator++ (int)
    {
        if (IsValid())
        {
            return s_int(iValue_++);
        }
        else
            return *this;
    }

    s_int& s_int::operator-- ()
    {
        if (IsValid())
        {
            --iValue_;
        }
        return *this;
    }

    s_int s_int::operator-- ( int )
    {
        if (IsValid())
        {
            return s_int(iValue_--);
        }
        else
            return *this;
    }

    s_int& s_int::operator<< ( const s_uint& uiValue )
    {
        if (IsValid())
        {
            iValue_ = iValue_ << uiValue.Get();
        }
        return *this;
    }

    s_uint s_int::GetNearestPowerOfTwo() const
    {
        if (IsValid())
        {
            s_uint i = 1;
            while (iValue_ > (int)i.Get())
                i = i << 1;

            return i;
        }
        else
            return s_uint(s_uint::INTEGER_NAN);
    }

    void s_int::Pow( const s_uint& uiPower )
    {
        if (IsValid() && uiPower.IsValid())
        {
            iValue_ = static_cast<int>(pow((float)iValue_, (int)uiPower.Get()));
        }
    }

    void s_int::Random( const s_int& iMin, const s_int& iMax )
    {
        if (iMin.IsValid() && iMax.IsValid())
        {
            mType_ = INTEGER;
            if (iMax < iMin)
                iValue_ = iMin.Get();
            else
                iValue_ = static_cast<int>(Ogre::Math::RangeRandom(iMin.Get(), iMax.Get()));
        }
    }

    s_int s_int::Sign()
    {
        if (IsValid())
        {
            if (iValue_ == 0)
                return s_int(1);
            else
                return s_int(static_cast<int>(iValue_/fabs(iValue_)));
        }
        else
            return s_int(INTEGER_NAN);
    }

    s_int s_int::Min( const s_int& iLeft, const s_int& iRight )
    {
        if (iLeft >= iRight)
            return iRight;
        else
            return iLeft;
    }

    s_int s_int::Max( const s_int& iLeft, const s_int& iRight )
    {
        if (iLeft <= iRight)
            return iRight;
        else
            return iLeft;
    }

    s_int s_int::Rand( const s_int& iMin, const s_int& iMax )
    {
        if (iMin.IsValid() && iMax.IsValid())
        {
            if (iMax < iMin)
            {
                return s_int(iMin);
            }
            else
            {
                s_int i(static_cast<int>(Ogre::Math::RangeRandom(iMin.Get(), iMax.Get())));
                return i;
            }
        }
        else
            return s_int(INTEGER_NAN);
    }

    s_str operator+ ( const char* sLeft, const s_int& iRight )
    {
        return s_str(sLeft) << iRight;
    }

    s_str s_int::operator+ ( const char* sValue ) const
    {
        return s_str(*this) + sValue;
    }

    s_str s_int::operator+ ( const s_str& sValue ) const
    {
        return s_str(*this) + sValue;
    }

    s_ctnr<s_int> s_int::operator, ( const s_int& iValue ) const
    {
        s_ctnr<s_int> mContainer;
        mContainer.Push(*this);
        mContainer.Push(iValue);
        return mContainer;
    }
}
