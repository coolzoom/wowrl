/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_BOOL_H
#define FROST_UTILS_BOOL_H

#include "frost_utils.h"

namespace Frost
{
    class s_str;
    template<class T> class s_ctnr;

    #ifdef USE_UNICODE
        typedef wchar_t string_element;
        typedef std::wstring string_object;
        typedef std::wstringstream string_stream;
        typedef std::wfstream file_stream;
    #else
        typedef char string_element;
        typedef std::string string_object;
        typedef std::stringstream string_stream;
        typedef std::fstream file_stream;
    #endif

    /// Base type : boolean
    /** Frost's base types are made to allow simpler
    *   manipulation of numbers, booleans and strings.
    *   They simplify conversions (mostly to string),
    *   allow easy access to math/string functions and
    *   implements new functions that are not present
    *   in the standard libraries.
    */
    class s_bool
    {
    public :

        s_bool();
        s_bool(const bool& bValue);
        explicit s_bool(const string_element* sValue);
        explicit s_bool(const string_object& sValue);
        explicit s_bool(const s_str& sValue);

        /// Returns a const reference to the boolean.
        /** \return A const reference to the boolean
        */
        inline const bool& Get() const { return bValue_; }

        /// Returns a reference to the boolean.
        /** \return A reference to the boolean
        */
        inline bool&   GetR() { return bValue_; }

        /// Returns the first argument if this boolean is true, and the second otherwise.
        /** \param sTrue  The string to return if 'true'
        *   \param sFalse The string to return if 'false'
        *   \return Either sTrue or sFalse.
        */
        const s_str&   GetAsString(const s_str& sTrue, const s_str& sFalse) const;

        bool           operator !  () const;
        typedef        const bool& (s_bool::*MemberFn)() const;
        operator       MemberFn() const;

        bool           operator == (const s_bool& bValue) const;
        bool           operator != (const s_bool& bValue) const;

        s_str          operator +  (const string_element* sValue) const;
        #ifdef USE_UNICODE
            s_str      operator +  (const char* sValue) const;
        #endif
        s_str          operator +  (const s_str& sValue) const;

        s_bool&        operator =  (const s_bool& bValue);

        s_ctnr<s_bool> operator,   (const s_bool& bValue) const;

        static bool bDummy;

        static const s_str CLASS_NAME;

    private :

        bool bValue_;
    };
}

#endif
