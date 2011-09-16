/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_VAR_H
#define FROST_UTILS_VAR_H

#include "frost_utils_types.h"
#include "frost_utils_log.h"

#include <typeinfo>

namespace Frost
{
    typedef std::type_info s_type;

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
    *   wisely</b>.
    *   \note This class is highly inspired from boost::any.
    *   \note The comparison operators '==' and '!=' only work with
    *         base Frost types (s_str, s_float, s_double, s_uint,
    *         s_int and s_bool), and they assume that the compared
    *         variables are of the same internal type.
    *         If you need to compare another type T, just write :
    *                   v1.Get<T>() == v2.Get<T>()
    */
    class s_var
    {
    public :

        /// Default constructor.
        s_var();

        /// Value assignment constructor.
        /** \param mValue The value to assign
        */
        template <class T>
        s_var(const T& mValue) : pValue_(new value<T>(mValue)) {}

        /// Copy constructor.
        /** \param mVar The s_var to copy
        */
        s_var(const s_var& mVar);

        s_var& operator = (const s_var& mVar);

        s_bool operator == (const s_var& mVar) const;

        s_bool operator != (const s_var& mVar) const;

        /// Swaps this value with another.
        /** \param mVar the value to swap with this one
        */
        void Swap(s_var& mVar);

        /// Returns the contained value.
        /** \return The contained value
        *   \note If the provided type doesn't match the
        *         contained value's, a Warning is printed
        *         in the log, and this function returns a
        *         default value.
        */
        template<class T>
        const T Get() const
        {
            const value<T>* pValue = dynamic_cast< const value<T>* >(pValue_.Get());
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
        s_bool IsEmpty() const;

        /// Returns the type of the contained value.
        /** \return The type of the contained value
        *   \note Returns typeid(void) if the variable is empty.
        */
        const s_type& GetType() const;

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
        s_str ToString() const;

        static const s_type& VALUE_NONE;
        static const s_type& VALUE_INT;
        static const s_type& VALUE_UINT;
        static const s_type& VALUE_FLOAT;
        static const s_type& VALUE_DOUBLE;
        static const s_type& VALUE_BOOL;
        static const s_type& VALUE_STRING;
        static const s_type& VALUE_POINTER;

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

            #ifdef MSVC
                // Note : Disable a false warning from Microsoft's Visual C++ :
                // "the inline specifier cannot be used when a friend declaration
                // refers to a specialization of a function template"
                #pragma warning( disable : 4396 )
            #endif
            friend const T s_var::Get<>() const;

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
