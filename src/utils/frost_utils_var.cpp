/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_var.h"
#include "frost_utils_array.h"
#include "frost_utils.h"

using namespace std;

namespace Frost
{
    const s_str s_var::CLASS_NAME = "s_var";

    s_var::s_var()
    {
        mType_ = VALUE_NONE;
        pPtr_ = NULL;
    }

    s_var::s_var( const s_var& vValue )
    {
        mType_ = vValue.mType_;
        if (vValue.pPtr_)
        {
            if (vValue.mType_ == VALUE_INT)
                pPtr_ = new s_int(*((s_int*)vValue.pPtr_));
            else if (vValue.mType_ == VALUE_UINT)
                pPtr_ = new s_uint(*((s_uint*)vValue.pPtr_));
            else if (vValue.mType_ == VALUE_FLOAT)
                pPtr_ = new s_float(*((s_float*)vValue.pPtr_));
            else if (vValue.mType_ == VALUE_DOUBLE)
                pPtr_ = new s_double(*((s_double*)vValue.pPtr_));
            else if (vValue.mType_ == VALUE_BOOL)
                pPtr_ = new s_bool(*((s_bool*)vValue.pPtr_));
            else if (vValue.mType_ == VALUE_STRING)
                pPtr_ = new s_str(*((s_str*)vValue.pPtr_));
            else if (vValue.mType_ == VALUE_POINTER)
                pPtr_ = vValue.pPtr_;
        }
    }

    s_var::~s_var()
    {
        Delete_();
    }

    s_var::s_var( const s_int& iValue )
    {
        mType_ = VALUE_INT;
        pPtr_ = new s_int(iValue);
    }

    s_var::s_var( const s_uint& uiValue )
    {
        mType_ = VALUE_UINT;
        pPtr_ = new s_uint(uiValue);
    }

    s_var::s_var( const s_float& fValue )
    {
        mType_ = VALUE_FLOAT;
        pPtr_ = new s_float(fValue);
    }

    s_var::s_var( const s_double& dValue )
    {
        mType_ = VALUE_DOUBLE;
        pPtr_ = new s_double(dValue);
    }

    s_var::s_var( const s_bool& bValue )
    {
        mType_ = VALUE_BOOL;
        pPtr_ = new s_bool(bValue);
    }

    s_var::s_var( const s_str& sValue )
    {
        mType_ = VALUE_STRING;
        pPtr_ = new s_str(sValue);
    }

    s_var::s_var( void* pValue )
    {
        mType_ = VALUE_POINTER;
        pPtr_ = pValue;
    }

    void s_var::SetI( const s_int& iValue )
    {
        if (mType_ == VALUE_INT)
            *(s_int*)pPtr_ = iValue;
        else
        {
            Delete_();
            mType_ = VALUE_INT;
            pPtr_ = new s_int(iValue);
        }
    }

    void s_var::SetUI( const s_uint& uiValue )
    {
        if (mType_ == VALUE_UINT)
            *(s_uint*)pPtr_ = uiValue;
        else
        {
            Delete_();
            mType_ = VALUE_UINT;
            pPtr_ = new s_uint(uiValue);
        }
    }

    void s_var::SetF( const s_float& fValue )
    {
        if (mType_ == VALUE_FLOAT)
            *(s_float*)pPtr_ = fValue;
        else
        {
            Delete_();
            mType_ = VALUE_FLOAT;
            pPtr_ = new s_float(fValue);
        }
    }

    void s_var::SetD( const s_double& dValue )
    {
        if (mType_ == VALUE_DOUBLE)
            *(s_double*)pPtr_ = dValue;
        else
        {
            Delete_();
            mType_ = VALUE_DOUBLE;
            pPtr_ = new s_double(dValue);
        }
    }

    void s_var::SetB( const s_bool& bValue )
    {
        if (mType_ == VALUE_BOOL)
            *(s_bool*)pPtr_ = bValue;
        else
        {
            Delete_();
            mType_ = VALUE_BOOL;
            pPtr_ = new s_bool(bValue);
        }
    }

    void s_var::SetS( const s_str& sValue )
    {
        if (mType_ == VALUE_STRING)
            *(s_str*)pPtr_ = sValue;
        else
        {
            Delete_();
            mType_ = VALUE_STRING;
            pPtr_ = new s_str(sValue);
        }
    }

    void s_var::SetP( void* pValue )
    {
        Delete_();
        mType_ = VALUE_POINTER;
        pPtr_ = pValue;
    }

    s_int s_var::GetI() const
    {
        if (pPtr_)
        {
            if (mType_ == VALUE_INT)
                return *((s_int*)pPtr_);
            else if (mType_ == VALUE_UINT)
                return s_int(*((s_uint*)pPtr_));
            else if (mType_ == VALUE_FLOAT)
                return s_int(*((s_float*)pPtr_));
            else if (mType_ == VALUE_DOUBLE)
                return s_int(*((s_double*)pPtr_));
            else if (mType_ == VALUE_BOOL)
                return s_int(*((s_bool*)pPtr_));
            else if (mType_ == VALUE_STRING)
                return s_int(*((s_str*)pPtr_));
            else if (mType_ == VALUE_POINTER)
                return s_int((int)(long)pPtr_);
        }

        return s_int(s_int::INTEGER_NAN);
    }

