/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_FLOAT_H
#define FROST_UTILS_FLOAT_H

#include "frost_utils.h"
#include "frost_utils_bool.h"

namespace Frost
{
    class s_int;
    class s_uint;
    class s_double;
    class s_bool;
    class s_str;
    template<class T> class s_ctnr;

    /// Base type : float
    /** Frost's base types are made to allow simpler
    *   manipulation of numbers, booleans and strings.
    *   They simplify conversions (mostly to string),
    *   allow easy access to math/string functions and
    *   implements new functions that are not present
    *   in the standard libraries.
    */
    class s_float
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

        s_float();
        s_float(const float& fValue);
        s_float(const s_float& fValue);
        explicit s_float(const FloatType& mType);
        explicit s_float(const s_double& dValue);
        explicit s_float(const s_int& iValue);
        explicit s_float(const s_uint& uiValue);
        explicit s_float(const s_bool& bValue);
        explicit s_float(const char* sValue);
        explicit s_float(const std::string& sValue);
        explicit s_float(const s_str& sValue);

        /// Converts this float to a angle in radian.
        /** \note It is assumed that the previous value was an angle in degree.
        */
        void          DegToRad();

        /// Converts this float to an angle in radian.
        /** \note It is assumed that the previous value was a non dimensionnal angle
        *         (1 = 2*PI rad = 360°).
        */
        s_float       GetRad() const;

        /// Converts this float to an angle in degree.
        /** \note It is assumed that the previous value was a non dimensionnal angle
        *         (1 = 2*PI rad = 360°).
        */
        s_float       GetDeg() const;

        /// Returns a const reference to the float.
        /** \return A const reference to the float
        */
        inline const float& Get(s_bool* bIsNaN = NULL) const
        {
            if (bIsNaN != NULL)
                *bIsNaN = (mType_ != FLOAT);

            if (mType_ != FLOAT)
                return fDummy;
            else
                return fValue_;
        }

        /// Returns a reference to the float.
        /** \return A reference to the float
        */
        inline float&  GetR(s_bool* bIsNaN = NULL)
        {
            if (bIsNaN != NULL)
                *bIsNaN = (mType_ != FLOAT);

            if (mType_ != FLOAT)
                return fDummy;
            else
                return fValue_;
        }

        /// Returns the power of two just above the actual value (or equal).
        /** \return The associated power of two (2^n) (superior of equal)
        */
        s_uint        GetNearestPowerOfTwo() const;

        /// Returns the type of this float.
        /** \return The type of this float (infinite, NaN, ...)
        */
        FloatType     GetType() const;

        /// Checks if this float is infinite and negative
        /** \return 'true' if this float is infinite and negative
        */
        s_bool        IsInfiniteMinus() const;

        /// Checks if this float is infinite and positive
        /** \return 'true' if this float is infinite and positive
        */
        s_bool        IsInfinitePlus() const;

        /// Checks if this float is a Not a Number (NaN)
        /** \return 'true' if this float is NaN
        */
        s_bool        IsNaN() const;

        /// Checks if this float equals zero.
        /** \return 'true' if this float equals zero (precision : 10^-10)
        */
        s_bool        IsNull() const;

        /// Checks if this float is a valid number.
        /** \return 'true' if this float is not infinite and a number
        */
        s_bool        IsValid() const;

        /// Elevates this float to a certain power (this^n).
        /** \param fPower The power...
        */
        void          Pow(const s_float& fPower);

        /// Converts this float to an angle in degree.
        /** \param bNegativeAllowed 'true' if you allow negative return values
        *   \note It is assumed that the previous value was an angle in radian.
        */
        void          RadToDeg(const s_bool& bNegativeAllowed = true);

        /// Sets the value of the float to a random number.
        /** \param fMin The lower bound (minimum)
        *   \param fMax The upper bound (maximum)
        */
        void          Random(const s_float& fMin = 0.0f, const s_float& fMax = 1.0f);

        /// Rounds the value.
        /** \param mRoundType How to round the value (see RoundType)
        */
        void          Round(const RoundType& mRoundType = ROUND_MID);

        /// Adjusts the float to fit into the provided range.
        /** \param fMin The lower bound (minimum)
        *   \param fMax The upper bound (maximum)
        */
        void          Saturate(const s_float& fMin = 0.0f, const s_float& fMax = 1.0f);

        /// Sets this float to infinite (positive).
        void          SetInfiniteMinus();

        /// Sets this float to infinite (negative).
        void          SetInfinitePlus();

        /// Set this float to Not a Number state.
        void          SetNaN();

        /// Returns the sign of this float.
        /** \return The sign of this float
        */
        s_int         Sign();

        /// Converts this float to an angle in degree.
        /** \param bNegativeAllowed 'true' if you allow negative return values
        *   \note It is assumed that the previous value was a non dimensionnal angle
        *         (1 = 2*PI rad = 360°).
        */
        void          ToDeg(const s_bool& bNegativeAllowed = true);

        /// Converts this float to an angle in radian.
        /** \note It is assumed that the previous value was a non dimensionnal angle
        *         (1 = 2*PI rad = 360°).
        */
        void          ToRad();

        bool          operator ! () const;
        typedef       void (s_float::*MemberFn)();
        operator      MemberFn() const;

        s_float       operator - () const;

        s_float       operator +  (const s_float& fValue) const;
        s_float       operator -  (const s_float& fValue) const;
        s_float       operator *  (const s_float& fValue) const;
        s_float       operator /  (const s_float& fValue) const;
        void          operator += (const s_float& fValue);
        void          operator -= (const s_float& fValue);
        void          operator *= (const s_float& fValue);
        void          operator /= (const s_float& fValue);

        s_str         operator +  (const char* sValue) const;
        s_str         operator +  (const s_str& sValue) const;

        bool          operator == (const s_float& fValue) const;
        bool          operator != (const s_float& fValue) const;
        bool          operator <  (const s_float& fValue) const;
        bool          operator >  (const s_float& fValue) const;
        bool          operator <= (const s_float& fValue) const;
        bool          operator >= (const s_float& fValue) const;

        s_float&      operator =  (const s_float& fValue);

        s_ctnr<s_float> operator ,  (const s_float& fValue) const;

        static const s_float NaN;
        static const s_float INFPLUS;
        static const s_float INFMINUS;
        static       float   fDummy;

        /// Returns the lowest value of the two provided ones.
        /** \param fLeft  The first value
        *   \param fRight The second value
        *   \return The lowest value of the two provided ones
        */
        static s_float Min(const s_float& fLeft, const s_float& fRight);

        /// Returns the highest value of the two provided ones.
        /** \param fLeft  The first value
        *   \param fRight The second value
        *   \return The highest value of the two provided ones
        */
        static s_float Max(const s_float& fLeft, const s_float& fRight);

        /// Returns a random float in the provided range.
        /** \param fMin The lower bound (minimum)
        *   \param fMax The upper bound (maximum)
        *   \return A random float in the provided range
        */
        static s_float Rand(const s_float& fMin = 0.0f, const s_float& fMax = 1.0f);

        static const s_str CLASS_NAME;

    private :

        float fValue_;

        FloatType mType_;
    };

    s_float operator+ (const float& fLeft, const s_float& fRight);
    s_float operator- (const float& fLeft, const s_float& fRight);
    s_float operator* (const float& fLeft, const s_float& fRight);
    s_float operator/ (const float& fLeft, const s_float& fRight);
}

#endif
