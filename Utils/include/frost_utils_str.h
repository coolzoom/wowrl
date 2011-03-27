// Warning : If you need to use this file, include frost_utils_types.h
#include <string>

namespace Frost
{
    /// Base type : string
    /** Frost's base types are made to allow simpler
    *   manipulation of numbers, booleans and strings.
    *   They simplify conversions (mostly to string),
    *   allow easy access to math/string functions and
    *   implements new functions that are not present
    *   in the standard libraries.<br>
    *   <b>Note</b> : Most of Frost's base types are
    *   slower than C++ base types. The difference is
    *   negligible for small operations, but if you need
    *   to do a lot of calculations it can bloat your
    *   performances. Use prebuilt types in this case.<br>
    *   <b>Note 2</b> : This class is almost as efficient
    *   as the standard std::string.
    */
    template<class T>
    class s_str_t
    {
    public :

        typedef T                                 character;
        typedef typename TypeTraits<T>::FrostType frost_character;
        typedef typename std::basic_string<T>     string;
        typedef typename string::iterator         iterator;
        typedef typename string::const_iterator   const_iterator;
        typedef s_range<iterator>                 range;
        typedef s_range<const_iterator>           const_range;

        /// Constructor.
        s_str_t();

        /// Copy constructor.
        /** \param s The string to copy
        */
        s_str_t(const s_str_t& s);

        /// Constructor.
        /** \param s The string to copy
        */
        s_str_t(const string& s);

        /// Constructor.
        /** \param s The character array to copy
        */
        s_str_t(const character* s);

        /// Constructor.
        /** \param s The character array to copy
        */
        s_str_t(character* s);

        /// Constructor.
        /** \param c The character to copy
        */
        s_str_t(const character& c);

        /// Constructor.
        /** \param c The character to copy
        */
        s_str_t(const frost_character& c);

        /// Creates "uiNbr" copies of the provided string.
        /** \param s     The string to copy
        *   \param uiNbr The number of times to copy the string
        *   \note If uiNbr equals 0, creates an empty string.
        */
        s_str_t(const s_str_t& s, const s_uint_t<default_uint>& uiNbr);

        /// Creates "uiNbr" copies of the provided character.
        /** \param c     The character to copy
        *   \param uiNbr The number of times to copy the character
        *   \note If uiNbr equals 0, creates an empty string.
        */
        s_str_t(const character& c, const s_uint_t<default_uint>& uiNbr);

        /// Creates "uiNbr" copies of the provided character.
        /** \param c     The character to copy
        *   \param uiNbr The number of times to copy the character
        *   \note If uiNbr equals 0, creates an empty string.
        */
        s_str_t(const frost_character& c, const s_uint_t<default_uint>& uiNbr);

        /// Generic constructor (only supports string -> string conversion).
        /** \param mValue    The value to convert
        *   \param mEnableIf Template trick to enable this constructor for convertible
        *                    types only, don't bother with it
        *   \note Calls StringConverter::Convert().
        */
        template<class N>
        s_str_t(const N& mValue, typename StringConverter<character, N>::IsDefined* mEnableIf = 0)
        {
            sValue_ = StringConverter<character, N>::Construct(mValue);
        }

        /// Changes the case of the first character.
        /** \param bCapitalStart If 'true', the first character will be
        *                        capitalized.
        */
        void CapitalStart(const s_bool& bCapitalStart);

        /// Removes the content of the string.
        void Clear();

        /// Cuts this string everytime the delineator is found.
        /** \param sDelim            The delineator
        *   \return A vector containing the separated strings
        *   \note If the delineator is not found in the string, this function
        *         returns the whole string, unchanged.
        *   \note Removes the delineator from the sub-strings.<br>
        *         This function <b>groups</b> occurences of the delineator.
        */
        s_ctnr<s_str_t> Cut(const s_str_t& sDelim) const;

