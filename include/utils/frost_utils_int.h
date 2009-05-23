/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_INT_H
#define FROST_UTILS_INT_H

#include "frost_utils.h"
#include "frost_utils_bool.h"

namespace Frost
{
    class s_uint;
    class s_float;
    class s_double;
    class s_bool;
    class s_str;
    template<class T> class s_ctnr;

    #ifdef USE_UNICODE
        typedef wchar_t string_element;
        typedef std::wstring string_object;
        typedef std::wstringstream string_stream;
        typedef std::wfstream file_stream;
    #else
        typedef char string_element;
        typedef std::string string_object;
        typedef std::stringstream string_stream;
        typedef std::fstream file_stream;
    #endif

    /// Base type : integer
    /** Frost's base types are made to allow simpler
    *   manipulation of numbers, booleans and strings.
    *   They simplify conversions (mostly to string),
    *   allow easy access to math/string functions and
    *   implements new functions that are not present
    *   in the standard libraries.
    */
    class s_int
    {
    public :

        enum IntegerType
        {
            INTEGER,
            INTEGER_INF_PLUS,
            INTEGER_INF_MINUS,
            INTEGER_NAN
        };

        s_int();
        s_int(const int& iValue);
        explicit s_int(const IntegerType& mType);
        explicit s_int(const float& fValue);
        explicit s_int(const double& dValue);
        explicit s_int(const s_uint& uiValue);
        explicit s_int(const s_float& fValue);
        explicit s_int(const s_double& dValue);
        explicit s_int(const s_bool& bValue);
        explicit s_int(const string_element* sValue);
        explicit s_int(const string_object& sValue);
        explicit s_int(const s_str& sValue);

        /// Returns a const reference to the int.
        /** \return A const reference to the int
        */
        inline const int& Get(s_bool* bIsNaN = NULL) const
        {
            if (bIsNaN != NULL)
                *bIsNaN = (mType_ != INTEGER);

            if (mType_ != INTEGER)
                return iDummy;
            else
                return iValue_;
        }

        /// Returns a reference to the int.
        /** \return A reference to the int
        */
        inline int&   GetR(s_bool* bIsNaN = NULL)
        {
            if (bIsNaN != NULL)
                *bIsNaN = (mType_ != INTEGER);

            if (mType_ != INTEGER)
                return iDummy;
            else
                return iValue_;
        }

        /// Returns the power of two just above the actual value (or equal).
        /** \return The associated power of two (2^n) (superior of equal)
        */
        s_uint        GetNearestPowerOfTwo() const;

        /// Returns the type of this int.
        /** \return The type of this int (infinite, NaN, ...)
        */
        IntegerType   GetType() const;

        /// Checks if this int is infinite and negative
        /** \return 'true' if this int is infinite and negative
        */
        s_bool        IsInfiniteMinus() const;

        /// Checks if this int is infinite and positive
        /** \return 'true' if this int is infinite and positive
        */
        s_bool        IsInfinitePlus() const;

        /// Checks if this int is a Not a Number (NaN)
        /** \return 'true' if this int is NaN
        */
        s_bool        IsNaN() const;

        /// Checks if this int equals zero.
        /** \return 'true' if this int equals zero
        */
        s_bool        IsNull() const;

        /// Checks if this int is a valid number.
        /** \return 'true' if this int is not infinite and a number
        */
        s_bool        IsValid() const;

        /// Elevates this int to a certain power (this^n).
        /** \param uiPower The power...
        */
        void          Pow(const s_uint& uiPower);

        /// Sets the value of the int to a random number.
        /** \param iMin The lower bound (minimum)
        *   \param iMax The upper bound (maximum)
        */
        void          Randomize(const s_int& iMin = 0, const s_int& iMax = 1);

        /// Sets this int to infinite (positive).
        void          SetInfiniteMinus();

        /// Sets this int to infinite (negative).
        void          SetInfinitePlus();

        /// Set this int to Not a Number state.
        void          SetNaN();

        /// Returns the sign of this int (either 1 or -1).
        /** \return The sign of this int (either 1 or -1)
        */
        s_int         Sign();

        s_int&        operator ++ ();
        s_int         operator ++ (int);
        s_int         operator -  () const;
        s_int&        operator -- ();
        s_int         operator -- (int);

        s_int         operator +  (const s_int& iValue) const;
        s_int         operator -  (const s_int& iValue) const;
        s_int         operator *  (const s_int& iValue) const;
        s_int         operator /  (const s_int& iValue) const;
        s_int         operator %  (const s_int& iValue) const;
        void          operator += (const s_int& iValue);
        void          operator -= (const s_int& iValue);
        void          operator *= (const s_int& iValue);
        void          operator /= (const s_int& iValue);
        void          operator %= (const s_int& iValue);

        s_str         operator +  (const string_element* sValue) const;
        #ifdef USE_UNICODE
            s_str     operator +  (const char* sValue) const;
        #endif
        s_str         operator +  (const s_str& sValue) const;

        s_int&        operator << (const s_uint& uiValue);

        s_bool        operator == (const s_int& iValue) const;
        s_bool        operator != (const s_int& iValue) const;
        s_bool        operator <  (const s_int& iValue) const;
        s_bool        operator >  (const s_int& iValue) const;
        s_bool        operator <= (const s_int& iValue) const;
        s_bool        operator >= (const s_int& iValue) const;

        s_int&        operator =  (const s_int& iValue);

        s_ctnr<s_int> operator ,  (const s_int& iValue) const;

        static const s_int NaN;
        static const s_int INFPLUS;
        static const s_int INFMINUS;
        static       int   iDummy;

        /// Returns the lowest value of the two provided ones.
        /** \param iLeft  The first value
        *   \param iRight The second value
        *   \return The lowest value of the two provided ones
        */
        static s_int Min(const s_int& iLeft, const s_int& iRight);

        /// Returns the highest value of the two provided ones.
        /** \param iLeft  The first value
        *   \param iRight The second value
        *   \return The highest value of the two provided ones
        */
        static s_int Max(const s_int& iLeft, const s_int& iRight);

        /// Returns a random int in the provided range.
        /** \param iMin The lower bound (minimum)
        *   \param iMax The upper bound (maximum)
        *   \return A random int in the provided range
        */
        static s_int Random(const s_int& iMin = 0, const s_int& iMax = 1);

        static const s_str CLASS_NAME;

    private :

        int iValue_;

        IntegerType mType_;
    };

    s_int operator+ (const int& iLeft, const s_int& iRight);
    s_int operator- (const int& iLeft, const s_int& iRight);
    s_int operator* (const int& iLeft, const s_int& iRight);
    s_int operator/ (const int& iLeft, const s_int& iRight);
}

#endif
