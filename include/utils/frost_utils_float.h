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
    */
    template <class T>
    class s_float_t
    {
    public :

        enum FloatType
        {
            FLOAT,
            FLOAT_INF_PLUS,
            FLOAT_INF_MINUS,
            FLOAT_NAN
        };

        enum RoundType
        {
            ROUND_CEIL,
            ROUND_MID,
            ROUND_FLOOR
        };

        s_float_t()
        {
            mType_ = FLOAT;
            fValue_ = 0.0f;
        }

        s_float_t(const T& fValue)
        {
            mType_ = FLOAT;
            fValue_ = fValue;
        }

        s_float_t(const s_float_t& fValue)
        {
            mType_ = fValue.mType_;
            fValue_ = fValue.fValue_;
        }

        explicit s_float_t(const FloatType& mType)
        {
            mType_ = mType;
            fValue_ = 0.0f;
        }

        template <class N>
        explicit s_float_t(const s_float_t<N>& fValue)
        {
            mType_ = (FloatType)(int)(fValue.GetType());
            fValue_ = static_cast<T>(fValue.Get());
        }

        template<class N>
        explicit s_float_t(const s_int_t<N>& iValue)
        {
            if (iValue.GetType() == s_int_t<N>::INTEGER_NAN)
            {
                mType_ = FLOAT_NAN;
                fValue_ = 0.0f;
            }
            else if (iValue.GetType() == s_int_t<N>::INTEGER_INF_PLUS)
            {
                mType_ = FLOAT_INF_PLUS;
                fValue_ = 0.0f;
            }
            else if (iValue.GetType() == s_int_t<N>::INTEGER_INF_MINUS)
            {
                mType_ = FLOAT_INF_MINUS;
                fValue_ = 0.0f;
            }
            else
            {
                mType_ = FLOAT;
                fValue_ = static_cast<T>(iValue.Get());
            }
        }

        template<class N>
        explicit s_float_t(const s_uint_t<N>& uiValue)
        {
            if (uiValue.GetType() == s_uint_t<N>::INTEGER_NAN)
            {
                mType_ = FLOAT_NAN;
                fValue_ = 0.0f;
            }
            else if (uiValue.GetType() == s_uint_t<N>::INTEGER_INF)
            {
                mType_ = FLOAT_INF_PLUS;
                fValue_ = 0.0f;
            }
            else
            {
                mType_ = FLOAT;
                fValue_ = static_cast<T>(uiValue.Get());
            }
        }

        template<class N>
        explicit s_float_t(const s_bool_t<N>& bValue)
        {
            mType_ = FLOAT;
            if (bValue)
                fValue_ = 1.0f;
            else
                fValue_ = 0.0f;
        }

        explicit s_float_t(const string_element* sValue)
        {
            mType_ = FLOAT;
            string_stream s(sValue);
            s >> fValue_;
        }

        explicit s_float_t(const string_object& sValue)
        {
            mType_ = FLOAT;
            string_stream s(sValue);
            s >> fValue_;
        }

        template<class N>
        explicit s_float_t(const s_str_t<N>& sValue)
        {
            mType_ = FLOAT;
            string_stream s(sValue.Get());
            s >> fValue_;
        }

        /// Adjusts this float's value to be contained into the provided interval.
        /** \param fMin The minimum value
        *   \param fMax The maximum value
        */
        void Clamp(const s_float_t& fMin, const s_float_t& fMax)
        {
            if (IsValid() && fMax.IsValid() && fMin.IsValid())
                fValue_ = (fValue_ < fMax.fValue_) ? ((fValue_ > fMin.fValue_) ? fValue_ : fMin.fValue_) : fMax.fValue_;
            else
                SetNaN();
        }

        /// Converts this float to a angle in radian.
        /** \note It is assumed that the previous value was an angle in degree.
        */
        void DegToRad()
        {
            if (IsValid())
            {
                fValue_ *= M_PI/180.0f;
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
                return fValue_*2*M_PI;
            }
            else
                return 0.0f;
        }

        /// Converts this float to an angle in degree.
        /** \note It is assumed that the previous value was a non dimensionnal angle
        *         (1 = 2*PI rad = 360°).
        */
        s_float_t GetDeg() const
        {
            if (IsValid())
            {
                return fValue_*360.0f;
            }
            else
                return 0.0f;
        }

        /// Returns a const reference to the float.
        /** \return A const reference to the float
        */
        inline const T& Get() const
        {
            if (mType_ != FLOAT)
                return fDummy;
            else
                return fValue_;
        }

        /// Returns a reference to the float.
        /** \return A reference to the float
        */
        inline T& GetR()
        {
            if (mType_ != FLOAT)
                return fDummy;
            else
                return fValue_;
        }

        /// Returns the power of two just above the actual value (or equal).
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
        FloatType GetType() const
        {
            return mType_;
        }

        /// Checks if this float is infinite and negative
        /** \return 'true' if this float is infinite and negative
        */
        s_bool IsInfiniteMinus() const
        {
            return (mType_ == FLOAT_INF_MINUS);
        }

        /// Checks if this float is infinite and positive
        /** \return 'true' if this float is infinite and positive
        */
        s_bool IsInfinitePlus() const
        {
            return (mType_ == FLOAT_INF_PLUS);
        }

        /// Checks if this float is a Not a Number (NaN)
        /** \return 'true' if this float is NaN
        */
        s_bool IsNaN() const
        {
            return (mType_ == FLOAT_NAN);
        }

        /// Checks if this float equals zero.
        /** \return 'true' if this float equals zero (precision : fEpsilon)
        */
        s_bool IsNull() const
        {
            return ( IsValid() && (fValue_ < fEpsilon) && (fValue_ > -fEpsilon) );
        }

        /// Checks if this float is a valid number.
        /** \return 'true' if this float is not infinite and a number
        */
        s_bool IsValid() const
        {
            return (mType_ == FLOAT);
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
                fValue_ *= 180.0f/M_PI;
                if ( (!bNegativeAllowed) && (fValue_ < 0.0f) )
                    fValue_ += 360.0f;
            }
        }

        /// Sets the value of the float to a random number.
        /** \param fMin The lower bound (minimum)
        *   \param fMax The upper bound (maximum)
        */
        void Randomize(const s_float_t& fMin = 0.0f, const s_float_t& fMax = 1.0f)
        {
            if (fMin.IsValid() && fMax.IsValid())
            {
                mType_ = FLOAT;
                if (fMax < fMin)
                    fValue_ = fMin.fValue_;
                else
                    fValue_ = (fMax.fValue_-fMin.fValue_)*(rand()/T(RAND_MAX)) + fMin.fValue_;
            }
        }

        /// Rounds the value.
        /** \param mRoundType How to round the value (see RoundType)
        */
        void Round(const RoundType& mRoundType = ROUND_MID)
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

        /// Sets this float to infinite (negative).
        void SetInfiniteMinus()
        {
            mType_ = FLOAT_INF_MINUS;
            fValue_ = 0.0f;
        }

        /// Sets this float to infinite (positive).
        void SetInfinitePlus()
        {
            mType_ = FLOAT_INF_PLUS;
            fValue_ = 0.0f;
        }

        /// Set this float to Not a Number state.
        void SetNaN()
        {
            mType_ = FLOAT_NAN;
            fValue_ = 0.0f;
        }

        /// Returns the sign of this float.
        /** \return The sign of this float
        */
        s_int_t<int> Sign()
        {
            if (IsValid())
            {
                if (fValue_ == 0.0f)
                    return 1;
                else
                    return (fValue_*fabs(fValue_) < 0) ? -1 : 1;
            }
            else
                return s_int_t<int>::NaN;
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
                fValue_ *= 360.0f;
                if ( (!bNegativeAllowed) && (fValue_ < 0.0f) )
                    fValue_ += 360.0f;
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
                fValue_ *= 2*M_PI;
            }
        }

        s_float_t operator - () const
        {
            if (!IsNaN())
            {
                if (IsInfinitePlus())
                    return s_float_t::INFMINUS;
                else if (IsInfiniteMinus())
                    return s_float_t::INFPLUS;
                else
                    return -fValue_;
            }
            else
                return s_float_t::NaN;
        }

        s_float_t operator + (const s_float_t& fValue) const
        {
            if (fValue.IsNaN() || IsNaN())
                return s_float_t::NaN;

            if (fValue.IsInfinitePlus())
            {
                if (IsInfiniteMinus())
                    return s_float_t::NaN;
                else
                    return s_float_t::INFPLUS;
            }
            else if (fValue.IsInfiniteMinus())
            {
                if (IsInfinitePlus())
                    return s_float_t::NaN;
                else
                    return s_float_t::INFMINUS;
            }
            else
            {
                return fValue_ + fValue.fValue_;
            }
        }

        s_float_t operator - (const s_float_t& fValue) const
        {
            if (fValue.IsNaN() || IsNaN())
                return s_float_t::NaN;

            if (fValue.IsInfinitePlus())
            {
                if (IsInfinitePlus())
                    return s_float_t::NaN;
                else
                    return s_float_t::INFMINUS;
            }
            else if (fValue.IsInfiniteMinus())
            {
                if (IsInfiniteMinus())
                    return s_float_t::NaN;
                else
                    return s_float_t::INFPLUS;
            }
            else
            {
                return fValue_ - fValue.fValue_;
            }
        }

        s_float_t operator * (const s_float_t& fValue) const
        {
            if (fValue.IsNaN() || IsNaN())
                return s_float_t::NaN;

            if (fValue.IsInfinitePlus())
            {
                if (IsInfiniteMinus())
                    return s_float_t::NaN;
                else
                {
                    if (fValue_ < 0.0f)
                        return s_float_t::INFMINUS;
                    else
                        return s_float_t::INFPLUS;
                }
            }
            else if (fValue.IsInfiniteMinus())
            {
                if (IsInfinitePlus())
                    return s_float_t::NaN;
                else
                {
                    if (fValue_ < 0.0f)
                        return s_float_t::INFPLUS;
                    else
                        return s_float_t::INFMINUS;
                }
            }
            else
            {
                return fValue_ * fValue.fValue_;
            }
        }

        s_float_t operator / (const s_float_t& fValue) const
        {
            if (fValue.IsNaN() || IsNaN() || fValue.fValue_ == 0.0f)
                return s_float_t::NaN;

            if (fValue.IsInfinitePlus())
            {
                if (IsInfiniteMinus())
                    return s_float_t::NaN;
                else
                    return 0.0f;
            }
            else if (fValue.IsInfiniteMinus())
            {
                if (IsInfinitePlus())
                    return s_float_t::NaN;
                else
                    return 0.0f;
            }
            else
            {
                return fValue_ / fValue.fValue_;
            }
        }

        void operator += (const s_float_t& fValue)
        {
            *this = *this + fValue;
        }

        void operator -= (const s_float_t& fValue)
        {
            *this = *this - fValue;
        }

        void operator *= (const s_float_t& fValue)
        {
            *this = *this * fValue;
        }

        void operator /= (const s_float_t& fValue)
        {
            *this = *this / fValue;
        }

        template<class N>
        s_str_t<N> operator + (const N* sValue) const
        {
            return s_str_t<N>(*this) + sValue;
        }

        #ifdef USE_UNICODE
            template<class N>
            s_str_t<N> operator + (const char* sValue) const
            {
                return s_str_t<N>(*this) + sValue;
            }
        #endif

        template<class N>
        s_str_t<N> operator + (const s_str_t<N>& sValue) const
        {
            return s_str_t<N>(*this) + sValue;
        }

        s_bool operator == (const s_float_t& fValue) const
        {
            if (fValue.IsNaN() || IsNaN())
                return false;
            else
            {
                if ( (mType_ == fValue.mType_) && (mType_ != FLOAT) )
                    return true;

                return (fValue_ == fValue.fValue_);
            }
        }

        s_bool operator != (const s_float_t& fValue) const
        {
            if (fValue.IsNaN() || IsNaN())
                return false;
            else
            {
                if (mType_ != fValue.mType_)
                    return true;

                return (fValue_ != fValue.fValue_);
            }
        }

        s_bool operator < (const s_float_t& fValue) const
        {
            if (fValue.IsNaN() || IsNaN())
                return false;
            else
            {
                if ( (mType_ == fValue.mType_) && (mType_ != FLOAT) )
                    return false;

                if (mType_ == FLOAT_INF_MINUS)
                    return true;
                if (mType_ == FLOAT_INF_PLUS)
                    return false;

                if (fValue.mType_ == FLOAT_INF_MINUS)
                    return false;
                if (fValue.mType_ == FLOAT_INF_PLUS)
                    return true;

                return (fValue_ < fValue.fValue_);
            }
        }

        s_bool operator > (const s_float_t& fValue) const
        {
            if (fValue.IsNaN() || IsNaN())
                return false;
            else
            {
                if ( (mType_ == fValue.mType_) && (mType_ != FLOAT) )
                    return false;

                if (mType_ == FLOAT_INF_MINUS)
                    return false;
                if (mType_ == FLOAT_INF_PLUS)
                    return true;

                if (fValue.mType_ == FLOAT_INF_MINUS)
                    return true;
                if (fValue.mType_ == FLOAT_INF_PLUS)
                    return false;

                return (fValue_ > fValue.fValue_);
            }
        }

        s_bool operator <= (const s_float_t& fValue) const
        {
            if (fValue.IsNaN() || IsNaN())
                return false;
            else
            {
                if ( (mType_ == fValue.mType_) && (mType_ != FLOAT) )
                    return true;

                if (mType_ == FLOAT_INF_MINUS)
                    return true;
                if (mType_ == FLOAT_INF_PLUS)
                    return false;

                if (fValue.mType_ == FLOAT_INF_MINUS)
                    return false;
                if (fValue.mType_ == FLOAT_INF_PLUS)
                    return true;

                return (fValue_ <= fValue.fValue_);
            }
        }

        s_bool operator >= (const s_float_t& fValue) const
        {
            if (fValue.IsNaN() || IsNaN())
                return false;
            else
            {
                if ( (mType_ == fValue.mType_) && (mType_ != FLOAT) )
                    return true;

                if (mType_ == FLOAT_INF_MINUS)
                    return false;
                if (mType_ == FLOAT_INF_PLUS)
                    return true;

                if (fValue.mType_ == FLOAT_INF_MINUS)
                    return true;
                if (fValue.mType_ == FLOAT_INF_PLUS)
                    return false;

                return (fValue_ >= fValue.fValue_);
            }
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
        static       T         fDummy;
        static const T         fEpsilon;
        static const s_float_t PI;

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
            if (fValue.IsValid() && fMax.IsValid() && fMin.IsValid())
                return (fValue.fValue_ < fMax.fValue_) ? ((fValue.fValue_ > fMin.fValue_) ? fValue.fValue_ : fMin.fValue_) : fMax.fValue_;
            else
                return s_float_t::NaN;
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
        static s_float_t Random(const s_float_t& fMin = 0.0f, const s_float_t& fMax = 1.0f)
        {
            if (fMin.IsValid() && fMax.IsValid())
            {
                if (fMax.fValue_ < fMin.fValue_)
                {
                    return fMin;
                }
                else
                {
                    return (fMax.fValue_ - fMin.fValue_)*(rand()/T(RAND_MAX)) + fMin.fValue_;
                }
            }
            else
                return s_float_t::NaN;
        }

        /// Rounds a float value.
        /** \param fValue     The value to round
        *   \param mRoundType How to round the value (see RoundType)
        */
        static s_float_t Round(const s_float_t& fValue, const RoundType& mRoundType = ROUND_MID)
        {
            switch (mRoundType)
            {
                case ROUND_CEIL :
                    return ceil(fValue.fValue_);
                case ROUND_MID :
                    return round(fValue.fValue_);
                case ROUND_FLOOR :
                    return floor(fValue.fValue_);
                default :
                    return s_float_t::NaN;
            }
        }

    private :

        T fValue_;

        FloatType mType_;
    };

    template <class T>
    const s_float_t<T> s_float_t<T>::NaN      = s_float_t<T>(FLOAT_NAN);
    template <class T>
    const s_float_t<T> s_float_t<T>::INFPLUS  = s_float_t<T>(FLOAT_INF_PLUS);
    template <class T>
    const s_float_t<T> s_float_t<T>::INFMINUS = s_float_t<T>(FLOAT_INF_MINUS);
    template <class T>
    T s_float_t<T>::fDummy = 0.0f;
    template <class T>
    const T s_float_t<T>::fEpsilon = 0.0f;
    template <class T>
    const s_float_t<T> s_float_t<T>::PI      = 3.1415;

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
        return s_float_t<T>(fLeft) + fRight;
    }

    template <class T>
    s_float_t<T> operator - (const int& fLeft, const s_float_t<T>& fRight)
    {
        return s_float_t<T>(fLeft) - fRight;
    }

    template <class T>
    s_float_t<T> operator * (const int& fLeft, const s_float_t<T>& fRight)
    {
        return s_float_t<T>(fLeft) * fRight;
    }

    template <class T>
    s_float_t<T> operator / (const int& fLeft, const s_float_t<T>& fRight)
    {
        return s_float_t<T>(fLeft) / fRight;
    }

    template <class T, class N>
    s_str_t<N> operator+ (const N* sLeft, const s_float_t<T>& fRight)
    {
        return s_str_t<N>(sLeft) << fRight;
    }

    #ifdef USE_UNICODE
        template <class T>
        s_str_t<char> operator+ ( const char* sLeft, const s_float_t<T>& fRight )
        {
            return s_str_t<char>(sLeft) << fRight;
        }
    #endif

    typedef s_float_t<float> s_float;
    template<> const float s_float::fEpsilon = FLT_MIN;
    template<> const s_float s_float::PI     = 3.141592f;

    typedef s_float_t<double> s_double;
    template<> const double s_double::fEpsilon = DBL_MIN;
    template<> const s_double s_double::PI     = 3.141592653589793;
}
