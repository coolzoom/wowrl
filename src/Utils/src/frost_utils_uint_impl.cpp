#include "frost_utils_types.h"

#include <cmath>

namespace Frost {

template <class T>
s_uint_t<T>::s_uint_t()
{
    mType_ = INTEGER;
    uiValue_ = 0u;
}

template <class T>
s_uint_t<T>::s_uint_t(const T& uiValue)
{
    mType_ = INTEGER;
    uiValue_ = uiValue;
}

template <class T>
s_uint_t<T>::s_uint_t(const Type& mType)
{
    mType_ = mType;
    uiValue_ = 0u;
}

template <class T>
s_uint_t<T>::s_uint_t(const float& fValue)
{
    mType_ = INTEGER;
    uiValue_ = static_cast<T>(round(fValue));
}

template <class T>
s_uint_t<T>::s_uint_t(const double& dValue)
{
    mType_ = INTEGER;
    uiValue_ = static_cast<T>(round(dValue));
}

template <class T>
s_uint_t<T>::s_uint_t(const s_bool& bValue)
{
    mType_ = INTEGER;
    if (bValue)
        uiValue_ = 1;
    else
        uiValue_ = 0;
}

template <class T>
void s_uint_t<T>::Clamp(const s_uint_t& uiMin, const s_uint_t& uiMax)
{
    uiValue_ = (*this < uiMax) ? ((*this > uiMin) ? uiValue_ : uiMin.uiValue_) : uiMax.uiValue_;
}

template <class T>
s_uint_t<T> s_uint_t<T>::GetNearestPowerOfTwo() const
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
        return s_uint_t(INTEGER_NAN);
}

template <class T>
typename s_uint_t<T>::Type s_uint_t<T>::GetType() const
{
    return mType_;
}

template <class T>
s_bool s_uint_t<T>::IsInfinite() const
{
    return (mType_ == INTEGER_INF);
}

template <class T>
s_bool s_uint_t<T>::IsInRange(const s_uint_t<T>& uiMin, const s_uint_t<T>& uiMax) const
{
    return ( (uiMin <= (*this)) && ((*this) <= uiMax) );
}

template <class T>
s_bool s_uint_t<T>::IsNaN() const
{
    return (mType_ == INTEGER_NAN);
}

template <class T>
s_bool s_uint_t<T>::IsNull() const
{
    return ( IsValid() && (uiValue_ == 0u) );
}

template <class T>
s_bool s_uint_t<T>::IsValid() const
{
    return (mType_ == INTEGER);
}

template <class T>
void s_uint_t<T>::Pow(const s_uint_t& uiPower)
{
    if (IsValid() && uiPower.IsValid())
    {
        uiValue_ = static_cast<uint>(pow(
            static_cast<float>(uiValue_),
            static_cast<int>(uiPower.Get())
        ));
    }
}

template <class T>
void s_uint_t<T>::Randomize(const s_uint_t& uiMin, const s_uint_t& uiMax)
{
    if (uiMin.IsValid() && uiMax.IsValid())
    {
        mType_ = INTEGER;
        if (uiMax.uiValue_ < uiMin.uiValue_)
            uiValue_ = uiMin.uiValue_;
        else
        {
            T uiRange = uiMax.uiValue_ - uiMin.uiValue_ + 1;
            uiValue_ = static_cast<T>(uiRange*(rand()/(RAND_MAX+1.0))) + uiMin.uiValue_;
        }
    }
}

template <class T>
void s_uint_t<T>::SetInfinite()
{
    mType_ = INTEGER_INF;
    uiValue_ = 0u;
}

template <class T>
void s_uint_t<T>::SetNaN()
{
    mType_ = INTEGER_NAN;
    uiValue_ = 0u;
}

template <class T>
s_uint_t<T>& s_uint_t<T>::operator ++ ()
{
    if (IsValid())
    {
        ++uiValue_;
    }
    return *this;
}

template <class T>
s_uint_t<T> s_uint_t<T>::operator ++ (int)
{
    if (IsValid())
    {
        return uiValue_++;
    }
    else
        return *this;
}

