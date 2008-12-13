/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_math.h"

#include "frost_utils_int.h"
#include "frost_utils_uint.h"
#include "frost_utils_float.h"
#include "frost_utils_double.h"

#include <math.h>

namespace Frost
{
// s_int

    s_float cos(const s_int& iValue)
    {
        return ::cos(iValue.Get());
    }

    s_float acos(const s_int& iValue)
    {
        return ::acos(iValue.Get());
    }

    s_float sin(const s_int& iValue)
    {
        return ::sin(iValue.Get());
    }

    s_float asin(const s_int& iValue)
    {
        return ::asin(iValue.Get());
    }

    s_float tan(const s_int& iValue)
    {
        return ::tan(iValue.Get());
    }

    s_float atan(const s_int& iValue)
    {
        return ::atan(iValue.Get());
    }

    s_int fabs(const s_int& iValue)
    {
        return static_cast<int>(::fabs(iValue.Get()));
    }

    s_float pow(const s_int& iValue, const s_float& fValue)
    {
        return ::pow(iValue.Get(), fValue.Get());
    }

    s_float pow(const s_int& iValue, const s_double& dValue)
    {
        return ::pow(iValue.Get(), dValue.Get());
    }

    s_float sqrt(const s_int& iValue)
    {
        return ::sqrt(iValue.Get());
    }

    s_float exp(const s_int& iValue)
    {
        return ::exp(iValue.Get());
    }

// s_uint

    s_float cos(const s_uint& uiValue)
    {
        return ::cos(uiValue.Get());
    }

    s_float acos(const s_uint& uiValue)
    {
        return ::acos(uiValue.Get());
    }

    s_float sin(const s_uint& uiValue)
    {
        return ::sin(uiValue.Get());
    }

    s_float asin(const s_uint& uiValue)
    {
        return ::asin(uiValue.Get());
    }

    s_float tan(const s_uint& uiValue)
    {
        return ::tan(uiValue.Get());
    }

    s_float atan(const s_uint& uiValue)
    {
        return ::atan(uiValue.Get());
    }

    s_float pow(const s_uint& uiValue, const s_float& fValue)
    {
        return ::pow(uiValue.Get(), fValue.Get());
    }

    s_float pow(const s_uint& uiValue, const s_double& dValue)
    {
        return ::pow(uiValue.Get(), dValue.Get());
    }

    s_float sqrt(const s_uint& uiValue)
    {
        return ::sqrt(uiValue.Get());
    }

    s_float exp(const s_uint& uiValue)
    {
        return ::exp(uiValue.Get());
    }

// s_float

    s_float cos(const s_float& fValue)
    {
        return ::cos(fValue.Get());
    }

    s_float acos(const s_float& fValue)
    {
        return ::acos(fValue.Get());
    }

    s_float sin(const s_float& fValue)
    {
        return ::sin(fValue.Get());
    }

    s_float asin(const s_float& fValue)
    {
        return ::asin(fValue.Get());
    }

    s_float tan(const s_float& fValue)
    {
        return ::tan(fValue.Get());
    }

    s_float atan(const s_float& fValue)
    {
        return ::atan(fValue.Get());
    }

    s_float fabs(const s_float& fValue)
    {
        return ::fabs(fValue.Get());
    }

    s_float pow(const s_float& fValue1, const s_float& fValue2)
    {
        return ::pow(fValue1.Get(), fValue2.Get());
    }

    s_float pow(const s_float& fValue, const s_double& dValue)
    {
        return ::pow(fValue.Get(), dValue.Get());
    }

    s_float sqrt(const s_float& fValue)
    {
        return ::sqrt(fValue.Get());
    }

    s_float exp(const s_float& fValue)
    {
        return ::exp(fValue.Get());
    }

// s_double

    s_double cos(const s_double& dValue)
    {
        return ::cos(dValue.Get());
    }

    s_double acos(const s_double& dValue)
    {
        return ::acos(dValue.Get());
    }

    s_double sin(const s_double& dValue)
    {
        return ::sin(dValue.Get());
    }

    s_double asin(const s_double& dValue)
    {
        return ::asin(dValue.Get());
    }

    s_double tan(const s_double& dValue)
    {
        return ::tan(dValue.Get());
    }

    s_double atan(const s_double& dValue)
    {
        return ::atan(dValue.Get());
    }

    s_double fabs(const s_double& dValue)
    {
        return ::fabs(dValue.Get());
    }

    s_double pow(const s_double& dValue1, const s_double& dValue2)
    {
        return ::pow(dValue1.Get(), dValue2.Get());
    }

    s_double pow(const s_double& dValue, const s_float& fValue)
    {
        return ::pow(fValue.Get(), dValue.Get());
    }

    s_double sqrt(const s_double& dValue)
    {
        return ::sqrt(dValue.Get());
    }

    s_double exp(const s_double& dValue)
    {
        return ::exp(dValue.Get());
    }
}