        /// Cuts this string everytime the delineator is found.
        /** \param sDelim            The delineator
        *   \param uiMaxCut          The maximum number of occurences.
        *   \return A vector containing the separated strings
        *   \note If the delineator is not found in the string, this function
        *         returns the whole string, unchanged.
        *   \note Removes the delineator from the sub-strings.<br>
        *         This function <b>groups</b> occurences of the delineator.
        */
        s_ctnr<s_str_t> Cut(const s_str_t& sDelim, const s_uint_t<default_uint>& uiMaxCut) const;

        /// Cuts this string everytime the delineator is found.
        /** \param sDelim            The delineator
        *   \return A vector containing the separated strings
        *   \note If the delineator is not found in the string, this function
        *         returns the whole string, unchanged.
        *   \note Removes the delineator from the sub-strings.<br>
        *         This function cuts the string <b>each time</b> the delineator
        *         is found.
        */
        s_ctnr<s_str_t> CutEach(const s_str_t& sDelim) const;

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
        s_ctnr<s_str_t> CutEach(const s_str_t& sDelim, const s_uint_t<default_uint>& uiMaxCut) const;

        /// Cuts this string everytime a delineator is found.
        /** \param lDelims The list of delineators (single characters)
        *   \return A vector containing the separated strings
        *   \note If no delineator is found in the string, this function
        *         returns the whole string, unchanged.
        *   \note Removes delineators from the sub-strings.<br>
        *         This function <b>groups</b> occurences of any delineator.
        *         Unlike the other versions, this function only supports
        *         cutting by *single* characters.
        */
        s_ctnr<s_str_t> Cut(const s_ctnr<character>& lDelims) const;

        /// Counts the number of time a certain pattern is found on the string.
        /** \param sPattern The string to search for
        *   \return The number of time this pattern is found
        */
        s_uint_t<default_uint> CountOccurences(const s_str_t& sPattern) const;

        /// Removes a certain number of character from a given position.
        /** \param uiStart From where to start erasing
        *   \param uiNbr   The number of character to erase
        */
        void Erase(const s_uint_t<default_uint>& uiStart, const s_uint_t<default_uint>& uiNbr);

        /// Removes a character.
        /** \param uiPos The character to erase
        */
        void Erase(const s_uint_t<default_uint>& uiPos);

        /// Removes a character.
        /** \param iter An iterator pointing at the caracter to erase
        */
        iterator Erase(iterator iter);

        /// Removes a certain number of character from the end of the string.
        /** \param uiNbr The number of character to erase
        */
        void EraseFromEnd(const s_uint_t<default_uint>& uiNbr);

        /// Removes a certain number of character from the beginning of the string.
        /** \param uiNbr The number of character to erase
        */
        void EraseFromStart(const s_uint_t<default_uint>& uiNbr);

        /// Removes all characters between the provided positions.
        /** \param uiStart From where to start erasing
        *   \param uiEnd   From where to end (exclusive)
        */
        void EraseRange(const s_uint_t<default_uint>& uiStart, const s_uint_t<default_uint>& uiEnd);

        /// Removes all characters between the provided positions.
        /** \param iterStart From where to start erasing
        *   \param iterEnd   From where to end (exclusive)
        */
        iterator EraseRange(iterator iterStart, iterator iterEnd);

        /// Removes all characters between the provided positions.
        /** \param mRange The range to erase
        */
        iterator EraseRange(range mRange);

        /// Returns a certain number of character from a given position.
        /** \param uiStart From where to start reading
        *   \param uiNbr   The number of character to read
        *   \return The resulting string
        */
        s_str_t Extract(const s_uint_t<default_uint>& uiStart, const s_uint_t<default_uint>& uiNbr = s_uint_t<default_uint>::INF) const;

        /// Returns all characters between the provided positions.
        /** \param uiStart From where to start reading
        *   \param uiEnd   From where to end (exclusive)
        *   \return The resulting string
        */
        s_str_t ExtractRange(const s_uint_t<default_uint>& uiStart, const s_uint_t<default_uint>& uiEnd) const;

