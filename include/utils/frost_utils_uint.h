/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_UINT_H
#define FROST_UTILS_UINT_H

#include "frost_utils.h"
#include "frost_utils_bool.h"

namespace Frost
{
    class s_int;
    class s_float;
    class s_double;
    class s_bool;
    class s_str;
    template<class T> class ctnr;

    /// Base type : unsigned integer
    /** Frost's base types are made to allow simpler
    *   manipulation of numbers, booleans and strings.
    *   They simplify conversions (mostly to string),
    *   allow easy access to math/string functions and
    *   implements new functions that are not present
    *   in the standard libraries.
    */
    class s_uint
    {
    public :

        enum IntegerType
        {
            INTEGER,
            INTEGER_INF,
            INTEGER_NAN
        };

        s_uint();
        s_uint(const uint& uiValue);
        explicit s_uint(const IntegerType& mType);
        explicit s_uint(const float& fValue);
        explicit s_uint(const double& dValue);
        explicit s_uint(const s_int& iValue);
        explicit s_uint(const s_float& fValue);
        explicit s_uint(const s_double& dValue);
        explicit s_uint(const s_bool& bValue);
        explicit s_uint(const char* sValue);
        explicit s_uint(const std::string& sValue);
        explicit s_uint(const s_str& sValue);

        /// Returns a const reference to the uint.
        /** \return A const reference to the uint
        */
        inline const uint& Get(s_bool* bIsNaN = NULL) const
        {
            if (bIsNaN != NULL)
                *bIsNaN = (mType_ != INTEGER);

            if (mType_ != INTEGER)
                return uiDummy;
            else
                return uiValue_;
        }

        /// Returns a reference to the uint.
        /** \return A reference to the uint
        */
        inline uint& GetR(s_bool* bIsNaN = NULL)
        {
            if (bIsNaN != NULL)
                *bIsNaN = (mType_ != INTEGER);

            if (mType_ != INTEGER)
                return uiDummy;
            else
                return uiValue_;
        }

        /// Returns the power of two just above the actual value (or equal).
        /** \return The associated power of two (2^n) (superior of equal)
        */
        s_uint       GetNearestPowerOfTwo() const;

        /// Returns the type of this uint.
        /** \return The type of this uint (infinite, NaN, ...)
        */
        IntegerType  GetType() const;

        /// Checks if this uint is infinite
        /** \return 'true' if this uint is infinite
        */
        s_bool       IsInfinite() const;

        /// Checks if this uint is a Not a Number (NaN)
        /** \return 'true' if this uint is NaN
        */
        s_bool       IsNaN() const;

        /// Checks if this uint equals zero.
        /** \return 'true' if this uint equals zero
        */
        s_bool       IsNull() const;

        /// Checks if this uint is a valid number.
        /** \return 'true' if this uint is not infinite and a number
        */
        s_bool       IsValid() const;

        /// Elevates this uint to a certain power (this^n).
        /** \param uiPower The power...
        */
        void         Pow(const s_uint& uiPower);

        /// Sets the value of the uint to a random number.
        /** \param uiMin The lower bound (minimum)
        *   \param uiMax The upper bound (maximum)
        */
        void         Random(const s_uint& uiMin = 0u, const s_uint& uiMax = 1u);

        /// Sets this uint to infinite.
        void         SetInfinite();

        /// Set this uint to Not a Number state.
        void         SetNaN();

        bool         operator !  () const;
        typedef      void (s_uint::*MemberFn)();
        operator     MemberFn() const;

        s_uint&      operator ++ ();
        s_uint       operator ++ (int);
        s_int        operator -  () const;
        s_uint&      operator -- ();
        s_uint       operator -- (int);

        s_uint       operator +  (const s_uint& uiValue) const;
        s_uint       operator -  (const s_uint& uiValue) const;
        s_uint       operator *  (const s_uint& uiValue) const;
        s_uint       operator /  (const s_uint& uiValue) const;
        s_uint       operator %  (const s_uint& uiValue) const;
        void         operator += (const s_uint& uiValue);
        void         operator -= (const s_uint& uiValue);
        void         operator *= (const s_uint& uiValue);
        void         operator /= (const s_uint& uiValue);
        void         operator %= (const s_uint& uiValue);

        /*s_float      operator +  (const s_float& fValue) const;
        s_float      operator -  (const s_float& fValue) const;
        s_float      operator *  (const s_float& fValue) const;
        s_float      operator /  (const s_float& fValue) const;*/

        s_str        operator +  (const char* sValue) const;
        s_str        operator +  (const s_str& sValue) const;

        s_uint&      operator << (const s_uint& uiValue);

        bool         operator == (const s_uint& uiValue) const;
        bool         operator != (const s_uint& uiValue) const;
        bool         operator <  (const s_uint& uiValue) const;
        bool         operator >  (const s_uint& uiValue) const;
        bool         operator <= (const s_uint& uiValue) const;
        bool         operator >= (const s_uint& uiValue) const;

        s_uint&      operator =  (const s_uint& uiValue);

        ctnr<s_uint> operator ,  (const s_uint& uiValue) const;

        static const s_uint NaN;
        static const s_uint INF;
        static       uint   uiDummy;

        /// Returns the lowest value of the two provided ones.
        /** \param uiLeft  The first value
        *   \param uiRight The second value
        *   \return The lowest value of the two provided ones
        */
        static s_uint Min(const s_uint& uiLeft, const s_uint& uiRight);

        /// Returns the highest value of the two provided ones.
        /** \param uiLeft  The first value
        *   \param uiRight The second value
        *   \return The highest value of the two provided ones
        */
        static s_uint Max(const s_uint& uiLeft, const s_uint& uiRight);

        /// Returns a random uint in the provided range.
        /** \param uiMin The lower bound (minimum)
        *   \param uiMax The upper bound (maximum)
        *   \return A random uint in the provided range
        */
        static s_uint Rand(const s_uint& uiMin = 0u, const s_uint& uiMax = 1u);

        static const s_str CLASS_NAME;

    private :

        uint uiValue_;

        IntegerType mType_;
    };

    s_uint operator+ (const uint& uiLeft, const s_uint& uiRight);
    s_uint operator- (const uint& uiLeft, const s_uint& uiRight);
    s_uint operator* (const uint& uiLeft, const s_uint& uiRight);
    s_uint operator/ (const uint& uiLeft, const s_uint& uiRight);
}

#endif