template <class T>
s_int_t<typename TypeTraits<T>::SignedType> s_uint_t<T>::operator - () const
{
    if (!IsNaN())
    {
        if (IsInfinite())
            return s_int_t<typename TypeTraits<T>::SignedType>::INFMINUS;
        else
            return -static_cast<typename TypeTraits<T>::SignedType>(uiValue_);
    }
    else
        return s_int_t<typename TypeTraits<T>::SignedType>::NaN;
}

template <class T>
s_uint_t<T>& s_uint_t<T>::operator -- ()
{
    if (IsValid())
    {
        --uiValue_;
    }
    return *this;
}

template <class T>
s_uint_t<T> s_uint_t<T>::operator -- (int)
{
    if (IsValid())
    {
        return uiValue_--;
    }
    else
        return *this;
}

template <class T>
s_uint_t<T> s_uint_t<T>::operator + (const s_uint_t& uiValue) const
{
    if (uiValue.IsNaN() || IsNaN())
        return s_uint_t::NaN;

    if (!uiValue.IsValid() || !this->IsValid())
        return s_uint_t::INF;
    else
    {
        return uiValue_ + uiValue.uiValue_;
    }
}

template <class T>
s_uint_t<T> s_uint_t<T>::operator - (const s_uint_t& uiValue) const
{
    if (uiValue.IsNaN() || IsNaN())
        return s_uint_t::NaN;

    if (!uiValue.IsValid() || !this->IsValid())
        return s_uint_t::INF;
    else
    {
        return uiValue_ - uiValue.uiValue_;
    }
}

template <class T>
s_uint_t<T> s_uint_t<T>::operator * (const s_uint_t& uiValue) const
{
    if (uiValue.IsNaN() || IsNaN())
        return s_uint_t::NaN;

    if (!uiValue.IsValid())
    {
        if (this->IsNull())
            return s_uint_t::NaN;
        else
            return s_uint_t::INF;
    }
    else if (!this->IsValid())
    {
        if (uiValue.IsNull())
            return s_uint_t::NaN;
        else
            return s_uint_t::INF;
    }
    else
    {
        return uiValue_ * uiValue.uiValue_;
    }
}

template <class T>
s_uint_t<T> s_uint_t<T>::operator / (const s_uint_t& uiValue) const
{
    if (uiValue.IsNaN() || IsNaN() || uiValue.IsNull())
        return s_uint_t::NaN;

    if (!uiValue.IsValid())
    {
        if (!this->IsValid())
            return s_uint_t::NaN;
        else
            return 0u;
    }
    else if (!this->IsValid())
    {
        return s_uint_t::INF;
    }
    else
    {
        return uiValue_ / uiValue.uiValue_;
    }
}

template <class T>
s_uint_t<T> s_uint_t<T>::operator % (const s_uint_t& uiValue) const
{
    if (uiValue.IsNaN() || IsNaN() || uiValue.IsNull())
        return s_uint_t::NaN;

    if (!uiValue.IsValid())
    {
        if (!this->IsValid())
            return 0u;
        else
            return *this;
    }
    else if (!this->IsValid())
    {
        return 0u;
    }
    else
    {
        return uiValue_ % uiValue.uiValue_;
    }
}

template <class T>
void s_uint_t<T>::operator += (const s_uint_t& uiValue)
{
    *this = *this + uiValue;
}

template <class T>
void s_uint_t<T>::operator -= (const s_uint_t& uiValue)
{
    *this = *this - uiValue;
}

template <class T>
void s_uint_t<T>::operator *= (const s_uint_t& uiValue)
{
    *this = *this * uiValue;
}

template <class T>
void s_uint_t<T>::operator /= (const s_uint_t& uiValue)
{
    *this = *this / uiValue;
}

template <class T>
void s_uint_t<T>::operator %= (const s_uint_t& uiValue)
{
    *this = *this % uiValue;
}

template <class T>
s_uint_t<T>& s_uint_t<T>::operator << (const s_uint_t& uiValue)
{
    if (IsValid())
    {
        uiValue_ = uiValue_ << uiValue.uiValue_;
    }
    return *this;
}

