/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_DOUBLE_H
#define FROST_UTILS_DOUBLE_H

#include "frost_utils.h"
#include "frost_utils_bool.h"

namespace Frost
{
    class s_int;
    class s_uint;
    class s_float;
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

    /// Base type : double
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
    class s_double
    {
    public :

        enum DoubleType
        {
            DOUBLE,
            DOUBLE_INF_PLUS,
            DOUBLE_INF_MINUS,
            DOUBLE_NAN
        };

        enum RoundType
        {
            ROUND_CEIL,
            ROUND_MID,
            ROUND_FLOOR
        };

        s_double();
        s_double(const double& dValue);
        explicit s_double(const DoubleType& mType);
        explicit s_double(const s_float& fValue);
        explicit s_double(const s_int& iValue);
        explicit s_double(const s_uint& uiValue);
        explicit s_double(const s_bool& bValue);
        explicit s_double(const string_element* sValue);
        explicit s_double(const string_object& sValue);
        explicit s_double(const s_str& sValue);

        /// Adjusts this double's value to be contained into the provided interval.
        /** \param dMin The minimum value
        *   \param dMax The maximum value
        */
        void             Clamp(const s_double& dMin, const s_double& dMax);

        /// Converts this double to a angle in radian.
        /** \note It is assumed that the previous value was an angle in degree.
        */
        void             DegToRad();

        /// Converts this float to an angle in radian.
        /** \note It is assumed that the previous value was a non dimensionnal angle
        *         (1 = 2*PI rad = 360°).
        */
        s_double         GetRad() const;

        /// Converts this float to an angle in degree.
        /** \note It is assumed that the previous value was a non dimensionnal angle
        *         (1 = 2*PI rad = 360°).
        */
        s_double         GetDeg() const;

        /// Returns a const reference to the .
        /** \return A const reference to the int
        */
        inline const double& Get() const
        {
            return (mType_ != DOUBLE) ? dDummy : dValue_;
        }

        /// Returns a reference to the double.
        /** \return A reference to the double
        */
        inline double&   GetR()
        {
            return (mType_ != DOUBLE) ? dDummy : dValue_;
        }

        /// Returns the power of two just above the actual value (or equal).
        /** \return The associated power of two (2^n) (superior of equal)
        */
        s_uint           GetNearestPowerOfTwo() const;

        /// Returns the type of this double.
        /** \return The type of this double (infinite, NaN, ...)
        */
        DoubleType       GetType() const;

        /// Checks if this double is infinite and negative
        /** \return 'true' if this double is infinite and negative
        */
        s_bool           IsInfiniteMinus() const;

        /// Checks if this double is infinite and positive
        /** \return 'true' if this double is infinite and positive
        */
        s_bool           IsInfinitePlus() const;

        /// Checks if this double is a Not a Number (NaN)
        /** \return 'true' if this double is NaN
        */
        s_bool           IsNaN() const;

        /// Checks if this double equals zero.
        /** \return 'true' if this double equals zero (precision : 10^-10)
        */
        s_bool           IsNull() const;

        /// Checks if this double is a valid number.
        /** \return 'true' if this double is not infinite and a number
        */
        s_bool           IsValid() const;

        /// Elevates this double to a certain power (this^n).
        /** \param dPower The power...
        */
        void             Pow(const s_double& dPower);

        /// Converts this double to an angle in degree.
        /** \param bNegativeAllowed 'true' if you allow negative return values
        *   \note It is assumed that the previous value was an angle in radian.
        */
        void             RadToDeg(const s_bool& bNegativeAllowed = true);

        /// Sets the value of the double to a random number.
        /** \param dMin The lower bound (minimum)
        *   \param dMax The upper bound (maximum)
        */
        void             Randomize(const s_double& dMin = 0.0, const s_double& dMax = 1.0);

        /// Rounds the value.
        /** \param mRoundType How to round the value (see RoundType)
        */
        void             Round(const RoundType& mRoundType = ROUND_MID);

