// Warning : If you need to use this file, include frost_utils_types.h
#ifdef MSVC
    #include <cfloat>
#endif
#include <limits>

namespace Frost
{
    /// Base type : float
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
    *   <b>Note</b> : Because the CPU can handle infinite
    *   and nan itself for float types, this class is
    *   more efficient than the others (as fast as the
    *   wrapped POD with compiler optimizations, 5 times
    *   slower in debug mode).
    */
    template <class T>
    class s_float_t
    {
    public :

        enum Type
        {
            FLOAT,
            FLOAT_INF_PLUS,
            FLOAT_INF_MINUS,
            FLOAT_NAN
        };

        s_float_t()
        {
            fValue_ = 0.0;
        }

        s_float_t(const T& fValue)
        {
            fValue_ = fValue;
        }

        explicit s_float_t(const Type& mType)
        {
            switch (mType)
            {
                case FLOAT_INF_PLUS  : SetInfinitePlus();
                case FLOAT_INF_MINUS : SetInfiniteMinus();
                case FLOAT_NAN       : SetNaN();
                default : fValue_ = 0.0;
            }
        }

        template <class N>
        explicit s_float_t(const s_float_t<N>& fValue)
        {
            fValue_ = static_cast<T>(fValue.Get());
        }

        template<class N>
        explicit s_float_t(const s_int_t<N>& iValue)
        {
            if (iValue.GetType() == s_int_t<N>::INTEGER_NAN)
            {
                SetNaN();
            }
            else if (iValue.GetType() == s_int_t<N>::INTEGER_INF_PLUS)
            {
                SetInfinitePlus();
            }
            else if (iValue.GetType() == s_int_t<N>::INTEGER_INF_MINUS)
            {
                SetInfiniteMinus();
            }
            else
            {
                fValue_ = static_cast<T>(iValue.Get());
            }
        }

        template<class N>
        explicit s_float_t(const s_uint_t<N>& uiValue)
        {
            if (uiValue.GetType() == s_uint_t<N>::INTEGER_NAN)
            {
                SetNaN();
            }
            else if (uiValue.GetType() == s_uint_t<N>::INTEGER_INF)
            {
                SetInfinitePlus();
            }
            else
            {
                fValue_ = static_cast<T>(uiValue.Get());
            }
        }

        template<class N>
        explicit s_float_t(const s_bool_t<N>& bValue)
        {
            if (bValue)
                fValue_ = 1.0;
            else
                fValue_ = 0.0;
        }

        explicit s_float_t(const string_element* sValue)
        {
            string_stream s(sValue);
            s >> fValue_;
        }

        explicit s_float_t(const string_object& sValue)
        {
            string_stream s(sValue);
            s >> fValue_;
        }

        template<class N>
        explicit s_float_t(const s_str_t<N>& sValue)
        {
            string_stream s(sValue.Get());
            s >> fValue_;
        }

        /// Adjusts this float's value to be contained into the provided interval.
        /** \param fMin The minimum value
        *   \param fMax The maximum value
        */
        void Clamp(const s_float_t& fMin, const s_float_t& fMax)
        {
            fValue_ = (fValue_ < fMax.fValue_) ? ((fValue_ > fMin.fValue_) ? fValue_ : fMin.fValue_) : fMax.fValue_;
        }

        /// Converts this float to a angle in radian.
        /** \note It is assumed that the previous value was an angle in degree.
        */
        void DegToRad()
        {
            if (IsValid())
            {
                fValue_ *= PI.fValue_/180.0;
            }
        }

        /// Converts this float to an angle in radian.
        /** \note It is assumed that the previous value was a non dimensionnal angle
        *         (1 = 2*PI rad = 360°).
        */
        s_float_t GetRad() const
        {
            if (IsValid())
            {
                return fValue_*2*PI.fValue_;
            }
            else
                return 0.0;
        }

        /// Converts this float to an angle in degree.
        /** \note It is assumed that the previous value was a non dimensionnal angle
        *         (1 = 2*PI rad = 360°).
        */
        s_float_t GetDeg() const
        {
            if (IsValid())
            {
                return fValue_*360.0;
            }
            else
                return 0.0;
        }

        /// Returns a const reference to the float.
        /** \return A const reference to the float
        */
        inline const T& Get() const
        {
            return fValue_;
        }