        /// Returns the position of the pattern in the string.
        /** \param sValue  The string to search for
        *   \return An iterator pointing at the pattern (End() if not found)
        */
        iterator Get(const s_str_t& sValue);

        /// Returns the position of the pattern in the string.
        /** \param sValue  The string to search for
        *   \param uiStart From where to start searching
        *   \return An iterator pointing at the pattern (End() if not found)
        */
        iterator Get(const s_str_t& sValue, const s_uint_t<default_uint>& uiStart);

        /// Returns the position of the pattern in the string.
        /** \param sValue  The string to search for
        *   \return The position of the pattern (NaN if not found)
        */
        const_iterator Get(const s_str_t& sValue) const;

        /// Returns the position of the pattern in the string.
        /** \param sValue  The string to search for
        *   \param uiStart From where to start searching
        *   \return The position of the pattern (NaN if not found)
        */
        const_iterator Get(const s_str_t& sValue, const s_uint_t<default_uint>& uiStart) const;

        /// Returns the position of the pattern in the string.
        /** \param sValue  The string to search for
        *   \return The position of the pattern (NaN if not found)
        */
        s_uint_t<default_uint> FindPos(const s_str_t& sValue) const;

        /// Returns the position of the pattern in the string.
        /** \param sValue  The string to search for
        *   \param uiStart From where to start searching
        *   \return The position of the pattern (NaN if not found)
        */
        s_uint_t<default_uint> FindPos(const s_str_t& sValue, const s_uint_t<default_uint>& uiStart) const;

        /// Returns all positions of the pattern in the string.
        /** \param sPattern The string to search for
        *   \return All positions of the pattern (empty if not found)
        */
        s_ctnr< s_uint_t<default_uint> > FindAllPos(const s_str_t& sPattern);

        /// Returns all positions of the pattern in the string.
        /** \param sPattern The string to search for
        *   \param uiStart  From where to start searching
        *   \return All positions of the pattern (empty if not found)
        */
        s_ctnr< s_uint_t<default_uint> > FindAllPos(const s_str_t& sPattern, const s_uint_t<default_uint>& uiStart);

        /// Returns true is the pattern is found in the string.
        /** \param sValue  The string to search for
        *   \return 'true' if the  pattern is found
        */
        s_bool Find(const s_str_t& sValue) const;

        /// Returns true is the pattern is found in the string.
        /** \param sValue  The string to search for
        *   \param uiStart From where to start searching
        *   \return 'true' if the  pattern is found
        */
        s_bool Find(const s_str_t& sValue, const s_uint_t<default_uint>& uiStart) const;

        /// Returns a const reference to the string.
        /** \return A const reference to the string
        */
        inline const string& Get() const { return sValue_; }

        /// Returns a reference to the string.
        /** \return A reference to the string
        */
        inline string& Get() { return sValue_; }

        /// Returns a C-style string.
        /** \return A C-style string
        */
        inline const T* c_str() const { return sValue_.c_str(); }

        /// Checks if the string is empty.
        /** \param bIgnoreSpaces Set to 'true' if you want a string to be reported as
        *                        empty if it only contains white spaces and or tabs
        *   \return 'true' if the string doesn't contain any character
        */
        s_bool IsEmpty(const s_bool& bIgnoreSpaces = false) const;

        /// Checks if the string is a number.
        /** \return 'true' if the string is a number
        */
        s_bool IsNumber() const;

        /// Checks if the string is a bool.
        /** \return 'true' if the string is a bool
        */
        s_bool IsBoolean() const;

        /// Returns the number of character in the string.
        /** \return The number of character in the string
        */
        s_uint_t<default_uint> GetLength() const;

        /// Returns the number of character in ths string.
        /** \return The number of character in the string
        */
        s_uint_t<default_uint> GetSize() const;

        /// Makes this string completely lower case.
        void ToLower();

        /// Makes this string completely UPPER CASE.
        void ToUpper();

        /// Removes the surrounding characters matching the provided pattern.
        /** \param cPattern The character to remove
        *   \return The number of character erased
        */
        s_uint_t<default_uint> Trim(const character& cPattern);