template <class T>
s_bool s_uint_t<T>::operator == (const s_uint_t& uiValue) const
{
    if (uiValue.IsNaN() || IsNaN())
        return false;
    else
    {
        if ( (mType_ == uiValue.mType_) && (mType_ != INTEGER) )
            return true;

        return (uiValue_ == uiValue.uiValue_);
    }
}

template <class T>
s_bool s_uint_t<T>::operator != (const s_uint_t& uiValue) const
{
    if (uiValue.IsNaN() || IsNaN())
        return false;
    else
    {
        if (mType_ != uiValue.mType_)
            return true;

        return (uiValue_ != uiValue.uiValue_);
    }
}

template <class T>
s_bool s_uint_t<T>::operator < (const s_uint_t& uiValue) const
{
    if (uiValue.IsNaN() || IsNaN())
        return false;
    else
    {
        if ( (mType_ == uiValue.mType_) && (mType_ != INTEGER) )
            return false;

        if (mType_ == INTEGER_INF)
            return false;

        if (uiValue.mType_ == INTEGER_INF)
            return true;

        return (uiValue_ < uiValue.uiValue_);
    }
}

template <class T>
s_bool s_uint_t<T>::operator > (const s_uint_t& uiValue) const
{
    if (uiValue.IsNaN() || IsNaN())
        return false;
    else
    {
        if ( (mType_ == uiValue.mType_) && (mType_ != INTEGER) )
            return false;

        if (mType_ == INTEGER_INF)
            return true;

        if (uiValue.mType_ == INTEGER_INF)
            return false;

        return (uiValue_ > uiValue.uiValue_);
    }
}

template <class T>
s_bool s_uint_t<T>::operator <= (const s_uint_t& uiValue) const
{
    if (uiValue.IsNaN() || IsNaN())
        return false;
    else
    {
        if ( (mType_ == uiValue.mType_) && (mType_ != INTEGER) )
            return true;

        if (mType_ == INTEGER_INF)
            return false;

        if (uiValue.mType_ == INTEGER_INF)
            return true;

        return (uiValue_ <= uiValue.uiValue_);
    }
}

template <class T>
s_bool s_uint_t<T>::operator >= (const s_uint_t& uiValue) const
{
    if (uiValue.IsNaN() || IsNaN())
        return false;
    else
    {
        if ( (mType_ == uiValue.mType_) && (mType_ != INTEGER) )
            return true;

        if (mType_ == INTEGER_INF)
            return true;

        if (uiValue.mType_ == INTEGER_INF)
            return false;

        return (uiValue_ >= uiValue.uiValue_);
    }
}

template <class T>
s_uint_t<T> s_uint_t<T>::Min(const s_uint_t& uiLeft, const s_uint_t& uiRight)
{
    return (uiLeft >= uiRight) ? uiRight : uiLeft;
}

template <class T>
s_uint_t<T> s_uint_t<T>::Max(const s_uint_t& uiLeft, const s_uint_t& uiRight)
{
    return (uiLeft <= uiRight) ? uiRight : uiLeft;
}

template <class T>
s_uint_t<T> s_uint_t<T>::Clamp(const s_uint_t& uiValue, const s_uint_t& uiMin, const s_uint_t& uiMax)
{
    return (uiValue < uiMax) ? ((uiValue > uiMin) ? uiValue : uiMin) : uiMax;
}

template <class T>
s_uint_t<T> s_uint_t<T>::Random(const s_uint_t& uiMin, const s_uint_t& uiMax)
{
    if (uiMin.IsValid() && uiMax.IsValid())
    {
        if (uiMax.uiValue_ < uiMin.uiValue_)
            return uiMin;
        else
        {
            T uiRange = uiMax.uiValue_ - uiMin.uiValue_ + 1;
            return static_cast<T>(uiRange*(rand()/(RAND_MAX+1.0))) + uiMin.uiValue_;
        }
    }
    else
        return s_uint_t::NaN;
}

template<class T>
const s_uint_t<T> s_uint_t<T>::NaN = s_uint_t<T>(INTEGER_NAN);
template<class T>
const s_uint_t<T> s_uint_t<T>::INF = s_uint_t<T>(INTEGER_INF);

