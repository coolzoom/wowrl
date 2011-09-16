/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_var.h"

namespace Frost
{
    const s_type& s_var::VALUE_NONE    = typeid(void);
    const s_type& s_var::VALUE_INT     = typeid(s_int);
    const s_type& s_var::VALUE_UINT    = typeid(s_uint);
    const s_type& s_var::VALUE_FLOAT   = typeid(s_float);
    const s_type& s_var::VALUE_DOUBLE  = typeid(s_double);
    const s_type& s_var::VALUE_BOOL    = typeid(s_bool);
    const s_type& s_var::VALUE_STRING  = typeid(s_str);
    const s_type& s_var::VALUE_POINTER = typeid(void*);


    s_var::s_var() : pValue_(NULL)
    {
    }

    s_var::s_var(const s_var& mVar) : pValue_(mVar.pValue_ ? mVar.pValue_->Clone() : NULL)
    {
    }

    s_var& s_var::operator = (const s_var& mVar)
    {
        if (&mVar != this)
        {
            s_var mTemp(mVar);
            Swap(mTemp);
        }
        return *this;
    }

    s_bool s_var::operator == (const s_var& mVar) const
    {
        if (GetType() == mVar.GetType())
        {
            if (IsOfType<s_str>())
            {
                s_str s1 = Get<s_str>();
                s_str s2 = mVar.Get<s_str>();
                return s1 == s2;
            }
            else if (IsOfType<s_float>())
            {
                s_float f1 = Get<s_float>();
                s_float f2 = mVar.Get<s_float>();
                return f1 == f2;
            }
            else if (IsOfType<s_double>())
            {
                s_double d1 = Get<s_double>();
                s_double d2 = mVar.Get<s_double>();
                return d1 == d2;
            }
            else if (IsOfType<s_int>())
            {
                s_int i1 = Get<s_int>();
                s_int i2 = mVar.Get<s_int>();
                return i1 == i2;
            }
            else if (IsOfType<s_uint>())
            {
                s_uint ui1 = Get<s_uint>();
                s_uint ui2 = mVar.Get<s_uint>();
                return ui1 == ui2;
            }
            else if (IsOfType<s_bool>())
            {
                s_bool b1 = Get<s_bool>();
                s_bool b2 = mVar.Get<s_bool>();
                return b1 == b2;
            }
            else if (IsOfType<void>())
                return true;
        }

        return false;
    }

    s_bool s_var::operator != (const s_var& mVar) const
    {
        return !(operator == (mVar));
    }

    void s_var::Swap(s_var& mVar)
    {
        std::swap(pValue_, mVar.pValue_);
    }

    s_bool s_var::IsEmpty() const
    {
        return (pValue_ == NULL);
    }


    const s_type& s_var::GetType() const
    {
        if (pValue_)
        {
            return pValue_->GetType();
        }
        else
        {
            return typeid(void);
        }
    }

    s_str s_var::ToString() const
    {
        const s_type& mType = GetType();
        if (mType == VALUE_INT) return s_str::Convert(Get<s_int>());
        else if (mType == VALUE_UINT) return s_str::Convert(Get<s_uint>())+"u";
        else if (mType == VALUE_FLOAT) return s_str::Convert(Get<s_float>())+"f";
        else if (mType == VALUE_DOUBLE) return s_str::Convert(Get<s_double>());
        else if (mType == VALUE_BOOL) return s_str::Convert(Get<s_bool>());
        else if (mType == VALUE_STRING) return "\""+Get<s_str>()+"\"";
        else if (mType == VALUE_POINTER) return s_str::Convert(s_ptr<void>(Get<void*>()));
        else return "<none>";
    }
}
