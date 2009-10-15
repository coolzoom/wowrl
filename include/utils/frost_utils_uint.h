// Warning : If you need to use this file, include frost_utils_types.h
namespace Frost
{
    /// Base type : unsigned integer
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
    class s_uint_t
    {
    public :

        enum Type
        {
            INTEGER,
            INTEGER_INF,
            INTEGER_NAN
        };

        s_uint_t()
        {
            mType_ = INTEGER;
            uiValue_ = 0u;
        }

        s_uint_t(const T& uiValue)
        {
            mType_ = INTEGER;
            uiValue_ = uiValue;
        }

        explicit s_uint_t(const Type& mType)
        {
            mType_ = mType;
            uiValue_ = 0u;
        }

        explicit s_uint_t(const float& fValue)
        {
            mType_ = INTEGER;
            uiValue_ = static_cast<T>(round(fValue));
        }

        explicit s_uint_t(const double& dValue)
        {
            mType_ = INTEGER;
            uiValue_ = static_cast<T>(round(dValue));
        }

        template<class N>
        explicit s_uint_t(const s_uint_t<N>& uiValue)
        {
            mType_ = static_cast<Type>(uiValue.GetType());
            uiValue_ = static_cast<T>(uiValue.Get());
        }

        template<class N>
        explicit s_uint_t(const s_int_t<N>& iValue)
        {
            if (iValue.GetType() == s_int_t<N>::INTEGER_NAN)
            {
                mType_ = INTEGER_NAN;
                uiValue_ = 0u;
            }
            else if ( (iValue.GetType() == s_int_t<N>::INTEGER) && (iValue >= 0u) )
            {
                mType_ = INTEGER;
                uiValue_ = static_cast<uint>(iValue.Get());
            }
            else
            {
                mType_ = INTEGER_INF;
                uiValue_ = 0u;
            }
        }

        template<class N>
        explicit s_uint_t(const s_float_t<N>& fValue)
        {
            if (fValue.GetType() == s_float_t<N>::FLOAT_NAN)
            {
                mType_ = INTEGER_NAN;
                uiValue_ = 0u;
            }
            else if ( (fValue.GetType() == s_float_t<N>::FLOAT) && (fValue >= 0.0f) )
            {
                mType_ = INTEGER;
                uiValue_ = static_cast<T>(round(fValue.Get()));
            }
            else
            {
                mType_ = INTEGER_INF;
                uiValue_ = 0u;
            }
        }

        template<class N>
        explicit s_uint_t(const s_bool_t<N>& bValue)
        {
            mType_ = INTEGER;
            if (bValue)
                uiValue_ = 1;
            else
                uiValue_ = 0;
        }

        explicit s_uint_t(const string_element* sValue)
        {
            T i;
            string_stream s(sValue);
            s >> i;
            if (i < 0)
            {
                mType_ = INTEGER_INF;
                uiValue_ = 0u;
            }
            else
            {
                mType_ = INTEGER;
                uiValue_ = static_cast<T>(i);
            }
        }

        explicit s_uint_t(const string_object& sValue)
        {
            T i;
            string_stream s(sValue);
            s >> i;
            if (i < 0)
            {
                mType_ = INTEGER_INF;
                uiValue_ = 0u;
            }
            else
            {
                mType_ = INTEGER;
                uiValue_ = static_cast<T>(i);
            }
        }

        template<class N>
        explicit s_uint_t(const s_str_t<N>& sValue)
        {
            T i;
            string_stream s(sValue.Get());
            s >> i;
            if (i < 0)
            {
                mType_ = INTEGER_INF;
                uiValue_ = 0u;
            }
            else
            {
                mType_ = INTEGER;
                uiValue_ = static_cast<T>(i);
            }
        }

        /// Returns a const reference to the uint.
        /** \return A const reference to the uint
        */
        inline const T& Get() const
        {
            if (mType_ != INTEGER)
                return uiDummy;
            else
                return uiValue_;
        }

        /// Returns a reference to the uint.
        /** \return A reference to the uint
        */
        inline T& GetR()
        {
            if (mType_ != INTEGER)
                return uiDummy;
            else
                return uiValue_;
        }