template<class T>
s_uint_t<T> operator + (const T& uiLeft, const s_uint_t<T>& uiRight)
{
    return s_uint_t<T>(uiLeft) + uiRight;
}

template<class T>
s_uint_t<T> operator - (const T& uiLeft, const s_uint_t<T>& uiRight)
{
    return s_uint_t<T>(uiLeft) - uiRight;
}

template<class T>
s_uint_t<T> operator * (const T& uiLeft, const s_uint_t<T>& uiRight)
{
    return s_uint_t<T>(uiLeft) * uiRight;
}

template<class T>
s_uint_t<T> operator / (const T& uiLeft, const s_uint_t<T>& uiRight)
{
    return s_uint_t<T>(uiLeft) / uiRight;
}

template<class T>
s_uint_t<T> operator % (const T& uiLeft, const s_uint_t<T>& uiRight)
{
    return s_uint_t<T>(uiLeft) % uiRight;
}

template<class T>
s_uint_t<T> operator + (const typename TypeTraits<T>::SignedType& iLeft, const s_uint_t<T>& uiRight)
{
    return s_uint_t<T>(s_int_t<typename TypeTraits<T>::SignedType>(iLeft)) + uiRight;
}

template<class T>
s_uint_t<T> operator - (const typename TypeTraits<T>::SignedType& iLeft, const s_uint_t<T>& uiRight)
{
    return s_uint_t<T>(s_int_t<typename TypeTraits<T>::SignedType>(iLeft)) - uiRight;
}

template<class T>
s_uint_t<T> operator * (const typename TypeTraits<T>::SignedType& iLeft, const s_uint_t<T>& uiRight)
{
    return s_uint_t<T>(s_int_t<typename TypeTraits<T>::SignedType>(iLeft)) * uiRight;
}

template<class T>
s_uint_t<T> operator / (const typename TypeTraits<T>::SignedType& iLeft, const s_uint_t<T>& uiRight)
{
    return s_uint_t<T>(s_int_t<typename TypeTraits<T>::SignedType>(iLeft)) / uiRight;
}

template<class T>
s_uint_t<T> operator % (const typename TypeTraits<T>::SignedType& iLeft, const s_uint_t<T>& uiRight)
{
    return s_uint_t<T>(s_int_t<typename TypeTraits<T>::SignedType>(iLeft)) % uiRight;
}

template class s_uint_t<uchar>;
template class s_uint_t<ushort>;
template class s_uint_t<uint>;
template class s_uint_t<ulong>;
template class s_uint_t<unsigned long long>;

template s_uint_t<uchar> operator + (const uchar& uiLeft, const s_uint_t<uchar>& uiRight);
template s_uint_t<uchar> operator - (const uchar& uiLeft, const s_uint_t<uchar>& uiRight);
template s_uint_t<uchar> operator * (const uchar& uiLeft, const s_uint_t<uchar>& uiRight);
template s_uint_t<uchar> operator / (const uchar& uiLeft, const s_uint_t<uchar>& uiRight);
template s_uint_t<uchar> operator % (const uchar& uiLeft, const s_uint_t<uchar>& uiRight);

template s_uint_t<uchar> operator + (const char& uiLeft, const s_uint_t<uchar>& uiRight);
template s_uint_t<uchar> operator - (const char& uiLeft, const s_uint_t<uchar>& uiRight);
template s_uint_t<uchar> operator * (const char& uiLeft, const s_uint_t<uchar>& uiRight);
template s_uint_t<uchar> operator / (const char& uiLeft, const s_uint_t<uchar>& uiRight);
template s_uint_t<uchar> operator % (const char& uiLeft, const s_uint_t<uchar>& uiRight);

template s_uint_t<ushort> operator + (const ushort& uiLeft, const s_uint_t<ushort>& uiRight);
template s_uint_t<ushort> operator - (const ushort& uiLeft, const s_uint_t<ushort>& uiRight);
template s_uint_t<ushort> operator * (const ushort& uiLeft, const s_uint_t<ushort>& uiRight);
template s_uint_t<ushort> operator / (const ushort& uiLeft, const s_uint_t<ushort>& uiRight);
template s_uint_t<ushort> operator % (const ushort& uiLeft, const s_uint_t<ushort>& uiRight);