        /// Returns a reference to the float.
        /** \return A reference to the float
        */
        inline T& GetR()
        {
            return fValue_;
        }

        /// Returns the power of two just above the value (or equal).
        /** \return The associated power of two (2^n) (superior of equal)
        */
        s_uint_t<uint> GetNearestPowerOfTwo() const
        {
            if (IsValid())
            {
                uint i = 1;
                while (fValue_ > i)
                    i = i << 1;

                return i;
            }
            else
                return s_uint_t<uint>::NaN;
        }

        /// Returns the type of this float.
        /** \return The type of this float (infinite, NaN, ...)
        */
        Type GetType() const
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

        /// Checks if this float is infinite and negative
        /** \return 'true' if this float is infinite and negative
        */
        s_bool IsInfiniteMinus() const
        {
            return (!IsValid() && (fValue_ < 0.0));
        }

        /// Checks if this float is infinite and positive
        /** \return 'true' if this float is infinite and positive
        */
        s_bool IsInfinitePlus() const
        {
            return (!IsValid() && (fValue_ > 0.0));
        }

        /// Checks if this float is contained into the provided range.
        s_bool IsInRange(const s_float_t<T>& fMin, const s_float_t<T>& fMax) const
        {
            return ( (fMin <= (*this)) && ((*this) <= fMax) );
        }

        /// Checks if this float is a Not a Number (NaN)
        /** \return 'true' if this float is NaN
        */
        s_bool IsNaN() const
        {
            #ifdef MSVC
                return _isnan(fValue_) != 0;
            #else
                return std::isnan(fValue_);
            #endif
        }

        /// Checks if this float equals zero.
        /** \return 'true' if this float equals zero (precision : EPS)
        */
        s_bool IsNull() const
        {
            return ( IsValid() && (fValue_ < EPS) && (fValue_ > -EPS) );
        }

        /// Checks if this float is a valid number.
        /** \return 'true' if this float is not infinite and a number
        */
        s_bool IsValid() const
        {
            #ifdef MSVC
                return _finite(fValue_) != 0;
            #else
                return std::isfinite(fValue_);
            #endif
        }

        /// Elevates this float to a certain power (this^n).
        /** \param fPower The power
        */
        void Pow(const s_float_t& fPower)
        {
            if (IsValid() && fPower.IsValid())
            {
                fValue_ = pow(fValue_, fPower.fValue_);
            }
        }

        /// Converts this float to an angle in degree.
        /** \param bNegativeAllowed 'true' if you allow negative return values
        *   \note It is assumed that the previous value was an angle in radian.
        */
        template<class N>
        void RadToDeg(const s_bool_t<N>& bNegativeAllowed = true)
        {
            if (IsValid())
            {
                fValue_ *= 180.0/PI.fValue_;
                if ( (!bNegativeAllowed) && (fValue_ < 0.0) )
                    fValue_ += 360.0;
            }
        }

        /// Sets the value of the float to a random number.
        /** \param fMin The lower bound (minimum)
        *   \param fMax The upper bound (maximum)
        */
        void Randomize(const s_float_t& fMin = 0.0, const s_float_t& fMax = 1.0)
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

        /// Rounds the value.
        /** \note If the decimal part is greater than 0.5, the number is rounded up.
        *         Else the number is rounded down.
        */
        void Round()
        {
            fValue_ = round(fValue_);
        }

        /// Rounds up the value.
        void RoundUp()
        {
            fValue_ = ceil(fValue_);
        }

        /// Rounds down the value.
        void RoundDown()
        {
            fValue_ = floor(fValue_);
        }

        /// Sets this float to infinite (negative).
        void SetInfiniteMinus()
        {
            fValue_ = INFMINUS.fValue_;
        }

        /// Sets this float to infinite (positive).
        void SetInfinitePlus()
        {
            fValue_ = INFPLUS.fValue_;
        }

        /// Set this float to Not a Number state.
        void SetNaN()
        {
            fValue_ = NaN.fValue_;
        }

        /// Returns the sign of this float.
        /** \return The sign of this float
        */
        s_float_t GetSign() const
        {
            if (IsValid())
            {
                return (fValue_ < 0.0) ? -1 : 1;
            }
            else
                return s_float_t::NaN;
        }

