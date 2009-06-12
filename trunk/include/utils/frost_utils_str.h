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
*   directive USE_UNICODE.<br>
*   <b>Note</b> : Most of Frost's base types are
*   slower than C++ base types. The difference is
*   negligible for small operations, but if you need
*   to do a lot of calculations (matrices, ...) it
*   can bloat your performances. Use prebuilt types
*   in this case.<br>
*   <b>Note 2</b> : This class is almost as efficient
*   as the standard std::string.
*/
template<class T>
class s_str_t
{
public :

    typedef typename string_object::iterator iterator;
    typedef typename string_object::const_iterator const_iterator;

    /// Constructor.
    s_str_t()
    {
        sValue_ = STRING("");
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
    }

    /// Constructor.
    /** \param sValue The string to use
    */
    s_str_t(const string_object& sValue)
    {
        sValue_ = sValue;
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
    }

    /// Constructor.
    /** \param sValue The string to use
    */
    s_str_t(const string_element* sValue)
    {
        sValue_ = sValue;
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
    }

    #ifdef USE_UNICODE
        /// Constructor.
        /** \param sValue The string to use
        */
        s_str_t(const std::string& sValue)
        {
            const uchar* str = reinterpret_cast<const uchar*>(sValue.c_str());
            sValue_ = std::wstring(str, str + sValue.size());
            mIntConvType_ = CONV_DECIMAL;
            mBoolConvType_ = CONV_TRUE_FALSE;
        }

        /// Constructor.
        /** \param sValue The string to use
        */
        s_str_t(const char* sValue)
        {
            std::string s = sValue;
            const uchar* str = reinterpret_cast<const uchar*>(s.c_str());
            sValue_ = std::wstring(str, str + s.size());
            mIntConvType_ = CONV_DECIMAL;
            mBoolConvType_ = CONV_TRUE_FALSE;
        }
    #endif