template s_uint_t<ushort> operator + (const short& uiLeft, const s_uint_t<ushort>& uiRight);
template s_uint_t<ushort> operator - (const short& uiLeft, const s_uint_t<ushort>& uiRight);
template s_uint_t<ushort> operator * (const short& uiLeft, const s_uint_t<ushort>& uiRight);
template s_uint_t<ushort> operator / (const short& uiLeft, const s_uint_t<ushort>& uiRight);
template s_uint_t<ushort> operator % (const short& uiLeft, const s_uint_t<ushort>& uiRight);

template s_uint_t<uint> operator + (const uint& uiLeft, const s_uint_t<uint>& uiRight);
template s_uint_t<uint> operator - (const uint& uiLeft, const s_uint_t<uint>& uiRight);
template s_uint_t<uint> operator * (const uint& uiLeft, const s_uint_t<uint>& uiRight);
template s_uint_t<uint> operator / (const uint& uiLeft, const s_uint_t<uint>& uiRight);
template s_uint_t<uint> operator % (const uint& uiLeft, const s_uint_t<uint>& uiRight);

template s_uint_t<uint> operator + (const int& uiLeft, const s_uint_t<uint>& uiRight);
template s_uint_t<uint> operator - (const int& uiLeft, const s_uint_t<uint>& uiRight);
template s_uint_t<uint> operator * (const int& uiLeft, const s_uint_t<uint>& uiRight);
template s_uint_t<uint> operator / (const int& uiLeft, const s_uint_t<uint>& uiRight);
template s_uint_t<uint> operator % (const int& uiLeft, const s_uint_t<uint>& uiRight);

template s_uint_t<ulong> operator + (const ulong& uiLeft, const s_uint_t<ulong>& uiRight);
template s_uint_t<ulong> operator - (const ulong& uiLeft, const s_uint_t<ulong>& uiRight);
template s_uint_t<ulong> operator * (const ulong& uiLeft, const s_uint_t<ulong>& uiRight);
template s_uint_t<ulong> operator / (const ulong& uiLeft, const s_uint_t<ulong>& uiRight);
template s_uint_t<ulong> operator % (const ulong& uiLeft, const s_uint_t<ulong>& uiRight);

template s_uint_t<ulong> operator + (const long& uiLeft, const s_uint_t<ulong>& uiRight);
template s_uint_t<ulong> operator - (const long& uiLeft, const s_uint_t<ulong>& uiRight);
template s_uint_t<ulong> operator * (const long& uiLeft, const s_uint_t<ulong>& uiRight);
template s_uint_t<ulong> operator / (const long& uiLeft, const s_uint_t<ulong>& uiRight);
template s_uint_t<ulong> operator % (const long& uiLeft, const s_uint_t<ulong>& uiRight);

template s_uint_t<ullong> operator + (const ullong& uiLeft, const s_uint_t<ullong>& uiRight);
template s_uint_t<ullong> operator - (const ullong& uiLeft, const s_uint_t<ullong>& uiRight);
template s_uint_t<ullong> operator * (const ullong& uiLeft, const s_uint_t<ullong>& uiRight);
template s_uint_t<ullong> operator / (const ullong& uiLeft, const s_uint_t<ullong>& uiRight);
template s_uint_t<ullong> operator % (const ullong& uiLeft, const s_uint_t<ullong>& uiRight);

template s_uint_t<ullong> operator + (const long long& uiLeft, const s_uint_t<ullong>& uiRight);
template s_uint_t<ullong> operator - (const long long& uiLeft, const s_uint_t<ullong>& uiRight);
template s_uint_t<ullong> operator * (const long long& uiLeft, const s_uint_t<ullong>& uiRight);
template s_uint_t<ullong> operator / (const long long& uiLeft, const s_uint_t<ullong>& uiRight);
template s_uint_t<ullong> operator % (const long long& uiLeft, const s_uint_t<ullong>& uiRight);

}