        /// Converts this float to an angle in degree.
        /** \param bNegativeAllowed 'true' if you allow negative return values
        *   \note It is assumed that the previous value was a non dimensionnal angle
        *         (1 = 2*PI rad = 360°).
        */
        template<class N>
        void ToDeg(const s_bool_t<N>& bNegativeAllowed = true)
        {
            if (IsValid())
            {
                fValue_ *= 360.0;
                if ( (!bNegativeAllowed) && (fValue_ < 0.0) )
                    fValue_ += 360.0;
            }
        }

        /// Converts this float to an angle in radian.
        /** \note It is assumed that the previous value was a non dimensionnal angle
        *         (1 = 2*PI rad = 360°).
        */
        void ToRad()
        {
            if (IsValid())
            {
                fValue_ *= 2*PI.fValue_;
            }
        }

        s_float_t operator - () const
        {
            return -fValue_;
        }

        s_float_t operator + (const s_float_t& fValue) const
        {
            return fValue_ + fValue.fValue_;
        }

        s_float_t operator - (const s_float_t& fValue) const
        {
            return fValue_ - fValue.fValue_;
        }

        s_float_t operator * (const s_float_t& fValue) const
        {
            return fValue_ * fValue.fValue_;
        }

        s_float_t operator / (const s_float_t& fValue) const
        {
            return fValue_ / fValue.fValue_;
        }

        void operator += (const s_float_t& fValue)
        {
            fValue_ += fValue.fValue_;
        }

        void operator -= (const s_float_t& fValue)
        {
            fValue_ -= fValue.fValue_;
        }

        void operator *= (const s_float_t& fValue)
        {
            fValue_ *= fValue.fValue_;
        }

        void operator /= (const s_float_t& fValue)
        {
            fValue_ /= fValue.fValue_;
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

        s_bool operator == (const s_float_t& fValue) const
        {
            return (fValue_ == fValue.fValue_);
        }

        s_bool operator != (const s_float_t& fValue) const
        {
            return (fValue_ != fValue.fValue_);
        }

        s_bool operator < (const s_float_t& fValue) const
        {
            return (fValue_ < fValue.fValue_);
        }

        s_bool operator > (const s_float_t& fValue) const
        {
            return (fValue_ > fValue.fValue_);
        }

        s_bool operator <= (const s_float_t& fValue) const
        {
            return (fValue_ <= fValue.fValue_);
        }

        s_bool operator >= (const s_float_t& fValue) const
        {
            return (fValue_ >= fValue.fValue_);
        }

        s_ctnr<s_float_t> operator , (const s_float_t& fValue) const
        {
            s_ctnr<s_float_t> mContainer;
            mContainer.PushBack(*this);
            mContainer.PushBack(fValue);
            return mContainer;
        }

        static const s_float_t NaN;
        static const s_float_t INFPLUS;
        static const s_float_t INFMINUS;
        static const T         EPS;
        static const s_float_t PI;
        static const uint      DIGIT;

        /// Converts a float to an angle in radian.
        /** \note It is assumed that the value was a non dimensionnal angle
        *         (1 = 2*PI rad = 360°).
        */
        static s_float_t ToRad(const s_float_t& fValue)
        {
            return fValue * 2*PI;
        }

        /// Converts this float to an angle in degree.
        /** \note It is assumed that the value was a non dimensionnal angle
        *         (1 = 2*PI rad = 360°).
        */
        static s_float_t ToDeg(const s_float_t& fValue)
        {
            return fValue * 360;
        }

        /// Converts a float to a angle in radian.
        /** \note It is assumed that the value was an angle in degree.
        */
        static s_float_t DegToRad(const s_float_t& fValue)
        {
            return fValue * PI / 180;
        }

        /// Converts a float to a angle in degree.
        /** \note It is assumed that the value was an angle in radian.
        */
        static s_float_t RadToDeg(const s_float_t& fValue)
        {
            return fValue * 180 / PI;
        }

        /// Clamps the provided value into the provided interval.
        /** \param fValue The value to clamp
        *   \param fMin   The minimum value
        *   \param fMax   The maximum value
        *   \return The clamped value
        */
        static s_float_t Clamp(const s_float_t& fValue, const s_float_t& fMin, const s_float_t& fMax)
        {
            return (fValue.fValue_ < fMax.fValue_) ? ((fValue.fValue_ > fMin.fValue_) ? fValue.fValue_ : fMin.fValue_) : fMax.fValue_;
        }

        /// Returns the lowest value of the two provided ones.
        /** \param fLeft  The first value
        *   \param fRight The second value
        *   \return The lowest value of the two provided ones
        */
        static s_float_t Min(const s_float_t& fLeft, const s_float_t& fRight)
        {
            return (fLeft >= fRight) ? fRight : fLeft;
        }

        /// Returns the highest value of the two provided ones.
        /** \param fLeft  The first value
        *   \param fRight The second value
        *   \return The highest value of the two provided ones
        */
        static s_float_t Max(const s_float_t& fLeft, const s_float_t& fRight)
        {
            return (fLeft <= fRight) ? fRight : fLeft;
        }

        /// Returns a random float in the provided range.
        /** \param fMin The lower bound (minimum)
        *   \param fMax The upper bound (maximum)
        *   \return A random float in the provided range
        */
        static s_float_t Random(const s_float_t& fMin = 0.0, const s_float_t& fMax = 1.0)
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

        /// Rounds a float value.
        /** \param fValue The value to round
        *   \note If the decimal part is greater than 0.5, the number is rounded up.
        *         Else the number is rounded down.
        */
        static s_float_t Round(const s_float_t& fValue)
        {
            return round(fValue.fValue_);
        }

        /// Rounds a float value up.
        /** \param fValue The value to round
        */
        static s_float_t RoundUp(const s_float_t& fValue)
        {
            return ceil(fValue.fValue_);
        }

        /// Rounds a float value down.
        /** \param fValue The value to round
        */
        static s_float_t RoundDown(const s_float_t& fValue)
        {
            return floor(fValue.fValue_);
        }

        /// Returns the sign of the provided float.
        /** \return The sign of the provided float
        */
        static s_float_t Sign(const s_float_t& fValue)
        {
            if (fValue.IsValid())
            {
                return (fValue.fValue_ < 0.0) ? -1 : 1;
            }
            else
                return s_float_t::NaN;
        }

    private :

        T fValue_;
    };

