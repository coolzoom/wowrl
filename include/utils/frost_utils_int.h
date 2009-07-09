// Warning : If you need to use this file, include frost_utils_types.h
namespace Frost
{
    /// Base type : integer
    /** Frost's base types are made to allow simpler
    *   manipulation of numbers, booleans and strings.
    *   They simplify conversions (mostly to string),
    *   allow easy access to math/string functions and
    *   implements new functions that are not present
    *   in the standard libraries.<br>
    *   <b>Note</b> : Most of Frost's base types are
    *   slower than C++ base types. The difference is
    *   negligible for small operations, but if you need
    *   to do a lot of calculations (matrices, ...) it
    *   can bloat your performances. Use prebuilt types
    *   in this case.
    *   <b>Note</b> : This class is about 60 times slower
    *   than the wrapped POD (20 times in debug, without
    *   any compiler optimizations).
    */
    template<class T>
    class s_int_t
    {
    public :

        enum IntegerType
        {
            INTEGER,
            INTEGER_INF_PLUS,
            INTEGER_INF_MINUS,
            INTEGER_NAN
        };

        s_int_t()
        {
            mType_ = INTEGER;
            iValue_ = 0;
        }

        s_int_t(const T& iValue)
        {
            mType_ = INTEGER;
            iValue_ = iValue;
        }

        explicit s_int_t(const IntegerType& mType)
        {
            mType_ = mType;
            iValue_ = 0;
        }

        explicit s_int_t(const float& fValue)
        {
            mType_ = INTEGER;
            iValue_ = static_cast<T>(round(fValue));
        }

        explicit s_int_t(const double& dValue)
        {
            mType_ = INTEGER;
            iValue_ = static_cast<T>(round(dValue));
        }

        template<class N>
        explicit s_int_t(const s_int_t<N>& iValue)
        {
            mType_ = static_cast<IntegerType>(iValue.GetType());
            iValue_ = static_cast<T>(iValue.Get());
        }

        template<class N>
        explicit s_int_t(const s_uint_t<N>& uiValue)
        {
            if (uiValue.GetType() == s_uint_t<N>::INTEGER_NAN)
            {
                mType_ = INTEGER_NAN;
                iValue_ = 0;
            }
            else if (uiValue.GetType() == s_uint_t<N>::INTEGER_INF)
            {
                mType_ = INTEGER_INF_PLUS;
                iValue_ = 0;
            }
            else
            {
                mType_ = INTEGER;
                iValue_ = static_cast<T>(uiValue.Get());
            }
        }

        template<class N>
        explicit s_int_t(const s_float_t<N>& fValue)
        {
            if (fValue.GetType() == s_float_t<N>::FLOAT_NAN)
            {
                mType_ = INTEGER_NAN;
                iValue_ = 0;
            }
            else if (fValue.GetType() == s_float_t<N>::FLOAT_INF_PLUS)
            {
                mType_ = INTEGER_INF_PLUS;
                iValue_ = 0;
            }
            else if (fValue.GetType() == s_float_t<N>::FLOAT_INF_MINUS)
            {
                mType_ = INTEGER_INF_MINUS;
                iValue_ = 0;
            }
            else
            {
                mType_ = INTEGER;
                iValue_ = static_cast<T>(round(fValue.Get()));
            }
        }

        template<class N>
        explicit s_int_t(const s_bool_t<N>& bValue)
        {
            mType_ = INTEGER;
            if (bValue)
                iValue_ = 1;
            else
                iValue_ = 0;
        }

        explicit s_int_t(const string_element* sValue)
        {
            mType_ = INTEGER;
            string_stream s(sValue);
            s >> iValue_;
        }

        explicit s_int_t(const string_object& sValue)
        {
            mType_ = INTEGER;
            string_stream s(sValue);
            s >> iValue_;
        }

        template<class N>
        explicit s_int_t(const s_str_t<N>& sValue)
        {
            mType_ = INTEGER;
            string_stream s(sValue.Get());
            s >> iValue_;
        }

