/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Types header              */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_TYPES_H
#define FROST_UTILS_TYPES_H

#include "frost_utils_config.h"

#include <cmath>
#include <cfloat>
#include <limits>

namespace Frost
{
    template<class T>               class s_str_t;
    template<class T>               class s_bool_t;
    template<class T>               class s_int_t;
    template<class T>               class s_uint_t;
    template<class T>               class s_float_t;
    template<class T, class C>      class s_ctnr_t;
    template<class T>               class s_ctnr;
    template<class T, uint N>       class s_array;
    template<class T, class N>      class s_pair;
    template<class T>               class s_range;
    template<class Key, class Data> class s_map;
    template<class Key, class Data> class s_multimap;
                                    class s_nullptr;
    template<class T>               class s_ptr;
    template<class T>               class s_refptr;
    template<class T>               class s_wptr;

    /// Stores various informations about a type.
    template<class T>
    class TypeTraits
    {
    public :
        typedef T         Type;
        typedef T         BaseType;
        typedef T         FrostType;
        typedef T&        RefType;
        typedef T const & CRefType;
        typedef T*        PointerType;

        static inline CRefType GetValue(CRefType m) { return m; }
    };

    /// Allows explicit string conversion and operator overloading.
    /** This class is used for explicit constructors and +, += and <<
    *   operators. It is specialized for most commonly used string
    *   types : s_str and s_ustr.<br>
    */
    template<class T, class N> class StringConverter;

    typedef char string_element;
    typedef std::string string_object;
    typedef std::stringstream string_stream;
    typedef std::fstream file_stream;

    typedef int  default_int;
    typedef uint default_uint;
}

#include "frost_utils_bool.h"
#include "frost_utils_int.h"
#include "frost_utils_uint.h"
#include "frost_utils_float.h"
#include "frost_utils_nullptr.h"
#include "frost_utils_ptr.h"
#include "frost_utils_refptr.h"
#include "frost_utils_wptr.h"
#include "frost_utils_range.h"
#include "frost_utils_pair.h"
#include "frost_utils_ctnr.h"
#include "frost_utils_array.h"
#include "frost_utils_map.h"
#include "frost_utils_multimap.h"
#include "frost_utils_str.h"
#include "frost_utils_str_conversion.h"
#include "frost_utils_makepair.h"

namespace Frost
{
    typedef s_int_t<char>      s_char;
    typedef s_int_t<short>     s_short;
    typedef s_int_t<int>       s_int;
    typedef s_int_t<long>      s_long;
    typedef s_int_t<long long> s_llong;
    typedef s_int_t<ptrdiff_t> s_ptrdiff;

    typedef s_uint_t<uchar>              s_uchar;
    typedef s_uint_t<ushort>             s_ushort;
    typedef s_uint_t<uint>               s_uint;
    typedef s_uint_t<ulong>              s_ulong;
    typedef s_uint_t<unsigned long long> s_ullong;

    typedef s_float_t<float>       s_float;
    typedef s_float_t<double>      s_double;
    typedef s_float_t<long double> s_ldouble;

    template<> const float s_float::EPS  = std::numeric_limits<float>::epsilon();
    template<> const s_float s_float::PI = 3.141592f;
    template<> const default_uint s_float::DIGIT = std::numeric_limits<float>::digits10;

    template<> const double s_double::EPS  = std::numeric_limits<double>::epsilon();
    template<> const s_double s_double::PI = 3.141592653589793;
    template<> const default_uint s_double::DIGIT  = std::numeric_limits<double>::digits10;

    template<> const long double s_ldouble::EPS = std::numeric_limits<long double>::epsilon();
    template<> const s_ldouble s_ldouble::PI    = 3.141592653589793238;
    template<> const default_uint s_ldouble::DIGIT      = std::numeric_limits<long double>::digits10;

    typedef s_str_t<string_element> s_str;
    typedef s_str_t<uint>           s_ustr;
}

#endif
