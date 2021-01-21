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

        enum Type
        {
            INTEGER,
            INTEGER_INF_PLUS,
            INTEGER_INF_MINUS,
            INTEGER_NAN
        };

        s_int_t();

        s_int_t(const T& iValue);

        explicit s_int_t(const Type& mType);

        explicit s_int_t(const float& fValue);

        explicit s_int_t(const double& dValue);

        template<class N>
        explicit s_int_t(const s_int_t<N>& iValue)
        {
            mType_ = static_cast<Type>(iValue.GetType());
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

        explicit s_int_t(const s_bool& bValue);

        template<class N>
        explicit s_int_t(const s_str_t<N>& sValue)
        {
            mType_ = INTEGER;
            iValue_ = StringToInt(sValue);
        }

        /// Returns a const reference to the int.
        /** \return A const reference to the int
        */
        inline const T& Get() const { return iValue_; }

        /// Returns a reference to the int.
        /** \return A reference to the int
        */
        inline T& GetR() { return iValue_; }

        /// Adjusts this int's value to be contained into the provided interval.
        /** \param iMin The minimum value
        *   \param iMax The maximum value
        */
        void Clamp(const s_int_t& iMin, const s_int_t& iMax);

        /// Returns the power of two just above the value (or equal).
        /** \return The associated power of two (2^n) (superior of equal)
        */
        s_uint_t<typename TypeTraits<T>::UnsignedType> GetNearestPowerOfTwo() const;

        /// Returns the type of this int.
        /** \return The type of this int (infinite, NaN, ...)
        */
        Type GetType() const;

        /// Checks if this int is infinite and negative.
        /** \return 'true' if this int is infinite and negative
        */
        s_bool IsInfiniteMinus() const;

        /// Checks if this int is infinite and positive.
        /** \return 'true' if this int is infinite and positive
        */
        s_bool IsInfinitePlus() const;

        /// Checks if this int is contained into the provided range.
        s_bool IsInRange(const s_int_t<T>& iMin, const s_int_t<T>& iMax) const;

        /// Checks if this int is a Not a Number (NaN).
        /** \return 'true' if this int is NaN
        */
        s_bool IsNaN() const;

        /// Checks if this int equals zero.
        /** \return 'true' if this int equals zero
        */
        s_bool IsNull() const;

        /// Checks if this int is a valid number.
        /** \return 'true' if this int is not infinite and a number
        */
        s_bool IsValid() const;

        /// Checks if this int is an infinite number.
        /** \return 'true' if this int is an infinite number
        */
        s_bool IsInfinite() const;

        /// Elevates this int to a certain power (this^n).
        /** \param uiPower The power
        */
        void Pow(const s_uint_t<typename TypeTraits<T>::UnsignedType>& uiPower);

        /// Sets the value of the int to a random number.
        /** \param iMin The lower bound (minimum)
        *   \param iMax The upper bound (maximum)
        */
        void Randomize(const s_int_t& iMin = 0, const s_int_t& iMax = 1);

        /// Sets this int to infinite (negative).
        void SetInfiniteMinus();

        /// Sets this int to infinite (positive).
        void SetInfinitePlus();

        /// Set this int to Not a Number state.
        void SetNaN();

        /// Returns the sign of this int (either 1 or -1).
        /** \return The sign of this int (either 1 or -1)
        */
        s_int_t GetSign() const;

        s_int_t& operator ++ ();

        s_int_t operator ++ (int);

        s_int_t operator - () const;

        s_int_t& operator -- ();

        s_int_t operator -- (int);

        s_int_t operator + (const s_int_t& iValue) const;

        s_int_t operator - (const s_int_t& iValue) const;

        s_int_t operator * (const s_int_t& iValue) const;

        s_int_t operator / (const s_int_t& iValue) const;

        s_int_t operator % (const s_int_t& iValue) const;

        void operator += (const s_int_t& iValue);

        void operator -= (const s_int_t& iValue);

        void operator *= (const s_int_t& iValue);

        void operator /= (const s_int_t& iValue);

        void operator %= (const s_int_t& iValue);

        template<class N>
        s_int_t& operator << (const s_uint_t<N>& uiValue)
        {
            if (IsValid())
            {
                iValue_ = iValue_ << uiValue.Get();
            }
            return *this;
        }

        s_int_t& operator << (const uint& uiValue);

        s_bool operator == (const s_int_t& iValue) const;

        s_bool operator != (const s_int_t& iValue) const;

        s_bool operator < (const s_int_t& iValue) const;

        s_bool operator > (const s_int_t& iValue) const;

        s_bool operator <= (const s_int_t& iValue) const;

        s_bool operator >= (const s_int_t& iValue) const;

        static const s_int_t NaN;
        static const s_int_t INFPLUS;
        static const s_int_t INFMINUS;

        /// Returns the lowest value of the two provided ones.
        /** \param iLeft  The first value
        *   \param iRight The second value
        *   \return The lowest value of the two provided ones
        */
        static s_int_t Min(const s_int_t& iLeft, const s_int_t& iRight);

        /// Returns the highest value of the two provided ones.
        /** \param iLeft  The first value
        *   \param iRight The second value
        *   \return The highest value of the two provided ones
        */
        static s_int_t Max(const s_int_t& iLeft, const s_int_t& iRight);

        /// Clamps the provided value into the provided interval.
        /** \param iValue The value to clamp
        *   \param iMin   The minimum value
        *   \param iMax   The maximum value
        *   \return The clamped value
        */
        static s_int_t Clamp(const s_int_t& iValue, const s_int_t& iMin, const s_int_t& iMax);

        /// Returns a random int in the provided range.
        /** \param iMin The lower bound (minimum)
        *   \param iMax The upper bound (maximum)
        *   \return A random int in the provided range
        */
        static s_int_t Random(const s_int_t& iMin = 0, const s_int_t& iMax = 1);

        /// Returns the sign of the provided int (either 1 or -1).
        /** \return The sign of the provided int (either 1 or -1)
        */
        s_int_t Sign(const s_int_t& iValue) const;

    private :

        T iValue_;

        Type mType_;
    };

    template<class T>
    s_int_t<T> operator + (const T& iLeft, const s_int_t<T>& iRight);
    template<class T>
    s_int_t<T> operator - (const T& iLeft, const s_int_t<T>& iRight);
    template<class T>
    s_int_t<T> operator * (const T& iLeft, const s_int_t<T>& iRight);
    template<class T>
    s_int_t<T> operator / (const T& iLeft, const s_int_t<T>& iRight);
    template<class T>
    s_int_t<T> operator % (const T& iLeft, const s_int_t<T>& iRight);

    template<class T>
    s_int_t<T> operator + (const typename TypeTraits<T>::UnsignedType& uiLeft, const s_int_t<T>& iRight);
    template<class T>
    s_int_t<T> operator - (const typename TypeTraits<T>::UnsignedType& uiLeft, const s_int_t<T>& iRight);
    template<class T>
    s_int_t<T> operator * (const typename TypeTraits<T>::UnsignedType& uiLeft, const s_int_t<T>& iRight);
    template<class T>
    s_int_t<T> operator / (const typename TypeTraits<T>::UnsignedType& uiLeft, const s_int_t<T>& iRight);
    template<class T>
    s_int_t<T> operator % (const typename TypeTraits<T>::UnsignedType& uiLeft, const s_int_t<T>& iRight);

    /** \cond NOT_REMOVE_FROM_DOC
    */
    template<> class TypeTraits<char>
    {
    public :
        typedef char          Type;
        typedef char          BaseType;
        typedef s_int_t<char> FrostType;
        typedef char          CharType;
        typedef char&         RefType;
        typedef const char&   CRefType;
        typedef char*         PointerType;

        typedef char  IsInteger;
        typedef uchar UnsignedType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };
    template<> class TypeTraits<short>
    {
    public :
        typedef short          Type;
        typedef short          BaseType;
        typedef s_int_t<short> FrostType;
        typedef short&         RefType;
        typedef const short&   CRefType;
        typedef short*         PointerType;

        typedef short  IsInteger;
        typedef ushort UnsignedType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };
    template<> class TypeTraits<int>
    {
    public :
        typedef int          Type;
        typedef int          BaseType;
        typedef s_int_t<int> FrostType;
        typedef int&         RefType;
        typedef const int&   CRefType;
        typedef int*         PointerType;

        typedef int  IsInteger;
        typedef uint UnsignedType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };
    template<> class TypeTraits<long>
    {
    public :
        typedef long          Type;
        typedef long          BaseType;
        typedef s_int_t<long> FrostType;
        typedef long&         RefType;
        typedef const long&   CRefType;
        typedef long*         PointerType;

        typedef long  IsInteger;
        typedef ulong UnsignedType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };
    template<> class TypeTraits<long long>
    {
    public :
        typedef long long          Type;
        typedef long long          BaseType;
        typedef s_int_t<long long> FrostType;
        typedef long long&         RefType;
        typedef const long long&   CRefType;
        typedef long long*         PointerType;

        typedef long long          IsInteger;
        typedef unsigned long long UnsignedType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };

    template<class T> class TypeTraits< s_int_t<T> >
    {
    public :
        typedef s_int_t<T>        Type;
        typedef T                 BaseType;
        typedef s_int_t<T>        FrostType;
        typedef s_int_t<T>&       RefType;
        typedef const s_int_t<T>& CRefType;
        typedef s_int_t<T>*       PointerType;

        typedef s_int_t<T> IsInteger;
        typedef s_uint_t<typename TypeTraits<T>::UnsignedType> UnsignedType;

        static inline typename TypeTraits<BaseType>::RefType  GetValue(RefType m)  { return m.GetR(); }
        static inline typename TypeTraits<BaseType>::CRefType GetValue(CRefType m) { return m.Get(); }
    };
    /** \endcond
    */
}
