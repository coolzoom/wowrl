/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_STR_H
#define FROST_UTILS_STR_H

#include "frost_utils.h"

#include "frost_utils_uint.h"

namespace Frost
{
    class s_int;
    class s_uint;
    class s_float;
    class s_double;
    class s_bool;
    class s_str;
    template<class T> class ctnr;

    enum IntegerConversionType
    {
        CONV_DECIMAL,
        CONV_HEXA,
        CONV_BIN
    };

    enum BoolConversionType
    {
        CONV_TRUE_FALSE,
        CONV_1_0,
        CONV_YES_NO
    };

    /// Base type : string
    /** Frost's base types are made to allow simpler
    *   manipulation of numbers, booleans and strings.
    *   They simplify conversions (mostly to string),
    *   allow easy access to math/string functions and
    *   implements new functions that are not present
    *   in the standard libraries.
    */
    class s_str
    {
    public :

        /// Constructor.
        s_str();

        /// std::string constructor.
        /** \param sValue The string to use
        */
        s_str(const std::string& sValue);

        /// const char* constructor.
        /** \param sValue The string to use
        */
        s_str(const char* sValue);

        /// s_int conversion constructor.
        /** \param iValue The s_int to convert
        */
        explicit s_str(const s_int& iValue);

        /// s_int conversion constructor.
        /** \param iValue    The s_int to convert
        *   \param uiCharNbr The minimum number of character
        *                    allowed (fills with zeros)
        */
        explicit s_str(const s_int& iValue, const s_uint& uiCharNbr);

        /// s_uint conversion constructor.
        /** \param uiValue The s_uint to convert
        */
        explicit s_str(const s_uint& uiValue);

        /// s_uint conversion constructor.
        /** \param uiValue   The s_uint to convert
        *   \param uiCharNbr The minimum number of character
        *                    allowed (fills with zeros)
        */
        explicit s_str(const s_uint& uiValue, const s_uint& uiCharNbr);

        /// s_float conversion constructor.
        /** \param fValue The s_float to convert
        */
        explicit s_str(const s_float& fValue);

        /// s_float conversion constructor.
        /** \param fValue    The s_float to convert
        *   \param uiCharNbr The minimum number of character
        *                    allowed (fills with zeros)
        */
        explicit s_str(const s_float& fValue, const s_uint& uiCharNbr);

        /// s_double conversion constructor.
        /** \param dValue The s_double to convert
        */
        explicit s_str(const s_double& dValue);

        /// s_double conversion constructor.
        /** \param dValue    The s_double to convert
        *   \param uiCharNbr The minimum number of character
        *                    allowed (fills with zeros)
        */
        explicit s_str(const s_double& dValue, const s_uint& uiCharNbr);

        /// s_bool conversion constructor.
        /** \param bValue The s_bool to convert
        */
        explicit s_str(const s_bool& bValue);

        /// char conversion constructor.
        /** \param cValue The char to convert
        */
        explicit s_str(const char& cValue);

        /// char conversion constructor.
        /** \param cValue    The char to convert
        *   \param uiCharNbr The number of time to copy this
        *                    character
        */
        explicit s_str(const char& cValue, const s_uint& uiCharNbr);

        /// Changes the case of the first character.
        /** \param bCapitalStart If 'true', the first character will be
        *                        capitalized.
        */
        void                CapitalStart(const s_bool& bCapitalStart);

        /// Removes the content of the string.
        void                Clear();

        /// Cuts this string everytime the delimitator is found.
        /** \param sDelim   The delimitator
        *   \param uiMaxCut The maximum number of occurences
        *   \return A vector containing the separated strings
        */
        std::vector<s_str>  Cut(const s_str& sDelim, const s_uint& uiMaxCut = s_uint::INF) const;

        /// Counts the number of time a certain pattern is found on the string.
        /** \param sPattern The string to search for
        *   \return The number of time this pattern is found
        */
        s_uint              CountOccurences(const s_str& sPattern) const;

        /// Removes a certain number of character from a given position.
        /** \param uiStart From where to start erasing
        *   \param uiNbr   The number of character to erase
        */
        void                Erase(const s_uint& uiStart = 0u, const s_uint& uiNbr = s_uint::INF);

        /// Removes a certain number of character from the end of the string.
        /** \param uiNbr The number of character to erase
        */
        void                EraseFromEnd(const s_uint& uiNbr);

        /// Removes a certain number of character from the beginning of the string.
        /** \param uiNbr The number of character to erase
        */
        void                EraseFromStart(const s_uint& uiNbr);

        /// Removes all characters between the provided positions.
        /** \param uiStart From where to start erasing
        *   \param uiEnd   From where to end (inclusive)
        */
        void                EraseRange(const s_uint& uiStart = 0u, const s_uint& uiEnd = s_uint::INF);


