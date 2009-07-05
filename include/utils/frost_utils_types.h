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
    template<class T, class N> class s_pair;
    template<class T> class s_range;
    template<class Key, class Data> class s_map;
    template<class Key, class Data> class s_multimap;
    template<class T> class s_ptr;
    template<class T> class s_refptr;

    /// Binds POD types to their corresponding Frost's types
    template<class T>
    class TypeTraits
    {
    public :
        typedef T Type;
    };

    typedef char string_element;
    typedef std::string string_object;
    typedef std::stringstream string_stream;
    typedef std::fstream file_stream;
}

#include "frost_utils_bool.h"
#include "frost_utils_int.h"
#include "frost_utils_uint.h"
#include "frost_utils_float.h"
#include "frost_utils_str.h"
#include "frost_utils_ctnr.h"
#include "frost_utils_array.h"
#include "frost_utils_pair.h"
#include "frost_utils_range.h"
#include "frost_utils_map.h"
#include "frost_utils_multimap.h"
#include "frost_utils_ptr.h"
#include "frost_utils_refptr.h"

#endif