        /// Returns a const reference to the int.
        /** \return A const reference to the int
        */
        inline const T& Get() const
        {
            if (mType_ != INTEGER)
                return iDummy;
            else
                return iValue_;
        }

        /// Returns a reference to the int.
        /** \return A reference to the int
        */
        inline T& GetR()
        {
            if (mType_ != INTEGER)
                return iDummy;
            else
                return iValue_;
        }

        /// Returns the power of two just above the actual value (or equal).
        /** \return The associated power of two (2^n) (superior of equal)
        */

        s_uint_t<T> GetNearestPowerOfTwo() const
        {
            if (IsValid())
            {
                int i = 1;
                while (iValue_ > i)
                    i = i << 1;

                return static_cast<uint>(i);
            }
            else
                return s_uint_t<T>::NaN;
        }

        /// Returns the type of this int.
        /** \return The type of this int (infinite, NaN, ...)
        */
        IntegerType GetType() const
        {
            return mType_;
        }

        /// Checks if this int is infinite and negative
        /** \return 'true' if this int is infinite and negative
        */
        s_bool IsInfiniteMinus() const
        {
            return (mType_ == INTEGER_INF_MINUS);
        }

        /// Checks if this int is infinite and positive
        /** \return 'true' if this int is infinite and positive
        */
        s_bool IsInfinitePlus() const
        {
            return (mType_ == INTEGER_INF_PLUS);
        }

        /// Checks if this int is a Not a Number (NaN)
        /** \return 'true' if this int is NaN
        */
        s_bool IsNaN() const
        {
            return (mType_ == INTEGER_NAN);
        }

        /// Checks if this int equals zero.
        /** \return 'true' if this int equals zero
        */
        s_bool IsNull() const
        {
            return ( IsValid() && (iValue_ == 0) );
        }

        /// Checks if this int is a valid number.
        /** \return 'true' if this int is not infinite and a number
        */
        s_bool IsValid() const
        {
            return (mType_ == INTEGER);
        }

        /// Elevates this int to a certain power (this^n).
        /** \param uiPower The power
        */
        template<class N>
        void Pow(const s_uint_t<N>& uiPower)
        {
            if (IsValid() && uiPower.IsValid())
            {
                iValue_ = static_cast<T>(pow(
                    static_cast<float>(iValue_), static_cast<int>(uiPower.Get())
                ));
            }
        }

        /// Sets the value of the int to a random number.
        /** \param iMin The lower bound (minimum)
        *   \param iMax The upper bound (maximum)
        */
        void Randomize(const s_int_t& iMin = 0, const s_int_t& iMax = 1)
        {
            if (iMin.IsValid() && iMax.IsValid())
            {
                mType_ = INTEGER;
                if (iMax.iValue_ < iMin.iValue_)
                    iValue_ = iMin.iValue_;
                else
                {
                    T iRange = iMax.iValue_ - iMin.iValue_ + 1;
                    if (iRange >= 100)
                        iValue_ = static_cast<T>(iRange*(rand()/(RAND_MAX+1.0))) + iMin.iValue_;
                    else if (iRange >= 10)
                    {
                        float fTemp = rand()/100.0f;
                        fTemp = (fTemp - static_cast<int>(fTemp))*100;
                        if (static_cast<int>(fTemp) == 0)
                            iValue_ = iMin.iValue_;
                        else
                            iValue_ = static_cast<T>(ceil(iRange*(fTemp/99.0f))) + iMin.iValue_ - 1;
                    }
                    else
                    {
                        float fTemp = rand()/10.0f;
                        fTemp = (fTemp - static_cast<int>(fTemp))*10;
                        if (static_cast<int>(fTemp) == 0)
                            iValue_ = iMin.iValue_;
                        else
                            iValue_ = static_cast<T>(ceil(iRange*(fTemp/9.0f))) + iMin.iValue_ - 1;
                    }
                }
            }
        }