    s_uint s_var::GetUI() const
    {
        if (pPtr_)
        {
            if (mType_ == VALUE_UINT)
                return *((s_uint*)pPtr_);
            else if (mType_ == VALUE_INT)
                return s_uint(*((s_int*)pPtr_));
            else if (mType_ == VALUE_FLOAT)
                return s_uint(*((s_float*)pPtr_));
            else if (mType_ == VALUE_DOUBLE)
                return s_uint(*((s_double*)pPtr_));
            else if (mType_ == VALUE_BOOL)
                return s_uint(*((s_bool*)pPtr_));
            else if (mType_ == VALUE_STRING)
                return s_uint(*((s_str*)pPtr_));
            else if (mType_ == VALUE_POINTER)
                return s_uint((uint)(long)pPtr_);
        }

        return s_uint(s_uint::INTEGER_NAN);
    }

    s_float s_var::GetF() const
    {
        if (pPtr_)
        {
            if (mType_ == VALUE_FLOAT)
                return *((s_float*)pPtr_);
            else if (mType_ == VALUE_UINT)
                return s_float(*((s_uint*)pPtr_));
            else if (mType_ == VALUE_INT)
                return s_float(*((s_int*)pPtr_));
            else if (mType_ == VALUE_DOUBLE)
                return s_float(*((s_double*)pPtr_));
            else if (mType_ == VALUE_BOOL)
                return s_float(*((s_bool*)pPtr_));
            else if (mType_ == VALUE_STRING)
                return s_float(*((s_str*)pPtr_));
            else if (mType_ == VALUE_POINTER)
                return s_float((float)(long)pPtr_);
        }

        return s_float(s_float::FLOAT_NAN);
    }

    s_double s_var::GetD() const
    {
        if (pPtr_)
        {
            if (mType_ == VALUE_DOUBLE)
                return *((s_double*)pPtr_);
            else if (mType_ == VALUE_UINT)
                return s_double(*((s_uint*)pPtr_));
            else if (mType_ == VALUE_FLOAT)
                return s_double(*((s_float*)pPtr_));
            else if (mType_ == VALUE_DOUBLE)
                return s_double(*((s_double*)pPtr_));
            else if (mType_ == VALUE_BOOL)
                return s_double(*((s_bool*)pPtr_));
            else if (mType_ == VALUE_STRING)
                return s_double(*((s_str*)pPtr_));
            else if (mType_ == VALUE_POINTER)
                return s_double((double)(long)pPtr_);
        }

        return s_double(s_double::DOUBLE_NAN);
    }

    s_bool s_var::GetB() const
    {
        if (pPtr_)
        {
            if (mType_ == VALUE_BOOL)
                return *((s_bool*)pPtr_);
            else if (mType_ == VALUE_STRING)
                return s_bool(*((s_str*)pPtr_));
        }

        return s_bool(false);
    }

    s_str s_var::GetS() const
    {
        if (pPtr_)
        {
            if (mType_ == VALUE_STRING)
                return *((s_str*)pPtr_);
            else if (mType_ == VALUE_UINT)
                return s_str(*((s_uint*)pPtr_));
            else if (mType_ == VALUE_FLOAT)
                return s_str(*((s_float*)pPtr_));
            else if (mType_ == VALUE_DOUBLE)
                return s_str(*((s_double*)pPtr_));
            else if (mType_ == VALUE_BOOL)
                return s_str(*((s_bool*)pPtr_));
            else if (mType_ == VALUE_INT)
                return s_str(*((s_int*)pPtr_));
            else if (mType_ == VALUE_POINTER)
                return s_str(s_uint((uint)(long)pPtr_));
        }

        return s_str("\0");
    }

    void* s_var::GetP() const
    {
        return pPtr_;
    }

    ValueType s_var::GetType() const
    {
        return mType_;
    }

