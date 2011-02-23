/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_var.h"

namespace Frost
{
    const s_type& s_var::VALUE_NONE    = typeid(void);
    const s_type& s_var::VALUE_INT     = typeid(s_int);
    const s_type& s_var::VALUE_UINT    = typeid(s_uint);
    const s_type& s_var::VALUE_FLOAT   = typeid(s_float);
    const s_type& s_var::VALUE_DOUBLE  = typeid(s_double);
    const s_type& s_var::VALUE_BOOL    = typeid(s_bool);
    const s_type& s_var::VALUE_STRING  = typeid(s_str);
    const s_type& s_var::VALUE_POINTER = typeid(void*);
}