        /// Sets this int to infinite (negative).
        void SetInfiniteMinus()
        {
            mType_ = INTEGER_INF_MINUS;
            iValue_ = 0;
        }

        /// Sets this int to infinite (positive).
        void SetInfinitePlus()
        {
            mType_ = INTEGER_INF_PLUS;
            iValue_ = 0;
        }

        /// Set this int to Not a Number state.
        void SetNaN()
        {
            mType_ = INTEGER_NAN;
            iValue_ = 0;
        }

        /// Returns the sign of this int (either 1 or -1).
        /** \return The sign of this int (either 1 or -1)
        */
        s_int_t Sign()
        {
            if (IsValid())
            {
                if (iValue_ == 0)
                    return 1;
                else
                    return (iValue_*fabs(iValue_) < 0) ? -1 : 1;
            }
            else
                return s_int_t::NaN;
        }

        s_int_t& operator ++ ()
        {
            if (IsValid())
            {
                ++iValue_;
            }
            return *this;
        }

        s_int_t operator ++ (int)
        {
            if (IsValid())
            {
                return iValue_++;
            }
            else
                return *this;
        }

        s_int_t operator - () const
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

        s_int_t& operator -- ()
        {
            if (IsValid())
            {
                --iValue_;
            }
            return *this;
        }

        s_int_t operator -- (int)
        {
            if (IsValid())
            {
                return iValue_--;
            }
            else
                return *this;
        }

        s_int_t operator + (const s_int_t& iValue) const
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

        s_int_t operator - (const s_int_t& iValue) const
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

        s_int_t operator * (const s_int_t& iValue) const
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

        s_int_t operator / (const s_int_t& iValue) const
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

        s_int_t operator % (const s_int_t& iValue) const
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

        void operator += (const s_int_t& iValue)
        {
            *this = *this + iValue;
        }

        void operator -= (const s_int_t& iValue)
        {
            *this = *this - iValue;
        }

        void operator *= (const s_int_t& iValue)
        {
            *this = *this * iValue;
        }

        void operator /= (const s_int_t& iValue)
        {
            *this = *this / iValue;
        }

        void operator %= (const s_int_t& iValue)
        {
            *this = *this % iValue;
        }

        template<class N>
        s_str_t<N> operator + (const N* sValue) const
        {
            return s_str_t<N>(*this) + sValue;
        }

        template<class N>
        s_str_t<N> operator + (const s_str_t<N>& sValue) const
        {
            return s_str_t<N>(*this) + sValue;
        }

        template<class N>
        s_int_t& operator << (const s_uint_t<N>& uiValue)
        {
            if (IsValid())
            {
                iValue_ = iValue_ << uiValue.Get();
            }
            return *this;
        }

        s_int_t& operator << (const uint& uiValue)
        {
            if (IsValid())
            {
                iValue_ = iValue_ << uiValue;
            }
            return *this;
        }

        s_bool operator == (const s_int_t& iValue) const
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

        s_bool operator != (const s_int_t& iValue) const
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

        s_bool operator < (const s_int_t& iValue) const
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

        s_bool operator > (const s_int_t& iValue) const
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

        s_bool operator <= (const s_int_t& iValue) const
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

        s_bool operator >= (const s_int_t& iValue) const
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

        s_ctnr<s_int_t> operator ,  (const s_int_t& iValue) const
        {
            s_ctnr<s_int_t> mContainer;
            mContainer.PushBack(*this);
            mContainer.PushBack(iValue);
            return mContainer;
        }

        static const s_int_t NaN;
        static const s_int_t INFPLUS;
        static const s_int_t INFMINUS;
        static       T       iDummy;

        /// Returns the lowest value of the two provided ones.
        /** \param iLeft  The first value
        *   \param iRight The second value
        *   \return The lowest value of the two provided ones
        */
        static s_int_t Min(const s_int_t& iLeft, const s_int_t& iRight)
        {
            return (iLeft >= iRight) ? iRight : iLeft;
        }