        /// Replaces a pattern by another string.
        /** \param sPattern     The string to search for
        *   \param sReplacement The string that will replace it
        *   \return The number of replacement
        */
        s_uint_t<default_uint> Replace(const s_str_t& sPattern, const s_str_t& sReplacement);

        /// Adds a new character at the end of the string.
        /** \param cChar The character to add
        */
        void PushBack(const character& cChar);

        /// Adds a new string at the end of this string.
        /** \param sValue The string to add
        */
        void PushBack(const s_str_t& sValue);

        /// Removes the last character.
        void PopBack();

        /// Returns a reference to the last character.
        /** \return A reference to the last character
        */
        T& Back();

        /// Returns a const reference to the last character.
        /** \return A const reference to the last character
        */
        const T& Back() const;

        /// Adds a new character at the beginning of the string.
        /** \param cChar The character to add
        */
        void PushFront(const character& cChar);

        /// Adds a new string at the beginning of this string.
        /** \param sValue The string to add
        */
        void PushFront(const s_str_t& sValue);

        /// Removes the first character.
        void PopFront();

        /// Returns a reference to the first character.
        /** \return A reference to the first character
        */
        T& Front();

        /// Returns a const reference to the first character.
        /** \return A const reference to the first character
        */
        const T& Front() const;

        /// Adds a new character somewhere in the string.
        /** \param cChar The character to add
        *   \param uiPos The position at which to insert the char
        */
        void Insert(const character& cChar, const s_uint_t<default_uint>& uiPos);

        /// Adds a new character somewhere in the string.
        /** \param cChar The character to add
        *   \param iter  The position at which to insert the char
        *   \return An iterator pointing after the inserted char
        */
        iterator Insert(const character& cChar, iterator iter);

        /// Adds another string somewhere in this string.
        /** \param sValue The string to insert
        *   \param uiPos The position at which to insert the char
        */
        void Insert(const s_str_t& sValue, const s_uint_t<default_uint>& uiPos);

        /// Adds another string somewhere in this string.
        /** \param sValue The string to insert
        *   \param iter   The position at which to insert the string
        *   \return An iterator pointing at the end of the inserted string
        */
        iterator Insert(const s_str_t& sValue, iterator iter);

        /// Inverts the order of the letters in the string.
        void Reverse();

        /// Checks if this string starts with the provided pattern.
        /** \param sPattern The pattern to search for
        *   \return 'true' if the pattern has been found
        */
        s_bool StartsWith(const s_str_t& sPattern) const;

        /// Checks if this string ends with the provided pattern.
        /** \param sPattern The pattern to search for
        *   \return 'true' if the pattern has been found
        */
        s_bool EndsWith(const s_str_t& sPattern) const;

        iterator begin();
        const_iterator begin() const;

        iterator end();
        const_iterator end() const;

        iterator Begin();
        const_iterator Begin() const;

        iterator End();
        const_iterator End() const;

        T& operator [] (const s_uint_t<default_uint>& uiIndex);
        const T& operator [] (const s_uint_t<default_uint>& uiIndex) const;
        T& operator [] (const default_uint& uiIndex);
        const T& operator [] (const default_uint& uiIndex) const;

        template<class N>
        s_str_t operator + (const N& mValue) const
        {
            return sValue_ + StringConverter<T, N>::Convert(mValue);
        }

        template<class N>
        void operator += (const N& mValue)
        {
            sValue_ += StringConverter<T, N>::Convert(mValue);
        }

        template<class N>
        s_str_t& operator << (const N& mValue)
        {
            sValue_ += StringConverter<T, N>::Convert(mValue);
            return *this;
        }

        s_bool operator == (const s_str_t& mValue) const;

        s_bool operator != (const s_str_t& mValue) const;

        s_bool operator < (const s_str_t& mValue) const;

        s_bool operator > (const s_str_t& mValue) const;

        s_bool operator <= (const s_str_t& mValue) const;

