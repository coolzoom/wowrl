/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_MATH_H
#define FROST_UTILS_MATH_H

#include "frost_utils_types.h"

namespace Frost
{
    // s_int
    s_float cos(const s_int& iValue);
    s_float acos(const s_int& iValue);
    s_float sin(const s_int& iValue);
    s_float asin(const s_int& iValue);
    s_float tan(const s_int& iValue);
    s_float atan(const s_int& iValue);
    s_float atan2(const s_int& iY, const s_int& iX);
    s_int   fabs(const s_int& iValue);
    s_float pow(const s_int& iValue, const s_float& fValue);
    s_double pow(const s_int& iValue, const s_double& dValue);
    s_float sqrt(const s_int& iValue);
    s_float exp(const s_int& iValue);

    // s_uint
    s_float cos(const s_uint& uiValue);
    s_float acos(const s_uint& uiValue);
    s_float sin(const s_uint& uiValue);
    s_float asin(const s_uint& uiValue);
    s_float tan(const s_uint& uiValue);
    s_float atan(const s_uint& uiValue);
    s_float atan2(const s_uint& uiY, const s_uint& uiX);
    s_float pow(const s_uint& uiValue, const s_float& fValue);
    s_double pow(const s_uint& uiValue, const s_double& dValue);
    s_float sqrt(const s_uint& uiValue);
    s_float exp(const s_uint& uiValue);

    // s_float
    s_float cos(const s_float& fValue);
    s_float acos(const s_float& fValue);
    s_float sin(const s_float& fValue);
    s_float asin(const s_float& fValue);
    s_float tan(const s_float& fValue);
    s_float atan(const s_float& fValue);
    s_float atan2(const s_float& fY, const s_float& fX);
    s_float fabs(const s_float& fValue);
    s_float pow(const s_float& fValue1, const s_float& fValue2);
    s_double pow(const s_float& fValue, const s_double& dValue);
    s_float sqrt(const s_float& fValue);
    s_float exp(const s_float& fValue);

    // s_double
    s_double cos(const s_double& dValue);
    s_double acos(const s_double& dValue);
    s_double sin(const s_double& dValue);
    s_double asin(const s_double& dValue);
    s_double tan(const s_double& dValue);
    s_double atan(const s_double& dValue);
    s_double atan2(const s_double& dY, const s_double& dX);
    s_double fabs(const s_double& dValue);
    s_double pow(const s_double& dValue1, const s_double& dValue2);
    s_double pow(const s_double& dValue, const s_float& fValue);
    s_double sqrt(const s_double& dValue);
    s_double exp(const s_double& dValue);
}

#endif