    float MakeFloat(const ulong& ul);

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
    const uint s_float_t<T>::DIGIT = 1;

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

    template <class T>
    s_float_t<T> operator + (const int& fLeft, const s_float_t<T>& fRight)
    {
        return s_float_t<T>(static_cast<float>(fLeft)) + fRight;
    }

    template <class T>
    s_float_t<T> operator - (const int& fLeft, const s_float_t<T>& fRight)
    {
        return s_float_t<T>(static_cast<float>(fLeft)) - fRight;
    }

    template <class T>
    s_float_t<T> operator * (const int& fLeft, const s_float_t<T>& fRight)
    {
        return s_float_t<T>(static_cast<float>(fLeft)) * fRight;
    }

    template <class T>
    s_float_t<T> operator / (const int& fLeft, const s_float_t<T>& fRight)
    {
        return s_float_t<T>(static_cast<float>(fLeft)) / fRight;
    }

    template <class T, class N>
    s_str_t<N> operator+ (const N* sLeft, const s_float_t<T>& fRight)
    {
        return s_str_t<N>(sLeft) << fRight;
    }

    typedef s_float_t<float>       s_float;
    typedef s_float_t<double>      s_double;
    typedef s_float_t<long double> s_ldouble;

    template<> const float s_float::EPS  = std::numeric_limits<float>::epsilon();
    template<> const s_float s_float::PI = 3.141592f;
    template<> const uint s_float::DIGIT = std::numeric_limits<float>::digits10;

    template<> const double s_double::EPS  = std::numeric_limits<double>::epsilon();
    template<> const s_double s_double::PI = 3.141592653589793;
    template<> const uint s_double::DIGIT  = std::numeric_limits<double>::digits10;

    template<> const long double s_ldouble::EPS = std::numeric_limits<long double>::epsilon();
    template<> const s_ldouble s_ldouble::PI    = 3.141592653589793238;
    template<> const uint s_ldouble::DIGIT      = std::numeric_limits<long double>::digits10;

    /** \cond NOT_REMOVE_FROM_DOC
    */
    template<> class TypeTraits<float>       { public : typedef s_float   Type; };
    template<> class TypeTraits<double>      { public : typedef s_double  Type; };
    template<> class TypeTraits<long double> { public : typedef s_ldouble Type; };
    /** \endcond
    */
}
