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
    if (val.GetType() == VALUE_BOOL)
        mType = RETURN_BOOLEAN;
    else if (val.GetType() == VALUE_STRING)
        mType = RETURN_STRING;
    else if ((val.GetType() == VALUE_INT) ||
             (val.GetType() == VALUE_UINT) ||
             (val.GetType() == VALUE_FLOAT) ||
             (val.GetType() == VALUE_DOUBLE))
        mType = RETURN_NUMBER;

    vValue = val;
}

ReturnValue::ReturnValue( const s_float& fVal )
{
    mType = RETURN_NUMBER;
    vValue.SetF(fVal);
}

ReturnValue::ReturnValue( const s_double& dVal )
{
    mType = RETURN_NUMBER;
    vValue.SetF(s_float(dVal));
}

ReturnValue::ReturnValue( const s_int& iVal )
{
    mType = RETURN_NUMBER;
    vValue.SetF(s_float(iVal));
}

ReturnValue::ReturnValue( const s_uint& uiVal )
{
    mType = RETURN_NUMBER;
    vValue.SetF(s_float(uiVal));
}

ReturnValue::ReturnValue( const s_bool& bVal )
{
    mType = RETURN_BOOLEAN;
    vValue.SetB(bVal);
}

ReturnValue::ReturnValue( const s_str& sVal )
{
    mType = RETURN_STRING;
    vValue.SetS(sVal);
}