        /// Adjusts the double to fit into the provided range.
        /** \param dMin The lower bound (minimum)
        *   \param dMax The upper bound (maximum)
        */
        void             Saturate(const s_double& dMin = 0.0, const s_double& dMax = 1.0);

        /// Sets this double to infinite (positive).
        void             SetInfiniteMinus();

        /// Sets this double to infinite (negative).
        void             SetInfinitePlus();

        /// Set this double to Not a Number state.
        void             SetNaN();

        /// Returns the sign of this double.
        /** \return The sign of this double
        */
        s_int            Sign();

        /// Converts this double to an angle in degree.
        /** \param bNegativeAllowed 'true' if you allow negative return values
        *   \note It is assumed that the previous value was a non dimensionnal angle
        *         (1 = 2*PI rad = 360°).
        */
        void             ToDeg(const s_bool& bNegativeAllowed = true);

        /// Converts this double to an angle in radian.
        /** \note It is assumed that the previous value was a non dimensionnal angle
        *         (1 = 2*PI rad = 360°).
        */
        void             ToRad();

        s_double         operator - () const;

        s_double         operator +  (const s_double& dValue) const;
        s_double         operator -  (const s_double& dValue) const;
        s_double         operator *  (const s_double& dValue) const;
        s_double         operator /  (const s_double& dValue) const;
        void             operator += (const s_double& dValue);
        void             operator -= (const s_double& dValue);
        void             operator *= (const s_double& dValue);
        void             operator /= (const s_double& dValue);

        s_str            operator +  (const string_element* sValue) const;
        #ifdef USE_UNICODE
            s_str        operator +  (const char* sValue) const;
        #endif
        s_str            operator +  (const s_str& sValue) const;

        s_bool           operator == (const s_double& dValue) const;
        s_bool           operator != (const s_double& dValue) const;
        s_bool           operator <  (const s_double& dValue) const;
        s_bool           operator >  (const s_double& dValue) const;
        s_bool           operator <= (const s_double& dValue) const;
        s_bool           operator >= (const s_double& dValue) const;

        s_double&        operator =  (const s_double& dValue);

        s_ctnr<s_double> operator ,  (const s_double& dValue) const;

        static const s_double NaN;
        static const s_double INFPLUS;
        static const s_double INFMINUS;
        static const s_double PI;
        static       double   dDummy;

        /// Clamps the provided value into the provided interval.
        /** \param dValue The value to clamp
        *   \param dMin   The minimum value
        *   \param dMax   The maximum value
        *   \return The clamped value
        */
        static s_double Clamp(const s_double& dValue, const s_double& dMin, const s_double& dMax);

        /// Returns the lowest value of the two provided ones.
        /** \param dLeft  The first value
        *   \param dRight The second value
        *   \return The lowest value of the two provided ones
        */
        static s_double Min(const s_double& dLeft, const s_double& dRight);

        /// Returns the highest value of the two provided ones.
        /** \param dLeft  The first value
        *   \param dRight The second value
        *   \return The highest value of the two provided ones
        */
        static s_double Max(const s_double& dLeft, const s_double& dRight);

        /// Returns a random double in the provided range.
        /** \param dMin The lower bound (minimum)
        *   \param dMax The upper bound (maximum)
        *   \return A random double in the provided range
        */
        static s_double Random(const s_double& dMin = 0.0, const s_double& dMax = 1.0);

        /// Rounds a double value.
        /** \param dValue     The value to round
        *   \param mRoundType How to round the value (see RoundType)
        */
        static s_double Round(const s_double& dValue, const RoundType& mRoundType = ROUND_MID);

        static const s_str CLASS_NAME;

    private :

        double dValue_;

        DoubleType mType_;
    };

    s_double operator+ (const double& dLeft, const s_double& dRight);
    s_double operator- (const double& dLeft, const s_double& dRight);
    s_double operator* (const double& dLeft, const s_double& dRight);
    s_double operator/ (const double& dLeft, const s_double& dRight);
}

#endif
