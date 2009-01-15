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

#include <sstream>

#ifdef FROST_LINUX
    #include <cstdlib>
    #include <algorithm>
#endif

using namespace std;

namespace Frost
{
    char s_str::cDummy = '\0';
    const s_str s_str::CLASS_NAME = "s_str";

    s_str::s_str()
    {
        sValue_ = "";
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
    }

    s_str::s_str( const string& sValue )
    {
        sValue_ = sValue;
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
    }

    s_str::s_str( const char* sValue )
    {
        sValue_ = sValue;
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
    }

    s_str::s_str( const char& cValue, const s_uint& uiCharNbr )
    {
        sValue_ = string(uiCharNbr.Get(), cValue);
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
    }

    s_str::s_str( const s_int& iValue )
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = "";
        this->operator<<(iValue);
    }

    s_str::s_str( const s_uint& uiValue )
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = "";
        this->operator<<(uiValue);
    }

    s_str::s_str( const s_float& fValue )
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = "";
        this->operator<<(fValue);
    }

    s_str::s_str( const s_double& dValue )
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = "";
        this->operator<<(dValue);
    }

    s_str::s_str( const s_bool& bValue )
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = "";
        this->operator<<(bValue);
    }

    s_str::s_str( const char& cValue )
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = "";
        this->operator+=(cValue);
    }

    s_str::s_str(const s_int& iValue, const s_uint& uiCharNbr)
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = "";
        this->operator<<(iValue);

        while (sValue_.length() < uiCharNbr.Get())
            sValue_ = '0' + sValue_;
    }

    s_str::s_str(const s_uint& uiValue, const s_uint& uiCharNbr)
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = "";
        this->operator<<(uiValue);

        while (sValue_.length() < uiCharNbr.Get())
            sValue_ = '0' + sValue_;
    }

    s_str::s_str(const s_float& fValue, const s_uint& uiCharNbr)
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = "";
        this->operator<<(fValue);

        while (sValue_.length() < uiCharNbr.Get())
            sValue_ = '0' + sValue_;
    }

    s_str::s_str(const s_double& dValue, const s_uint& uiCharNbr)
    {
        mIntConvType_ = CONV_DECIMAL;
        mBoolConvType_ = CONV_TRUE_FALSE;
        sValue_ = "";
        this->operator<<(dValue);

        while (sValue_.length() < uiCharNbr.Get())
            sValue_ = '0' + sValue_;
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

    s_bool s_str::IsNumber() const
    {
        stringstream mTemp(sValue_);
        double dValue;
        mTemp >> dValue;
        return !mTemp.fail();
    }

    s_bool s_str::IsBoolean() const
    {
        switch (mBoolConvType_)
        {
            case CONV_TRUE_FALSE :
                return ((sValue_ == "false") || (sValue_ == "true"));
            case CONV_1_0 :
                return ((sValue_ == "0")     || (sValue_ == "1"));
            case CONV_YES_NO :
                return ((sValue_ == "no")    || (sValue_ == "yes"));
            default :
                return false;
        }
    }

    char& s_str::operator[] ( const s_uint& uiIndex )
    {
        if (uiIndex.Get() < sValue_.size())
            return sValue_[uiIndex.Get()];
        else
            return cDummy;
    }

    s_str s_str::operator+ ( const s_str& mValue ) const
    {
        return s_str(sValue_ + mValue.sValue_);
    }

    s_str s_str::operator+ ( const char& cValue ) const
    {
        string sTemp = sValue_;
        sTemp.append(1, cValue);
        return s_str(sTemp);
    }

    s_str s_str::operator+ (const char* sValue) const
    {
        return s_str(sValue_ + sValue);
    }

    s_str s_str::operator+ (const s_int& iValue) const
    {
        return s_str(sValue_) + s_str(iValue);
    }

    s_str s_str::operator+ (const s_uint& uiValue) const
    {
        return s_str(sValue_) + s_str(uiValue);
    }

    s_str s_str::operator+ (const s_float& fValue) const
    {
        return s_str(sValue_) + s_str(fValue);
    }

    s_str s_str::operator+ (const s_double& dValue) const
    {
        return s_str(sValue_) + s_str(dValue);
    }

    s_str s_str::operator+ (const s_bool& bValue) const
    {
        return s_str(sValue_) + s_str(bValue);
    }

    s_str s_str::operator- ( const s_uint& uiNumber ) const
    {
        if (uiNumber.Get() < sValue_.size())
        {
            string sTemp = sValue_;
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

    void s_str::operator+= ( const char& cValue )
    {
        sValue_.append(1, cValue);
    }

    void s_str::operator-= ( const s_uint& uiNumber )
    {
        if (uiNumber.Get() < sValue_.size())
            sValue_ = sValue_.substr(0, sValue_.size()-uiNumber.Get());
        else
            sValue_ = "";
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

    s_str& s_str::operator<< ( const char* sValue )
    {
        sValue_ += sValue;
        return *this;
    }

    #ifdef FROST_LINUX
    // Code taken from : http://www.jb.man.ac.uk/~slowe/cpp/itoa.html
    char* itoa ( int iValue, char* sResult, int iBase )
    {
        // Check that the base if valid
        if (iBase < 2 || iBase > 16)
        {
            *sResult = 0;
            return sResult;
        }

        char* sOut = sResult;
        int iQuotient = iValue;

        do
        {
            *sOut = "0123456789abcdef"[ std::abs( iQuotient % iBase ) ];

            ++sOut;

            iQuotient /= iBase;

        } while (iQuotient);

        // Only apply negative sign for base 10
        if ( iValue < 0 && iBase == 10) *sOut++ = '-';

        std::reverse(sResult, sOut);
        *sOut = 0;

        return sResult;

    }
    #endif

    s_str& s_str::operator<< ( const int& iValue )
    {
        unsigned int uiBase = 10;
        switch (mIntConvType_)
        {
            case CONV_DECIMAL : uiBase = 10; break;
            case CONV_HEXA : uiBase = 16; break;
            case CONV_BIN : uiBase = 2; break;
        }
        char* sTemp = new char[12];
        itoa(iValue, sTemp, uiBase);
        sValue_ += sTemp;
        delete sTemp;
        return *this;
    }

    s_str& s_str::operator<< ( const uint& uiValue )
    {
        return operator<<(static_cast<int>(uiValue));
    }

    s_str& s_str::operator<< ( const float& fValue )
    {
        ostringstream sStream;
        sStream << fValue;
        sValue_ += sStream.str();
        return *this;
    }

    s_str& s_str::operator<< ( const double& dValue )
    {
        ostringstream sStream;
        sStream << dValue;
        sValue_ += sStream.str();
        return *this;
    }

    s_str& s_str::operator<< ( const bool& bValue )
    {
        string sChoice[2] = { "", "" };
        switch (mBoolConvType_)
        {
            case CONV_TRUE_FALSE : sChoice[0] = "false"; sChoice[1] = "true"; break;
            case CONV_1_0 : sChoice[0] = "0"; sChoice[1] = "1"; break;
            case CONV_YES_NO : sChoice[0] = "no"; sChoice[1] = "yes"; break;
        }

        sValue_ += sChoice[(int)bValue];
        return *this;
    }

    s_str& s_str::operator<< ( const char& cValue )
    {
        this->operator+=(cValue);
        return *this;
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

    s_uint s_str::Find( const s_str& sValue, const s_uint& uiStart ) const
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


    void s_str::CapitalStart( const s_bool& bCapitalStart )
    {
        char cFirst = (*this)[0];
        if (bCapitalStart)
            (*this)[0] = toupper(cFirst);
        else
            (*this)[0] = tolower(cFirst);
    }

    std::vector<s_str> s_str::Cut( const s_str& sDelim, const s_uint& uiMaxCut ) const
    {
        vector<s_str> lPieces;
        s_uint uiPos = Find(sDelim);
        s_uint uiLastPos;
        s_uint uiCount;
        s_uint uiCurSize;
        while (uiPos.IsValid())
        {
            uiCurSize = uiPos - uiLastPos;
            if (!uiCurSize.IsNull())
                lPieces.push_back(Extract(uiLastPos, uiCurSize));
            uiLastPos = uiPos + sDelim.Length();
            uiPos = Find(sDelim, uiLastPos);
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
        s_uint uiPos = Find(sPattern);
        while (uiPos.IsValid())
        {
            uiCount++;
            uiPos++;
            uiPos = Find(sPattern, uiPos);
        }

        return uiCount;
    }

    void s_str::Erase( const s_uint& uiStart, const s_uint& uiNbr )
    {
        if (!uiNbr)
            sValue_.erase(uiStart.Get(), string::npos);
        else
            sValue_.erase(uiStart.Get(), uiNbr.Get());
    }

    void s_str::EraseRange( const s_uint& uiStart, const s_uint& uiEnd )
    {
        if (!uiEnd)
            sValue_.erase(uiStart.Get(), string::npos);
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
        if (!uiNbr)
            return s_str(sValue_.substr(uiStart.Get(), string::npos));
        else
            return s_str(sValue_.substr(uiStart.Get(), uiNbr.Get()));
    }

    s_str s_str::ExtractRange( const s_uint& uiStart, const s_uint& uiEnd ) const
    {
        if (!uiEnd)
            return s_str(sValue_.substr(uiStart.Get(), string::npos));
        else
            return s_str(sValue_.substr(uiStart.Get(), (uiEnd-uiStart).Get()));
    }

    s_uint s_str::Length() const
    {
        return s_uint((uint)sValue_.length());
    }

    s_uint s_str::Size() const
    {
        return s_uint((uint)sValue_.size());
    }

    s_uint s_str::Trim( const char& cPattern )
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
        s_uint uiPos = Find(sPattern);

        while (uiPos.IsValid())
        {
            sValue_.replace(uiPos.Get(), sPattern.Length().Get(), sReplacement.Get());
            uiPos = Find(sPattern, uiPos+sReplacement.Length());
            uiCount++;
        }

        return uiCount;
    }

    s_str operator+( const char* sLeft, const s_str& sRight)
    {
        return s_str(sLeft) + sRight;
    }

    s_ctnr<s_str> s_str::operator, ( const s_str& sValue ) const
    {
        s_ctnr<s_str> mContainer;
        mContainer.Push(*this);
        mContainer.Push(sValue);
        return mContainer;
    }
}
