/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Types header              */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_TYPES_H
#define FROST_UTILS_TYPES_H

#include "frost_utils_config.h"

#ifdef MSVC
    // Note : Microsoft's Visual C++ doesn't include the round function.
    // Definition of this function is in frost_utils_math.cpp.
    long round( double x );
#else
    // Forward declaration to avoid including <cmath>.
    // Used by some template functions.
    #ifdef FROST_LINUX
    extern "C" double round( double x ) throw();
    #else
    extern "C" double __cdecl round( double x );
    #endif
#endif

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
#ifndef HAS_NULLPTR
                                    class s_nullptr;
#endif
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
    *   \note T : character type to convert to
    *         N : type to convert from
    */
    template<class T, class N> class StringConverter;
}

#include "frost_utils_bool.h"
#include "frost_utils_int.h"
#include "frost_utils_uint.h"
#include "frost_utils_float.h"
#include "frost_utils_str.h"
#include "frost_utils_str_conversion.h"

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

    typedef s_str_t<string_element> s_str;
    typedef s_str_t<uint>           s_ustr;

    long StringToInt(const s_str& s);
    long StringToInt(const s_ustr& s);
    ulong StringToUInt(const s_str& s);
    ulong StringToUInt(const s_ustr& s);
    double StringToFloat(const s_str& s);
    double StringToFloat(const s_ustr& s);
}

#include "frost_utils_nullptr.h"
#include "frost_utils_ptr.h"
#include "frost_utils_refptr.h"
#include "frost_utils_wptr.h"
#include "frost_utils_ctnr.h"
#include "frost_utils_array.h"
#include "frost_utils_map.h"
#include "frost_utils_multimap.h"
#include "frost_utils_makepair.h"

#endif