        s_bool operator >= (const s_str_t& mValue) const;

        static const s_str_t EMPTY;

        /// Makes the provided string lower case.
        /** \param sValue The string to modify
        *   \return The lower cased string
        */
        static s_str_t ToLower(const s_str_t& sValue);

        /// Makes the provided string UPPER CASE.
        /** \param sValue The string to modify
        *   \return The upper cased string
        */
        static s_str_t ToUpper(const s_str_t& sValue);

        /// Reverses the provided string.
        /** \param sValue The string to reverse
        *   \return The reversed string
        */
        static s_str_t Reverse(const s_str_t& sValue);

        /// Generic conversion function.
        /** \param mValue The value to convert
        *   \note Calls StringConverter::Convert().
        */
        template<class N>
        static s_str_t Convert(const N& mValue)
        {
            return StringConverter<T, N>::Convert(mValue);
        }

        /// Generic conversion function.
        /** \param mValue  The value to convert
        *   \param mParam1 The first parameter
        */
        template<class N, class M>
        static s_str_t Convert(const N& mValue, const M& mParam1)
        {
            return StringConverter<T, N>::Convert(mValue, mParam1);
        }

        /// Generic conversion function.
        /** \param mValue  The value to convert
        *   \param mParam1 The first parameter
        *   \param mParam2 The second parameter
        */
        template<class N, class M, class O>
        static s_str_t Convert(const N& mValue, const M& mParam1, const O& mParam2)
        {
            return StringConverter<T, N>::Convert(mValue, mParam1, mParam2);
        }

        /// Checks if the provided character is a number.
        /** \param cValue The character to test
        *   \return 'true' if the provided character is a number
        */
        static s_bool IsNumber(const character& cValue);

        /// Checks if the provided character is a number.
        /** \param cValue The character to test
        *   \return 'true' if the provided character is a number
        */
        static s_bool IsNumber(const frost_character& cValue);

        /// Checks if the provided character is a letter or a number.
        /** \param cValue The character to test
        *   \return 'true' if the provided character is a letter or a number
        */
        static s_bool IsAlphaNumeric(const character& cValue);

        /// Checks if the provided character is a letter or a number.
        /** \param cValue The character to test
        *   \return 'true' if the provided character is a letter or a number
        */
        static s_bool IsAlphaNumeric(const frost_character& cValue);

    private :

        string sValue_;

    };

    template<class T, class N>
    s_str_t<T> operator + (const T* sLeft, const s_str_t<N>& sRight)
    {
        return s_str_t<T>(sLeft) + s_str_t<T>::Convert(sRight);
    }

    template<class T, class N>
    s_str_t<T> operator + (const T* sLeft, const N& mRight)
    {
        return s_str_t<T>(sLeft) + s_str_t<T>::Convert(mRight);
    }

    template<class T, class N>
    s_str_t<T> operator + (const N& mLeft, const T* sRight)
    {
        return s_str_t<T>::Convert(mLeft) + s_str_t<T>(sRight);
    }

    /*template<class T, class N>
    s_str_t<T> operator + (const std::basic_string<T> sLeft, const N& mRight)
    {
        return s_str_t<T>(sLeft) + s_str_t<T>::Convert(mRight);
    }
    template<class T, class N>
    s_str_t<T> operator + (const N& mLeft, const std::basic_string<T> sRight)
    {
        return s_str_t<T>::Convert(mLeft) + s_str_t<T>(sRight);
    }
    */

    template<class T, class N>
    s_str_t<T> operator + (const N& mLeft, const s_str_t<T>& sRight)
    {
        s_str_t<T> s = s_str_t<T>::Convert(mLeft);
        s.PushBack(sRight);
        return s;
    }

