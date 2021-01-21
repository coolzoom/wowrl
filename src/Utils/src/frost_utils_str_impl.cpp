#include "frost_utils_types.h"

#include <algorithm>
#include <sstream>

namespace Frost {

typedef std::stringstream string_stream;

template <class T>
s_str_t<T>::s_str_t()
{
}

template <class T>
s_str_t<T>::s_str_t(const s_str_t& s)
{
    sValue_ = s.sValue_;
}

template <class T>
s_str_t<T>::s_str_t(const string& s)
{
    sValue_ = s;
}

template <class T>
s_str_t<T>::s_str_t(const character* s)
{
    sValue_ = s;
}

template <class T>
s_str_t<T>::s_str_t(character* s)
{
    sValue_ = s;
}

template <class T>
s_str_t<T>::s_str_t(const character& c)
{
    sValue_.push_back(c);
}

template <class T>
s_str_t<T>::s_str_t(const frost_character& c)
{
    sValue_.push_back(c.Get());
}

template <class T>
s_str_t<T>::s_str_t(const s_str_t& s, const s_uint_t<default_uint>& uiNbr)
{
    if (uiNbr.IsValid())
    {
        for (default_uint uiCounter = 0; uiCounter < uiNbr.Get(); ++uiCounter)
            sValue_ += s.Get();
    }
}

template <class T>
s_str_t<T>::s_str_t(const character& c, const s_uint_t<default_uint>& uiNbr)
{
    if (uiNbr.IsValid())
        sValue_ = string(uiNbr.Get(), c);
}

template <class T>
s_str_t<T>::s_str_t(const frost_character& c, const s_uint_t<default_uint>& uiNbr)
{
    if (uiNbr.IsValid())
        sValue_ = string(uiNbr.Get(), c.Get());
}

template <class T>
void s_str_t<T>::CapitalStart(const s_bool& bCapitalStart)
{
    T cFirst = (*this)[0];
    if (bCapitalStart)
        (*this)[0] = toupper(cFirst);
    else
        (*this)[0] = tolower(cFirst);
}

template <class T>
void s_str_t<T>::Clear()
{
    sValue_.clear();
}

template <class T>
s_ctnr< s_str_t<T> > s_str_t<T>::Cut(const s_str_t& sDelim) const
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

template <class T>
s_ctnr< s_str_t<T> > s_str_t<T>::Cut(const s_str_t& sDelim, const s_uint_t<default_uint>& uiMaxCut) const
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

template <class T>
s_ctnr< s_str_t<T> > s_str_t<T>::CutEach(const s_str_t& sDelim) const
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

template <class T>
s_ctnr< s_str_t<T> > s_str_t<T>::CutEach(const s_str_t& sDelim, const s_uint_t<default_uint>& uiMaxCut) const
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

template <class T>
s_ctnr< s_str_t<T> > s_str_t<T>::Cut(const s_ctnr<character>& lDelims) const
{
    s_ctnr<s_str_t> lPieces;

    string sTemp;
    const_iterator iter;
    for (iter = sValue_.begin(); iter != sValue_.end(); ++iter)
    {
        if (lDelims.Find(*iter))
        {
            if (!sTemp.empty())
            {
                lPieces.PushBack(sTemp);
                sTemp.clear();
            }
        }
        else
            sTemp.push_back(*iter);
    }

    if (!sTemp.empty())
        lPieces.PushBack(sTemp);

    return lPieces;
}

template <class T>
s_uint_t<default_uint> s_str_t<T>::CountOccurences(const s_str_t& sPattern) const
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

template <class T>
void s_str_t<T>::Erase(const s_uint_t<default_uint>& uiStart, const s_uint_t<default_uint>& uiNbr)
{
    if (uiStart.IsValid())
    {
        if (!uiNbr.IsValid())
            sValue_.erase(uiStart.Get(), string::npos);
        else
            sValue_.erase(uiStart.Get(), uiNbr.Get());
    }
}

template <class T>
void s_str_t<T>::Erase(const s_uint_t<default_uint>& uiPos)
{
    if (uiPos.IsValid())
        sValue_.erase(uiPos.Get(), 1);
}

template <class T>
typename s_str_t<T>::iterator s_str_t<T>::Erase(iterator iter)
{
    return sValue_.erase(iter);
}

template <class T>
void s_str_t<T>::EraseFromEnd(const s_uint_t<default_uint>& uiNbr)
{
    if (uiNbr.IsValid())
        sValue_.erase((GetLength() - uiNbr).Get(), uiNbr.Get());
    else
        sValue_.clear();
}

template <class T>
void s_str_t<T>::EraseFromStart(const s_uint_t<default_uint>& uiNbr)
{
    if (uiNbr.IsValid())
        sValue_.erase(0, uiNbr.Get());
    else
        sValue_.clear();
}

template <class T>
void s_str_t<T>::EraseRange(const s_uint_t<default_uint>& uiStart, const s_uint_t<default_uint>& uiEnd)
{
    if (uiStart.IsValid())
    {
        if (!uiEnd.IsValid())
            sValue_.erase(uiStart.Get(), string::npos);
        else
            sValue_.erase(uiStart.Get(), (uiEnd-uiStart).Get());
    }
}

template <class T>
typename s_str_t<T>::iterator s_str_t<T>::EraseRange(iterator iterStart, iterator iterEnd)
{
    return sValue_.erase(iterStart, iterEnd);
}

template <class T>
typename s_str_t<T>::iterator s_str_t<T>::EraseRange(range mRange)
{
    return sValue_.erase(mRange.Begin(), mRange.End());
}

template <class T>
s_str_t<T> s_str_t<T>::Extract(const s_uint_t<default_uint>& uiStart, const s_uint_t<default_uint>& uiNbr) const
{
    if (!uiNbr.IsValid())
        return s_str_t(sValue_.substr(uiStart.Get(), string::npos));
    else
        return s_str_t(sValue_.substr(uiStart.Get(), uiNbr.Get()));
}

template <class T>
s_str_t<T> s_str_t<T>::ExtractRange(const s_uint_t<default_uint>& uiStart, const s_uint_t<default_uint>& uiEnd) const
{
    if (!uiEnd.IsValid())
        return s_str_t(sValue_.substr(uiStart.Get(), string::npos));
    else
        return s_str_t(sValue_.substr(uiStart.Get(), (uiEnd-uiStart).Get()));
}

template <class T>
typename s_str_t<T>::iterator s_str_t<T>::Get(const s_str_t& sValue)
{
    size_t uiResult = sValue_.find(sValue.Get());
    if (uiResult != sValue_.npos)
        return Begin() + uiResult;
    else
        return End();
}

template <class T>
typename s_str_t<T>::iterator s_str_t<T>::Get(const s_str_t& sValue, const s_uint_t<default_uint>& uiStart)
{
    if (uiStart.IsValid())
    {
        size_t uiResult = sValue_.find(sValue.Get(), uiStart.Get());
        if (uiResult != sValue_.npos)
            return Begin() + uiResult;
    }

    return End();
}

template <class T>
typename s_str_t<T>::const_iterator s_str_t<T>::Get(const s_str_t& sValue) const
{
    size_t uiResult = sValue_.find(sValue.Get());
    if (uiResult != sValue_.npos)
        return Begin() + uiResult;
    else
        return End();
}

template <class T>
typename s_str_t<T>::const_iterator s_str_t<T>::Get(const s_str_t& sValue, const s_uint_t<default_uint>& uiStart) const
{
    if (uiStart.IsValid())
    {
        size_t uiResult = sValue_.find(sValue.Get(), uiStart.Get());
        if (uiResult != sValue_.npos)
            return Begin() + uiResult;
    }

    return End();
}

template <class T>
s_uint_t<default_uint> s_str_t<T>::FindPos(const s_str_t& sValue) const
{
    size_t uiResult = sValue_.find(sValue.Get());
    if (uiResult != sValue_.npos)
        return uiResult;
    else
        return s_uint_t<default_uint>::NaN;
}

template <class T>
s_uint_t<default_uint> s_str_t<T>::FindPos(const s_str_t& sValue, const s_uint_t<default_uint>& uiStart) const
{
    if (uiStart.IsValid())
    {
        size_t uiResult = sValue_.find(sValue.Get(), uiStart.Get());
        if (uiResult != sValue_.npos)
            return uiResult;
    }

    return s_uint_t<default_uint>::NaN;
}

template <class T>
s_ctnr< s_uint_t<default_uint> > s_str_t<T>::FindAllPos(const s_str_t& sPattern)
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

template <class T>
s_ctnr< s_uint_t<default_uint> > s_str_t<T>::FindAllPos(const s_str_t& sPattern, const s_uint_t<default_uint>& uiStart)
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

template <class T>
s_bool s_str_t<T>::Find(const s_str_t& sValue) const
{
    size_t uiResult = sValue_.find(sValue.Get());
    if (uiResult != sValue_.npos)
        return true;
    else
        return false;
}

template <class T>
s_bool s_str_t<T>::Find(const s_str_t& sValue, const s_uint_t<default_uint>& uiStart) const
{
    if (uiStart.IsValid())
    {
        size_t uiResult = sValue_.find(sValue.Get(), uiStart.Get());
        if (uiResult != sValue_.npos)
            return true;
    }

    return false;
}

template <class T>
s_bool s_str_t<T>::IsEmpty(const s_bool& bIgnoreSpaces) const
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

template <class T>
s_bool s_str_t<T>::IsNumber() const
{
    string_stream mTemp(StringConverter<string_element, string>::Convert(sValue_));
    double dValue;
    mTemp >> dValue;
    return !mTemp.fail();
}

template <class T>
s_bool s_str_t<T>::IsBoolean() const
{
    return ((*this) == "false") || ((*this) == "true") ||
           ((*this) == "0")     || ((*this) == "1")    ||
           ((*this) == "no")    || ((*this) == "yes");
}

template <class T>
s_uint_t<default_uint> s_str_t<T>::GetLength() const
{
    return sValue_.length();
}

template <class T>
s_uint_t<default_uint> s_str_t<T>::GetSize() const
{
    return sValue_.size();
}

template <class T>
void s_str_t<T>::ToLower()
{
    typename s_str_t<string_element>::string sTemp = StringConverter<string_element, string>::Convert(sValue_);
    std::transform(sTemp.begin(), sTemp.end(), sTemp.begin(), ::tolower);
    sValue_ = StringConverter<character, typename s_str_t<string_element>::string>::Convert(sTemp);
}

template <class T>
void s_str_t<T>::ToUpper()
{
    typename s_str_t<string_element>::string sTemp = StringConverter<string_element, string>::Convert(sValue_);
    std::transform(sTemp.begin(), sTemp.end(), sTemp.begin(), ::toupper);
    sValue_ = StringConverter<character, typename s_str_t<string_element>::string>::Convert(sTemp);
}

template <class T>
s_uint_t<default_uint> s_str_t<T>::Trim(const character& cPattern)
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

template <class T>
s_uint_t<default_uint> s_str_t<T>::Replace(const s_str_t& sPattern, const s_str_t& sReplacement)
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

template <class T>
void s_str_t<T>::PushBack(const character& cChar)
{
    sValue_.push_back(cChar);
}

template <class T>
void s_str_t<T>::PushBack(const s_str_t& sValue)
{
    sValue_.append(sValue.sValue_);
}

template <class T>
void s_str_t<T>::PopBack()
{
    if (!sValue_.empty())
        sValue_.erase(sValue_.length()-1, 1);
}

template <class T>
T& s_str_t<T>::Back()
{
    return sValue_[sValue_.length()-1];
}

template <class T>
const T& s_str_t<T>::Back() const
{
    return sValue_[sValue_.length()-1];
}

template <class T>
void s_str_t<T>::PushFront(const character& cChar)
{
    sValue_.insert(0, 1, cChar);
}

template <class T>
void s_str_t<T>::PushFront(const s_str_t& sValue)
{
    sValue_.insert(0, sValue.sValue_);
}

template <class T>
void s_str_t<T>::PopFront()
{
    if (!sValue_.empty())
        sValue_.erase(0, 1);
}

template <class T>
T& s_str_t<T>::Front()
{
    return sValue_[0];
}

template <class T>
const T& s_str_t<T>::Front() const
{
    return sValue_[0];
}

template <class T>
void s_str_t<T>::Insert(const character& cChar, const s_uint_t<default_uint>& uiPos)
{
    if (uiPos.IsValid())
        sValue_.insert(uiPos.Get(), 1, cChar);
    else
        sValue_.push_back(cChar);
}

template <class T>
typename s_str_t<T>::iterator s_str_t<T>::Insert(const character& cChar, iterator iter)
{
    return sValue_.insert(iter, cChar)+1;
}

template <class T>
void s_str_t<T>::Insert(const s_str_t& sValue, const s_uint_t<default_uint>& uiPos)
{
    if (uiPos.IsValid())
        sValue_.insert(uiPos.Get(), sValue.sValue_);
    else
        sValue_.append(sValue.sValue_);
}

template <class T>
typename s_str_t<T>::iterator s_str_t<T>::Insert(const s_str_t& sValue, iterator iter)
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

template <class T>
void s_str_t<T>::Reverse()
{
    std::reverse(sValue_.begin(), sValue_.end());
}

template <class T>
s_bool s_str_t<T>::StartsWith(const s_str_t& sPattern) const
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

template <class T>
s_bool s_str_t<T>::EndsWith(const s_str_t& sPattern) const
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

template <class T>
typename s_str_t<T>::iterator s_str_t<T>::begin()
{
    return sValue_.begin();
}

template <class T>
typename s_str_t<T>::const_iterator s_str_t<T>::begin() const
{
    return sValue_.begin();
}

template <class T>
typename s_str_t<T>::iterator s_str_t<T>::end()
{
    return sValue_.end();
}

template <class T>
typename s_str_t<T>::const_iterator s_str_t<T>::end() const
{
    return sValue_.end();
}

template <class T>
typename s_str_t<T>::iterator s_str_t<T>::Begin()
{
    return sValue_.begin();
}

template <class T>
typename s_str_t<T>::const_iterator s_str_t<T>::Begin() const
{
    return sValue_.begin();
}

template <class T>
typename s_str_t<T>::iterator s_str_t<T>::End()
{
    return sValue_.end();
}

template <class T>
typename s_str_t<T>::const_iterator s_str_t<T>::End() const
{
    return sValue_.end();
}

template <class T>
T& s_str_t<T>::operator [] (const s_uint_t<default_uint>& uiIndex)
{
    return sValue_[uiIndex.Get()];
}

template <class T>
const T& s_str_t<T>::operator [] (const s_uint_t<default_uint>& uiIndex) const
{
    return sValue_[uiIndex.Get()];
}

template <class T>
T& s_str_t<T>::operator [] (const default_uint& uiIndex)
{
    return sValue_[uiIndex];
}

template <class T>
const T& s_str_t<T>::operator [] (const default_uint& uiIndex) const
{
    return sValue_[uiIndex];
}

template <class T>
s_bool s_str_t<T>::operator == (const s_str_t& mValue) const
{
    return (sValue_ == mValue.sValue_);
}

template <class T>
s_bool s_str_t<T>::operator != (const s_str_t& mValue) const
{
    return (sValue_ != mValue.sValue_);
}

template <class T>
s_bool s_str_t<T>::operator < (const s_str_t& mValue) const
{
    return (sValue_ < mValue.sValue_);
}

template <class T>
s_bool s_str_t<T>::operator > (const s_str_t& mValue) const
{
    return (sValue_ > mValue.sValue_);
}

template <class T>
s_bool s_str_t<T>::operator <= (const s_str_t& mValue) const
{
    return (sValue_ <= mValue.sValue_);
}

template <class T>
s_bool s_str_t<T>::operator >= (const s_str_t& mValue) const
{
    return (sValue_ >= mValue.sValue_);
}

template <class T>
s_str_t<T> s_str_t<T>::ToLower(const s_str_t& sValue)
{
    s_str_t sCopy = sValue;
    sCopy.ToLower();
    return sCopy;
}

template <class T>
s_str_t<T> s_str_t<T>::ToUpper(const s_str_t& sValue)
{
    s_str_t sCopy = sValue;
    sCopy.ToUpper();
    return sCopy;
}

template <class T>
s_str_t<T> s_str_t<T>::Reverse(const s_str_t& sValue)
{
    s_str_t sCopy = sValue;
    sCopy.Reverse();
    return sCopy;
}

template <class T>
s_bool s_str_t<T>::IsNumber(const character& cValue)
{
    string_stream mTemp(typename s_str_t<string_element>::string(1, StringConverter<string_element, character>::ConvertChar(cValue)));
    double dValue;
    mTemp >> dValue;
    return !mTemp.fail();
}

template <class T>
s_bool s_str_t<T>::IsNumber(const frost_character& cValue)
{
    string_stream mTemp(typename s_str_t<string_element>::string(1, StringConverter<string_element, frost_character>::ConvertChar(cValue).Get()));
    double dValue;
    mTemp >> dValue;
    return !mTemp.fail();
}

template <class T>
s_bool s_str_t<T>::IsAlphaNumeric(const character& cValue)
{
    return isalnum(StringConverter<string_element, character>::ConvertChar(cValue));
}

template <class T>
s_bool s_str_t<T>::IsAlphaNumeric(const frost_character& cValue)
{
    return isalnum(StringConverter<string_element, frost_character>::ConvertChar(cValue).Get());
}

template<class T>
const s_str_t<T> s_str_t<T>::EMPTY = "";

template<class T>
s_str_t<T> operator + (const T* sLeft, const s_str_t<T>& sRight)
{
    return s_str_t<T>(sLeft) + sRight;
}

template<class T>
s_str_t<T> operator + (const std::basic_string<T> sLeft, const s_str_t<T>& sRight)
{
    return s_str_t<T>(sLeft) + sRight;
}

template class s_str_t<string_element>;
template class s_str_t<uint>;

typedef s_str_t<string_element> s_str;
typedef s_str_t<uint>           s_ustr;

template s_str operator + (const string_element* sLeft, const s_str& sRight);
template s_str operator + (const std::basic_string<string_element> sLeft, const s_str& sRight);

template s_ustr operator + (const uint* sLeft, const s_ustr& sRight);
template s_ustr operator + (const std::basic_string<uint> sLeft, const s_ustr& sRight);

}
