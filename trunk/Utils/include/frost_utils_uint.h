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

        s_uint_t();

        s_uint_t(const T& uiValue);

        explicit s_uint_t(const Type& mType);

        explicit s_uint_t(const float& fValue);

        explicit s_uint_t(const double& dValue);

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

        explicit s_uint_t(const s_bool& bValue);

        template<class N>
        explicit s_uint_t(const s_str_t<N>& sValue)
        {
            typename TypeTraits<T>::SignedType i = StringToInt(sValue);

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
        inline const T& Get() const { return uiValue_; }

        /// Returns a reference to the uint.
        /** \return A reference to the uint
        */
        inline T& GetR() { return uiValue_; }

        /// Adjusts this uint's value to be contained into the provided interval.
        /** \param uiMin The minimum value
        *   \param uiMax The maximum value
        */
        void Clamp(const s_uint_t& uiMin, const s_uint_t& uiMax);

        /// Returns the power of two just above the value (or equal).
        /** \return The associated power of two (2^n) (superior of equal)
        */
        s_uint_t GetNearestPowerOfTwo() const;

        /// Returns the type of this uint.
        /** \return The type of this uint (infinite, NaN, ...)
        */
        Type GetType() const;

        /// Checks if this uint is infinite
        /** \return 'true' if this uint is infinite
        */
        s_bool IsInfinite() const;

        /// Checks if this uint is contained into the provided range.
        s_bool IsInRange(const s_uint_t<T>& uiMin, const s_uint_t<T>& uiMax) const;

        /// Checks if this uint is a Not a Number (NaN)
        /** \return 'true' if this uint is NaN
        */
        s_bool IsNaN() const;

        /// Checks if this uint equals zero.
        /** \return 'true' if this uint equals zero
        */
        s_bool IsNull() const;

        /// Checks if this uint is a valid number.
        /** \return 'true' if this uint is not infinite and a number
        */
        s_bool IsValid() const;

        /// Elevates this uint to a certain power (this^n).
        /** \param uiPower The power...
        */
        void Pow(const s_uint_t& uiPower);

        /// Sets the value of the uint to a random number.
        /** \param uiMin The lower bound (minimum)
        *   \param uiMax The upper bound (maximum)
        */
        void Randomize(const s_uint_t& uiMin = 0u, const s_uint_t& uiMax = 1u);

        /// Sets this uint to infinite.
        void SetInfinite();

        /// Set this uint to Not a Number state.
        void SetNaN();

        s_int_t<typename TypeTraits<T>::SignedType> operator - () const;

        s_uint_t& operator ++ ();
        s_uint_t operator ++ (int);
        s_uint_t& operator -- ();
        s_uint_t operator -- (int);

        s_uint_t operator + (const s_uint_t& uiValue) const;
        s_uint_t operator - (const s_uint_t& uiValue) const;
        s_uint_t operator * (const s_uint_t& uiValue) const;
        s_uint_t operator / (const s_uint_t& uiValue) const;
        s_uint_t operator % (const s_uint_t& uiValue) const;

        void operator += (const s_uint_t& uiValue);
        void operator -= (const s_uint_t& uiValue);
        void operator *= (const s_uint_t& uiValue);
        void operator /= (const s_uint_t& uiValue);
        void operator %= (const s_uint_t& uiValue);

        s_uint_t& operator << (const s_uint_t& uiValue);

        s_bool operator == (const s_uint_t& uiValue) const;
        s_bool operator != (const s_uint_t& uiValue) const;
        s_bool operator < (const s_uint_t& uiValue) const;
        s_bool operator > (const s_uint_t& uiValue) const;
        s_bool operator <= (const s_uint_t& uiValue) const;
        s_bool operator >= (const s_uint_t& uiValue) const;

        static const s_uint_t NaN;
        static const s_uint_t INF;

        /// Returns the lowest value of the two provided ones.
        /** \param uiLeft  The first value
        *   \param uiRight The second value
        *   \return The lowest value of the two provided ones
        */
        static s_uint_t Min(const s_uint_t& uiLeft, const s_uint_t& uiRight);

        /// Returns the highest value of the two provided ones.
        /** \param uiLeft  The first value
        *   \param uiRight The second value
        *   \return The highest value of the two provided ones
        */
        static s_uint_t Max(const s_uint_t& uiLeft, const s_uint_t& uiRight);

        /// Clamps the provided value into the provided interval.
        /** \param uiValue The value to clamp
        *   \param uiMin   The minimum value
        *   \param uiMax   The maximum value
        *   \return The clamped value
        */
        static s_uint_t Clamp(const s_uint_t& uiValue, const s_uint_t& uiMin, const s_uint_t& uiMax);

        /// Returns a random uint in the provided range.
        /** \param uiMin The lower bound (minimum)
        *   \param uiMax The upper bound (maximum)
        *   \return A random uint in the provided range
        */
        static s_uint_t Random(const s_uint_t& uiMin = 0u, const s_uint_t& uiMax = 1u);

    private :

        T uiValue_;

        Type mType_;
    };

    template<class T>
    s_uint_t<T> operator + (const T& uiLeft, const s_uint_t<T>& uiRight);
    template<class T>
    s_uint_t<T> operator - (const T& uiLeft, const s_uint_t<T>& uiRight);
    template<class T>
    s_uint_t<T> operator * (const T& uiLeft, const s_uint_t<T>& uiRight);
    template<class T>
    s_uint_t<T> operator / (const T& uiLeft, const s_uint_t<T>& uiRight);
    template<class T>
    s_uint_t<T> operator % (const T& uiLeft, const s_uint_t<T>& uiRight);

    template<class T>
    s_uint_t<T> operator + (const typename TypeTraits<T>::SignedType& iLeft, const s_uint_t<T>& uiRight);
    template<class T>
    s_uint_t<T> operator - (const typename TypeTraits<T>::SignedType& iLeft, const s_uint_t<T>& uiRight);
    template<class T>
    s_uint_t<T> operator * (const typename TypeTraits<T>::SignedType& iLeft, const s_uint_t<T>& uiRight);
    template<class T>
    s_uint_t<T> operator / (const typename TypeTraits<T>::SignedType& iLeft, const s_uint_t<T>& uiRight);
    template<class T>
    s_uint_t<T> operator % (const typename TypeTraits<T>::SignedType& iLeft, const s_uint_t<T>& uiRight);

    /** \cond NOT_REMOVE_FROM_DOC
    */
    template<> class TypeTraits<uchar>
    {
    public :
        typedef uchar           Type;
        typedef uchar           BaseType;
        typedef s_uint_t<uchar> FrostType;
        typedef uchar           CharType;
        typedef uchar&          RefType;
        typedef uchar           CRefType;
        typedef uchar*          PointerType;

        typedef uchar IsInteger;
        typedef char  SignedType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };
    template<> class TypeTraits<ushort>
    {
    public :
        typedef ushort           Type;
        typedef ushort           BaseType;
        typedef s_uint_t<ushort> FrostType;
        typedef ushort&          RefType;
        typedef ushort           CRefType;
        typedef ushort*          PointerType;

        typedef ushort IsInteger;
        typedef short  SignedType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };
    template<> class TypeTraits<uint>
    {
    public :
        typedef uint           Type;
        typedef uint           BaseType;
        typedef s_uint_t<uint> FrostType;
        typedef uint           CharType;
        typedef uint&          RefType;
        typedef const uint&    CRefType;
        typedef uint*          PointerType;

        typedef uint IsInteger;
        typedef int  SignedType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };
    template<> class TypeTraits<ulong>
    {
    public :
        typedef ulong           Type;
        typedef ulong           BaseType;
        typedef s_uint_t<ulong> FrostType;
        typedef ulong&          RefType;
        typedef const ulong&    CRefType;
        typedef ulong*          PointerType;

        typedef ulong IsInteger;
        typedef long  SignedType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };
    template<> class TypeTraits<unsigned long long>
    {
    public :
        typedef unsigned long long           Type;
        typedef unsigned long long           BaseType;
        typedef s_uint_t<unsigned long long> FrostType;
        typedef unsigned long long&          RefType;
        typedef const unsigned long long&    CRefType;
        typedef unsigned long long*          PointerType;

        typedef unsigned long long IsInteger;
        typedef long long          SignedType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };

    template<class T> class TypeTraits< s_uint_t<T> >
    {
    public :
        typedef s_uint_t<T>        Type;
        typedef T                  BaseType;
        typedef s_uint_t<T>        FrostType;
        typedef s_uint_t<T>&       RefType;
        typedef const s_uint_t<T>& CRefType;
        typedef s_uint_t<T>*       PointerType;

        typedef s_uint_t<T> IsInteger;
        typedef s_int_t<typename TypeTraits<T>::SignedType> SignedType;

        static inline typename TypeTraits<BaseType>::RefType  GetValue(RefType m)  { return m.Get(); }
        static inline typename TypeTraits<BaseType>::CRefType GetValue(CRefType m) { return m.Get(); }
    };
    /** \endcond
    */
}