        /// Returns the highest value of the two provided ones.
        /** \param iLeft  The first value
        *   \param iRight The second value
        *   \return The highest value of the two provided ones
        */
        static s_int_t Max(const s_int_t& iLeft, const s_int_t& iRight)
        {
            return (iLeft <= iRight) ? iRight : iLeft;
        }

        /// Returns a random int in the provided range.
        /** \param iMin The lower bound (minimum)
        *   \param iMax The upper bound (maximum)
        *   \return A random int in the provided range
        */
        static s_int_t Random(const s_int_t& iMin = 0, const s_int_t& iMax = 1)
        {
            if (iMin.IsValid() && iMax.IsValid())
            {
                if (iMax.iValue_ < iMin.iValue_)
                {
                    return iMin;
                }
                else
                {
                    T iRange = iMax.iValue_ - iMin.iValue_ + 1;
                    if (iRange >= 100)
                        return static_cast<T>(iRange*(rand()/(RAND_MAX+1.0))) + iMin.iValue_;
                    else if (iRange >= 10)
                    {
                        float fTemp = rand()/100.0f;
                        fTemp = (fTemp - static_cast<int>(fTemp))*100;
                        if (static_cast<int>(fTemp) == 0)
                            return iMin.iValue_;
                        else
                            return static_cast<T>(ceil(iRange*(fTemp/99.0f))) + iMin.iValue_ - 1;
                    }
                    else
                    {
                        float fTemp = rand()/10.0f;
                        fTemp = (fTemp - static_cast<int>(fTemp))*10;
                        if (static_cast<int>(fTemp) == 0)
                            return iMin.iValue_;
                        else
                            return static_cast<T>(ceil(iRange*(fTemp/9.0f))) + iMin.iValue_ - 1;
                    }
                }
            }
            else
                return s_int_t::NaN;
        }

    private :

        T iValue_;

        IntegerType mType_;
    };

    template<class T>
    const s_int_t<T> s_int_t<T>::NaN      = s_int_t<T>(INTEGER_NAN);
    template<class T>
    const s_int_t<T> s_int_t<T>::INFPLUS  = s_int_t<T>(INTEGER_INF_PLUS);
    template<class T>
    const s_int_t<T> s_int_t<T>::INFMINUS = s_int_t<T>(INTEGER_INF_MINUS);
    template<class T>
    T s_int_t<T>::iDummy = 0;

    template<class T>
    s_int_t<T> operator + (const int& iLeft, const s_int_t<T>& iRight)
    {
        return s_int_t<T>(iLeft) + iRight;
    }
    template<class T>
    s_int_t<T> operator - (const int& iLeft, const s_int_t<T>& iRight)
    {
        return s_int_t<T>(iLeft) - iRight;
    }
    template<class T>
    s_int_t<T> operator * (const int& iLeft, const s_int_t<T>& iRight)
    {
        return s_int_t<T>(iLeft) * iRight;
    }
    template<class T>
    s_int_t<T> operator / (const int& iLeft, const s_int_t<T>& iRight)
    {
        return s_int_t<T>(iLeft) / iRight;
    }
    template<class T>
    s_int_t<T> operator % (const int& iLeft, const s_int_t<T>& iRight)
    {
        return s_int_t<T>(iLeft) % iRight;
    }

    template<class T, class N>
    s_str_t<N> operator + ( const N* sLeft, const s_int_t<T>& iRight )
    {
        return s_str_t<N>(sLeft) << iRight;
    }

    typedef s_int_t<char>      s_char;
    typedef s_int_t<short>     s_short;
    typedef s_int_t<int>       s_int;
    typedef s_int_t<long>      s_long;
    typedef s_int_t<ptrdiff_t> s_ptrdiff;

    template<> class TypeTraits<char>      { public : typedef s_char    Type; };
    template<> class TypeTraits<short>     { public : typedef s_short   Type; };
    template<> class TypeTraits<int>       { public : typedef s_int     Type; };
    template<> class TypeTraits<long>      { public : typedef s_long    Type; };
}
