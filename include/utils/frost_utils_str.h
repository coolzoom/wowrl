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
    *   in the standard libraries.<br>
    *   This class can be compiled to use either Unicode
    *   or ASCII characters. Just set the preprocessor
    *   directive USE_UNICODE.
    */
    class s_str
    {
    public :

        /// Constructor.
        s_str();

        /// Constructor.
        /** \param sValue The string to use
        */
        s_str(const string_object& sValue);

        /// Constructor.
        /** \param sValue The string to use
        */
        s_str(const string_element* sValue);

        #ifdef USE_UNICODE
            /// Constructor.
            /** \param sValue The string to use
            */
            s_str(const std::string& sValue);

            /// Constructor.
            /** \param sValue The string to use
            */
            s_str(const char* sValue);
        #endif

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

        /// char/wchar_t conversion constructor.
        /** \param cValue The char/wchar_t to convert
        */
        explicit s_str(const string_element& cValue);

        /// char/wchar_t conversion constructor.
        /** \param cValue    The char/wchar_t to convert
        *   \param uiCharNbr The number of time to copy this
        *                    character
        */
        explicit s_str(const string_element& cValue, const s_uint& uiCharNbr);

        /// Changes the case of the first character.
        /** \param bCapitalStart If 'true', the first character will be
        *                        capitalized.
        */
        void                CapitalStart(const s_bool& bCapitalStart);

        /// Removes the content of the string.
        void                Clear();

        /// Cuts this string everytime the delineator is found.
        /** \param sDelim            The delineator
        *   \param uiMaxCut          The maximum number of occurences.
        *   \return A vector containing the separated strings
        *   \note If the delineator is not found in the string, this function
        *         returns the whole string, unchanged.
        *   \note Removes the delineator from the sub-strings.<br>
        *         This function <b>groups</b> occurences of the delineator.
        */
        std::vector<s_str>  Cut(const s_str& sDelim, const s_uint& uiMaxCut = s_uint::INF) const;

        /// Cuts this string everytime the delineator is found.
        /** \param sDelim            The delineator
        *   \param uiMaxCut          The maximum number of occurences.
        *   \return A vector containing the separated strings
        *   \note If the delineator is not found in the string, this function
        *         returns the whole string, unchanged.
        *   \note Removes the delineator from the sub-strings.<br>
        *         This function cuts the string <b>each time</b> the delineator
        *         is found.
        */
        std::vector<s_str>  CutEach(const s_str& sDelim, const s_uint& uiMaxCut = s_uint::INF) const;

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
        s_uint              FindPos(const s_str& sValue, const s_uint& uiStart = 0u) const;

        /// Returns true is the pattern is found in the string.
        /** \param sValue  The string to search for
        *   \param uiStart From where to start searching
        *   \return 'true' if the  pattern is found
        */
        s_bool              Find(const s_str& sValue, const s_uint& uiStart = 0u) const;

        /// Returns a const reference to the string.
        /** \return A const reference to the string
        */
        inline const string_object& Get() const { return sValue_; }

        /// Returns the string converted to ASCII.
        /** \return The string converted to ASCII
        */
        std::string GetASCII() const;

        /// Returns the string converted to Unicode.
        /** \return The string converted to Unicode
        */
        std::wstring GetUnicode() const;

        /// Returns a C-style string.
        /** \return A C-style string
        */
        inline const string_element* c_str() const { return sValue_.c_str(); }

        /// Returns a reference to the string.
        /** \return A reference to the string
        */
        inline string_object& GetR() { return sValue_; }

        /// Returns this string converted to a uint
        /** \return This string converted to a uint
        *   \note Assumes this string is an hexadecimal number : '0F', 'FF', '12A0', ...
        */
        s_uint              HexToUInt() const;

        /// Checks if the string is empty.
        /** \param bIgnoreSpaces Set to 'true' if you want a string to be reported as
        *                        empty if it only contains white spaces and or tabs
        *   \return 'true' if the string doesn't contain any character
        */
        s_bool              IsEmpty(const s_bool& bIgnoreSpaces = false) const;

        /// Checks if the string is a number.
        /** \return 'true' if the string is a number
        */
        s_bool              IsNumber() const;

        /// Checks if the string is a bool.
        /** \return 'true' if the string is a bool
        */
        s_bool              IsBoolean() const;

        /// Returns the number of character in the string.
        /** \return The number of character in the string
        */
        s_uint              Length() const;

        /// Returns the number of character in ths string.
        /** \return The number of character in the string
        */
        s_uint              Size() const;

        /// Makes this string completely lower case.
        void                ToLower();

        /// Makes this string completely UPPER CASE.
        void                ToUpper();

        /// Removes the surrounding characters matching the provided pattern.
        /** \param cPattern The character to remove
        *   \return The number of character erased
        */
        s_uint              Trim(const string_element& cPattern);

        /// Replaces a pattern by another string.
        /** \param sPattern     The string to search for
        *   \param sReplacement The string that will replace it
        *   \return The number of replacement
        */
        s_uint              Replace(const s_str& sPattern, const s_str& sReplacement);

        /// Adds a new character at the end of the string.
        /** \param cChar The character to add
        */
        void                PushBack(const string_element& cChar);

        /// Adds a new character at the beginning of the string.
        /** \param cChar The character to add
        */
        void                PushFront(const string_element& cChar);

        /// Adds a new character somewhere in the string.
        /** \param cChar The character to add
        *   \param uiPos The position at which to insert the char
        */
        void                Insert(const string_element& cChar, const s_uint& uiPos);

        string_element&     operator [] (const s_uint& uiIndex);
        const string_element& operator [] (const s_uint& uiIndex) const;
        s_str               operator +  (const s_str& mValue) const;
        s_str               operator +  (const string_element* sValue) const;
        #ifdef USE_UNICODE
        s_str               operator +  (const char* sValue) const;
        #endif
        s_str               operator +  (const string_element& cValue) const;
        s_str               operator +  (const s_int& iValue) const;
        s_str               operator +  (const s_uint& uiValue) const;
        s_str               operator +  (const s_float& fValue) const;
        s_str               operator +  (const s_double& dValue) const;
        s_str               operator +  (const s_bool& bValue) const;
        s_str               operator -  (const s_uint& uiNumber) const;
        void                operator += (const s_str& mValue);
        void                operator += (const string_element& cValue);
        void                operator -= (const s_uint& uiNumber);

        bool                operator == (const s_str& mValue) const;
        bool                operator != (const s_str& mValue) const;
        bool                operator <  (const s_str& mValue) const;
        bool                operator >  (const s_str& mValue) const;
        bool                operator <= (const s_str& mValue) const;
        bool                operator >= (const s_str& mValue) const;

        s_str&              operator =  (const s_str& mValue);

        s_str&              operator << (const s_str& mValue);
        s_str&              operator << (const string_element* sValue);
        s_str&              operator << (const int& iValue);
        s_str&              operator << (const uint& uiValue);
        s_str&              operator << (const float& fValue);
        s_str&              operator << (const double& dValue);
        s_str&              operator << (const bool& bValue);
        s_str&              operator << (const string_element& cValue);
        s_str&              operator << (const void* pValue);
        s_str&              operator << (const s_int& iValue);
        s_str&              operator << (const s_uint& uiValue);
        s_str&              operator << (const s_float& fValue);
        s_str&              operator << (const s_double& dValue);
        s_str&              operator << (const s_bool& bValue);
        s_str&              operator << (const IntegerConversionType& mIntConvType);
        s_str&              operator << (const BoolConversionType& mBoolConvType);

        s_ctnr<s_str>       operator ,  (const s_str& sValue) const;

        static string_element cDummy;
        static const s_str CLASS_NAME;

        /// Makes the provided string lower case.
        /** \param sValue The string to modify
        *   \return The lower cased string
        */
        static s_str ToLower(const s_str& sValue);

        /// Makes the provided string UPPER CASE.
        /** \param sValue The string to modify
        *   \return The upper cased string
        */
        static s_str ToUpper(const s_str& sValue);

        // For STL iteration macros compatibility :
        class iterator
        {
        public :

            iterator();
            iterator(s_str* pParent, const s_uint& uiPos);

            string_element& operator * ();
            iterator  operator + (const s_int& iOffset) const;
            iterator  operator + (const s_uint& uiOffset) const;
            iterator  operator - (const s_int& iOffset) const;
            iterator  operator - (const s_uint& uiOffset) const;
            s_uint    operator - (const iterator& iter) const;
            iterator& operator ++ ();
            iterator  operator ++ (int);
            iterator& operator -- ();
            iterator  operator -- (int);
            s_bool iterator::operator != (iterator iter);

        private :

            s_str* pParent_;
            s_uint uiPos_;

        };

        class const_iterator
        {
        public :

            const_iterator();
            const_iterator(const s_str* pParent, const s_uint& uiPos);

            const string_element& operator * ();
            const_iterator  operator + (const s_int& iOffset) const;
            const_iterator  operator + (const s_uint& uiOffset) const;
            const_iterator  operator - (const s_int& iOffset) const;
            const_iterator  operator - (const s_uint& uiOffset) const;
            s_uint          operator - (const const_iterator& iter) const;
            const_iterator& operator ++ ();
            const_iterator  operator ++ (int);
            const_iterator& operator -- ();
            const_iterator  operator -- (int);
            s_bool const_iterator::operator != (const_iterator iter);

        private :

            const s_str* pParent_;
            s_uint uiPos_;

        };

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;

    private :

        string_object         sValue_;
        IntegerConversionType mIntConvType_;
        BoolConversionType    mBoolConvType_;

    };

    s_str operator+ (const string_element* sLeft, const s_str& sRight);
    s_str operator+ (const string_element* sLeft, const s_int& iRight);
    s_str operator+ (const string_element* sLeft, const s_uint& uiRight);
    s_str operator+ (const string_element* sLeft, const s_float& fRight);
    s_str operator+ (const string_element* sLeft, const s_double& dRight);
    s_str operator+ (const string_element* sLeft, const s_bool& bRight);

    #ifdef USE_UNICODE
        s_str operator+ (const char* sLeft, const s_str& sRight);
        s_str operator+ (const char* sLeft, const s_int& iRight);
        s_str operator+ (const char* sLeft, const s_uint& uiRight);
        s_str operator+ (const char* sLeft, const s_float& fRight);
        s_str operator+ (const char* sLeft, const s_double& dRight);
        s_str operator+ (const char* sLeft, const s_bool& bRight);
    #endif
}

#endif