        /// Returns a certain number of character from a given position.
        /** \param uiStart From where to start reading
        *   \param uiNbr   The number of character to read
        *   \return The resulting string
        */
        s_str               Extract(const s_uint& uiStart = 0u, const s_uint& uiNbr = s_uint::INF) const;

        /// Returns all characters between the provided positions.
        /** \param uiStart From where to start reading
        *   \param uiEnd   From where to end (inlusive)
        *   \return The resulting string
        */
        s_str               ExtractRange(const s_uint& uiStart = 0u, const s_uint& uiEnd = s_uint::INF) const;

        /// Returns the position of the pattern in the string.
        /** \param sValue  The string to search for
        *   \param uiStart From where to start searching
        *   \return The position of the pattern (NaN if not found)
        */
        s_uint              Find(const s_str& sValue, const s_uint& uiStart = 0u) const;

        /// Returns a const reference to the string.
        /** \return A const reference to the string
        */
        inline const std::string& Get() const { return sValue_; }

        /// Returns the character at the given index.
        /** \return The character at the given index
        */
        inline char         Get(const s_uint& uiIndex) const
        {
            if (uiIndex.Get() < sValue_.size()) return sValue_[uiIndex.Get()];
            else return cDummy;
        }

        /// Returns a C-style string.
        /** \return A C-style string
        */
        inline const char*  c_str() const { return sValue_.c_str(); }

        /// Returns a reference to the string.
        /** \return A reference to the string
        */
        inline std::string& GetR() { return sValue_; }

        /// Checks if the string is empty.
        /** \return 'true' if the string doesn't contain any character
        */
        s_bool              IsEmpty() const;

        /// Returns the number of character in the string.
        /** \return The number of character in the string
        */
        s_uint              Length() const;

        /// Returns the number of character in ths string.
        /** \return The number of character in the string
        */
        s_uint              Size() const;

        /// Removes the surrounding characters matching the provided pattern.
        /** \param cPattern The character to remove
        *   \return The number of character erased
        */
        s_uint              Trim(const char& cPattern);

        /// Replaces a pattern by another string.
        /** \param sPattern     The string to search for
        *   \param sReplacement The string that will replace it
        *   \return The number of replacement
        */
        s_uint              Replace(const s_str& sPattern, const s_str& sReplacement);

        char&               operator [] (const s_uint& uiIndex);
        s_str               operator +  (const s_str& mValue) const;
        s_str               operator +  (const char* sValue) const;
        s_str               operator +  (const char& cValue) const;
        s_str               operator +  (const s_int& iValue) const;
        s_str               operator +  (const s_uint& uiValue) const;
        s_str               operator +  (const s_float& fValue) const;
        s_str               operator +  (const s_double& dValue) const;
        s_str               operator +  (const s_bool& bValue) const;
        s_str               operator -  (const s_uint& uiNumber) const;
        void                operator += (const s_str& mValue);
        void                operator += (const char& cValue);
        void                operator -= (const s_uint& uiNumber);

        bool                operator == (const s_str& mValue) const;
        bool                operator != (const s_str& mValue) const;
        bool                operator <  (const s_str& mValue) const;
        bool                operator >  (const s_str& mValue) const;
        bool                operator <= (const s_str& mValue) const;
        bool                operator >= (const s_str& mValue) const;

        s_str&              operator =  (const s_str& mValue);

        s_str&              operator << (const s_str& mValue);
        s_str&              operator << (const char* sValue);
        s_str&              operator << (const int& iValue);
        s_str&              operator << (const uint& uiValue);
        s_str&              operator << (const float& fValue);
        s_str&              operator << (const double& dValue);
        s_str&              operator << (const bool& bValue);
        s_str&              operator << (const char& cValue);
        s_str&              operator << (const s_int& iValue);
        s_str&              operator << (const s_uint& uiValue);
        s_str&              operator << (const s_float& fValue);
        s_str&              operator << (const s_double& dValue);
        s_str&              operator << (const s_bool& bValue);
        s_str&              operator << (const IntegerConversionType& mIntConvType);
        s_str&              operator << (const BoolConversionType& mBoolConvType);

        ctnr<s_str>         operator ,  (const s_str& sValue) const;

        static char cDummy;
        static const s_str CLASS_NAME;

    private :

        std::string           sValue_;
        IntegerConversionType mIntConvType_;
        BoolConversionType    mBoolConvType_;

    };

    s_str operator+ (const char* sLeft, const s_str& sRight);
    s_str operator+ (const char* sLeft, const s_int& iRight);
    s_str operator+ (const char* sLeft, const s_uint& uiRight);
    s_str operator+ (const char* sLeft, const s_float& fRight);
    s_str operator+ (const char* sLeft, const s_double& dRight);
    s_str operator+ (const char* sLeft, const s_bool& bRight);
}

#endif