        /// Returns the power of two just above the actual value (or equal).
        /** \return The associated power of two (2^n) (superior of equal)
        */
        s_uint_t GetNearestPowerOfTwo() const
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

        /// Returns the type of this uint.
        /** \return The type of this uint (infinite, NaN, ...)
        */
        Type GetType() const
        {
            return mType_;
        }

        /// Checks if this uint is infinite
        /** \return 'true' if this uint is infinite
        */
        s_bool IsInfinite() const
        {
            return (mType_ == INTEGER_INF);
        }

        /// Checks if this uint is contained into the provided range.
        s_bool IsInRange(const s_uint_t<T>& uiMin, const s_uint_t<T>& uiMax) const
        {
            return ( (uiMin <= (*this)) && ((*this) <= uiMax) );
        }

        /// Checks if this uint is a Not a Number (NaN)
        /** \return 'true' if this uint is NaN
        */
        s_bool IsNaN() const
        {
            return (mType_ == INTEGER_NAN);
        }

        /// Checks if this uint equals zero.
        /** \return 'true' if this uint equals zero
        */
        s_bool IsNull() const
        {
            return ( IsValid() && (uiValue_ == 0u) );
        }

        /// Checks if this uint is a valid number.
        /** \return 'true' if this uint is not infinite and a number
        */
        s_bool IsValid() const
        {
            return (mType_ == INTEGER);
        }

        /// Elevates this uint to a certain power (this^n).
        /** \param uiPower The power...
        */
        void Pow(const s_uint_t& uiPower)
        {
            if (IsValid() && uiPower.IsValid())
            {
                uiValue_ = static_cast<uint>(pow(
                    static_cast<float>(uiValue_),
                    static_cast<int>(uiPower.Get())
                ));
            }
        }

        /// Sets the value of the uint to a random number.
        /** \param uiMin The lower bound (minimum)
        *   \param uiMax The upper bound (maximum)
        */
        void Randomize(const s_uint_t& uiMin = 0u, const s_uint_t& uiMax = 1u)
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

        /// Sets this uint to infinite.
        void SetInfinite()
        {
            mType_ = INTEGER_INF;
            uiValue_ = 0u;
        }

        /// Set this uint to Not a Number state.
        void SetNaN()
        {
            mType_ = INTEGER_NAN;
            uiValue_ = 0u;
        }

        s_uint_t& operator ++ ()
        {
            if (IsValid())
            {
                ++uiValue_;
            }
            return *this;
        }

        s_uint_t operator ++ (int)
        {
            if (IsValid())
            {
                return uiValue_++;
            }
            else
                return *this;
        }

        s_int_t<int> operator - () const
        {
            if (!IsNaN())
            {
                if (IsInfinite())
                    return s_int_t<int>::INFMINUS;
                else
                    return -static_cast<int>(uiValue_);
            }
            else
                return s_int_t<int>::NaN;
        }

        s_uint_t& operator -- ()
        {
            if (IsValid())
            {
                --uiValue_;
            }
            return *this;
        }

        s_uint_t operator -- (int)
        {
            if (IsValid())
            {
                return uiValue_--;
            }
            else
                return *this;
        }

        s_uint_t operator + (const s_uint_t& uiValue) const
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

        s_uint_t operator - (const s_uint_t& uiValue) const
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

        s_uint_t operator * (const s_uint_t& uiValue) const
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

        s_uint_t operator / (const s_uint_t& uiValue) const
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

        s_uint_t operator % (const s_uint_t& uiValue) const
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

        void operator += (const s_uint_t& uiValue)
        {
            *this = *this + uiValue;
        }

        void operator -= (const s_uint_t& uiValue)
        {
            *this = *this - uiValue;
        }

        void operator *= (const s_uint_t& uiValue)
        {
            *this = *this * uiValue;
        }

