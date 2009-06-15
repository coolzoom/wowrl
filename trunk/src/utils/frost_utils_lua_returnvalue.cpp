/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Lua source               */
/*                                        */

#include "frost_utils_lua_state.h"
#include "frost_utils_lua_function.h"

using namespace std;
using namespace Frost;
using namespace Frost::Lua;

ReturnValue::ReturnValue()
{
    mType = RETURN_NIL;
}

ReturnValue::ReturnValue( ReturnValueType type )
{
    mType = type;
}

ReturnValue::ReturnValue( const s_var& val )
{
    vValue = val;
}