    /** \cond NOT_REMOVE_FROM_DOC
    */
    template<> class TypeTraits<s_str_t<string_element>::string>
    {
    public :

        typedef s_str_t<string_element>::string string_object;

        typedef string_object           Type;
        typedef string_object           BaseType;
        typedef s_str_t<string_element> FrostType;
        typedef string_element          CharType;
        typedef string_object&          RefType;
        typedef const string_object&    CRefType;
        typedef string_object*          PointerType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };
    template<> class TypeTraits<const string_element*>
    {
    public :
        typedef const string_element*          Type;
        typedef const string_element*          BaseType;
        typedef s_str_t<string_element>        FrostType;
        typedef string_element                 CharType;
        typedef const string_element*&         RefType;
        typedef string_element const * const & CRefType;
        typedef const string_element**         PointerType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };
    template<> class TypeTraits<string_element*>
    {
    public :
        typedef string_element*          Type;
        typedef string_element*          BaseType;
        typedef s_str_t<string_element>  FrostType;
        typedef string_element           CharType;
        typedef string_element*&         RefType;
        typedef string_element * const & CRefType;
        typedef string_element**         PointerType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };
    template<int N> class TypeTraits<const string_element[N]>
    {
    public :
        typedef const string_element*          Type;
        typedef const string_element*          BaseType;
        typedef s_str_t<string_element>        FrostType;
        typedef string_element                 CharType;
        typedef const string_element*&         RefType;
        typedef string_element const * const & CRefType;
        typedef const string_element**         PointerType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };
    template<int N> class TypeTraits<string_element[N]>
    {
    public :
        typedef string_element*         Type;
        typedef string_element*         BaseType;
        typedef s_str_t<string_element> FrostType;
        typedef string_element          CharType;
        typedef string_element*&        RefType;
        typedef string_element* const & CRefType;
        typedef string_element**        PointerType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };

    template<> class TypeTraits< std::basic_string<uint> >
    {
    public :
        typedef std::basic_string<uint>        Type;
        typedef std::basic_string<uint>        BaseType;
        typedef s_str_t<uint>                  FrostType;
        typedef uint                           CharType;
        typedef std::basic_string<uint>&       RefType;
        typedef const std::basic_string<uint>& CRefType;
        typedef std::basic_string<uint>*       PointerType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };
    template<> class TypeTraits<const uint*>
    {
    public :
        typedef const uint*          Type;
        typedef const uint*          BaseType;
        typedef s_str_t<uint>        FrostType;
        typedef uint                 CharType;
        typedef const uint*&         RefType;
        typedef uint const * const & CRefType;
        typedef const uint**         PointerType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };
    template<> class TypeTraits<uint*>
    {
    public :
        typedef uint*          Type;
        typedef uint*          BaseType;
        typedef s_str_t<uint>  FrostType;
        typedef uint           CharType;
        typedef uint*&         RefType;
        typedef uint * const & CRefType;
        typedef uint**         PointerType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };
    template<int N> class TypeTraits<const uint[N]>
    {
    public :
        typedef const uint*          Type;
        typedef const uint*          BaseType;
        typedef s_str_t<uint>        FrostType;
        typedef uint                 CharType;
        typedef const uint*&         RefType;
        typedef uint const * const & CRefType;
        typedef const uint**         PointerType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };
    template<int N> class TypeTraits<uint[N]>
    {
    public :
        typedef uint*         Type;
        typedef uint*         BaseType;
        typedef s_str_t<uint> FrostType;
        typedef uint          CharType;
        typedef uint*&        RefType;
        typedef uint* const & CRefType;
        typedef uint**        PointerType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };

    template<class T> class TypeTraits< s_str_t<T> >
    {
    public :
        typedef s_str_t<T>           Type;
        typedef std::basic_string<T> BaseType;
        typedef s_str_t<T>           FrostType;
        typedef T                    CharType;
        typedef s_str_t<T>&          RefType;
        typedef const s_str_t<T>&    CRefType;
        typedef s_str_t<T>*          PointerType;

        static inline typename TypeTraits<BaseType>::RefType  GetValue(RefType m)  { return m.Get(); }
        static inline typename TypeTraits<BaseType>::CRefType GetValue(CRefType m) { return m.Get(); }
    };
    /** \endcond
    */
}
