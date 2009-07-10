/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_types.h"

#include <limits>

namespace Frost
{
    template<> const float s_float::EPS  = std::numeric_limits<float>::epsilon();
    template<> const s_float s_float::PI = 3.141592f;
    template<> const uint s_float::DIGIT = std::numeric_limits<float>::digits10;

    template<> const double s_double::EPS  = std::numeric_limits<double>::epsilon();
    template<> const s_double s_double::PI = 3.141592653589793;
    template<> const uint s_double::DIGIT  = std::numeric_limits<double>::digits10;

    template<> const long double s_ldouble::EPS = std::numeric_limits<long double>::epsilon();
    template<> const s_ldouble s_ldouble::PI    = 3.141592653589793238;
    template<> const uint s_ldouble::DIGIT      = std::numeric_limits<long double>::digits10;

    float MakeFloat(const ulong& ul)
    {
        union {
            float f;
            ulong ul;
        } x;

        x.ul = ul;
        return x.f;
    }
}
