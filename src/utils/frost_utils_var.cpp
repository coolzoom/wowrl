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

    s_var::s_var(const s_float& fValue)
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
        Delete_();
        mType_ = VALUE_INT;
        pPtr_ = new s_int(iValue);
    }

    void s_var::SetUI( const s_uint& uiValue )
    {
        Delete_();
        mType_ = VALUE_UINT;
        pPtr_ = new s_uint(uiValue);
    }

    void s_var::SetF( const s_float& fValue )
    {
        Delete_();
        mType_ = VALUE_FLOAT;
        pPtr_ = new s_float(fValue);
    }

    void s_var::SetD( const s_double& dValue )
    {
        Delete_();
        mType_ = VALUE_DOUBLE;
        pPtr_ = new s_double(dValue);
    }

    void s_var::SetB( const s_bool& bValue )
    {
        Delete_();
        mType_ = VALUE_BOOL;
        pPtr_ = new s_bool(bValue);
    }

    void s_var::SetS( const s_str& sValue )
    {
        Delete_();
        mType_ = VALUE_STRING;
        pPtr_ = new s_str(sValue);
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
            if (mType_ == VALUE_INT)
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

        return s_str('\0');
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

        return *this;
    }

    s_ctnr<s_var> s_var::operator, ( const s_var& vValue ) const
    {
        s_ctnr<s_var> mContainer;
        mContainer.Push(*this);
        mContainer.Push(vValue);
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
}
