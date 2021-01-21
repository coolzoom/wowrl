// Warning : If you need to use this file, include frost_utils_types.h
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

        s_float_t();

        s_float_t(const T& fValue);

        explicit s_float_t(const Type& mType);

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

        explicit s_float_t(const s_bool& bValue);

        template<class N>
        explicit s_float_t(const s_str_t<N>& sValue)
        {
            fValue_ = StringToFloat(sValue);
        }

        /// Adjusts this float's value to be contained into the provided interval.
        /** \param fMin The minimum value
        *   \param fMax The maximum value
        */
        void Clamp(const s_float_t& fMin, const s_float_t& fMax);

        /// Converts this float to a angle in radian.
        /** \note It is assumed that the previous value was an angle in degree.
        */
        void DegToRad();

        /// Converts this float to an angle in radian.
        /** \note It is assumed that the previous value was a non dimensionnal angle
        *         (1 = 2*PI rad = 360°).
        */
        s_float_t GetRad() const;

        /// Converts this float to an angle in degree.
        /** \note It is assumed that the previous value was a non dimensionnal angle
        *         (1 = 2*PI rad = 360°).
        */
        s_float_t GetDeg() const;

        /// Returns a const reference to the float.
        /** \return A const reference to the float
        */
        inline const T& Get() const { return fValue_; }

        /// Returns a reference to the float.
        /** \return A reference to the float
        */
        inline T& GetR() { return fValue_; }

        /// Returns the power of two just above the value (or equal).
        /** \return The associated power of two (2^n) (superior of equal)
        */
        s_uint_t<default_uint> GetNearestPowerOfTwo() const;

        /// Returns the type of this float.
        /** \return The type of this float (infinite, NaN, ...)
        */
        Type GetType() const;

        /// Checks if this float is infinite and negative
        /** \return 'true' if this float is infinite and negative
        */
        s_bool IsInfiniteMinus() const;

        /// Checks if this float is infinite and positive
        /** \return 'true' if this float is infinite and positive
        */
        s_bool IsInfinitePlus() const;

        /// Checks if this float is contained into the provided range.
        s_bool IsInRange(const s_float_t<T>& fMin, const s_float_t<T>& fMax) const;

        /// Checks if this float is contained into the provided range, but not equal to either bounds
        s_bool IsInRangeOpen(const s_float_t<T>& fMin, const s_float_t<T>& fMax) const;

        /// Checks if this float is a Not a Number (NaN)
        /** \return 'true' if this float is NaN
        */
        s_bool IsNaN() const;

        /// Checks if this float equals zero.
        /** \return 'true' if this float equals zero (precision : EPS)
        */
        s_bool IsNull() const;

        /// Checks if this float is a valid number.
        /** \return 'true' if this float is not infinite and a number
        */
        s_bool IsValid() const;

        /// Checks if this float is an infinite number.
        /** \return 'true' if this float is an infinite number
        */
        s_bool IsInfinite() const;

        /// Elevates this float to a certain power (this^n).
        /** \param fPower The power
        */
        void Pow(const s_float_t& fPower);

        /// Converts this float to an angle in degree.
        /** \param bNegativeAllowed 'true' if you allow negative return values
        *   \note It is assumed that the previous value was an angle in radian.
        */
        void RadToDeg(const s_bool& bNegativeAllowed = true);

        /// Sets the value of the float to a random number.
        /** \param fMin The lower bound (minimum)
        *   \param fMax The upper bound (maximum)
        */
        void Randomize(const s_float_t& fMin = 0.0, const s_float_t& fMax = 1.0);

        /// Rounds the value.
        /** \note If the decimal part is greater than 0.5, the number is rounded up.
        *         Else the number is rounded down.
        */
        void Round();

        /// Rounds up the value.
        void RoundUp();

        /// Rounds down the value.
        void RoundDown();

        /// Sets this float to infinite (negative).
        void SetInfiniteMinus();

        /// Sets this float to infinite (positive).
        void SetInfinitePlus();

        /// Set this float to Not a Number state.
        void SetNaN();

        /// Returns the sign of this float.
        /** \return The sign of this float
        */
        s_float_t GetSign() const;

        /// Converts this float to an angle in degree.
        /** \param bNegativeAllowed 'true' if you allow negative return values
        *   \note It is assumed that the previous value was a non dimensionnal angle
        *         (1 = 2*PI rad = 360°).
        */
        void ToDeg(const s_bool& bNegativeAllowed);

        /// Converts this float to an angle in radian.
        /** \note It is assumed that the previous value was a non dimensionnal angle
        *         (1 = 2*PI rad = 360°).
        */
        void ToRad();

        s_float_t operator - () const;
        s_float_t operator + (const s_float_t& fValue) const;
        s_float_t operator - (const s_float_t& fValue) const;
        s_float_t operator * (const s_float_t& fValue) const;
        s_float_t operator / (const s_float_t& fValue) const;

        void operator += (const s_float_t& fValue);
        void operator -= (const s_float_t& fValue);
        void operator *= (const s_float_t& fValue);
        void operator /= (const s_float_t& fValue);

        s_bool operator == (const s_float_t& fValue) const;
        s_bool operator != (const s_float_t& fValue) const;
        s_bool operator < (const s_float_t& fValue) const;
        s_bool operator > (const s_float_t& fValue) const;
        s_bool operator <= (const s_float_t& fValue) const;
        s_bool operator >= (const s_float_t& fValue) const;

        static const s_float_t    NaN;
        static const s_float_t    INFPLUS;
        static const s_float_t    INFMINUS;
        static const T            EPS;
        static const s_float_t    PI;
        static const default_uint DIGIT;

        /// Converts a float to an angle in radian.
        /** \note It is assumed that the value was a non dimensionnal angle
        *         (1 = 2*PI rad = 360°).
        */
        static s_float_t ToRad(const s_float_t& fValue);

        /// Converts this float to an angle in degree.
        /** \note It is assumed that the value was a non dimensionnal angle
        *         (1 = 2*PI rad = 360°).
        */
        static s_float_t ToDeg(const s_float_t& fValue);

        /// Converts a float to a angle in radian.
        /** \note It is assumed that the value was an angle in degree.
        */
        static s_float_t DegToRad(const s_float_t& fValue);

        /// Converts a float to a angle in degree.
        /** \note It is assumed that the value was an angle in radian.
        */
        static s_float_t RadToDeg(const s_float_t& fValue);

        /// Clamps the provided value into the provided interval.
        /** \param fValue The value to clamp
        *   \param fMin   The minimum value
        *   \param fMax   The maximum value
        *   \return The clamped value
        */
        static s_float_t Clamp(const s_float_t& fValue, const s_float_t& fMin, const s_float_t& fMax);

        /// Returns the lowest value of the two provided ones.
        /** \param fLeft  The first value
        *   \param fRight The second value
        *   \return The lowest value of the two provided ones
        */
        static s_float_t Min(const s_float_t& fLeft, const s_float_t& fRight);

        /// Returns the highest value of the two provided ones.
        /** \param fLeft  The first value
        *   \param fRight The second value
        *   \return The highest value of the two provided ones
        */
        static s_float_t Max(const s_float_t& fLeft, const s_float_t& fRight);

        /// Returns a random float in the provided range.
        /** \param fMin The lower bound (minimum)
        *   \param fMax The upper bound (maximum)
        *   \return A random float in the provided range
        */
        static s_float_t Random(const s_float_t& fMin = 0.0, const s_float_t& fMax = 1.0);

        /// Rounds a float value.
        /** \param fValue The value to round
        *   \note If the decimal part is greater than 0.5, the number is rounded up.
        *         Else the number is rounded down.
        */
        static s_float_t Round(const s_float_t& fValue);

        /// Rounds a float value up.
        /** \param fValue The value to round
        */
        static s_float_t RoundUp(const s_float_t& fValue);

        /// Rounds a float value down.
        /** \param fValue The value to round
        */
        static s_float_t RoundDown(const s_float_t& fValue);

        /// Returns the sign of the provided float.
        /** \return The sign of the provided float
        */
        static s_float_t Sign(const s_float_t& fValue);

    private :

        T fValue_;
    };

    float MakeFloat(const ulong& ul);

    template <class T>
    s_float_t<T> operator + (const T& fLeft, const s_float_t<T>& fRight);
    template <class T>
    s_float_t<T> operator - (const T& fLeft, const s_float_t<T>& fRight);
    template <class T>
    s_float_t<T> operator * (const T& fLeft, const s_float_t<T>& fRight);
    template <class T>
    s_float_t<T> operator / (const T& fLeft, const s_float_t<T>& fRight);

    /** \cond NOT_REMOVE_FROM_DOC
    */
    template<> class TypeTraits<float>
    {
    public :
        typedef float            Type;
        typedef float            BaseType;
        typedef s_float_t<float> FrostType;
        typedef float&           RefType;
        typedef const float&     CRefType;
        typedef float*           PointerType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };
    template<> class TypeTraits<double>
    {
    public :
        typedef double            Type;
        typedef double            BaseType;
        typedef s_float_t<double> FrostType;
        typedef double&           RefType;
        typedef const double&     CRefType;
        typedef double*           PointerType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };
    template<> class TypeTraits<long double>
    {
    public :
        typedef long double            Type;
        typedef long double            BaseType;
        typedef s_float_t<long double> FrostType;
        typedef long double&           RefType;
        typedef const long double&     CRefType;
        typedef long double*           PointerType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };

    template<class T> class TypeTraits< s_float_t<T> >
    {
    public :
        typedef s_float_t<T>        Type;
        typedef T                   BaseType;
        typedef s_float_t<T>        FrostType;
        typedef s_float_t<T>&       RefType;
        typedef const s_float_t<T>& CRefType;
        typedef s_float_t<T>*       PointerType;

        static inline typename TypeTraits<BaseType>::RefType  GetValue(RefType m)  { return m.GetR(); }
        static inline typename TypeTraits<BaseType>::CRefType GetValue(CRefType m) { return m.Get(); }
    };
    /** \endcond
    */
}