    /// s_int_t conversion constructor.
    /** \param iValue The s_int_t to convert
    */
    template<class N>
    explicit s_str_t(const s_int_t<N>& iValue)
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = STRING("");
        this->operator<<(iValue);
    }

    /// s_int_t conversion constructor.
    /** \param iValue    The s_int_t to convert
    *   \param uiCharNbr The minimum number of character
    *                    allowed (fills with zeros)
    */
    template<class N>
    explicit s_str_t(const s_int_t<N>& iValue, const s_uint& uiCharNbr)
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = STRING("");
        this->operator<<(iValue);

        while (sValue_.length() < uiCharNbr.Get())
            sValue_ = STRING('0') + sValue_;
    }

    /// s_uint_t conversion constructor.
    /** \param uiValue The s_uint_t to convert
    */
    template<class N>
    explicit s_str_t(const s_uint_t<N>& uiValue)
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = STRING("");
        this->operator<<(uiValue);
    }

    /// s_uint_t conversion constructor.
    /** \param uiValue   The s_uint_t to convert
    *   \param uiCharNbr The minimum number of character
    *                    allowed (fills with zeros)
    */
    template<class N>
    explicit s_str_t(const s_uint_t<N>& uiValue, const s_uint& uiCharNbr)
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = STRING("");
        this->operator<<(uiValue);

        while (sValue_.length() < uiCharNbr.Get())
            sValue_ = STRING('0') + sValue_;
    }

    /// s_float_t conversion constructor.
    /** \param fValue The s_float_t to convert
    */
    template<class N>
    explicit s_str_t(const s_float_t<N>& fValue)
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = STRING("");
        this->operator<<(fValue);
    }

    /// s_float conversion constructor.
    /** \param fValue    The s_float to convert
    *   \param uiCharNbr The minimum number of character
    *                    allowed (fills with zeros)
    */
    template<class N>
    explicit s_str_t(const s_float_t<N>& fValue, const s_uint& uiCharNbr)
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = STRING("");
        this->operator<<(fValue);

        while (sValue_.length() < uiCharNbr.Get())
            sValue_ = STRING('0') + sValue_;
    }

    /// s_bool conversion constructor.
    /** \param bValue The s_bool to convert
    */
    template<class N>
    explicit s_str_t(const s_bool_t<N>& bValue)
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = STRING("");
        this->operator<<(bValue);
    }

    /// char/wchar_t conversion constructor.
    /** \param cValue The char/wchar_t to convert
    */
    explicit s_str_t(const string_element& cValue)
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = STRING("");
        this->operator+=(cValue);
    }

    /// char/wchar_t conversion constructor.
    /** \param cValue    The char/wchar_t to convert
    *   \param uiCharNbr The number of time to copy this
    *                    character
    */
    explicit s_str_t(const string_element& cValue, const s_uint& uiCharNbr)
    {
        sValue_ = string_object(uiCharNbr.Get(), cValue);
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
    }

    /// Changes the case of the first character.
    /** \param bCapitalStart If 'true', the first character will be
    *                        capitalized.
    */
    void CapitalStart(const s_bool& bCapitalStart)
    {
        string_element cFirst = (*this)[0];
        if (bCapitalStart)
            (*this)[0] = toupper(cFirst);
        else
            (*this)[0] = tolower(cFirst);
    }

    /// Removes the content of the string.
    void Clear()
    {
        sValue_.clear();
    }

    /// Cuts this string everytime the delineator is found.
    /** \param sDelim            The delineator
    *   \param uiMaxCut          The maximum number of occurences.
    *   \return A vector containing the separated strings
    *   \note If the delineator is not found in the string, this function
    *         returns the whole string, unchanged.
    *   \note Removes the delineator from the sub-strings.<br>
    *         This function <b>groups</b> occurences of the delineator.
    */
    s_ctnr<s_str_t> Cut(const s_str_t& sDelim, const s_uint& uiMaxCut = s_uint::INF) const
    {
        s_ctnr<s_str_t> lPieces;
        s_uint uiPos = FindPos(sDelim);
        s_uint uiLastPos;
        s_uint uiCount;
        s_uint uiCurSize;
        while (uiPos.IsValid())
        {
            uiCurSize = uiPos - uiLastPos;
            if (!uiCurSize.IsNull())
                lPieces.PushBack(Extract(uiLastPos, uiCurSize));
            uiLastPos = uiPos + sDelim.Length();
            uiPos = FindPos(sDelim, uiLastPos);
            ++uiCount;

            if (uiCount >= uiMaxCut)
                break;
        }

        lPieces.PushBack(Extract(uiLastPos));

        return lPieces;
    }

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
    s_ctnr<s_str_t>  CutEach(const s_str_t& sDelim, const s_uint& uiMaxCut = s_uint::INF) const
    {
        s_ctnr<s_str_t> lPieces;
        s_uint uiPos = FindPos(sDelim);
        s_uint uiLastPos;
        s_uint uiCount;
        s_uint uiCurSize;
        while (uiPos.IsValid())
        {
            uiCurSize = uiPos - uiLastPos;
            lPieces.PushBack(Extract(uiLastPos, uiCurSize));
            uiLastPos = uiPos + sDelim.Length();
            uiPos = FindPos(sDelim, uiLastPos);
            ++uiCount;

            if (uiCount >= uiMaxCut)
                break;
        }

        lPieces.PushBack(Extract(uiLastPos));

        return lPieces;
    }

    /// Counts the number of time a certain pattern is found on the string.
    /** \param sPattern The string to search for
    *   \return The number of time this pattern is found
    */
    s_uint CountOccurences(const s_str_t& sPattern) const
    {
        s_uint uiCount;
        s_uint uiPos = FindPos(sPattern);
        while (uiPos.IsValid())
        {
            ++uiCount;
            ++uiPos;
            uiPos = FindPos(sPattern, uiPos);
        }

        return uiCount;
    }

    /// Removes a certain number of character from a given position.
    /** \param uiStart From where to start erasing
    *   \param uiNbr   The number of character to erase
    */
    void Erase(const s_uint& uiStart = 0u, const s_uint& uiNbr = s_uint::INF)
    {
        if (!uiNbr.IsValid())
            sValue_.erase(uiStart.Get(), string_object::npos);
        else
            sValue_.erase(uiStart.Get(), uiNbr.Get());
    }

    /// Removes a certain number of character from the end of the string.
    /** \param uiNbr The number of character to erase
    */
    void EraseFromEnd(const s_uint& uiNbr)
    {
        if (uiNbr.IsValid())
            sValue_.erase((Length()-uiNbr).Get(), uiNbr.Get());
    }

    /// Removes a certain number of character from the beginning of the string.
    /** \param uiNbr The number of character to erase
    */
    void EraseFromStart(const s_uint& uiNbr)
    {
        if (uiNbr.IsValid())
            sValue_.erase(0, uiNbr.Get());
    }

    /// Removes all characters between the provided positions.
    /** \param uiStart From where to start erasing
    *   \param uiEnd   From where to end (inclusive)
    */
    void EraseRange(const s_uint& uiStart = 0u, const s_uint& uiEnd = s_uint::INF)
    {
        if (!uiEnd.IsValid())
            sValue_.erase(uiStart.Get(), string_object::npos);
        else
            sValue_.erase(uiStart.Get(), (uiEnd-uiStart).Get());
    }

    /// Returns a certain number of character from a given position.
    /** \param uiStart From where to start reading
    *   \param uiNbr   The number of character to read
    *   \return The resulting string
    */
    s_str_t Extract(const s_uint& uiStart = 0u, const s_uint& uiNbr = s_uint::INF) const
    {
        if (!uiNbr.IsValid())
            return s_str_t(sValue_.substr(uiStart.Get(), string_object::npos));
        else
            return s_str_t(sValue_.substr(uiStart.Get(), uiNbr.Get()));
    }

    /// Returns all characters between the provided positions.
    /** \param uiStart From where to start reading
    *   \param uiEnd   From where to end (inlusive)
    *   \return The resulting string
    */
    s_str_t ExtractRange(const s_uint& uiStart = 0u, const s_uint& uiEnd = s_uint::INF) const
    {
        if (!uiEnd.IsValid())
            return s_str_t(sValue_.substr(uiStart.Get(), string_object::npos));
        else
            return s_str_t(sValue_.substr(uiStart.Get(), (uiEnd-uiStart).Get()));
    }

    /// Returns the position of the pattern in the string.
    /** \param sValue  The string to search for
    *   \param uiStart From where to start searching
    *   \return The position of the pattern (NaN if not found)
    */
    iterator FindIter(const s_str_t& sValue, const s_uint& uiStart = 0u)
    {
        if (uiStart.IsValid())
        {
            // string::npos is an int for Linux, and an uint for Windows
            #ifdef FROST_LINUX
                int iResult = sValue_.find(sValue.Get(), uiStart.Get());
                if (iResult >= 0)
                    return Begin() + iResult;
            #else
                uint uiResult = sValue_.find(sValue.Get(), uiStart.Get());
                if (uiResult != sValue_.npos)
                    return Begin() + uiResult;
            #endif
        }
        return End();
    }

    /// Returns the position of the pattern in the string.
    /** \param sValue  The string to search for
    *   \param uiStart From where to start searching
    *   \return The position of the pattern (NaN if not found)
    */
    s_uint FindPos(const s_str_t& sValue, const s_uint& uiStart = 0u) const
    {
        if (uiStart.IsValid())
        {
            // string::npos is an int for Linux, and an uint for Windows
            #ifdef FROST_LINUX
                int iResult = sValue_.find(sValue.Get(), uiStart.Get());
                if (iResult >= 0)
                    return (uint)iResult;
            #else
                uint uiResult = sValue_.find(sValue.Get(), uiStart.Get());
                if (uiResult != sValue_.npos)
                    return uiResult;
            #endif
        }
        return s_uint::NaN;
    }

    /// Returns true is the pattern is found in the string.
    /** \param sValue  The string to search for
    *   \param uiStart From where to start searching
    *   \return 'true' if the  pattern is found
    */
    s_bool Find(const s_str_t& sValue, const s_uint& uiStart = 0u) const
    {
        if (uiStart.IsValid())
        {
            // string::npos is an int for Linux, and an uint for Windows
            #ifdef FROST_LINUX
                int iResult = sValue_.find(sValue.Get(), uiStart.Get());
                if (iResult >= 0)
                    return true;
            #else
                uint uiResult = sValue_.find(sValue.Get(), uiStart.Get());
                if (uiResult != sValue_.npos)
                    return true;
            #endif
        }
        return false;
    }

    /// Returns a const reference to the string.
    /** \return A const reference to the string
    */
    inline const string_object& Get() const { return sValue_; }

    /// Returns the string converted to ASCII.
    /** \return The string converted to ASCII
    */
    std::string GetASCII() const
    {
        #ifdef USE_UNICODE
            return std::string(sValue_.c_str(), sValue_.c_str() + sValue_.size());
        #else
            return sValue_;
        #endif
    }

    /// Returns the string converted to Unicode.
    /** \return The string converted to Unicode
    */
    std::wstring GetUnicode() const
    {
        #ifdef USE_UNICODE
            return sValue_;
        #else
            return std::wstring(sValue_.c_str(), sValue_.c_str() + sValue_.size());
        #endif
    }

    /// Returns a C-style string.
    /** \return A C-style string
    */
    inline const string_element* c_str() const
    {
        return sValue_.c_str();
    }

    /// Returns a reference to the string.
    /** \return A reference to the string
    */
    inline string_object& GetR()
    {
        return sValue_;
    }

    /// Returns this string converted to a uint
    /** \return This string converted to a uint
    *   \note Assumes this string is an hexadecimal number : '0F', 'FF', '12A0', ...
    */
    s_uint HexToUInt() const
    {
        uint i;
        string_stream s;
        s << sValue_;
        s >> std::hex >> i;
        return i;
    }

    /// Checks if the string is empty.
    /** \param bIgnoreSpaces Set to 'true' if you want a string to be reported as
    *                        empty if it only contains white spaces and or tabs
    *   \return 'true' if the string doesn't contain any character
    */
    s_bool IsEmpty(const s_bool& bIgnoreSpaces = false) const
    {
        if (sValue_.empty())
        {
            return true;
        }
        else
        {
            if (bIgnoreSpaces)
            {
                s_bool bEmpty = true;
                for (uint i = 0; i < sValue_.length(); ++i)
                {
                    if ( (sValue_[i] != STRING(' ')) && (sValue_[i] != STRING('	')) )
                    {
                        bEmpty = false;
                        break;
                    }
                }

                return bEmpty;
            }
            else
            {
                return false;
            }
        }
    }

    /// Checks if the string is a number.
    /** \return 'true' if the string is a number
    */
    s_bool IsNumber() const
    {
        string_stream mTemp(sValue_);
        double dValue;
        mTemp >> dValue;
        return !mTemp.fail();
    }

    /// Checks if the string is a bool.
    /** \return 'true' if the string is a bool
    */
    s_bool IsBoolean() const
    {
        switch (mBoolConvType_)
        {
            case CONV_TRUE_FALSE :
                return ((sValue_ == STRING("false")) || (sValue_ == STRING("true")));
            case CONV_1_0 :
                return ((sValue_ == STRING("0"))     || (sValue_ == STRING("1")));
            case CONV_YES_NO :
                return ((sValue_ == STRING("no"))    || (sValue_ == STRING("yes")));
            default :
                return false;
        }
    }

    /// Returns the number of character in the string.
    /** \return The number of character in the string
    */
    s_uint Length() const
    {
        return s_uint((uint)sValue_.length());
    }

    /// Returns the number of character in ths string.
    /** \return The number of character in the string
    */
    s_uint Size() const
    {
        return s_uint((uint)sValue_.size());
    }

    /// Makes this string completely lower case.
    void ToLower()
    {
        std::transform(sValue_.begin(), sValue_.end(), sValue_.begin(), ::tolower);
    }

    /// Makes this string completely UPPER CASE.
    void ToUpper()
    {
        std::transform(sValue_.begin(), sValue_.end(), sValue_.begin(), ::toupper);
    }

    /// Removes the surrounding characters matching the provided pattern.
    /** \param cPattern The character to remove
    *   \return The number of character erased
    */
    s_uint Trim(const string_element& cPattern)
    {
        s_uint uiCount;
        while ((*this)[0] == cPattern)
        {
            EraseFromStart(1);
            ++uiCount;
        }
        while ((*this)[Length().Get()-1] == cPattern)
        {
            EraseFromEnd(1);
            ++uiCount;
        }
        return uiCount;
    }

    /// Replaces a pattern by another string.
    /** \param sPattern     The string to search for
    *   \param sReplacement The string that will replace it
    *   \return The number of replacement
    */
    s_uint Replace(const s_str_t& sPattern, const s_str_t& sReplacement)
    {
        s_uint uiCount;
        s_uint uiPos = FindPos(sPattern);

        while (uiPos.IsValid())
        {
            sValue_.replace(uiPos.Get(), sPattern.Length().Get(), sReplacement.Get());
            uiPos = FindPos(sPattern, uiPos+sReplacement.Length());
            ++uiCount;
        }

        return uiCount;
    }

    /// Adds a new character at the end of the string.
    /** \param cChar The character to add
    */
    void PushBack(const string_element& cChar)
    {
        sValue_.push_back(cChar);
    }

    /// Adds a new character at the beginning of the string.
    /** \param cChar The character to add
    */
    void PushFront(const string_element& cChar)
    {
        sValue_.insert(0, 1, cChar);
    }

    /// Adds a new character somewhere in the string.
    /** \param cChar The character to add
    *   \param uiPos The position at which to insert the char
    */
    void Insert(const string_element& cChar, const s_uint& uiPos)
    {
        if (uiPos.IsValid())
            sValue_.insert(uiPos.Get(), 1, cChar);
        else
            sValue_.push_back(cChar);
    }

    template<class N>
    string_element& operator [] (const s_uint_t<N>& uiIndex)
    {
        if ( uiIndex.IsValid() &&  (uiIndex.Get() < sValue_.size()) )
            return sValue_[uiIndex.Get()];
        else
            return cDummy;
    }

    template<class N>
    const string_element& operator [] (const s_uint_t<N>& uiIndex) const
    {
        if ( uiIndex.IsValid() && (uiIndex.Get() < sValue_.size()) )
            return sValue_[uiIndex.Get()];
        else
            return cDummy;
    }

    string_element& operator [] (const uint& uiIndex)
    {
        if (uiIndex < sValue_.size())
            return sValue_[uiIndex];
        else
            return cDummy;
    }

    const string_element& operator [] (const uint& uiIndex) const
    {
        if (uiIndex < sValue_.size())
            return sValue_[uiIndex];
        else
            return cDummy;
    }

    s_str_t operator + (const s_str_t& mValue) const
    {
        return sValue_ + mValue.sValue_;
    }

    s_str_t operator + (const string_element* sValue) const
    {
        return sValue_ + sValue;
    }

    #ifdef USE_UNICODE
        s_str_t operator + (const char* sValue) const
        {
            std::string s = sValue;
            const uchar* str = reinterpret_cast<const uchar*>(s.c_str());
            return sValue_ + std::wstring(str, str + s.size());
        }
    #endif

    s_str_t operator + (const string_element& cValue) const
    {
        string_object sTemp = sValue_;
        sTemp.push_back(cValue);
        return sTemp;
    }

    template <class N>
    s_str_t operator + (const s_int_t<N>& iValue) const
    {
        return *this + s_str_t(iValue);
    }

    template <class N>
    s_str_t operator + (const s_uint_t<N>& uiValue) const
    {
        return *this + s_str_t(uiValue);
    }

    template <class N>
    s_str_t operator + (const s_float_t<N>& fValue) const
    {
        return *this + s_str_t(fValue);
    }

    template <class N>
    s_str_t operator + (const s_bool_t<N>& bValue) const
    {
        return *this + s_str_t(bValue);
    }

    template <class N>
    s_str_t operator - (const s_uint_t<N>& uiNumber) const
    {
        if (uiNumber.Get() < sValue_.size())
        {
            string_object sTemp = sValue_;
            sTemp = sTemp.substr(0, sTemp.size()-uiNumber.Get());
            return sTemp;
        }
        else
            return s_str_t::EMPTY;
    }

    void operator += (const s_str_t& mValue)
    {
        sValue_ += mValue.sValue_;
    }

    void operator += (const string_element& cValue)
    {
        sValue_.push_back(cValue);
    }

    template <class N>
    void operator -= (const s_uint_t<N>& uiNumber)
    {
        if (uiNumber.Get() < sValue_.size())
            sValue_ = sValue_.substr(0, sValue_.size()-uiNumber.Get());
        else
            sValue_ = STRING("");
    }

    s_bool operator == (const s_str_t& mValue) const
    {
        return (sValue_ == mValue.sValue_);
    }

    s_bool operator != (const s_str_t& mValue) const
    {
        return (sValue_ != mValue.sValue_);
    }

    s_bool operator < (const s_str_t& mValue) const
    {
        return (sValue_ < mValue.sValue_);
    }

    s_bool operator > (const s_str_t& mValue) const
    {
        return (sValue_ > mValue.sValue_);
    }

    s_bool operator <= (const s_str_t& mValue) const
    {
        return (sValue_ <= mValue.sValue_);
    }

    s_bool operator >= (const s_str_t& mValue) const
    {
        return (sValue_ >= mValue.sValue_);
    }

    s_str_t& operator << (const s_str_t& mValue)
    {
        sValue_ += mValue.sValue_;
        return *this;
    }

    s_str_t& operator << (const string_element* sValue)
    {
        sValue_ += sValue;
        return *this;
    }

    s_str_t& operator << (const int& iValue)
    {
        string_stream sStream;

        switch (mIntConvType_)
        {
            case CONV_DECIMAL :
            {
                sStream << iValue;
                break;
            }
            case CONV_HEXA :
            {
                sStream << std::hex << iValue;
                break;
            }
            case CONV_BIN :
            {
                // TODO : Implementer la conversion en binaire, si necessaire
                break;
            }
        }

        string_object sValue;
        sStream >> sValue;
        sValue_ += sValue;

        return *this;
    }

    s_str_t& operator << (const uint& uiValue)
    {
        return operator<<(static_cast<int>(uiValue));
    }

    s_str_t& operator << (const float& fValue)
    {
        string_stream sStream;
        sStream << fValue;
        sValue_ += sStream.str();
        return *this;
    }

    s_str_t& operator << (const double& dValue)
    {
        string_stream sStream;
        sStream << dValue;
        sValue_ += sStream.str();
        return *this;
    }

    s_str_t& operator << (const bool& bValue)
    {
        string_object sChoice[2] = { STRING(""), STRING("") };
        switch (mBoolConvType_)
        {
            case CONV_TRUE_FALSE : sChoice[0] = STRING("false"); sChoice[1] = STRING("true"); break;
            case CONV_1_0 : sChoice[0] = STRING("0"); sChoice[1] = STRING("1"); break;
            case CONV_YES_NO : sChoice[0] = STRING("no"); sChoice[1] = STRING("yes"); break;
        }

        sValue_ += sChoice[(int)bValue];
        return *this;
    }

    s_str_t& operator << (const string_element& cValue)
    {
        operator+=(cValue);
        return *this;
    }

    s_str_t& operator << (const void* pValue)
    {
        if (pValue == NULL)
            return operator<<("NULL");
        else
        {
            #ifdef WIN32
                return operator<<(s_uint((uint)pValue));
            #else
                return operator<<(s_uint((uint)(ulong)pValue));
            #endif
        }
    }

    template <class N>
    s_str_t& operator << (const s_int_t<N>& iValue)
    {
        if (iValue.IsValid())
            return operator<<(iValue.Get());
        else
        {
            if (iValue.GetType() == s_int_t<N>::INTEGER_NAN)
                operator<<("nan");
            else if (iValue.GetType() == s_int_t<N>::INTEGER_INF_PLUS)
                operator<<("inf+");
            else if (iValue.GetType() == s_int_t<N>::INTEGER_INF_MINUS)
                operator<<("inf-");

            return *this;
        }
    }

    template <class N>
    s_str_t& operator << (const s_uint_t<N>& uiValue)
    {
        if (uiValue.IsValid())
            return operator<<(uiValue.Get());
        else
        {
            if (uiValue.GetType() == s_uint_t<N>::INTEGER_NAN)
                operator<<("nan");
            else if (uiValue.GetType() == s_uint_t<N>::INTEGER_INF)
                operator<<("inf");

            return *this;
        }
    }

    template <class N>
    s_str_t& operator << (const s_float_t<N>& fValue)
    {
        if (fValue.IsValid())
            return operator<<(fValue.Get());
        else
        {
            if (fValue.GetType() == s_float_t<N>::FLOAT_NAN)
                operator<<("nan");
            else if (fValue.GetType() == s_float_t<N>::FLOAT_INF_PLUS)
                operator<<("inf+");
            else if (fValue.GetType() == s_float_t<N>::FLOAT_INF_MINUS)
                operator<<("inf-");

            return *this;
        }
    }

    template <class N>
    s_str_t& operator << (const s_bool_t<N>& bValue)
    {
        return operator<<(bValue.Get());
    }

    template <class N, int M>
    s_str_t& operator << (const s_array<N, M>& lValue)
    {
        operator<<(STRING("("));
        for (uint i = 0; i < M; ++i)
        {
            if (i == M-1)
                operator<<(lValue[i]);
            else
                operator<<(lValue[i]) << STRING(", ");
        }
        operator<<(STRING(")"));
    }

    template <class N>
    s_str_t& operator << (const s_ctnr<N>& lValue)
    {
        operator<<(STRING("("));
        for (s_uint i = 0; i < lValue.GetSize(); ++i)
        {
            if (i == lValue.GetSize()-s_uint(1u))
                operator<<(lValue[i]);
            else
                operator<<(lValue[i]) << STRING(", ");
        }
        operator<<(STRING(")"));
    }

    s_str_t& operator << (const IntegerConversionType& mIntConvType)
    {
        mIntConvType_ = mIntConvType;
        return *this;
    }

    s_str_t& operator << (const BoolConversionType& mBoolConvType)
    {
        mBoolConvType_ = mBoolConvType;
        return *this;
    }

    s_ctnr<s_str_t> operator , (const s_str_t& sValue) const
    {
        s_ctnr<s_str_t> mContainer;
        mContainer.PushBack(*this);
        mContainer.PushBack(sValue);
        return mContainer;
    }

    static const s_str_t EMPTY;

    static string_element cDummy;

    /// Makes the provided string lower case.
    /** \param sValue The string to modify
    *   \return The lower cased string
    */
    static s_str_t ToLower(const s_str_t& sValue)
    {
        s_str_t sCopy = sValue;
        sCopy.ToLower();
        return sCopy;
    }

    /// Makes the provided string UPPER CASE.
    /** \param sValue The string to modify
    *   \return The upper cased string
    */
    static s_str_t ToUpper(const s_str_t& sValue)
    {
        s_str_t sCopy = sValue;
        sCopy.ToUpper();
        return sCopy;
    }

    iterator begin()
    {
        return sValue_.begin();
    }

    const_iterator begin() const
    {
        return sValue_.begin();
    }

    iterator end()
    {
        return sValue_.end();
    }
    const_iterator end() const
    {
        return sValue_.end();
    }

    iterator Begin()
    {
        return sValue_.begin();
    }

    const_iterator Begin() const
    {
        return sValue_.begin();
    }

    iterator End()
    {
        return sValue_.end();
    }

    const_iterator End() const
    {
        return sValue_.end();
    }

private :

    string_object         sValue_;
    IntegerConversionType mIntConvType_;
    BoolConversionType    mBoolConvType_;

};

template<class T>
string_element s_str_t<T>::cDummy = STRING('\0');
template<class T>
const s_str_t<T> s_str_t<T>::EMPTY = "";

template<class T>
s_str_t<T> operator + (const string_element* sLeft, const s_str_t<T>& sRight)
{
    return s_str_t<T>(sLeft) + sRight;
}

#ifdef USE_UNICODE
    template<class T>
    s_str_t<T> operator + (const char* sLeft, const s_str_t<T>& sRight)
    {
        return s_str_t<T>(sLeft) + sRight;
    }
#endif

typedef s_str_t<char> s_str;
