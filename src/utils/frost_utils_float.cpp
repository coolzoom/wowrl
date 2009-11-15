/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_types.h"

namespace Frost
{
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