    s_var& s_var::operator = ( const s_var& vValue )
    {
        if (vValue.mType_ == mType_)
        {
            if (vValue.pPtr_)
            {
                if (vValue.mType_ == VALUE_INT)
                    *(s_int*)pPtr_ = *(s_int*)vValue.pPtr_;
                else if (vValue.mType_ == VALUE_UINT)
                    *(s_uint*)pPtr_ = *(s_uint*)vValue.pPtr_;
                else if (vValue.mType_ == VALUE_FLOAT)
                    *(s_float*)pPtr_ = *(s_float*)vValue.pPtr_;
                else if (vValue.mType_ == VALUE_DOUBLE)
                    *(s_double*)pPtr_ = *(s_double*)vValue.pPtr_;
                else if (vValue.mType_ == VALUE_BOOL)
                    *(s_bool*)pPtr_ = *(s_bool*)vValue.pPtr_;
                else if (vValue.mType_ == VALUE_STRING)
                    *(s_str*)pPtr_ = *(s_str*)vValue.pPtr_;
                else if (vValue.mType_ == VALUE_POINTER)
                    pPtr_ = vValue.pPtr_;
            }
        }
        else
        {
            Delete_();
            mType_ = vValue.mType_;
            if (vValue.pPtr_)
            {
                if (vValue.mType_ == VALUE_INT)
                    pPtr_ = new s_int(*((s_int*)vValue.pPtr_));
                else if (vValue.mType_ == VALUE_UINT)
                    pPtr_ = new s_uint(*((s_uint*)vValue.pPtr_));
                else if (vValue.mType_ == VALUE_FLOAT)
                    pPtr_ = new s_float(*((s_float*)vValue.pPtr_));
                else if (vValue.mType_ == VALUE_DOUBLE)
                    pPtr_ = new s_double(*((s_double*)vValue.pPtr_));
                else if (vValue.mType_ == VALUE_BOOL)
                    pPtr_ = new s_bool(*((s_bool*)vValue.pPtr_));
                else if (vValue.mType_ == VALUE_STRING)
                    pPtr_ = new s_str(*((s_str*)vValue.pPtr_));
                else if (vValue.mType_ == VALUE_POINTER)
                    pPtr_ = vValue.pPtr_;
            }
            else
                pPtr_ = NULL;
        }

        return *this;
    }

    s_ctnr<s_var> s_var::operator, ( const s_var& vValue ) const
    {
        s_ctnr<s_var> mContainer;
        mContainer.PushBack(*this);
        mContainer.PushBack(vValue);
        return mContainer;
    }

    void s_var::Delete_()
    {
        if (mType_ == VALUE_INT)
            delete (s_int*)pPtr_;
        else if (mType_ == VALUE_UINT)
            delete (s_uint*)pPtr_;
        else if (mType_ == VALUE_FLOAT)
            delete (s_float*)pPtr_;
        else if (mType_ == VALUE_DOUBLE)
            delete (s_double*)pPtr_;
        else if (mType_ == VALUE_BOOL)
            delete (s_bool*)pPtr_;
        else if (mType_ == VALUE_STRING)
            delete (s_str*)pPtr_;

        pPtr_= NULL;
    }

    s_str operator + ( const string_element* sLeft, const s_var& vRight )
    {
        switch (vRight.GetType())
        {
            case VALUE_INT : return s_str(sLeft) + vRight.GetI();
            case VALUE_UINT : return s_str(sLeft) + vRight.GetUI();
            case VALUE_FLOAT : return s_str(sLeft) + vRight.GetF();
            case VALUE_DOUBLE : return s_str(sLeft) + vRight.GetD();
            case VALUE_BOOL : return s_str(sLeft) + vRight.GetB();
            case VALUE_STRING : return s_str(sLeft) + vRight.GetS();
            case VALUE_POINTER : return s_str(sLeft) << vRight.GetP();
            default : return s_str(sLeft) + "<none>";
        }
    }

    #ifdef USE_UNICODE
        s_str operator+ ( const char* sLeft, const s_var& vRight )
        {
            switch (vRight.GetType())
            {
                case VALUE_INT : return s_str(sLeft) + vRight.GetI();
                case VALUE_UINT : return s_str(sLeft) + vRight.GetUI();
                case VALUE_FLOAT : return s_str(sLeft) + vRight.GetF();
                case VALUE_DOUBLE : return s_str(sLeft) + vRight.GetD();
                case VALUE_BOOL : return s_str(sLeft) + vRight.GetB();
                case VALUE_STRING : return s_str(sLeft) + vRight.GetS();
                case VALUE_POINTER : return s_str(sLeft) << vRight.GetP();
                default : return s_str(sLeft) + "<none>";
            }
        }
    #endif

    s_str operator + ( const s_str& sLeft, const s_var& vRight )
    {
        switch (vRight.GetType())
        {
            case VALUE_INT : return sLeft + vRight.GetI();
            case VALUE_UINT : return sLeft + vRight.GetUI();
            case VALUE_FLOAT : return sLeft + vRight.GetF();
            case VALUE_DOUBLE : return sLeft + vRight.GetD();
            case VALUE_BOOL : return sLeft + vRight.GetB();
            case VALUE_STRING : return sLeft + vRight.GetS();
            case VALUE_POINTER : return s_str(sLeft) << vRight.GetP();
            default : return sLeft + "<none>";
        }
    }

    s_str& operator << ( s_str& sLeft, const s_var& vRight )
    {
        switch (vRight.GetType())
        {
            case VALUE_INT : return sLeft << vRight.GetI();
            case VALUE_UINT : return sLeft << vRight.GetUI();
            case VALUE_FLOAT : return sLeft << vRight.GetF();
            case VALUE_DOUBLE : return sLeft << vRight.GetD();
            case VALUE_BOOL : return sLeft << vRight.GetB();
            case VALUE_STRING : return sLeft << vRight.GetS();
            case VALUE_POINTER : return sLeft << vRight.GetP();
            default : return sLeft << "<none>";
        }
    }
}
