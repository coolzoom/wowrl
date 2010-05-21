// Warning : If you need to use this file, include frost_utils_types.h
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

        typedef typename std::basic_string<T>   string;
        typedef typename string::iterator       iterator;
        typedef typename string::const_iterator const_iterator;
        typedef s_range<iterator>               range;
        typedef s_range<const_iterator>         const_range;

        /// Constructor.
        s_str_t()
        {
        }

        /// Copy constructor.
        /** \param s The string to copy
        */
        s_str_t(const s_str_t& s)
        {
            sValue_ = s.sValue_;
        }

        /// Constructor.
        /** \param s The string to copy
        */
        s_str_t(const string& s)
        {
            sValue_ = s;
        }

        /// Constructor.
        /** \param s The character array to copy
        */
        s_str_t(const T* s)
        {
            sValue_ = s;
        }

        /// Constructor.
        /** \param s The character array to copy
        */
        s_str_t(T* s)
        {
            sValue_ = s;
        }

        /// Constructor.
        /** \param s The character array to copy
        */
        s_str_t(const T& c)
        {
            sValue_.push_back(c);
        }

                /// Constructor.
        /** \param s The character array to copy
        */
        s_str_t(const typename TypeTraits<T>::FrostType& c)
        {
            sValue_.push_back(c.Get());
        }

        /// Creates "uiNbr" copies of the provided string.
        /** \param s     The string to copy
        *   \param uiNbr The number of times to copy the string
        *   \note If uiNbr equals 0, creates an empty string.
        */
        s_str_t(const s_str_t& s, const s_uint_t<default_uint>& uiNbr)
        {
            sValue_ = StringConverter<T>::Convert(s, uiNbr);
        }

        /// Creates "uiNbr" copies of the provided character.
        /** \param c     The character to copy
        *   \param uiNbr The number of times to copy the character
        *   \note If uiNbr equals 0, creates an empty string.
        */
        s_str_t(const T& c, const s_uint_t<default_uint>& uiNbr)
        {
            sValue_ = StringConverter<T>::Convert(c, uiNbr);
        }

        /// Creates "uiNbr" copies of the provided character.
        /** \param c     The character to copy
        *   \param uiNbr The number of times to copy the character
        *   \note If uiNbr equals 0, creates an empty string.
        */
        s_str_t(const typename TypeTraits<T>::FrostType& c, const s_uint_t<default_uint>& uiNbr)
        {
            sValue_ = StringConverter<T>::Convert(c, uiNbr);
        }

        /// Generic explicit constructor.
        /** \param mValue The value to convert
        *   \note Calls StringConverter::Convert().
        */
        template<class N>
        s_str_t(const N& mValue, typename StringConverter<T>::template IsDefined<N>::Type* mEnableIf = 0)
        {
            sValue_ = StringConverter<T>::Construct(mValue);
        }

        /// Changes the case of the first character.
        /** \param bCapitalStart If 'true', the first character will be
        *                        capitalized.
        */
        void CapitalStart(const s_bool& bCapitalStart)
        {
            T cFirst = (*this)[0];
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
        *   \return A vector containing the separated strings
        *   \note If the delineator is not found in the string, this function
        *         returns the whole string, unchanged.
        *   \note Removes the delineator from the sub-strings.<br>
        *         This function <b>groups</b> occurences of the delineator.
        */
        s_ctnr<s_str_t> Cut(const s_str_t& sDelim) const
        {
            s_ctnr<s_str_t> lPieces;
            s_uint_t<default_uint> uiPos = FindPos(sDelim);
            s_uint_t<default_uint> uiLastPos;
            s_uint_t<default_uint> uiCount;
            s_uint_t<default_uint> uiCurSize;
            while (uiPos.IsValid())
            {
                uiCurSize = uiPos - uiLastPos;
                if (!uiCurSize.IsNull())
                    lPieces.PushBack(Extract(uiLastPos, uiCurSize));
                uiLastPos = uiPos + sDelim.GetLength();
                uiPos = FindPos(sDelim, uiLastPos);
                ++uiCount;
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
        *         This function <b>groups</b> occurences of the delineator.
        */
        s_ctnr<s_str_t> Cut(const s_str_t& sDelim, const s_uint_t<default_uint>& uiMaxCut) const
        {
            s_ctnr<s_str_t> lPieces;
            s_uint_t<default_uint> uiPos = FindPos(sDelim);
            s_uint_t<default_uint> uiLastPos;
            s_uint_t<default_uint> uiCount;
            s_uint_t<default_uint> uiCurSize;
            while (uiPos.IsValid())
            {
                uiCurSize = uiPos - uiLastPos;
                if (!uiCurSize.IsNull())
                    lPieces.PushBack(Extract(uiLastPos, uiCurSize));
                uiLastPos = uiPos + sDelim.GetLength();
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
        *   \return A vector containing the separated strings
        *   \note If the delineator is not found in the string, this function
        *         returns the whole string, unchanged.
        *   \note Removes the delineator from the sub-strings.<br>
        *         This function cuts the string <b>each time</b> the delineator
        *         is found.
        */
        s_ctnr<s_str_t> CutEach(const s_str_t& sDelim) const
        {
            s_ctnr<s_str_t> lPieces;
            s_uint_t<default_uint> uiPos = FindPos(sDelim);
            s_uint_t<default_uint> uiLastPos;
            s_uint_t<default_uint> uiCount;
            s_uint_t<default_uint> uiCurSize;
            while (uiPos.IsValid())
            {
                uiCurSize = uiPos - uiLastPos;
                lPieces.PushBack(Extract(uiLastPos, uiCurSize));
                uiLastPos = uiPos + sDelim.GetLength();
                uiPos = FindPos(sDelim, uiLastPos);
                ++uiCount;
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
        template<class N>
        s_ctnr<s_str_t> CutEach(const s_str_t& sDelim, const s_uint_t<default_uint>& uiMaxCut) const
        {
            s_ctnr<s_str_t> lPieces;
            s_uint_t<default_uint> uiPos = FindPos(sDelim);
            s_uint_t<default_uint> uiLastPos;
            s_uint_t<default_uint> uiCount;
            s_uint_t<default_uint> uiCurSize;
            while (uiPos.IsValid())
            {
                uiCurSize = uiPos - uiLastPos;
                lPieces.PushBack(Extract(uiLastPos, uiCurSize));
                uiLastPos = uiPos + sDelim.GetLength();
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
        s_uint_t<default_uint> CountOccurences(const s_str_t& sPattern) const
        {
            s_uint_t<default_uint> uiCount;
            s_uint_t<default_uint> uiPos = FindPos(sPattern);
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
        void Erase(const s_uint_t<default_uint>& uiStart, const s_uint_t<default_uint>& uiNbr = s_uint_t<default_uint>::INF)
        {
            if (uiStart.IsValid())
            {
                if (!uiNbr.IsValid())
                    sValue_.erase(uiStart.Get(), string::npos);
                else
                    sValue_.erase(uiStart.Get(), uiNbr.Get());
            }
        }

        /// Removes a character.
        /** \param uiPos The character to erase
        */
        void Erase(const s_uint_t<default_uint>& uiPos)
        {
            if (uiPos.IsValid())
                sValue_.erase(uiPos.Get(), 1);
        }

        /// Removes a character.
        /** \param iter An iterator pointing at the caracter to erase
        */
        iterator Erase(iterator iter)
        {
            return sValue_.erase(iter);
        }

        /// Removes a certain number of character from the end of the string.
        /** \param uiNbr The number of character to erase
        */
        void EraseFromEnd(const s_uint_t<default_uint>& uiNbr)
        {
            if (uiNbr.IsValid())
                sValue_.erase((GetLength() - uiNbr).Get(), uiNbr.Get());
            else
                sValue_.clear();
        }

        /// Removes a certain number of character from the beginning of the string.
        /** \param uiNbr The number of character to erase
        */
        void EraseFromStart(const s_uint_t<default_uint>& uiNbr)
        {
            if (uiNbr.IsValid())
                sValue_.erase(0, uiNbr.Get());
            else
                sValue_.clear();
        }

        /// Removes all characters between the provided positions.
        /** \param uiStart From where to start erasing
        *   \param uiEnd   From where to end (exclusive)
        */
        void EraseRange(const s_uint_t<default_uint>& uiStart, const s_uint_t<default_uint>& uiEnd)
        {
            if (uiStart.IsValid())
            {
                if (!uiEnd.IsValid())
                    sValue_.erase(uiStart.Get(), string::npos);
                else
                    sValue_.erase(uiStart.Get(), (uiEnd-uiStart).Get());
            }
        }

        /// Removes all characters between the provided positions.
        /** \param iterStart From where to start erasing
        *   \param iterEnd   From where to end (exclusive)
        */
        iterator EraseRange(iterator iterStart, iterator iterEnd)
        {
            return sValue_.erase(iterStart, iterEnd);
        }

        /// Removes all characters between the provided positions.
        /** \param mRange The range to erase
        */
        iterator EraseRange(range mRange)
        {
            return sValue_.erase(mRange.Begin(), mRange.End());
        }

        /// Returns a certain number of character from a given position.
        /** \param uiStart From where to start reading
        *   \param uiNbr   The number of character to read
        *   \return The resulting string
        */
        s_str_t Extract(const s_uint_t<default_uint>& uiStart, const s_uint_t<default_uint>& uiNbr = s_uint_t<default_uint>::INF) const
        {
            if (!uiNbr.IsValid())
                return s_str_t(sValue_.substr(uiStart.Get(), string::npos));
            else
                return s_str_t(sValue_.substr(uiStart.Get(), uiNbr.Get()));
        }

        /// Returns all characters between the provided positions.
        /** \param uiStart From where to start reading
        *   \param uiEnd   From where to end (exclusive)
        *   \return The resulting string
        */
        s_str_t ExtractRange(const s_uint_t<default_uint>& uiStart, const s_uint_t<default_uint>& uiEnd) const
        {
            if (!uiEnd.IsValid())
                return s_str_t(sValue_.substr(uiStart.Get(), string::npos));
            else
                return s_str_t(sValue_.substr(uiStart.Get(), (uiEnd-uiStart).Get()));
        }

        /// Returns the position of the pattern in the string.
        /** \param sValue  The string to search for
        *   \return An iterator pointing at the pattern (End() if not found)
        */
        iterator Get(const s_str_t& sValue)
        {
            // string::npos is an int for Linux, and an uint for Windows
            #ifdef FROST_LINUX
                int iResult = sValue_.find(sValue.Get());
                if (iResult >= 0)
                    return Begin() + iResult;
            #else
                uint uiResult = sValue_.find(sValue.Get());
                if (uiResult != sValue_.npos)
                    return Begin() + uiResult;
            #endif

            return End();
        }

        /// Returns the position of the pattern in the string.
        /** \param sValue  The string to search for
        *   \param uiStart From where to start searching
        *   \return An iterator pointing at the pattern (End() if not found)
        */
        iterator Get(const s_str_t& sValue, const s_uint_t<default_uint>& uiStart)
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
        *   \return The position of the pattern (NaN if not found)
        */
        const_iterator Get(const s_str_t& sValue) const
        {
            // string::npos is an int for Linux, and an uint for Windows
            #ifdef FROST_LINUX
                int iResult = sValue_.find(sValue.Get());
                if (iResult >= 0)
                    return Begin() + iResult;
            #else
                uint uiResult = sValue_.find(sValue.Get());
                if (uiResult != sValue_.npos)
                    return Begin() + uiResult;
            #endif

            return End();
        }

        /// Returns the position of the pattern in the string.
        /** \param sValue  The string to search for
        *   \param uiStart From where to start searching
        *   \return The position of the pattern (NaN if not found)
        */
        const_iterator Get(const s_str_t& sValue, const s_uint_t<default_uint>& uiStart) const
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
        *   \return The position of the pattern (NaN if not found)
        */
        s_uint_t<default_uint> FindPos(const s_str_t& sValue) const
        {
            // string::npos is an int for Linux, and an uint for Windows
            #ifdef FROST_LINUX
                int iResult = sValue_.find(sValue.Get());
                if (iResult >= 0)
                    return (default_uint)iResult;
            #else
                uint uiResult = sValue_.find(sValue.Get());
                if (uiResult != sValue_.npos)
                    return uiResult;
            #endif

            return s_uint_t<default_uint>::NaN;
        }

        /// Returns the position of the pattern in the string.
        /** \param sValue  The string to search for
        *   \param uiStart From where to start searching
        *   \return The position of the pattern (NaN if not found)
        */
        s_uint_t<default_uint> FindPos(const s_str_t& sValue, const s_uint_t<default_uint>& uiStart) const
        {
            if (uiStart.IsValid())
            {
                // string::npos is an int for Linux, and an uint for Windows
                #ifdef FROST_LINUX
                    int iResult = sValue_.find(sValue.Get(), uiStart.Get());
                    if (iResult >= 0)
                        return (default_uint)iResult;
                #else
                    uint uiResult = sValue_.find(sValue.Get(), uiStart.Get());
                    if (uiResult != sValue_.npos)
                        return uiResult;
                #endif
            }
            return s_uint_t<default_uint>::NaN;
        }

        /// Returns all positions of the pattern in the string.
        /** \param sPattern The string to search for
        *   \param uiStart  From where to start searching
        *   \return All positions of the pattern (empty if not found)
        */
        template<class N>
        s_ctnr< s_uint_t<default_uint> > FindAllPos(const s_str_t& sPattern)
        {
            s_ctnr< s_uint_t<default_uint> > lList;
            s_uint_t<default_uint> ui = FindPos(sPattern);
            while (ui.IsValid())
            {
                lList.PushBack(ui);
                ui = FindPos(sPattern, ui + sPattern.GetSize());
            }

            return lList;
        }

        /// Returns all positions of the pattern in the string.
        /** \param sPattern The string to search for
        *   \param uiStart  From where to start searching
        *   \return All positions of the pattern (empty if not found)
        */
        s_ctnr< s_uint_t<default_uint> > FindAllPos(const s_str_t& sPattern, const s_uint_t<default_uint>& uiStart)
        {
            s_ctnr< s_uint_t<default_uint> > lList;

            if (uiStart.IsValid())
            {
                s_uint_t<default_uint> ui = FindPos(sPattern, uiStart);
                while (ui.IsValid())
                {
                    lList.PushBack(ui);
                    ui = FindPos(sPattern, ui + sPattern.GetSize());
                }
            }

            return lList;
        }

        /// Returns true is the pattern is found in the string.
        /** \param sValue  The string to search for
        *   \return 'true' if the  pattern is found
        */
        s_bool Find(const s_str_t& sValue) const
        {
            // string::npos is an int for Linux, and an uint for Windows
            #ifdef FROST_LINUX
                int iResult = sValue_.find(sValue.Get());
                if (iResult >= 0)
                    return true;
            #else
                uint uiResult = sValue_.find(sValue.Get());
                if (uiResult != sValue_.npos)
                    return true;
            #endif

            return false;
        }

        /// Returns true is the pattern is found in the string.
        /** \param sValue  The string to search for
        *   \param uiStart From where to start searching
        *   \return 'true' if the  pattern is found
        */
        s_bool Find(const s_str_t& sValue, const s_uint_t<default_uint>& uiStart) const
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
        inline const string& Get() const
        {
            return sValue_;
        }

        /// Returns a reference to the string.
        /** \return A reference to the string
        */
        inline string& Get()
        {
            return sValue_;
        }

        /// Returns a C-style string.
        /** \return A C-style string
        */
        inline const T* c_str() const
        {
            return sValue_.c_str();
        }

        /// Returns this string converted to a uint
        /** \return This string converted to a uint
        *   \note Assumes this string is an hexadecimal number : '0F', 'FF', '12A0', ...
        */
        s_uint_t<default_uint> HexToUInt() const
        {
            default_uint i;
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
                        if ( (sValue_[i] != ' ') && (sValue_[i] != '	') )
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
            string_stream mTemp(StringConverter<T>::ConvertToStandard(sValue_));
            double dValue;
            mTemp >> dValue;
            return !mTemp.fail();
        }

        /// Checks if the string is a bool.
        /** \return 'true' if the string is a bool
        */
        s_bool IsBoolean() const
        {
            return (sValue_ == "false") || (sValue_ == "true") ||
                   (sValue_ == "0")     || (sValue_ == "1")    ||
                   (sValue_ == "no")    || (sValue_ == "yes");
        }

        /// Returns the number of character in the string.
        /** \return The number of character in the string
        */
        s_uint_t<default_uint> GetLength() const
        {
            return sValue_.length();
        }

        /// Returns the number of character in ths string.
        /** \return The number of character in the string
        */
        s_uint_t<default_uint> GetSize() const
        {
            return sValue_.size();
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
        s_uint_t<default_uint> Trim(const T& cPattern)
        {
            default_uint uiCount = 0;
            while (!IsEmpty() && (*this)[0] == cPattern)
            {
                EraseFromStart(1);
                ++uiCount;
            }
            while (!IsEmpty() && (*this)[GetLength().Get()-1] == cPattern)
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
        s_uint_t<default_uint> Replace(const s_str_t& sPattern, const s_str_t& sReplacement)
        {
            default_uint uiCount = 0;
            s_uint_t<default_uint> uiPos = FindPos(sPattern);

            while (uiPos.IsValid())
            {
                sValue_.replace(uiPos.Get(), sPattern.GetLength().Get(), sReplacement.Get());
                uiPos = FindPos(sPattern, uiPos + sReplacement.GetLength());
                ++uiCount;
            }

            return uiCount;
        }

        /// Adds a new character at the end of the string.
        /** \param cChar The character to add
        */
        void PushBack(const T& cChar)
        {
            sValue_.push_back(cChar);
        }

        /// Removes the last character.
        void PopBack()
        {
            if (!sValue_.empty())
                sValue_.erase(sValue_.length()-1, 1);
        }

        /// Returns a reference to the last character.
        /** \return A reference to the last character
        */
        T& Back()
        {
            return sValue_[sValue_.length()-1];
        }

        /// Returns a const reference to the last character.
        /** \return A const reference to the last character
        */
        const T& Back() const
        {
            return sValue_[sValue_.length()-1];
        }

        /// Adds a new character at the beginning of the string.
        /** \param cChar The character to add
        */
        void PushFront(const T& cChar)
        {
            sValue_.insert(0, 1, cChar);
        }

        /// Removes the first character.
        void PopFront()
        {
            if (!sValue_.empty())
                sValue_.erase(0, 1);
        }

        /// Returns a reference to the first character.
        /** \return A reference to the first character
        */
        T& Front()
        {
            return sValue_[0];
        }

        /// Returns a const reference to the first character.
        /** \return A const reference to the first character
        */
        const T& Front() const
        {
            return sValue_[0];
        }

        /// Adds a new character somewhere in the string.
        /** \param cChar The character to add
        *   \param uiPos The position at which to insert the char
        */
        void Insert(const T& cChar, const s_uint_t<default_uint>& uiPos)
        {
            if (uiPos.IsValid())
                sValue_.insert(uiPos.Get(), 1, cChar);
            else
                sValue_.push_back(cChar);
        }

        /// Adds a new character somewhere in the string.
        /** \param cChar The character to add
        *   \param iter  The position at which to insert the char
        *   \return An iterator pointing after the inserted char
        */
        iterator Insert(const T& cChar, iterator iter)
        {
            return sValue_.insert(iter, cChar)+1;
        }

        /// Adds another string somewhere in this string.
        /** \param sValue The string to insert
        *   \param uiPos The position at which to insert the char
        */
        void Insert(const s_str_t& sValue, const s_uint_t<default_uint>& uiPos)
        {
            if (uiPos.IsValid())
                sValue_.insert(uiPos.Get(), sValue.Value_);
            else
                sValue_.append(sValue.Value_);
        }

        /// Adds another string somewhere in this string.
        /** \param cChar The string to insert
        *   \param iter  The position at which to insert the string
        *   \return An iterator pointing at the end of the inserted string
        */
        iterator Insert(const s_str_t& sValue, iterator iter)
        {
            if (iter == sValue_.end())
            {
                sValue_.append(sValue.sValue_);
                return sValue_.end();
            }
            else
            {
                size_t uiPos = iter - sValue_.begin();
                sValue_.insert(uiPos, sValue.sValue_);
                return sValue_.begin() + uiPos + sValue.sValue_.size();
            }
        }

        /// Inverts the order of the letters in the string.
        void Reverse()
        {
            std::reverse(sValue_.begin(), sValue_.end());
        }

        /// Checks if this string starts with the provided pattern.
        /** \param sPattern The pattern to search for
        *   \return 'true' if the pattern has been found
        */
        s_bool StartsWith(const s_str_t& sPattern) const
        {
            if (sPattern.GetSize() <= GetSize())
            {
                const_iterator iterSelf = Begin();
                const_iterator iterPattern;
                for (iterPattern = sPattern.Begin(); iterPattern != sPattern.End(); ++iterPattern)
                {
                    if (*iterPattern != *iterSelf)
                        return false;

                    ++iterSelf;
                }

                return true;
            }
            else
                return false;
        }

        /// Checks if this string ends with the provided pattern.
        /** \param sPattern The pattern to search for
        *   \return 'true' if the pattern has been found
        */
        s_bool EndsWith(const s_str_t& sPattern) const
        {
            if (sPattern.GetSize() <= GetSize())
            {
                const_iterator iterSelf = End() - sPattern.GetSize().Get();
                const_iterator iterPattern;
                for (iterPattern = sPattern.Begin(); iterPattern != sPattern.End(); ++iterPattern)
                {
                    if (*iterPattern != *iterSelf)
                        return false;

                    ++iterSelf;
                }

                return true;
            }
            else
                return false;
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

        T& operator [] (const s_uint_t<default_uint>& uiIndex)
        {
            return sValue_[uiIndex.Get()];
        }

        const T& operator [] (const s_uint_t<default_uint>& uiIndex) const
        {
            return sValue_[uiIndex.Get()];
        }

        T& operator [] (const default_uint& uiIndex)
        {
            return sValue_[uiIndex];
        }

        const T& operator [] (const default_uint& uiIndex) const
        {
            return sValue_[uiIndex];
        }

        template<class N>
        s_str_t operator + (const N& mValue) const
        {
            return sValue_ + StringConverter<T>::Convert(mValue);
        }

        template<class N>
        void operator += (const N& mValue)
        {
            sValue_ += StringConverter<T>::Convert(mValue);
        }

        template<class N>
        s_str_t& operator << (const N& mValue)
        {
            sValue_ += StringConverter<T>::Convert(mValue);
            return *this;
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

        s_ctnr<s_str_t> operator , (const s_str_t& sValue) const
        {
            s_ctnr<s_str_t> mContainer;
            mContainer.PushBack(*this);
            mContainer.PushBack(sValue);
            return mContainer;
        }

        static const s_str_t EMPTY;

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

        /// Reverses the provided string.
        /** \param sValue The string to reverse
        *   \return The reversed string
        */
        static s_str_t Reverse(const s_str_t& sValue)
        {
            s_str_t sCopy = sValue;
            sCopy.Reverse();
            return sCopy;
        }

        /// Generic conversion function.
        /** \param mValue The value to convert
        *   \note Calls StringConverter::Convert().
        */
        template<class N>
        static s_str_t Convert(const N& mValue)
        {
            return StringConverter<T>::Convert(mValue);
        }

        /// Generic conversion function.
        /** \param mValue  The value to convert
        *   \param mParam1 The first parameter
        */
        template<class N, class M>
        static s_str_t Convert(const N& mValue, const M& mParam1)
        {
            return StringConverter<T>::Convert(mValue, mParam1);
        }

        /// Generic conversion function.
        /** \param mValue  The value to convert
        *   \param mParam1 The first parameter
        *   \param mParam2 The second parameter
        */
        template<class N, class M, class O>
        static s_str_t Convert(const N& mValue, const M& mParam1, const O& mParam2)
        {
            return StringConverter<T>::Convert(mValue, mParam1, mParam2);
        }

        /// Checks if the provided character is a number.
        /** \param cValue The character to test
        *   \return 'true' if the provided character is a number
        */
        static s_bool IsNumber(const T& cValue)
        {
            string_stream mTemp(string_object(1, StringConverter<T>::ConvertToStandard(cValue)));
            double dValue;
            mTemp >> dValue;
            return !mTemp.fail();
        }

        /// Checks if the provided character is a number.
        /** \param cValue The character to test
        *   \return 'true' if the provided character is a number
        */
        static s_bool IsNumber(const typename TypeTraits<T>::FrostType& cValue)
        {
            string_stream mTemp(string_object(1, StringConverter<T>::ConvertToStandard(cValue).Get()));
            double dValue;
            mTemp >> dValue;
            return !mTemp.fail();
        }

        /// Checks if the provided character is a letter or a number.
        /** \param cValue The character to test
        *   \return 'true' if the provided character is a letter or a number
        */
        static s_bool IsAlphaNumeric(const T& cValue)
        {
            return isalnum(StringConverter<T>::ConvertToStandard(cValue));
        }

        /// Checks if the provided character is a letter or a number.
        /** \param cValue The character to test
        *   \return 'true' if the provided character is a letter or a number
        */
        static s_bool IsAlphaNumeric(const typename TypeTraits<T>::FrostType& cValue)
        {
            return isalnum(StringConverter<T>::ConvertToStandard(cValue).Get());
        }

    private :

        string sValue_;

    };

    template<class T>
    const s_str_t<T> s_str_t<T>::EMPTY = "";

    template<class T>
    s_str_t<T> operator + (const T* sLeft, const s_str_t<T>& sRight)
    {
        return s_str_t<T>(sLeft) + sRight;
    }

    /** \cond NOT_REMOVE_FROM_DOC
    */
    template<> class TypeTraits<string_object>
    {
    public :
        typedef string_object           Type;
        typedef string_object           BaseType;
        typedef s_str_t<string_element> FrostType;
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
        typedef string_element*&        RefType;
        typedef string_element* const & CRefType;
        typedef string_element**         PointerType;

        static inline RefType  GetValue(RefType m)  { return m; }
        static inline CRefType GetValue(CRefType m) { return m; }
    };

    template<class T> class TypeTraits< s_str_t<T> >
    {
    public :
        typedef s_str_t<T>           Type;
        typedef std::basic_string<T> BaseType;
        typedef s_str_t<T>           FrostType;
        typedef s_str_t<T>&          RefType;
        typedef const s_str_t<T>&    CRefType;
        typedef s_str_t<T>*          PointerType;

        static inline typename TypeTraits<BaseType>::RefType  GetValue(RefType m)  { return m.Get(); }
        static inline typename TypeTraits<BaseType>::CRefType GetValue(CRefType m) { return m.Get(); }
    };
    /** \endcond
    */
}
