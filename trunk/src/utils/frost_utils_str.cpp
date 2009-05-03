/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_int.h"
#include "frost_utils_uint.h"
#include "frost_utils_float.h"
#include "frost_utils_double.h"
#include "frost_utils_str.h"
#include "frost_utils_bool.h"
#include "frost_utils_array.h"
#include "frost_utils.h"
#include "frost_utils_log.h"

#include <algorithm>

#ifdef FROST_LINUX
    #include <cstdlib>
#endif

using namespace std;

namespace Frost
{
    string_element s_str::cDummy = STRING('\0');
    const s_str s_str::CLASS_NAME = "s_str";

    s_str::s_str()
    {
        sValue_ = STRING("");
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
    }


    s_str::s_str( const string_object& sValue )
    {
        sValue_ = sValue;
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
    }

    #ifdef USE_UNICODE
        s_str::s_str( const string& sValue )
        {
            const uchar* str = reinterpret_cast<const uchar*>(sValue.c_str());
            sValue_ = wstring(str, str + sValue.size());
            mIntConvType_ = CONV_DECIMAL;
            mBoolConvType_ = CONV_TRUE_FALSE;
        }

        s_str::s_str( const char* sValue )
        {
            string s = sValue;
            const uchar* str = reinterpret_cast<const uchar*>(s.c_str());
            sValue_ = wstring(str, str + s.size());
            mIntConvType_ = CONV_DECIMAL;
            mBoolConvType_ = CONV_TRUE_FALSE;
        }
    #endif

    s_str::s_str( const string_element* sValue )
    {
        sValue_ = sValue;
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
    }

    s_str::s_str( const string_element& cValue, const s_uint& uiCharNbr )
    {
        sValue_ = string_object(uiCharNbr.Get(), cValue);
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
    }

