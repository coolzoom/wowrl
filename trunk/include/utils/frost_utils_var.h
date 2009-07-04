/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_VAR_H
#define FROST_UTILS_VAR_H

#include "frost_utils.h"
#include "frost_utils_types.h"
#include "frost_utils_log.h"

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

        /// Default constructor.
        s_var() : pValue_(NULL) {}

        /// Value assignment constructor.
        /** \param mValue The value to assign
        */
        template <class T>
        s_var(const T& mValue) : pValue_(new value<T>(mValue)) {}

        /// Copy constructor.
        /** \param mVar The s_var to copy
        */
        s_var(const s_var& mVar) : pValue_(mVar.pValue_ ? mVar.pValue_->Clone() : NULL) {}

        /// Destructor.
        ~s_var()
        {
        }

        s_var& operator = (const s_var& mVar)
        {
            if (&mVar != this)
            {
                s_var mTemp(mVar);
                Swap(mTemp);
            }
            return *this;
        }

        /// Swaps this value with another.
        /** \param mVar the value to swap with this one
        */
        void Swap(s_var& mVar)
        {
            std::swap(pValue_, mVar.pValue_);
        }

        /// Returns the contained value.
        /** \return The contained value
        *   \note If the provided type doesn't math the
        *         contained value's, a Warning is printed
        *         in the log, and this function returns a
        *         default value.
        */
        template<class T>
        T Get() const
        {
            value<T>* pValue = dynamic_cast< value<T>* >(pValue_.Get());
            if (pValue)
            {
                return pValue->mT_;
            }
            else
            {
                if (pValue_)
                    Warning("s_var",
                        "Conversion from type \""+s_str(pValue_->GetType().name())+
                        "\" to \""+s_str(typeid(T).name())+"\" failed. Returning default "
                        "value."
                    );

                return T();
            }
        }

        /// Checks if this variable is empty.
        /** \return 'true' if this variable is empty
        *   \note Only the default constructor of s_var returns
        *         an empty variable.
        */
        s_bool IsEmpty() const
        {
            return (pValue_ == NULL);
        }

        /// Returns the type of the contained value.
        /** \return The type of the contained value
        *   \note Returns typeid(void) if the variable is empty.
        */
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

        /// Checks the contained value's type.
        /** \return 'true' if the contained value's type is the one
        *           you provided
        */
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

        /// Converts this variable to a string.
        /** \return This variable converted to a string
        */
        s_str ToString() const
        {
            const s_type& mType = GetType();
            if (mType == VALUE_INT) return s_str(Get<s_int>());
            else if (mType == VALUE_UINT) return s_str(Get<s_uint>())+"u";
            else if (mType == VALUE_FLOAT) return s_str(Get<s_float>())+"f";
            else if (mType == VALUE_DOUBLE) return s_str(Get<s_double>());
            else if (mType == VALUE_BOOL) return s_str(Get<s_bool>());
            else if (mType == VALUE_STRING) return "\""+Get<s_str>()+"\"";
            else if (mType == VALUE_POINTER) return s_str() << Get<void*>();
            else return "<none>";
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

        s_refptr<value_base> pValue_;
    };
}

#endif
