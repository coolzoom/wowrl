/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_VAR_H
#define FROST_UTILS_VAR_H

#include "frost_utils.h"
#include "frost_utils_types.h"

namespace Frost
{
    typedef std::type_info s_type;

    const s_type& VALUE_NONE    = typeid(void);
    const s_type& VALUE_INT     = typeid(s_int);
    const s_type& VALUE_UINT    = typeid(s_uint);
    const s_type& VALUE_FLOAT   = typeid(s_float);
    const s_type& VALUE_DOUBLE  = typeid(s_double);
    const s_type& VALUE_BOOL    = typeid(s_bool);
    const s_type& VALUE_STRING  = typeid(s_str);
    const s_type& VALUE_POINTER = typeid(void*);

    /// Base type : untyped variable
    /** The purpose of this class is to have an untyped
    *   return value / argument. It allows simple manipulation
    *   of events, and a lot of other things.<br><br>
    *   It uses a little bit more memory than a simple object :<br>
    *   sizeof(void*) + sizeof(T)<br>... and has the overhead of
    *   using the v-table (because of inheritance). At last, s_var
    *   does a dynamic_cast (which is not the fastest thing in the
    *   world) each time you call Get().<br>
    *   So this class is definatly slower than base types : <b>use it
    *   wisely</b>.<br><br>
    *   <u>Note</u> : This class is highly inspired from boost::any.
    */
    class s_var
    {
    public :

        s_var() : pValue_(NULL) {}

        template <class T>
        s_var(const T& pValue) : pValue_(new value<T>(pValue)) {}

        s_var(const s_var& mAny) : pValue_(mAny.pValue_ ? mAny.pValue_->Clone() : 0) {}

        ~s_var()
        {
            delete pValue_;
        }

        s_var& operator = (const s_var& mAny)
        {
            if (&mAny != this)
            {
                s_var mTemp(mAny);
                Swap(mTemp);
            }
            return *this;
        }

        void Swap(s_var& mAny)
        {
            std::swap(pValue_, mAny.pValue_);
        }

        template<class T>
        T Get() const
        {
            value<T>* pValue = dynamic_cast< value<T>* >(pValue_);
            if (pValue)
            {
                return pValue->mT_;
            }
            else
            {
                return T();
            }
        }

        s_bool IsEmpty() const
        {
            return (pValue_ == NULL);
        }

        const s_type& GetType() const
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

        template<class T>
        s_bool IsOfType() const
        {
            if (pValue_)
            {
                return pValue_->GetType() == typeid(T);
            }
            else
            {
                return typeid(void) == typeid(T);
            }
        }

    private :

        /** \cond NOT_REMOVE_FROM_DOC
        */

        class value_base
        {
        public :

            virtual ~value_base() {}
            virtual value_base* Clone() const = 0;
            virtual const s_type& GetType() const = 0;
        };

        template <class T>
        class value : public value_base
        {
        public :

            friend T s_var::Get<>() const;

            value(const T& mT) : mT_(mT) {}

            value_base* Clone() const
            {
                return new value(mT_);
            }

            const s_type& GetType() const
            {
                return typeid(T);
            }

        private :

            T mT_;
        };

        /** \endcond
        */

        value_base* pValue_;
    };

    s_str operator + ( const string_element* sLeft, const s_var& vRight )
    {
        const s_type& mType = vRight.GetType();
        if (mType == VALUE_INT) return s_str(sLeft) + vRight.Get<s_int>();
        else if (mType == VALUE_UINT) return s_str(sLeft) + vRight.Get<s_uint>();
        else if (mType == VALUE_FLOAT) return s_str(sLeft) + vRight.Get<s_float>();
        else if (mType == VALUE_DOUBLE) return s_str(sLeft) + vRight.Get<s_double>();
        else if (mType == VALUE_BOOL) return s_str(sLeft) + vRight.Get<s_bool>();
        else if (mType == VALUE_STRING) return s_str(sLeft) + vRight.Get<s_str>();
        else if (mType == VALUE_POINTER) return s_str(sLeft) << vRight.Get<void*>();
        else return s_str(sLeft) + "<none>";
    }

    #ifdef USE_UNICODE
        s_str operator+ ( const char* sLeft, const s_var& vRight )
        {
            const s_type& mType = vRight.GetType();
            if (mType == VALUE_INT) return s_str(sLeft) + vRight.Get<s_int>();
            else if (mType == VALUE_UINT) return s_str(sLeft) + vRight.Get<s_uint>();
            else if (mType == VALUE_FLOAT) return s_str(sLeft) + vRight.Get<s_float>();
            else if (mType == VALUE_DOUBLE) return s_str(sLeft) + vRight.Get<s_double>();
            else if (mType == VALUE_BOOL) return s_str(sLeft) + vRight.Get<s_bool>();
            else if (mType == VALUE_STRING) return s_str(sLeft) + vRight.Get<s_str>();
            else if (mType == VALUE_POINTER) return s_str(sLeft) << vRight.Get<void*>();
            else return s_str(sLeft) + "<none>";
        }
    #endif

    s_str operator + ( const s_str& sLeft, const s_var& vRight )
    {
        const s_type& mType = vRight.GetType();
        if (mType == VALUE_INT) return sLeft + vRight.Get<s_int>();
        else if (mType == VALUE_UINT) return sLeft + vRight.Get<s_uint>();
        else if (mType == VALUE_FLOAT) return sLeft + vRight.Get<s_float>();
        else if (mType == VALUE_DOUBLE) return sLeft + vRight.Get<s_double>();
        else if (mType == VALUE_BOOL) return sLeft + vRight.Get<s_bool>();
        else if (mType == VALUE_STRING) return sLeft + vRight.Get<s_str>();
        else if (mType == VALUE_POINTER) return s_str(sLeft) << vRight.Get<void*>();
        else return sLeft + "<none>";
    }

    s_str& operator << ( s_str& sLeft, const s_var& vRight )
    {
        const s_type& mType = vRight.GetType();
        if (mType == VALUE_INT) return sLeft << vRight.Get<s_int>();
        else if (mType == VALUE_UINT) return sLeft << vRight.Get<s_uint>();
        else if (mType == VALUE_FLOAT) return sLeft << vRight.Get<s_float>();
        else if (mType == VALUE_DOUBLE) return sLeft << vRight.Get<s_double>();
        else if (mType == VALUE_BOOL) return sLeft << vRight.Get<s_bool>();
        else if (mType == VALUE_STRING) return sLeft << vRight.Get<s_str>();
        else if (mType == VALUE_POINTER) return sLeft << vRight.Get<void*>();
        else return sLeft << "<none>";
    }
}

#endif
