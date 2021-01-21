#include "frost_utils_types.h"

#include <cfloat>
#include <limits>
#include <cmath>

namespace Frost {

template <class T>
s_float_t<T>::s_float_t()
{
    fValue_ = 0.0;
}

template <class T>
s_float_t<T>::s_float_t(const T& fValue)
{
    fValue_ = fValue;
}

template <class T>
s_float_t<T>::s_float_t(const Type& mType)
{
    switch (mType)
    {
        case FLOAT_INF_PLUS  : SetInfinitePlus();
        case FLOAT_INF_MINUS : SetInfiniteMinus();
        case FLOAT_NAN       : SetNaN();
        default : fValue_ = 0.0;
    }
}

template <class T>
s_float_t<T>::s_float_t(const s_bool& bValue)
{
    if (bValue)
        fValue_ = 1.0;
    else
        fValue_ = 0.0;
}

template <class T>
void s_float_t<T>::Clamp(const s_float_t& fMin, const s_float_t& fMax)
{
    fValue_ = (fValue_ < fMax.fValue_) ? ((fValue_ > fMin.fValue_) ? fValue_ : fMin.fValue_) : fMax.fValue_;
}

template <class T>
void s_float_t<T>::DegToRad()
{
    if (IsValid())
    {
        fValue_ *= PI.fValue_/180.0;
    }
}

template <class T>
s_float_t<T> s_float_t<T>::GetRad() const
{
    if (IsValid())
    {
        return fValue_*2*PI.fValue_;
    }
    else
        return 0.0;
}

template <class T>
s_float_t<T> s_float_t<T>::GetDeg() const
{
    if (IsValid())
    {
        return fValue_*360.0;
    }
    else
        return 0.0;
}

template <class T>
s_uint_t<default_uint> s_float_t<T>::GetNearestPowerOfTwo() const
{
    if (IsValid())
    {
        default_uint i = 1;
        while (fValue_ > i)
            i = i << 1;

        return i;
    }
    else
        return s_uint_t<default_uint>::NaN;
}

template <class T>
typename s_float_t<T>::Type s_float_t<T>::GetType() const
{
    if (IsNaN())
        return FLOAT_NAN;
    else
    {
        if (IsValid())
            return FLOAT;
        else
        {
            if (fValue_ < 0.0)
                return FLOAT_INF_MINUS;
            else
                return FLOAT_INF_PLUS;
        }
    }
}

template <class T>
s_bool s_float_t<T>::IsInfiniteMinus() const
{
    return (IsInfinite() && (fValue_ < 0.0));
}

template <class T>
s_bool s_float_t<T>::IsInfinitePlus() const
{
    return (IsInfinite() && (fValue_ > 0.0));
}

template <class T>
s_bool s_float_t<T>::IsInRange(const s_float_t<T>& fMin, const s_float_t<T>& fMax) const
{
    return ( (fMin <= (*this)) && ((*this) <= fMax) );
}

template <class T>
s_bool s_float_t<T>::IsInRangeOpen(const s_float_t<T>& fMin, const s_float_t<T>& fMax) const
{
    return ( (fMin < (*this)) && ((*this) < fMax) );
}

template <class T>
s_bool s_float_t<T>::IsNaN() const
{
    #ifdef MSVC
        return _isnan(fValue_) != 0;
    #else
        return std::isnan(fValue_);
    #endif
}

template <class T>
s_bool s_float_t<T>::IsNull() const
{
    return ( IsValid() && (fValue_ < EPS) && (fValue_ > -EPS) );
}

template <class T>
s_bool s_float_t<T>::IsValid() const
{
    #ifdef MSVC
        return _finite(fValue_) != 0;
    #else
        return std::isfinite(fValue_);
    #endif
}

template <class T>
s_bool s_float_t<T>::IsInfinite() const
{
    #ifdef MSVC
        return (_finite(fValue_) == 0) && (_isnan(fValue_) == 0);
    #else
        return !std::isfinite(fValue_) && !std::isnan(fValue_);
    #endif
}

template <class T>
void s_float_t<T>::Pow(const s_float_t& fPower)
{
    if (IsValid() && fPower.IsValid())
    {
        fValue_ = pow(fValue_, fPower.fValue_);
    }
}

template <class T>
void s_float_t<T>::RadToDeg(const s_bool& bNegativeAllowed)
{
    if (IsValid())
    {
        fValue_ *= 180.0/PI.fValue_;
        if ( (!bNegativeAllowed) && (fValue_ < 0.0) )
            fValue_ += 360.0;
    }
}

template <class T>
void s_float_t<T>::Randomize(const s_float_t& fMin, const s_float_t& fMax)
{
    if (fMin.IsValid() && fMax.IsValid())
    {
        if (fMax < fMin)
            fValue_ = fMin.fValue_;
        else
        {
            T fRange = fMax.fValue_ - fMin.fValue_;
            fValue_ = fRange*(rand()/((T)RAND_MAX)) + fMin.fValue_;
        }
    }
}

template <class T>
void s_float_t<T>::Round()
{
    fValue_ = round(fValue_);
}

template <class T>
void s_float_t<T>::RoundUp()
{
    fValue_ = ceil(fValue_);
}

template <class T>
void s_float_t<T>::RoundDown()
{
    fValue_ = floor(fValue_);
}

template <class T>
void s_float_t<T>::SetInfiniteMinus()
{
    fValue_ = INFMINUS.fValue_;
}

template <class T>
void s_float_t<T>::SetInfinitePlus()
{
    fValue_ = INFPLUS.fValue_;
}

template <class T>
void s_float_t<T>::SetNaN()
{
    fValue_ = NaN.fValue_;
}

template <class T>
s_float_t<T> s_float_t<T>::GetSign() const
{
    if (IsValid())
    {
        return (fValue_ < 0.0) ? -1 : 1;
    }
    else
        return s_float_t::NaN;
}

template <class T>
void s_float_t<T>::ToDeg(const s_bool& bNegativeAllowed)
{
    if (IsValid())
    {
        fValue_ *= 360.0;
        if ( (!bNegativeAllowed) && (fValue_ < 0.0) )
            fValue_ += 360.0;
    }
}

template <class T>
void s_float_t<T>::ToRad()
{
    if (IsValid())
    {
        fValue_ *= 2*PI.fValue_;
    }
}

template <class T>
s_float_t<T> s_float_t<T>::operator - () const
{
    return -fValue_;
}

template <class T>
s_float_t<T> s_float_t<T>::operator + (const s_float_t& fValue) const
{
    return fValue_ + fValue.fValue_;
}

template <class T>
s_float_t<T> s_float_t<T>::operator - (const s_float_t& fValue) const
{
    return fValue_ - fValue.fValue_;
}

template <class T>
s_float_t<T> s_float_t<T>::operator * (const s_float_t& fValue) const
{
    return fValue_ * fValue.fValue_;
}

template <class T>
s_float_t<T> s_float_t<T>::operator / (const s_float_t& fValue) const
{
    return fValue_ / fValue.fValue_;
}

template <class T>
void s_float_t<T>::operator += (const s_float_t& fValue)
{
    fValue_ += fValue.fValue_;
}

template <class T>
void s_float_t<T>::operator -= (const s_float_t& fValue)
{
    fValue_ -= fValue.fValue_;
}

template <class T>
void s_float_t<T>::operator *= (const s_float_t& fValue)
{
    fValue_ *= fValue.fValue_;
}

template <class T>
void s_float_t<T>::operator /= (const s_float_t& fValue)
{
    fValue_ /= fValue.fValue_;
}

template <class T>
s_bool s_float_t<T>::operator == (const s_float_t& fValue) const
{
    return (fValue_ == fValue.fValue_);
}

template <class T>
s_bool s_float_t<T>::operator != (const s_float_t& fValue) const
{
    return (fValue_ != fValue.fValue_);
}

template <class T>
s_bool s_float_t<T>::operator < (const s_float_t& fValue) const
{
    return (fValue_ < fValue.fValue_);
}

template <class T>
s_bool s_float_t<T>::operator > (const s_float_t& fValue) const
{
    return (fValue_ > fValue.fValue_);
}

template <class T>
s_bool s_float_t<T>::operator <= (const s_float_t& fValue) const
{
    return (fValue_ <= fValue.fValue_);
}

template <class T>
s_bool s_float_t<T>::operator >= (const s_float_t& fValue) const
{
    return (fValue_ >= fValue.fValue_);
}

template <class T>
s_float_t<T> s_float_t<T>::ToRad(const s_float_t& fValue)
{
    return fValue * 2*PI;
}

template <class T>
s_float_t<T> s_float_t<T>::ToDeg(const s_float_t& fValue)
{
    return fValue * 360;
}

template <class T>
s_float_t<T> s_float_t<T>::DegToRad(const s_float_t& fValue)
{
    return fValue * PI / 180;
}

template <class T>
s_float_t<T> s_float_t<T>::RadToDeg(const s_float_t& fValue)
{
    return fValue * 180 / PI;
}

template <class T>
s_float_t<T> s_float_t<T>::Clamp(const s_float_t& fValue, const s_float_t& fMin, const s_float_t& fMax)
{
    return (fValue.fValue_ < fMax.fValue_) ? ((fValue.fValue_ > fMin.fValue_) ? fValue.fValue_ : fMin.fValue_) : fMax.fValue_;
}

template <class T>
s_float_t<T> s_float_t<T>::Min(const s_float_t& fLeft, const s_float_t& fRight)
{
    return (fLeft >= fRight) ? fRight : fLeft;
}

template <class T>
s_float_t<T> s_float_t<T>::Max(const s_float_t& fLeft, const s_float_t& fRight)
{
    return (fLeft <= fRight) ? fRight : fLeft;
}

template <class T>
s_float_t<T> s_float_t<T>::Random(const s_float_t& fMin, const s_float_t& fMax)
{
    if (fMin.IsValid() && fMax.IsValid())
    {
        if (fMax.fValue_ < fMin.fValue_)
        {
            return fMin;
        }
        else
        {
            T fRange = fMax.fValue_ - fMin.fValue_;
            return fRange*(rand()/((T)RAND_MAX)) + fMin.fValue_;
        }
    }
    else
        return s_float_t::NaN;
}

template <class T>
s_float_t<T> s_float_t<T>::Round(const s_float_t& fValue)
{
    return round(fValue.fValue_);
}

template <class T>
s_float_t<T> s_float_t<T>::RoundUp(const s_float_t& fValue)
{
    return ceil(fValue.fValue_);
}

template <class T>
s_float_t<T> s_float_t<T>::RoundDown(const s_float_t& fValue)
{
    return floor(fValue.fValue_);
}

template <class T>
s_float_t<T> s_float_t<T>::Sign(const s_float_t& fValue)
{
    if (fValue.IsValid())
    {
        return (fValue.fValue_ < 0.0) ? -1 : 1;
    }
    else
        return s_float_t::NaN;
}

template <class T>
const s_float_t<T> s_float_t<T>::NaN      = MakeFloat(0xFFC00000);
template <class T>
const s_float_t<T> s_float_t<T>::INFPLUS  = MakeFloat(0x7F800000);
template <class T>
const s_float_t<T> s_float_t<T>::INFMINUS = MakeFloat(0xFF800000);
template <class T>
const T s_float_t<T>::EPS = 0.0;
template <class T>
const s_float_t<T> s_float_t<T>::PI = 3.1415;
template <class T>
const default_uint s_float_t<T>::DIGIT = 1;

template <class T>
s_float_t<T> operator + (const T& fLeft, const s_float_t<T>& fRight)
{
    return s_float_t<T>(fLeft) + fRight;
}

template <class T>
s_float_t<T> operator - (const T& fLeft, const s_float_t<T>& fRight)
{
    return s_float_t<T>(fLeft) - fRight;
}

template <class T>
s_float_t<T> operator * (const T& fLeft, const s_float_t<T>& fRight)
{
    return s_float_t<T>(fLeft) * fRight;
}

template <class T>
s_float_t<T> operator / (const T& fLeft, const s_float_t<T>& fRight)
{
    return s_float_t<T>(fLeft) / fRight;
}

typedef s_float_t<float>       s_float;
typedef s_float_t<double>      s_double;
typedef s_float_t<long double> s_ldouble;

template<> const float s_float::EPS  = std::numeric_limits<float>::epsilon();
template<> const s_float s_float::PI = 3.141592f;
template<> const default_uint s_float::DIGIT = std::numeric_limits<float>::digits10;

template<> const double s_double::EPS  = std::numeric_limits<double>::epsilon();
template<> const s_double s_double::PI = 3.141592653589793;
template<> const default_uint s_double::DIGIT  = std::numeric_limits<double>::digits10;

template<> const long double s_ldouble::EPS = std::numeric_limits<long double>::epsilon();
template<> const s_ldouble s_ldouble::PI    = 3.141592653589793238;
template<> const default_uint s_ldouble::DIGIT      = std::numeric_limits<long double>::digits10;

template class s_float_t<float>;
template class s_float_t<double>;
template class s_float_t<long double>;


template s_float_t<float> operator + (const float& fLeft, const s_float& fRight);
template s_float_t<float> operator - (const float& fLeft, const s_float& fRight);
template s_float_t<float> operator * (const float& fLeft, const s_float& fRight);
template s_float_t<float> operator / (const float& fLeft, const s_float& fRight);

template s_float_t<double> operator + (const double& fLeft, const s_double& fRight);
template s_float_t<double> operator - (const double& fLeft, const s_double& fRight);
template s_float_t<double> operator * (const double& fLeft, const s_double& fRight);
template s_float_t<double> operator / (const double& fLeft, const s_double& fRight);

template s_float_t<long double> operator + (const long double& fLeft, const s_ldouble& fRight);
template s_float_t<long double> operator - (const long double& fLeft, const s_ldouble& fRight);
template s_float_t<long double> operator * (const long double& fLeft, const s_ldouble& fRight);
template s_float_t<long double> operator / (const long double& fLeft, const s_ldouble& fRight);

}
