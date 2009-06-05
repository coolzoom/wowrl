/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Types header              */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_TYPES_H
#define FROST_UTILS_TYPES_H

#include "frost_utils.h"

#include <cmath>
#include <cfloat>

namespace Frost
{
    template<class T> class s_str_t;
    template<class T> class s_bool_t;
    template<class T> class s_int_t;
    template<class T> class s_uint_t;
    template<class T> class s_float_t;
    template<class T> class s_ctnr;
    template<class T, uint N> class s_array;
    template<class T> class s_ptr;
    template<class T> class s_refptr;

    #ifdef USE_UNICODE
        #define STRING(X) L##X
        typedef wchar_t string_element;
        typedef std::wstring string_object;
        typedef std::wstringstream string_stream;
        typedef std::wfstream file_stream;
    #else
        #define STRING(X) X
        typedef char string_element;
        typedef std::string string_object;
        typedef std::stringstream string_stream;
        typedef std::fstream file_stream;
    #endif

    #include "frost_utils_bool.h"
    #include "frost_utils_int.h"
    #include "frost_utils_uint.h"
    #include "frost_utils_float.h"
    #include "frost_utils_str.h"
    #include "frost_utils_ctnr.h"
    #include "frost_utils_array.h"
    #include "frost_utils_ptr.h"
    #include "frost_utils_refptr.h"
}

#endif
