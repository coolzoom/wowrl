#include "frost_utils_types.h"

#include <cmath>

namespace Frost {

template <class T>
s_int_t<T>::s_int_t()
{
    mType_ = INTEGER;
    iValue_ = 0;
}

template <class T>
s_int_t<T>::s_int_t(const T& iValue)
{
    mType_ = INTEGER;
    iValue_ = iValue;
}

template <class T>
s_int_t<T>::s_int_t(const Type& mType)
{
    mType_ = mType;
    iValue_ = 0;
}

template <class T>
s_int_t<T>::s_int_t(const float& fValue)
{
    mType_ = INTEGER;
    iValue_ = static_cast<T>(round(fValue));
}

template <class T>
s_int_t<T>::s_int_t(const double& dValue)
{
    mType_ = INTEGER;
    iValue_ = static_cast<T>(round(dValue));
}

template <class T>
s_int_t<T>::s_int_t(const s_bool& bValue)
{
    mType_ = INTEGER;
    if (bValue)
        iValue_ = 1;
    else
        iValue_ = 0;
}

template <class T>
void s_int_t<T>::Clamp(const s_int_t& iMin, const s_int_t& iMax)
{
    iValue_ = (*this < iMax) ? ((*this > iMin) ? iValue_ : iMin.iValue_) : iMax.iValue_;
}

template <class T>
s_uint_t<typename TypeTraits<T>::UnsignedType> s_int_t<T>::GetNearestPowerOfTwo() const
{
    if (IsValid())
    {
        int i = 1;
        while (iValue_ > i)
            i = i << 1;

        return static_cast<typename TypeTraits<T>::UnsignedType>(i);
    }
    else
        return s_uint_t<typename TypeTraits<T>::UnsignedType>::NaN;
}

template <class T>
typename s_int_t<T>::Type s_int_t<T>::GetType() const
{
    return mType_;
}

template <class T>
s_bool s_int_t<T>::IsInfiniteMinus() const
{
    return (mType_ == INTEGER_INF_MINUS);
}

template <class T>
s_bool s_int_t<T>::IsInfinitePlus() const
{
    return (mType_ == INTEGER_INF_PLUS);
}

template <class T>
void s_int_t<T>::Pow(const s_uint_t<typename TypeTraits<T>::UnsignedType>& uiPower)
{
    if (IsValid() && uiPower.IsValid())
    {
        iValue_ = static_cast<T>(pow(
            static_cast<float>(iValue_), static_cast<T>(uiPower.Get())
        ));
    }
}

template <class T>
s_bool s_int_t<T>::IsInRange(const s_int_t<T>& iMin, const s_int_t<T>& iMax) const
{
    return ( (iMin <= (*this)) && ((*this) <= iMax) );
}

template <class T>
s_bool s_int_t<T>::IsNaN() const
{
    return (mType_ == INTEGER_NAN);
}

template <class T>
s_bool s_int_t<T>::IsNull() const
{
    return ( IsValid() && (iValue_ == 0) );
}

template <class T>
s_bool s_int_t<T>::IsValid() const
{
    return (mType_ == INTEGER);
}

template <class T>
s_bool s_int_t<T>::IsInfinite() const
{
    return (mType_ == INTEGER_INF_MINUS) || (mType_ == INTEGER_INF_PLUS);
}

template <class T>
void s_int_t<T>::Randomize(const s_int_t& iMin, const s_int_t& iMax)
{
    if (iMin.IsValid() && iMax.IsValid())
    {
        mType_ = INTEGER;
        if (iMax.iValue_ < iMin.iValue_)
            iValue_ = iMin.iValue_;
        else
        {
            T iRange = iMax.iValue_ - iMin.iValue_ + 1;
            iValue_ = static_cast<T>(iRange*(rand()/(RAND_MAX+1.0))) + iMin.iValue_;
        }
    }
}

template <class T>
void s_int_t<T>::SetInfiniteMinus()
{
    mType_ = INTEGER_INF_MINUS;
    iValue_ = 0;
}

template <class T>
void s_int_t<T>::SetInfinitePlus()
{
    mType_ = INTEGER_INF_PLUS;
    iValue_ = 0;
}

template <class T>
void s_int_t<T>::SetNaN()
{
    mType_ = INTEGER_NAN;
    iValue_ = 0;
}

template <class T>
s_int_t<T> s_int_t<T>::GetSign() const
{
    if (IsValid())
    {
        return (iValue_ < 0) ? -1 : 1;
    }
    else
        return s_int_t::NaN;
}

template <class T>
s_int_t<T>& s_int_t<T>::operator ++ ()
{
    if (IsValid())
    {
        ++iValue_;
    }
    return *this;
}

template <class T>
s_int_t<T> s_int_t<T>::operator ++ (int)
{
    if (IsValid())
    {
        return iValue_++;
    }
    else
        return *this;
}

template <class T>
s_int_t<T> s_int_t<T>::operator - () const
{
    if (!IsNaN())
    {
        if (IsInfinitePlus())
            return s_int_t::INFMINUS;
        else if (IsInfiniteMinus())
            return s_int_t::INFPLUS;
        else
            return -iValue_;
    }
    else
        return s_int_t::NaN;
}

template <class T>
s_int_t<T>& s_int_t<T>::operator -- ()
{
    if (IsValid())
    {
        --iValue_;
    }
    return *this;
}

template <class T>
s_int_t<T> s_int_t<T>::operator -- (int)
{
    if (IsValid())
    {
        return iValue_--;
    }
    else
        return *this;
}

template <class T>
s_int_t<T> s_int_t<T>::operator + (const s_int_t& iValue) const
{
    if (iValue.IsNaN() || IsNaN())
        return s_int_t::NaN;

    if (!iValue.IsValid())
    {
        if (iValue.IsInfinitePlus())
        {
            if (IsInfiniteMinus())
                return s_int_t::NaN;
            else
                return s_int_t::INFPLUS;
        }
        else
        {
            if (IsInfinitePlus())
                return s_int_t::NaN;
            else
                return s_int_t::INFMINUS;
        }
    }
    else if (!this->IsValid())
    {
        if (IsInfinitePlus())
            return s_int_t::INFPLUS;
        else
            return s_int_t::INFMINUS;
    }
    else
    {
        return iValue_ + iValue.iValue_;
    }
}

template <class T>
s_int_t<T> s_int_t<T>::operator - (const s_int_t& iValue) const
{
    if (iValue.IsNaN() || IsNaN())
        return s_int_t::NaN;

    if (!iValue.IsValid())
    {
        if (iValue.IsInfinitePlus())
        {
            if (IsInfinitePlus())
                return s_int_t::NaN;
            else
                return s_int_t::INFMINUS;
        }
        else
        {
            if (IsInfiniteMinus())
                return s_int_t::NaN;
            else
                return s_int_t::INFPLUS;
        }
    }
    else if (!this->IsValid())
    {
        if (IsInfinitePlus())
            return s_int_t::INFMINUS;
        else
            return s_int_t::INFPLUS;
    }
    else
    {
        return iValue_ - iValue.iValue_;
    }
}

template <class T>
s_int_t<T> s_int_t<T>::operator * (const s_int_t& iValue) const
{
    if (iValue.IsNaN() || IsNaN())
        return s_int_t::NaN;

    if (!iValue.IsValid())
    {
        if (iValue.IsInfinitePlus())
        {
            if (IsInfiniteMinus() || IsNull())
                return s_int_t::NaN;
            else if (IsInfinitePlus())
                return s_int_t::INFPLUS;
            else
            {
                if (iValue_ > 0)
                    return s_int_t::INFPLUS;
                else
                    return s_int_t::INFMINUS;
            }
        }
        else
        {
            if (IsInfinitePlus() || IsNull())
                return s_int_t::NaN;
            else if (IsInfiniteMinus())
                return s_int_t::INFMINUS;
            else
            {
                if (iValue_ > 0)
                    return s_int_t::INFMINUS;
                else
                    return s_int_t::INFPLUS;
            }
        }
    }
    else if (!this->IsValid())
    {
        if (iValue.IsNull())
            return s_int_t::NaN;

        if (IsInfinitePlus())
        {
            if (iValue.iValue_ > 0)
                return s_int_t::INFPLUS;
            else
                return s_int_t::INFMINUS;
        }
        else
        {
            if (iValue.iValue_ > 0)
                return s_int_t::INFMINUS;
            else
                return s_int_t::INFPLUS;
        }
    }
    else
    {
        return iValue_ * iValue.iValue_;
    }
}

template <class T>
s_int_t<T> s_int_t<T>::operator / (const s_int_t& iValue) const
{
    if (iValue.IsNaN() || IsNaN() || iValue.IsNull())
        return s_int_t::NaN;

    if (!iValue.IsValid())
    {
        if (!this->IsValid())
            return s_int_t::NaN;
        else
            return 0;
    }
    else if (!this->IsValid())
    {
        if (IsInfinitePlus())
        {
            if (iValue.iValue_ > 0)
                return s_int_t::INFPLUS;
            else
                return s_int_t::INFMINUS;
        }
        else
        {
            if (iValue.iValue_ > 0)
                return s_int_t::INFMINUS;
            else
                return s_int_t::INFPLUS;
        }
    }
    else
    {
        return iValue_ / iValue.iValue_;
    }
}

template <class T>
s_int_t<T> s_int_t<T>::operator % (const s_int_t& iValue) const
{
    if (iValue.IsNaN() || IsNaN() || iValue.IsNull())
        return s_int_t::NaN;

    if (!iValue.IsValid())
    {
        if (!this->IsValid())
            return 0;
        else
            return *this;
    }
    else if (!this->IsValid())
    {
        return 0;
    }
    else
    {
        return iValue_ % iValue.iValue_;
    }
}

template <class T>
void s_int_t<T>::operator += (const s_int_t& iValue)
{
    *this = *this + iValue;
}

template <class T>
void s_int_t<T>::operator -= (const s_int_t& iValue)
{
    *this = *this - iValue;
}

template <class T>
void s_int_t<T>::operator *= (const s_int_t& iValue)
{
    *this = *this * iValue;
}

template <class T>
void s_int_t<T>::operator /= (const s_int_t& iValue)
{
    *this = *this / iValue;
}

template <class T>
void s_int_t<T>::operator %= (const s_int_t& iValue)
{
    *this = *this % iValue;
}

template <class T>
s_int_t<T>& s_int_t<T>::operator << (const uint& uiValue)
{
    if (IsValid())
    {
        iValue_ = iValue_ << uiValue;
    }
    return *this;
}

template <class T>
s_bool s_int_t<T>::operator == (const s_int_t& iValue) const
{
    if (iValue.IsNaN() || IsNaN())
        return false;
    else
    {
        if ( (mType_ == iValue.mType_) && (mType_ != INTEGER) )
            return true;

        return (iValue_ == iValue.iValue_);
    }
}

template <class T>
s_bool s_int_t<T>::operator != (const s_int_t& iValue) const
{
    if (iValue.IsNaN() || IsNaN())
        return false;
    else
    {
        if (mType_ != iValue.mType_)
            return true;

        return (iValue_ != iValue.iValue_);
    }
}

template <class T>
s_bool s_int_t<T>::operator < (const s_int_t& iValue) const
{
    if (iValue.IsNaN() || IsNaN())
        return false;
    else
    {
        if ( (mType_ == iValue.mType_) && (mType_ != INTEGER) )
            return false;

        if (mType_ == INTEGER_INF_MINUS)
            return true;
        if (mType_ == INTEGER_INF_PLUS)
            return false;

        if (iValue.mType_ == INTEGER_INF_MINUS)
            return false;
        if (iValue.mType_ == INTEGER_INF_PLUS)
            return true;

        return (iValue_ < iValue.iValue_);
    }
}

template <class T>
s_bool s_int_t<T>::operator > (const s_int_t& iValue) const
{
    if (iValue.IsNaN() || IsNaN())
        return false;
    else
    {
        if ( (mType_ == iValue.mType_) && (mType_ != INTEGER) )
            return false;

        if (mType_ == INTEGER_INF_MINUS)
            return false;
        if (mType_ == INTEGER_INF_PLUS)
            return true;

        if (iValue.mType_ == INTEGER_INF_MINUS)
            return true;
        if (iValue.mType_ == INTEGER_INF_PLUS)
            return false;

        return (iValue_ > iValue.iValue_);
    }
}

template <class T>
s_bool s_int_t<T>::operator <= (const s_int_t& iValue) const
{
    if (iValue.IsNaN() || IsNaN())
        return false;
    else
    {
        if ( (mType_ == iValue.mType_) && (mType_ != INTEGER) )
            return true;

        if (mType_ == INTEGER_INF_MINUS)
            return true;
        if (mType_ == INTEGER_INF_PLUS)
            return false;

        if (iValue.mType_ == INTEGER_INF_MINUS)
            return false;
        if (iValue.mType_ == INTEGER_INF_PLUS)
            return true;

        return (iValue_ <= iValue.iValue_);
    }
}

template <class T>
s_bool s_int_t<T>::operator >= (const s_int_t& iValue) const
{
    if (iValue.IsNaN() || IsNaN())
        return false;
    else
    {
        if ( (mType_ == iValue.mType_) && (mType_ != INTEGER) )
            return true;

        if (mType_ == INTEGER_INF_MINUS)
            return false;
        if (mType_ == INTEGER_INF_PLUS)
            return true;

        if (iValue.mType_ == INTEGER_INF_MINUS)
            return true;
        if (iValue.mType_ == INTEGER_INF_PLUS)
            return false;

        return (iValue_ >= iValue.iValue_);
    }
}

template <class T>
s_int_t<T> s_int_t<T>::Min(const s_int_t& iLeft, const s_int_t& iRight)
{
    return (iLeft >= iRight) ? iRight : iLeft;
}

template <class T>
s_int_t<T> s_int_t<T>::Max(const s_int_t& iLeft, const s_int_t& iRight)
{
    return (iLeft <= iRight) ? iRight : iLeft;
}

template <class T>
s_int_t<T> s_int_t<T>::Clamp(const s_int_t& iValue, const s_int_t& iMin, const s_int_t& iMax)
{
    return (iValue < iMax) ? ((iValue > iMin) ? iValue : iMin) : iMax;
}

template <class T>
s_int_t<T> s_int_t<T>::Random(const s_int_t& iMin, const s_int_t& iMax)
{
    if (iMin.IsValid() && iMax.IsValid())
    {
        if (iMax.iValue_ < iMin.iValue_)
            return iMin;
        else
        {
            T iRange = iMax.iValue_ - iMin.iValue_ + 1;
            return static_cast<T>(iRange*(rand()/(RAND_MAX+1.0))) + iMin.iValue_;
        }
    }
    else
        return s_int_t::NaN;
}

template <class T>
s_int_t<T> s_int_t<T>::Sign(const s_int_t& iValue) const
{
    if (iValue.IsValid())
    {
        return (iValue.iValue_ < 0) ? -1 : 1;
    }
    else
        return s_int_t::NaN;
}

template<class T>
const s_int_t<T> s_int_t<T>::NaN      = s_int_t<T>(INTEGER_NAN);
template<class T>
const s_int_t<T> s_int_t<T>::INFPLUS  = s_int_t<T>(INTEGER_INF_PLUS);
template<class T>
const s_int_t<T> s_int_t<T>::INFMINUS = s_int_t<T>(INTEGER_INF_MINUS);

template<class T>
s_int_t<T> operator + (const T& iLeft, const s_int_t<T>& iRight)
{
    return s_int_t<T>(iLeft) + iRight;
}
template<class T>
s_int_t<T> operator - (const T& iLeft, const s_int_t<T>& iRight)
{
    return s_int_t<T>(iLeft) - iRight;
}
template<class T>
s_int_t<T> operator * (const T& iLeft, const s_int_t<T>& iRight)
{
    return s_int_t<T>(iLeft) * iRight;
}
template<class T>
s_int_t<T> operator / (const T& iLeft, const s_int_t<T>& iRight)
{
    return s_int_t<T>(iLeft) / iRight;
}
template<class T>
s_int_t<T> operator % (const T& iLeft, const s_int_t<T>& iRight)
{
    return s_int_t<T>(iLeft) % iRight;
}

template<class T>
s_int_t<T> operator + (const typename TypeTraits<T>::UnsignedType& uiLeft, const s_int_t<T>& iRight)
{
    return s_int_t<T>(s_uint_t<typename TypeTraits<T>::UnsignedType>(uiLeft)) + iRight;
}
template<class T>
s_int_t<T> operator - (const typename TypeTraits<T>::UnsignedType& uiLeft, const s_int_t<T>& iRight)
{
    return s_int_t<T>(s_uint_t<typename TypeTraits<T>::UnsignedType>(uiLeft)) - iRight;
}
template<class T>
s_int_t<T> operator * (const typename TypeTraits<T>::UnsignedType& uiLeft, const s_int_t<T>& iRight)
{
    return s_int_t<T>(s_uint_t<typename TypeTraits<T>::UnsignedType>(uiLeft)) * iRight;
}
template<class T>
s_int_t<T> operator / (const typename TypeTraits<T>::UnsignedType& uiLeft, const s_int_t<T>& iRight)
{
    return s_int_t<T>(s_uint_t<typename TypeTraits<T>::UnsignedType>(uiLeft)) / iRight;
}
template<class T>
s_int_t<T> operator % (const typename TypeTraits<T>::UnsignedType& uiLeft, const s_int_t<T>& iRight)
{
    return s_int_t<T>(s_uint_t<typename TypeTraits<T>::UnsignedType>(uiLeft)) % iRight;
}

template class s_int_t<char>;
template class s_int_t<short>;
template class s_int_t<int>;
template class s_int_t<long>;
template class s_int_t<long long>;

template s_int_t<char> operator + (const char& iLeft, const s_int_t<char>& iRight);
template s_int_t<char> operator - (const char& iLeft, const s_int_t<char>& iRight);
template s_int_t<char> operator * (const char& iLeft, const s_int_t<char>& iRight);
template s_int_t<char> operator / (const char& iLeft, const s_int_t<char>& iRight);
template s_int_t<char> operator % (const char& iLeft, const s_int_t<char>& iRight);

template s_int_t<char> operator + (const uchar& iLeft, const s_int_t<char>& iRight);
template s_int_t<char> operator - (const uchar& iLeft, const s_int_t<char>& iRight);
template s_int_t<char> operator * (const uchar& iLeft, const s_int_t<char>& iRight);
template s_int_t<char> operator / (const uchar& iLeft, const s_int_t<char>& iRight);
template s_int_t<char> operator % (const uchar& iLeft, const s_int_t<char>& iRight);

template s_int_t<short> operator + (const short& iLeft, const s_int_t<short>& iRight);
template s_int_t<short> operator - (const short& iLeft, const s_int_t<short>& iRight);
template s_int_t<short> operator * (const short& iLeft, const s_int_t<short>& iRight);
template s_int_t<short> operator / (const short& iLeft, const s_int_t<short>& iRight);
template s_int_t<short> operator % (const short& iLeft, const s_int_t<short>& iRight);

template s_int_t<short> operator + (const ushort& iLeft, const s_int_t<short>& iRight);
template s_int_t<short> operator - (const ushort& iLeft, const s_int_t<short>& iRight);
template s_int_t<short> operator * (const ushort& iLeft, const s_int_t<short>& iRight);
template s_int_t<short> operator / (const ushort& iLeft, const s_int_t<short>& iRight);
template s_int_t<short> operator % (const ushort& iLeft, const s_int_t<short>& iRight);

template s_int_t<int> operator + (const int& iLeft, const s_int_t<int>& iRight);
template s_int_t<int> operator - (const int& iLeft, const s_int_t<int>& iRight);
template s_int_t<int> operator * (const int& iLeft, const s_int_t<int>& iRight);
template s_int_t<int> operator / (const int& iLeft, const s_int_t<int>& iRight);
template s_int_t<int> operator % (const int& iLeft, const s_int_t<int>& iRight);

template s_int_t<int> operator + (const uint& iLeft, const s_int_t<int>& iRight);
template s_int_t<int> operator - (const uint& iLeft, const s_int_t<int>& iRight);
template s_int_t<int> operator * (const uint& iLeft, const s_int_t<int>& iRight);
template s_int_t<int> operator / (const uint& iLeft, const s_int_t<int>& iRight);
template s_int_t<int> operator % (const uint& iLeft, const s_int_t<int>& iRight);

template s_int_t<long> operator + (const long& iLeft, const s_int_t<long>& iRight);
template s_int_t<long> operator - (const long& iLeft, const s_int_t<long>& iRight);
template s_int_t<long> operator * (const long& iLeft, const s_int_t<long>& iRight);
template s_int_t<long> operator / (const long& iLeft, const s_int_t<long>& iRight);
template s_int_t<long> operator % (const long& iLeft, const s_int_t<long>& iRight);

template s_int_t<long long> operator + (const long long& iLeft, const s_int_t<long long>& iRight);
template s_int_t<long long> operator - (const long long& iLeft, const s_int_t<long long>& iRight);
template s_int_t<long long> operator * (const long long& iLeft, const s_int_t<long long>& iRight);
template s_int_t<long long> operator / (const long long& iLeft, const s_int_t<long long>& iRight);
template s_int_t<long long> operator % (const long long& iLeft, const s_int_t<long long>& iRight);

template s_int_t<long long> operator + (const ullong& iLeft, const s_int_t<long long>& iRight);
template s_int_t<long long> operator - (const ullong& iLeft, const s_int_t<long long>& iRight);
template s_int_t<long long> operator * (const ullong& iLeft, const s_int_t<long long>& iRight);
template s_int_t<long long> operator / (const ullong& iLeft, const s_int_t<long long>& iRight);
template s_int_t<long long> operator % (const ullong& iLeft, const s_int_t<long long>& iRight);

}
