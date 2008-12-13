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
    const s_str s_uint::CLASS_NAME = "s_uint";

    const s_uint s_uint::NaN = s_uint(INTEGER_NAN);
    const s_uint s_uint::INF = s_uint(INTEGER_INF);

    uint s_uint::uiDummy = 0u;

    s_uint::s_uint()
    {
        mType_ = INTEGER;
        uiValue_ = 0u;
    }

    s_uint::s_uint( const uint& uiValue )
    {
        mType_ = INTEGER;
        uiValue_ = uiValue;
    }

    s_uint::s_uint( const s_int& mValue )
    {
        if (mValue.GetType() == s_int::INTEGER_NAN)
        {
            mType_ = INTEGER_NAN;
            uiValue_ = 0u;
        }
        else if ( (mValue.GetType() == s_int::INTEGER) && (mValue >= 0u) )
        {
            mType_ = INTEGER;
            uiValue_ = static_cast<uint>(mValue.Get());
        }
        else
        {
            mType_ = INTEGER_INF;
            uiValue_ = 0u;
        }
    }

    s_uint::s_uint( const s_float& mValue )
    {
        if (mValue.GetType() == s_float::FLOAT_NAN)
        {
            mType_ = INTEGER_NAN;
            uiValue_ = 0u;
        }
        else if ( (mValue.GetType() == s_float::FLOAT) && (mValue >= 0.0f) )
        {
            mType_ = INTEGER;
            uiValue_ = static_cast<uint>(round(mValue.Get()));
        }
        else
        {
            mType_ = INTEGER_INF;
            uiValue_ = 0u;
        }
    }

    s_uint::s_uint( const s_double& mValue )
    {
        if (mValue.GetType() == s_double::DOUBLE_NAN)
        {
            mType_ = INTEGER_NAN;
            uiValue_ = 0u;
        }
        else if ( (mValue.GetType() == s_double::DOUBLE) && (mValue >= 0.0) )
        {
            mType_ = INTEGER;
            uiValue_ = static_cast<uint>(round(mValue.Get()));
        }
        else
        {
            mType_ = INTEGER_INF;
            uiValue_ = 0u;
        }
    }

    s_uint::s_uint( const s_bool& mValue )
    {
        mType_ = INTEGER;
        if (mValue)
            uiValue_ = 1;
        else
            uiValue_ = 0;
    }

    s_uint::s_uint( const float& fValue )
    {
        mType_ = INTEGER;
        uiValue_ = static_cast<uint>(round(fValue));
    }

    s_uint::s_uint( const double& dValue )
    {
        mType_ = INTEGER;
        uiValue_ = static_cast<uint>(round(dValue));
    }

    s_uint::s_uint( const IntegerType& mType )
    {
        mType_ = mType;
        uiValue_ = 0u;
    }

    s_uint::s_uint( const char* sValue )
    {
        s_int i = atoi(sValue);
        if (i < 0u)
        {
            mType_ = INTEGER_INF;
            uiValue_ = 0u;
        }
        else
        {
            mType_ = INTEGER;
            uiValue_ = static_cast<uint>(i.Get());
        }
    }

    s_uint::s_uint( const string& sValue )
    {
        s_int i = atoi(sValue.c_str());
        if (i < 0u)
        {
            mType_ = INTEGER_INF;
            uiValue_ = 0u;
        }
        else
        {
            mType_ = INTEGER;
            uiValue_ = static_cast<uint>(i.Get());
        }
    }

    s_uint::s_uint( const s_str& sValue )
    {
        s_int i = atoi(sValue.c_str());
        if (i < 0u)
        {
            mType_ = INTEGER_INF;
            uiValue_ = 0u;
        }
        else
        {
            mType_ = INTEGER;
            uiValue_ = static_cast<uint>(i.Get());
        }
    }

    s_uint::operator MemberFn() const
    {
        if (mType_ == INTEGER)
            return &s_uint::SetNaN;
        else
            return NULL;
    }

    s_uint::IntegerType s_uint::GetType() const
    {
        return mType_;
    }

    void s_uint::SetInfinite()
    {
        mType_ = INTEGER_INF;
        uiValue_ = 0u;
    }

    void s_uint::SetNaN()
    {
        mType_ = INTEGER_NAN;
        uiValue_ = 0u;
    }

    s_bool s_uint::IsInfinite() const
    {
        return (mType_ == INTEGER_INF);
    }

    s_bool s_uint::IsNaN() const
    {
        return (mType_ == INTEGER_NAN);
    }

    s_bool s_uint::IsNull() const
    {
        return ( IsValid() && (uiValue_ == 0u) );
    }

    s_uint s_uint::operator+ ( const s_uint& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return s_uint(INTEGER_NAN);

        if (!mValue || !(*this))
            return s_uint(INTEGER_INF);
        else
        {
            return s_uint(uiValue_ + mValue.uiValue_);
        }
    }

    s_uint s_uint::operator- ( const s_uint& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return s_uint(INTEGER_NAN);

        if (!mValue || !(*this))
            return s_uint(INTEGER_INF);
        else
        {
            return s_uint(uiValue_ - mValue.uiValue_);
        }
    }

    s_uint s_uint::operator* ( const s_uint& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return s_uint(INTEGER_NAN);

        if (!mValue)
        {
            if (IsNull())
                return s_uint(INTEGER_NAN);
            else
                return s_uint(INTEGER_INF);
        }
        else if (!(*this))
        {
            if (mValue.IsNull())
                return s_uint(INTEGER_NAN);
            else
                return s_uint(INTEGER_INF);
        }
        else
        {
            return s_uint(uiValue_ * mValue.uiValue_);
        }
    }

    s_uint s_uint::operator/ ( const s_uint& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN() || mValue.IsNull())
            return s_uint(INTEGER_NAN);

        if (!mValue)
        {
            if (!(*this))
                return s_uint(INTEGER_NAN);
            else
                return s_uint(0u);
        }
        else if (!(*this))
        {
            return s_uint(INTEGER_INF);
        }
        else
        {
            return s_uint(uiValue_ / mValue.uiValue_);
        }
    }

    s_uint s_uint::operator% ( const s_uint& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN() || mValue.IsNull())
            return s_uint(INTEGER_NAN);

        if (!mValue)
        {
            if (!(*this))
                return s_uint(0u);
            else
                return *this;
        }
        else if (!(*this))
        {
            return s_uint(0u);
        }
        else
        {
            return s_uint(uiValue_ % mValue.uiValue_);
        }
    }

    s_uint operator+ ( const uint& uiLeft, const s_uint& uiRight )
    {
        return s_uint(uiLeft) + uiRight;
    }

    s_uint operator- ( const uint& uiLeft, const s_uint& uiRight )
    {
        return s_uint(uiLeft) - uiRight;
    }

    s_uint operator* ( const uint& uiLeft, const s_uint& uiRight )
    {
        return s_uint(uiLeft) * uiRight;
    }

    s_uint operator/ ( const uint& uiLeft, const s_uint& uiRight )
    {
        return s_uint(uiLeft) / uiRight;
    }

    s_uint operator% ( const uint& uiLeft, const s_uint& uiRight )
    {
        return s_uint(uiLeft) % uiRight;
    }

    /*s_float s_uint::operator+ ( const s_float& fValue ) const
    {
        if (fValue.IsNaN() || IsNaN())
            return s_float(s_float::FLOAT_NAN);

        if (!fValue)
        {
            if (fValue.IsInfinitePlus())
            {
                return s_float(s_float::FLOAT_INF_PLUS);
            }
            else
            {
                if (IsInfinite())
                    return s_float(s_float::FLOAT_NAN);
                else
                    return s_float(s_float::FLOAT_INF_MINUS);
            }
        }
        else if (!(*this))
        {
            return s_float(s_float::FLOAT_INF_PLUS);
        }
        else
        {
            return s_float(uiValue_ + fValue.Get());
        }
    }

    s_float s_uint::operator- ( const s_float& fValue ) const
    {
        if (fValue.IsNaN() || IsNaN())
            return s_float(s_float::FLOAT_NAN);

        if (!fValue)
        {
            if (fValue.IsInfinitePlus())
            {
                if (IsInfinite())
                    return s_float(s_float::FLOAT_NAN);
                else
                    return s_float(s_float::FLOAT_INF_MINUS);
            }
            else
            {
                return s_float(s_float::FLOAT_INF_PLUS);
            }
        }
        else if (!(*this))
        {
            return s_float(s_float::FLOAT_INF_MINUS);
        }
        else
        {
            return s_float(uiValue_ - fValue.Get());
        }
    }

    s_float s_uint::operator* ( const s_float& fValue ) const
    {
        if (fValue.IsNaN() || IsNaN())
            return s_float(s_float::FLOAT_NAN);

        if (!fValue)
        {
            if (fValue.IsInfinitePlus())
            {
                if (IsNull())
                    return s_float(s_float::FLOAT_NAN);
                else
                    return s_float(s_float::FLOAT_INF_PLUS);
            }
            else
            {
                if (IsInfinite() || IsNull())
                    return s_float(s_float::FLOAT_NAN);
                else
                    return s_float(s_float::FLOAT_INF_MINUS);
            }
        }
        else if (!(*this))
        {
            if (fValue.IsNull())
                return s_float(s_float::FLOAT_NAN);

            if (fValue.Get() > 0.0f)
                return s_float(s_float::FLOAT_INF_PLUS);
            else
                return s_float(s_float::FLOAT_INF_MINUS);
        }
        else
        {
            return s_float(uiValue_ * fValue.Get());
        }
    }

    s_float s_uint::operator/ ( const s_float& fValue ) const
    {
        if (fValue.IsNaN() || IsNaN() || fValue.IsNull())
            return s_float(s_float::FLOAT_NAN);

        if (!fValue)
        {
            if (!(*this))
                return s_float(s_float::FLOAT_NAN);
            else
                return s_float(0.0f);
        }
        else if (!(*this))
        {
            if (fValue.Get() > 0.0f)
                return s_float(s_float::FLOAT_INF_PLUS);
            else
                return s_float(s_float::FLOAT_INF_MINUS);
        }
        else
        {
            return s_float(uiValue_ / fValue.Get());
        }
    }*/

    void s_uint::operator+= ( const s_uint& mValue )
    {
        *this = *this + mValue;
    }

    void s_uint::operator-= ( const s_uint& mValue )
    {
        *this = *this - mValue;
    }

    void s_uint::operator*= ( const s_uint& mValue )
    {
        *this = *this * mValue;
    }

    void s_uint::operator/= ( const s_uint& mValue )
    {
        *this = *this / mValue;
    }

    void s_uint::operator%= ( const s_uint& mValue )
    {
        *this = *this % mValue;
    }

    bool s_uint::operator== ( const s_uint& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return false;
        else
        {
            if ( (mType_ == mValue.mType_) && (mType_ != INTEGER) )
                return true;

            return (uiValue_ == mValue.uiValue_);
        }
    }

    bool s_uint::operator!= ( const s_uint& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return false;
        else
        {
            if (mType_ != mValue.mType_)
                return true;

            return (uiValue_ != mValue.uiValue_);
        }
    }

    bool s_uint::operator< ( const s_uint& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return false;
        else
        {
            if ( (mType_ == mValue.mType_) && (mType_ != INTEGER) )
                return false;

            if (mType_ == INTEGER_INF)
                return false;

            if (mValue.mType_ == INTEGER_INF)
                return true;

            return (uiValue_ < mValue.uiValue_);
        }
    }

    bool s_uint::operator> ( const s_uint& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return false;
        else
        {
            if ( (mType_ == mValue.mType_) && (mType_ != INTEGER) )
                return false;

            if (mType_ == INTEGER_INF)
                return true;

            if (mValue.mType_ == INTEGER_INF)
                return false;

            return (uiValue_ > mValue.uiValue_);
        }
    }

    bool s_uint::operator<= ( const s_uint& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return false;
        else
        {
            if ( (mType_ == mValue.mType_) && (mType_ != INTEGER) )
                return true;

            if (mType_ == INTEGER_INF)
                return false;

            if (mValue.mType_ == INTEGER_INF)
                return true;

            return (uiValue_ <= mValue.uiValue_);
        }
    }

    bool s_uint::operator>= ( const s_uint& mValue ) const
    {
        if (mValue.IsNaN() || IsNaN())
            return false;
        else
        {
            if ( (mType_ == mValue.mType_) && (mType_ != INTEGER) )
                return true;

            if (mType_ == INTEGER_INF)
                return true;

            if (mValue.mType_ == INTEGER_INF)
                return false;

            return (uiValue_ >= mValue.uiValue_);
        }
    }

    s_uint& s_uint::operator= ( const s_uint& mValue )
    {
        uiValue_ = mValue.uiValue_;
        mType_ = mValue.mType_;
        return *this;
    }

    bool s_uint::operator! () const
    {
        return (mType_ != INTEGER);
    }

    s_bool s_uint::IsValid() const
    {
        return (mType_ == INTEGER);
    }

    s_int s_uint::operator- () const
    {
        if (!IsNaN())
        {
            if (IsInfinite())
                return s_int(s_int::INTEGER_INF_MINUS);
            else
                return -s_int((int)uiValue_);
        }
        else
            return s_int(INTEGER_NAN);
    }

    s_uint& s_uint::operator++ ()
    {
        if (IsValid())
        {
            ++uiValue_;
        }
        return *this;
    }

    s_uint s_uint::operator++ (int)
    {
        if (IsValid())
        {
            return s_uint(uiValue_++);
        }
        else
            return *this;
    }

    s_uint& s_uint::operator-- ()
    {
        if (IsValid())
        {
            --uiValue_;
        }
        return *this;
    }

    s_uint s_uint::operator-- ( int )
    {
        if (IsValid())
        {
            return s_uint(uiValue_--);
        }
        else
            return *this;
    }

    s_uint& s_uint::operator<< ( const s_uint& uiValue )
    {
        if (IsValid())
        {
            uiValue_ = uiValue_ << uiValue.Get();
        }
        return *this;
    }

    s_uint s_uint::GetNearestPowerOfTwo() const
    {
        if (IsValid())
        {
            uint i = 1;
            while (uiValue_ > i)
            {
                i = i << 1;
            }

            return i;
        }
        else
            return s_uint(INTEGER_NAN);
    }

    void s_uint::Pow( const s_uint& uiPower )
    {
        if (IsValid() && uiPower.IsValid())
        {
            uiValue_ = static_cast<uint>(pow((float)uiValue_, (int)uiPower.Get()));
        }
    }

    void s_uint::Random( const s_uint& uiMin, const s_uint& uiMax )
    {
        if (uiMin.IsValid() && uiMax.IsValid())
        {
            mType_ = INTEGER;
            if (uiMax < uiMin)
                uiValue_ = uiMin.Get();
            else
                uiValue_ = static_cast<uint>(Ogre::Math::RangeRandom(uiMin.Get(), uiMax.Get()));
        }
    }

    s_uint s_uint::Min( const s_uint& uiLeft, const s_uint& uiRight )
    {
        if (uiLeft >= uiRight)
            return uiRight;
        else
            return uiLeft;
    }

    s_uint s_uint::Max( const s_uint& uiLeft, const s_uint& uiRight )
    {
        if (uiLeft <= uiRight)
            return uiRight;
        else
            return uiLeft;
    }

    s_uint s_uint::Rand( const s_uint& uiMin, const s_uint& uiMax )
    {
        if (uiMin.IsValid() && uiMax.IsValid())
        {
            if (uiMax < uiMin)
            {
                return s_uint(uiMin);
            }
            else
            {
                s_uint ui(static_cast<uint>(Ogre::Math::RangeRandom(uiMin.Get(), uiMax.Get())));
                return ui;
            }
        }
        else
            return s_uint(INTEGER_NAN);
    }

    s_str operator+ ( const char* sLeft, const s_uint& uiRight )
    {
        return s_str(sLeft) << uiRight;
    }

    s_str s_uint::operator+ ( const char* sValue ) const
    {
        return s_str(*this) + sValue;
    }

    s_str s_uint::operator+ ( const s_str& sValue ) const
    {
        return s_str(*this) + sValue;
    }

    ctnr<s_uint> s_uint::operator, ( const s_uint& uiValue ) const
    {
        ctnr<s_uint> mContainer;
        mContainer.Push(*this);
        mContainer.Push(uiValue);
        return mContainer;
    }
}