    s_str::s_str( const s_int& iValue )
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = STRING("");
        this->operator<<(iValue);
    }

    s_str::s_str( const s_uint& uiValue )
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = STRING("");
        this->operator<<(uiValue);
    }

    s_str::s_str( const s_float& fValue )
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = STRING("");
        this->operator<<(fValue);
    }

    s_str::s_str( const s_double& dValue )
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = STRING("");
        this->operator<<(dValue);
    }

    s_str::s_str( const s_bool& bValue )
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = STRING("");
        this->operator<<(bValue);
    }

    s_str::s_str( const string_element& cValue )
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = STRING("");
        this->operator+=(cValue);
    }

    s_str::s_str(const s_int& iValue, const s_uint& uiCharNbr)
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = STRING("");
        this->operator<<(iValue);

        while (sValue_.length() < uiCharNbr.Get())
            sValue_ = STRING('0') + sValue_;
    }

    s_str::s_str(const s_uint& uiValue, const s_uint& uiCharNbr)
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = STRING("");
        this->operator<<(uiValue);

        while (sValue_.length() < uiCharNbr.Get())
            sValue_ = STRING('0') + sValue_;
    }

    s_str::s_str(const s_float& fValue, const s_uint& uiCharNbr)
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = STRING("");
        this->operator<<(fValue);

        while (sValue_.length() < uiCharNbr.Get())
            sValue_ = STRING('0') + sValue_;
    }

    s_str::s_str(const s_double& dValue, const s_uint& uiCharNbr)
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = STRING("");
        this->operator<<(dValue);

        while (sValue_.length() < uiCharNbr.Get())
            sValue_ = STRING('0') + sValue_;
    }

    void s_str::Clear()
    {
        sValue_.clear();
    }

    s_bool s_str::IsEmpty( const s_bool& bIgnoreSpaces ) const
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
                for (uint i = 0; i < sValue_.length(); i++)
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

    s_bool s_str::IsNumber() const
    {
        string_stream mTemp(sValue_);
        double dValue;
        mTemp >> dValue;
        return !mTemp.fail();
    }

    s_bool s_str::IsBoolean() const
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

    string_element& s_str::operator[] ( const s_uint& uiIndex )
    {
        if ( uiIndex.IsValid() &&  (uiIndex.Get() < sValue_.size()) )
            return sValue_[uiIndex.Get()];
        else
            return cDummy;
    }

    const string_element& s_str::operator[] ( const s_uint& uiIndex ) const
    {
        if ( uiIndex.IsValid() && (uiIndex.Get() < sValue_.size()) )
            return sValue_[uiIndex.Get()];
        else
            return cDummy;
    }

    s_str s_str::operator+ ( const s_str& mValue ) const
    {
        return s_str(sValue_ + mValue.sValue_);
    }

    s_str s_str::operator+ ( const string_element& cValue ) const
    {
        string_object sTemp = sValue_;
        sTemp.push_back(cValue);
        return s_str(sTemp);
    }

    s_str s_str::operator+ (const string_element* sValue) const
    {
        return s_str(sValue_ + sValue);
    }

    #ifdef USE_UNICODE
        s_str s_str::operator+ (const char* sValue) const
        {
            string s = sValue;
            const uchar* str = reinterpret_cast<const uchar*>(s.c_str());
            return sValue_ + wstring(str, str + s.size());
        }
    #endif

    s_str s_str::operator+ (const s_int& iValue) const
    {
        return *this + s_str(iValue);
    }

    s_str s_str::operator+ (const s_uint& uiValue) const
    {
        return *this + s_str(uiValue);
    }

    s_str s_str::operator+ (const s_float& fValue) const
    {
        return *this + s_str(fValue);
    }

    s_str s_str::operator+ (const s_double& dValue) const
    {
        return *this + s_str(dValue);
    }

    s_str s_str::operator+ (const s_bool& bValue) const
    {
        return *this + s_str(bValue);
    }

    s_str s_str::operator- ( const s_uint& uiNumber ) const
    {
        if (uiNumber.Get() < sValue_.size())
        {
            string_object sTemp = sValue_;
            sTemp = sTemp.substr(0, sTemp.size()-uiNumber.Get());
            return s_str(sTemp);
        }
        else
            return s_str("");
    }

    void s_str::operator+= ( const s_str& mValue )
    {
        sValue_ += mValue.sValue_;
    }

    void s_str::operator+= ( const string_element& cValue )
    {
        sValue_.push_back(cValue);
    }

    void s_str::operator-= ( const s_uint& uiNumber )
    {
        if (uiNumber.Get() < sValue_.size())
            sValue_ = sValue_.substr(0, sValue_.size()-uiNumber.Get());
        else
            sValue_ = STRING("");
    }

    bool s_str::operator== ( const s_str& mValue ) const
    {
        return (sValue_ == mValue.sValue_);
    }

    bool s_str::operator!= ( const s_str& mValue ) const
    {
        return (sValue_ != mValue.sValue_);
    }

    bool s_str::operator< ( const s_str& mValue ) const
    {
        return (sValue_ < mValue.sValue_);
    }

    bool s_str::operator> ( const s_str& mValue ) const
    {
        return (sValue_ > mValue.sValue_);
    }

    bool s_str::operator<= ( const s_str& mValue ) const
    {
        return (sValue_ <= mValue.sValue_);
    }

    bool s_str::operator>= ( const s_str& mValue ) const
    {
        return (sValue_ >= mValue.sValue_);
    }

    s_str& s_str::operator= ( const s_str& mValue )
    {
        sValue_ = mValue.sValue_;
        mIntConvType_ = mValue.mIntConvType_;
        mBoolConvType_ = mValue.mBoolConvType_;
        return *this;
    }

    s_str& s_str::operator<< ( const s_str& mValue )
    {
        sValue_ += mValue.sValue_;
        return *this;
    }

    s_str& s_str::operator<< ( const string_element* sValue )
    {
        sValue_ += sValue;
        return *this;
    }

    s_str& s_str::operator<< ( const int& iValue )
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
                sStream << hex << iValue;
                break;
            }
            case CONV_BIN :
            {
                // TODO
                break;
            }
        }

        string_object sValue;
        sStream >> sValue;
        sValue_ += sValue;

        return *this;
    }

    s_str& s_str::operator<< ( const uint& uiValue )
    {
        return operator<<(static_cast<int>(uiValue));
    }

    s_str& s_str::operator<< ( const float& fValue )
    {
        string_stream sStream;
        sStream << fValue;
        sValue_ += sStream.str();
        return *this;
    }

    s_str& s_str::operator<< ( const double& dValue )
    {
        string_stream sStream;
        sStream << dValue;
        sValue_ += sStream.str();
        return *this;
    }

    s_str& s_str::operator<< ( const bool& bValue )
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

    s_str& s_str::operator<< ( const string_element& cValue )
    {
        this->operator+=(cValue);
        return *this;
    }

    s_str& s_str::operator<< ( const void* pValue )
    {
        if (pValue == NULL)
            return this->operator<<("NULL");
        else
            return this->operator<<(s_uint((uint)pValue));
    }

    s_str& s_str::operator<< ( const s_int& iValue )
    {
        if (iValue.IsValid())
            return operator<<(iValue.Get());
        else
        {
            if (iValue.GetType() == s_int::INTEGER_NAN)
                operator<<("nan");
            else if (iValue.GetType() == s_int::INTEGER_INF_PLUS)
                operator<<("inf+");
            else if (iValue.GetType() == s_int::INTEGER_INF_MINUS)
                operator<<("inf-");

            return *this;
        }
    }

    s_str& s_str::operator<< ( const s_uint& uiValue )
    {
        if (uiValue.IsValid())
            return operator<<(uiValue.Get());
        else
        {
            if (uiValue.GetType() == s_uint::INTEGER_NAN)
                operator<<("nan");
            else if (uiValue.GetType() == s_uint::INTEGER_INF)
                operator<<("inf");

            return *this;
        }
    }

    s_str& s_str::operator<< ( const s_float& fValue )
    {
        if (fValue.IsValid())
            return operator<<(fValue.Get());
        else
        {
            if (fValue.GetType() == s_float::FLOAT_NAN)
                operator<<("nan");
            else if (fValue.GetType() == s_float::FLOAT_INF_PLUS)
                operator<<("inf+");
            else if (fValue.GetType() == s_float::FLOAT_INF_MINUS)
                operator<<("inf-");

            return *this;
        }
    }

    s_str& s_str::operator<< ( const s_double& dValue )
    {
        if (dValue.IsValid())
            return operator<<(dValue.Get());
        else
        {
            if (dValue.GetType() == s_double::DOUBLE_NAN)
                operator<<("nan");
            else if (dValue.GetType() == s_double::DOUBLE_INF_PLUS)
                operator<<("inf+");
            else if (dValue.GetType() == s_double::DOUBLE_INF_MINUS)
                operator<<("inf-");

            return *this;
        }
    }

    s_str& s_str::operator<< ( const s_bool& bValue )
    {
        return operator<<(bValue.Get());
    }

    s_str& s_str::operator<< ( const IntegerConversionType& mIntConvType )
    {
        mIntConvType_ = mIntConvType;
        return *this;
    }

    s_str& s_str::operator<< ( const BoolConversionType& mBoolConvType )
    {
        mBoolConvType_ = mBoolConvType;
        return *this;
    }

    s_uint s_str::FindPos( const s_str& sValue, const s_uint& uiStart ) const
    {
        if (uiStart.IsValid())
        {
            // string::npos is an int for Linux, and an uint for Windows
            #ifdef FROST_LINUX
                int iResult = sValue_.find(sValue.Get(), uiStart.Get());
                if (iResult >= 0)
                    return s_uint(s_int(iResult));
            #else
                uint uiResult = sValue_.find(sValue.Get(), uiStart.Get());
                if (uiResult != sValue_.npos)
                    return s_uint(uiResult);
            #endif
        }
        return s_uint(s_uint::INTEGER_NAN);
    }

    s_bool s_str::Find( const s_str& sValue, const s_uint& uiStart ) const
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

    void s_str::CapitalStart( const s_bool& bCapitalStart )
    {
        string_element cFirst = (*this)[0];
        if (bCapitalStart)
            (*this)[0] = toupper(cFirst);
        else
            (*this)[0] = tolower(cFirst);
    }

    std::vector<s_str> s_str::Cut( const s_str& sDelim, const s_uint& uiMaxCut ) const
    {
        vector<s_str> lPieces;
        s_uint uiPos = FindPos(sDelim);
        s_uint uiLastPos;
        s_uint uiCount;
        s_uint uiCurSize;
        while (uiPos.IsValid())
        {
            uiCurSize = uiPos - uiLastPos;
            if (!uiCurSize.IsNull())
                lPieces.push_back(Extract(uiLastPos, uiCurSize));
            uiLastPos = uiPos + sDelim.Length();
            uiPos = FindPos(sDelim, uiLastPos);
            uiCount++;

            if (uiCount >= uiMaxCut)
                break;
        }

        lPieces.push_back(Extract(uiLastPos));

        return lPieces;
    }

    std::vector<s_str> s_str::CutEach( const s_str& sDelim, const s_uint& uiMaxCut ) const
    {
        vector<s_str> lPieces;
        s_uint uiPos = FindPos(sDelim);
        s_uint uiLastPos;
        s_uint uiCount;
        s_uint uiCurSize;
        while (uiPos.IsValid())
        {
            uiCurSize = uiPos - uiLastPos;
            lPieces.push_back(Extract(uiLastPos, uiCurSize));
            uiLastPos = uiPos + sDelim.Length();
            uiPos = FindPos(sDelim, uiLastPos);
            uiCount++;

            if (uiCount >= uiMaxCut)
                break;
        }

        lPieces.push_back(Extract(uiLastPos));

        return lPieces;
    }

    s_uint s_str::CountOccurences( const s_str& sPattern ) const
    {
        s_uint uiCount;
        s_uint uiPos = FindPos(sPattern);
        while (uiPos.IsValid())
        {
            uiCount++;
            uiPos++;
            uiPos = FindPos(sPattern, uiPos);
        }

        return uiCount;
    }

    void s_str::Erase( const s_uint& uiStart, const s_uint& uiNbr )
    {
        if (!uiNbr.IsValid())
            sValue_.erase(uiStart.Get(), string_object::npos);
        else
            sValue_.erase(uiStart.Get(), uiNbr.Get());
    }

    void s_str::EraseRange( const s_uint& uiStart, const s_uint& uiEnd )
    {
        if (!uiEnd.IsValid())
            sValue_.erase(uiStart.Get(), string_object::npos);
        else
            sValue_.erase(uiStart.Get(), (uiEnd-uiStart).Get());
    }

    void s_str::EraseFromEnd( const s_uint& uiNbr )
    {
        if (uiNbr.IsValid())
            sValue_.erase((Length()-uiNbr).Get(), uiNbr.Get());
    }

    void s_str::EraseFromStart( const s_uint& uiNbr )
    {
        if (uiNbr.IsValid())
            sValue_.erase(0, uiNbr.Get());
    }

    s_str s_str::Extract( const s_uint& uiStart, const s_uint& uiNbr ) const
    {
        if (!uiNbr.IsValid())
            return s_str(sValue_.substr(uiStart.Get(), string_object::npos));
        else
            return s_str(sValue_.substr(uiStart.Get(), uiNbr.Get()));
    }

    s_str s_str::ExtractRange( const s_uint& uiStart, const s_uint& uiEnd ) const
    {
        if (!uiEnd.IsValid())
            return s_str(sValue_.substr(uiStart.Get(), string_object::npos));
        else
            return s_str(sValue_.substr(uiStart.Get(), (uiEnd-uiStart).Get()));
    }

    s_uint s_str::HexToUInt() const
    {
        uint i;
        string_stream s;
        s << sValue_;
        s >> hex >> i;
        return i;
    }

    s_uint s_str::Length() const
    {
        return s_uint((uint)sValue_.length());
    }

    s_uint s_str::Size() const
    {
        return s_uint((uint)sValue_.size());
    }

    void s_str::ToLower()
    {
        std::transform(sValue_.begin(), sValue_.end(), sValue_.begin(), ::tolower);
    }

    void s_str::ToUpper()
    {
        std::transform(sValue_.begin(), sValue_.end(), sValue_.begin(), ::toupper);
    }

    s_uint s_str::Trim( const string_element& cPattern )
    {
        s_uint uiCount;
        while ((*this)[0] == cPattern)
        {
            EraseFromStart(1);
            uiCount++;
        }
        while ((*this)[Length().Get()-1] == cPattern)
        {
            EraseFromEnd(1);
            uiCount++;
        }
        return uiCount;
    }

    s_uint s_str::Replace( const s_str& sPattern, const s_str& sReplacement )
    {
        s_uint uiCount;
        s_uint uiPos = FindPos(sPattern);

        while (uiPos.IsValid())
        {
            sValue_.replace(uiPos.Get(), sPattern.Length().Get(), sReplacement.Get());
            uiPos = FindPos(sPattern, uiPos+sReplacement.Length());
            uiCount++;
        }

        return uiCount;
    }

    s_str operator+ ( const string_element* sLeft, const s_str& sRight )
    {
        return s_str(sLeft) + sRight;
    }

    #ifdef USE_UNICODE
        s_str operator+ ( const char* sLeft, const s_str& sRight )
        {
            return s_str(sLeft) + sRight;
        }
    #endif

    s_ctnr<s_str> s_str::operator, ( const s_str& sValue ) const
    {
        s_ctnr<s_str> mContainer;
        mContainer.Push(*this);
        mContainer.Push(sValue);
        return mContainer;
    }

    s_str s_str::ToLower( const s_str& sValue )
    {
        s_str sCopy = sValue;
        sCopy.ToLower();
        return sCopy;
    }

    s_str s_str::ToUpper( const s_str& sValue )
    {
        s_str sCopy = sValue;
        sCopy.ToUpper();
        return sCopy;
    }

    void s_str::PushBack( const string_element& cChar )
    {
        sValue_.push_back(cChar);
    }

    void s_str::PushFront( const string_element& cChar )
    {
        sValue_.insert(0, 1, cChar);
    }

    void s_str::Insert( const string_element& cChar, const s_uint& uiPos )
    {
        if (uiPos.IsValid())
            sValue_.insert(uiPos.Get(), 1, cChar);
        else
            sValue_.push_back(cChar);
    }

    s_str::iterator::iterator()
    {
        pParent_ = NULL;
    }

    s_str::iterator::iterator(s_str* pParent, const s_uint& uiPos)
    {
        pParent_ = pParent;
        uiPos_ = uiPos;
    }

    string_element& s_str::iterator::operator * ()
    {
        return (*pParent_)[uiPos_];
    }

    s_str::iterator s_str::iterator::operator + ( const s_int& iOffset ) const
    {
        s_uint uiNewPos = uiPos_;
        if (pParent_)
        {
            if (iOffset > 0)
            {
                uiNewPos += s_uint(iOffset);
                if (uiNewPos > pParent_->Length())
                    uiNewPos = s_uint::NaN;
            }
            if (iOffset < 0)
            {
                s_uint uiOffset = s_uint(iOffset);
                if (uiOffset < uiNewPos)
                    uiNewPos -= s_uint(iOffset);
                else
                    uiNewPos = 0u;
            }
        }
        return iterator(pParent_, uiNewPos);
    }

    s_str::iterator s_str::iterator::operator + ( const s_uint& uiOffset ) const
    {
        s_uint uiNewPos = uiPos_;
        if (pParent_)
        {
            uiNewPos += uiOffset;
            if (uiNewPos > pParent_->Length())
                uiNewPos = s_uint::NaN;
        }
        return iterator(pParent_, uiNewPos);
    }

    s_str::iterator s_str::iterator::operator - ( const s_int& iOffset ) const
    {
        s_uint uiNewPos = uiPos_;
        if (pParent_)
        {
            if (iOffset < 0)
            {
                uiNewPos += s_uint(iOffset);
                if (uiNewPos > pParent_->Length())
                    uiNewPos = s_uint::NaN;
            }
            if (iOffset > 0)
            {
                s_uint uiOffset = s_uint(iOffset);
                if (uiOffset < uiNewPos)
                    uiNewPos -= s_uint(iOffset);
                else
                    uiNewPos = 0u;
            }
        }
        return iterator(pParent_, uiNewPos);
    }

    s_str::iterator s_str::iterator::operator - ( const s_uint& uiOffset ) const
    {
        s_uint uiNewPos = uiPos_;
        if (pParent_)
        {
            if (uiOffset < uiNewPos)
                uiNewPos -= uiOffset;
            else
                uiNewPos = 0u;
        }
        return iterator(pParent_, uiNewPos);
    }

    s_uint s_str::iterator::operator - ( const s_str::iterator& iter ) const
    {
        if ( (iter.pParent_ == pParent_) && (iter.uiPos_ < uiPos_) )
        {
            return uiPos_-iter.uiPos_;
        }
        else
            return 0;
    }

    s_str::iterator& s_str::iterator::operator ++ ()
    {
        ++uiPos_;
        if (uiPos_ == pParent_->Length())
            uiPos_ = s_uint::NaN;

        return *this;
    }

    s_str::iterator s_str::iterator::operator ++ (int)
    {
        uiPos_++;
        if (uiPos_ == pParent_->Length())
            uiPos_ = s_uint::NaN;

        return *this;
    }

    s_str::iterator& s_str::iterator::operator -- ()
    {
        if (uiPos_.IsValid())
        {
            if (uiPos_ != 0)
                --uiPos_;
        }
        else
        {
            if (pParent_->IsEmpty())
                uiPos_ = s_uint::NaN;
            else
                uiPos_ = pParent_->Length()-1;
        }

        return *this;
    }

    s_str::iterator s_str::iterator::operator -- (int)
    {
        if (uiPos_.IsValid())
        {
            if (uiPos_ != 0)
                uiPos_--;
        }
        else
        {
            if (pParent_->IsEmpty())
                uiPos_ = s_uint::NaN;
            else
                uiPos_ = pParent_->Length()-1;
        }

        return *this;
    }

    s_bool s_str::iterator::operator != (s_str::iterator iter)
    {
        if (uiPos_.IsNaN())
        {
            if (iter.uiPos_.IsNaN())
                return (pParent_ != iter.pParent_);
            else
                return true;
        }
        else
        {
            if (iter.uiPos_.IsNaN())
                return true;
            else
                return (pParent_ != iter.pParent_) || (uiPos_ != iter.uiPos_);
        }
    }

    s_str::const_iterator::const_iterator()
    {
        pParent_ = NULL;
    }

    s_str::const_iterator::const_iterator(const s_str* pParent, const s_uint& uiPos)
    {
        pParent_ = pParent;
        uiPos_ = uiPos;
    }

    const string_element& s_str::const_iterator::operator * ()
    {
        return (*pParent_)[uiPos_];
    }

    s_str::const_iterator s_str::const_iterator::operator + ( const s_int& iOffset ) const
    {
        s_uint uiNewPos = uiPos_;
        if (pParent_)
        {
            if (iOffset > 0)
            {
                uiNewPos += s_uint(iOffset);
                if (uiNewPos > pParent_->Length())
                    uiNewPos = s_uint::NaN;
            }
            if (iOffset < 0)
            {
                s_uint uiOffset = s_uint(iOffset);
                if (uiOffset < uiNewPos)
                    uiNewPos -= s_uint(iOffset);
                else
                    uiNewPos = 0u;
            }
        }
        return const_iterator(pParent_, uiNewPos);
    }

    s_str::const_iterator s_str::const_iterator::operator + ( const s_uint& uiOffset ) const
    {
        s_uint uiNewPos = uiPos_;
        if (pParent_)
        {
            uiNewPos += uiOffset;
            if (uiNewPos > pParent_->Length())
                uiNewPos = s_uint::NaN;
        }
        return const_iterator(pParent_, uiNewPos);
    }

    s_str::const_iterator s_str::const_iterator::operator - ( const s_int& iOffset ) const
    {
        s_uint uiNewPos = uiPos_;
        if (pParent_)
        {
            if (iOffset < 0)
            {
                uiNewPos += s_uint(iOffset);
                if (uiNewPos > pParent_->Length())
                    uiNewPos = s_uint::NaN;
            }
            if (iOffset > 0)
            {
                s_uint uiOffset = s_uint(iOffset);
                if (uiOffset < uiNewPos)
                    uiNewPos -= s_uint(iOffset);
                else
                    uiNewPos = 0u;
            }
        }
        return const_iterator(pParent_, uiNewPos);
    }

    s_str::const_iterator s_str::const_iterator::operator - ( const s_uint& uiOffset ) const
    {
        s_uint uiNewPos = uiPos_;
        if (pParent_)
        {
            if (uiOffset < uiNewPos)
                uiNewPos -= uiOffset;
            else
                uiNewPos = 0u;
        }
        return const_iterator(pParent_, uiNewPos);
    }


    s_uint s_str::const_iterator::operator - ( const s_str::const_iterator& iter ) const
    {
        if ( (iter.pParent_ == pParent_) && (iter.uiPos_ < uiPos_) )
        {
            return uiPos_-iter.uiPos_;
        }
        else
            return 0;
    }

    s_str::const_iterator& s_str::const_iterator::operator ++ ()
    {
        ++uiPos_;
        if (uiPos_ == pParent_->Length())
            uiPos_ = s_uint::NaN;

        return *this;
    }

    s_str::const_iterator s_str::const_iterator::operator ++ (int)
    {
        uiPos_++;
        if (uiPos_ == pParent_->Length())
            uiPos_ = s_uint::NaN;

        return *this;
    }

    s_str::const_iterator& s_str::const_iterator::operator -- ()
    {
        if (uiPos_.IsValid())
        {
            if (uiPos_ != 0)
                --uiPos_;
        }
        else
        {
            if (pParent_->IsEmpty())
                uiPos_ = s_uint::NaN;
            else
                uiPos_ = pParent_->Length()-1;
        }

        return *this;
    }

    s_str::const_iterator s_str::const_iterator::operator -- (int)
    {
        if (uiPos_.IsValid())
        {
            if (uiPos_ != 0)
                uiPos_--;
        }
        else
        {
            if (pParent_->IsEmpty())
                uiPos_ = s_uint::NaN;
            else
                uiPos_ = pParent_->Length()-1;
        }

        return *this;
    }

    s_bool s_str::const_iterator::operator != (s_str::const_iterator iter)
    {
        if (uiPos_.IsNaN())
        {
            if (iter.uiPos_.IsNaN())
                return (pParent_ != iter.pParent_);
            else
                return true;
        }
        else
        {
            if (iter.uiPos_.IsNaN())
                return true;
            else
                return (pParent_ != iter.pParent_) || (uiPos_ != iter.uiPos_);
        }
    }

    s_str::iterator s_str::begin()
    {
        if (sValue_.length() == 0)
            return iterator(this, s_uint::NaN);
        else
            return iterator(this, 0);
    }

    s_str::const_iterator s_str::begin() const
    {
        if (sValue_.length() == 0)
            return const_iterator(this, s_uint::NaN);
        else
            return const_iterator(this, 0);
    }

    s_str::iterator s_str::end()
    {
        return iterator(this, s_uint::NaN);
    }

    s_str::const_iterator s_str::end() const
    {
        return const_iterator(this, s_uint::NaN);
    }

    string s_str::GetASCII() const
    {
        #ifdef USE_UNICODE
            return string(sValue_.c_str(), sValue_.c_str() + sValue_.size());
        #else
            return sValue_;
        #endif
    }

    wstring s_str::GetUnicode() const
    {
        #ifdef USE_UNICODE
            return sValue_;
        #else
            return wstring(sValue_.c_str(), sValue_.c_str() + sValue_.size());
        #endif
    }
}