        void operator /= (const s_uint_t& uiValue)
        {
            *this = *this / uiValue;
        }
        void operator %= (const s_uint_t& uiValue)
        {
            *this = *this % uiValue;
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

        s_uint_t& operator << (const s_uint_t& uiValue)
        {
            if (IsValid())
            {
                uiValue_ = uiValue_ << uiValue.uiValue_;
            }
            return *this;
        }

        s_bool operator == (const s_uint_t& uiValue) const
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

        s_bool operator != (const s_uint_t& uiValue) const
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

        s_bool operator < (const s_uint_t& uiValue) const
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
        s_bool operator > (const s_uint_t& uiValue) const
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

        s_bool operator <= (const s_uint_t& uiValue) const
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

        s_bool operator >= (const s_uint_t& uiValue) const
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

        s_ctnr<s_uint_t> operator , (const s_uint_t& uiValue) const
        {
            s_ctnr<s_uint_t> mContainer;
            mContainer.PushBack(*this);
            mContainer.PushBack(uiValue);
            return mContainer;
        }

        static const s_uint_t NaN;
        static const s_uint_t INF;
        static       T        uiDummy;

        /// Returns the lowest value of the two provided ones.
        /** \param uiLeft  The first value
        *   \param uiRight The second value
        *   \return The lowest value of the two provided ones
        */
        static s_uint_t Min(const s_uint_t& uiLeft, const s_uint_t& uiRight)
        {
            return (uiLeft >= uiRight) ? uiRight : uiLeft;
        }

        /// Returns the highest value of the two provided ones.
        /** \param uiLeft  The first value
        *   \param uiRight The second value
        *   \return The highest value of the two provided ones
        */
        static s_uint_t Max(const s_uint_t& uiLeft, const s_uint_t& uiRight)
        {
            return (uiLeft <= uiRight) ? uiRight : uiLeft;
        }

        /// Returns a random uint in the provided range.
        /** \param uiMin The lower bound (minimum)
        *   \param uiMax The upper bound (maximum)
        *   \return A random uint in the provided range
        */
        static s_uint_t Random(const s_uint_t& uiMin = 0u, const s_uint_t& uiMax = 1u)
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

    private :

        T uiValue_;

        Type mType_;
    };

    template<class T>
    const s_uint_t<T> s_uint_t<T>::NaN = s_uint_t<T>(INTEGER_NAN);
    template<class T>
    const s_uint_t<T> s_uint_t<T>::INF = s_uint_t<T>(INTEGER_INF);
    template<class T>
    T s_uint_t<T>::uiDummy = 0u;

    template<class T>
    s_uint_t<T> operator + (const uint& uiLeft, const s_uint_t<T>& uiRight)
    {
        return s_uint_t<T>(uiLeft) + uiRight;
    }

    template<class T>
    s_uint_t<T> operator - (const uint& uiLeft, const s_uint_t<T>& uiRight)
    {
        return s_uint_t<T>(uiLeft) - uiRight;
    }

    template<class T>
    s_uint_t<T> operator * (const uint& uiLeft, const s_uint_t<T>& uiRight)
    {
        return s_uint_t<T>(uiLeft) * uiRight;
    }

    template<class T>
    s_uint_t<T> operator / (const uint& uiLeft, const s_uint_t<T>& uiRight)
    {
        return s_uint_t<T>(uiLeft) / uiRight;
    }

    template<class T>
    s_uint_t<T> operator % (const uint& uiLeft, const s_uint_t<T>& uiRight)
    {
        return s_uint_t<T>(uiLeft) % uiRight;
    }

    template<class T, class N>
    s_str_t<N> operator + ( const N* sLeft, const s_uint_t<T>& uiRight )
    {
        return s_str_t<N>(sLeft) << uiRight;
    }

    typedef s_uint_t<uchar>              s_uchar;
    typedef s_uint_t<ushort>             s_ushort;
    typedef s_uint_t<uint>               s_uint;
    typedef s_uint_t<ulong>              s_ulong;
    typedef s_uint_t<unsigned long long> s_ullong;

    /** \cond NOT_REMOVE_FROM_DOC
    */
    template<> class TypeTraits<uchar>              { public : typedef s_uchar  Type; };
    template<> class TypeTraits<ushort>             { public : typedef s_ushort Type; };
    template<> class TypeTraits<uint>               { public : typedef s_uint   Type; };
    template<> class TypeTraits<ulong>              { public : typedef s_ulong  Type; };
    template<> class TypeTraits<unsigned long long> { public : typedef s_ullong Type; };
    /** \endcond
    */
}
