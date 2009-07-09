/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_types.h"

namespace Frost
{
    template<> const float s_float::fEpsilon = FLT_MIN;
    template<> const s_float s_float::PI     = 3.141592f;

    template<> const double s_double::fEpsilon = DBL_MIN;
    template<> const s_double s_double::PI     = 3.141592653589793;

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
