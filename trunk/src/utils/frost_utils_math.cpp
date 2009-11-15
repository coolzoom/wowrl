/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_math.h"

#include <cmath>

#ifdef MSVC
    // Note : Microsoft's Visual C++ doesn't include the round function.
    long round( double x )
    {
        int i = static_cast<long>(x);
        if (x >= 0.0)
            return (x-i >= 0.5) ? i + 1 : i;
        else
            return (i-x >= 0.5) ? i - 1 : i;
    }
#endif

namespace Frost
{
// s_int

    s_float cos(const s_int& iValue)
    {
        return ::cos(iValue.Get()*2*s_float::PI.Get());
    }

    s_float acos(const s_int& iValue)
    {
        return ::acos(static_cast<float>(iValue.Get()))/(2*s_float::PI);
    }

    s_float sin(const s_int& iValue)
    {
        return ::sin(iValue.Get()*2*s_float::PI.Get());
    }

    s_float asin(const s_int& iValue)
    {
        return ::asin(static_cast<float>(iValue.Get()))/(2*s_float::PI);
    }

    s_float tan(const s_int& iValue)
    {
        return ::tan(iValue.Get()*2*s_float::PI.Get());
    }

    s_float atan(const s_int& iValue)
    {
        return ::atan(static_cast<float>(iValue.Get()))/(2*s_float::PI);
    }

    s_float atan2(const s_int& iX, const s_int& iY)
    {
        return ::atan2(static_cast<float>(iX.Get()), static_cast<float>(iY.Get()))/(2*s_float::PI.Get());
    }

    s_int fabs(const s_int& iValue)
    {
        return static_cast<int>(::fabs(static_cast<float>(iValue.Get())));
    }

    s_float pow(const s_int& iValue, const s_float& fValue)
    {
        return ::pow(static_cast<float>(iValue.Get()), static_cast<float>(fValue.Get()));
    }

    s_double pow(const s_int& iValue, const s_double& dValue)
    {
        return ::pow(static_cast<double>(iValue.Get()), static_cast<double>(dValue.Get()));
    }

    s_float sqrt(const s_int& iValue)
    {
        return ::sqrt(static_cast<float>(iValue.Get()));
    }

    s_float exp(const s_int& iValue)
    {
        return ::exp(static_cast<float>(iValue.Get()));
    }

// s_uint

    s_float cos(const s_uint& uiValue)
    {
        return ::cos(uiValue.Get()*2*s_float::PI.Get());
    }

    s_float acos(const s_uint& uiValue)
    {
        return ::acos(static_cast<float>(uiValue.Get()))/(2*s_float::PI);
    }

    s_float sin(const s_uint& uiValue)
    {
        return ::sin(uiValue.Get()*2*s_float::PI.Get());
    }

    s_float asin(const s_uint& uiValue)
    {
        return ::asin(static_cast<float>(uiValue.Get()))/(2*s_float::PI);
    }

    s_float tan(const s_uint& uiValue)
    {
        return ::tan(uiValue.Get()*2*s_float::PI.Get());
    }

    s_float atan(const s_uint& uiValue)
    {
        return ::atan(static_cast<float>(uiValue.Get()))/(2*s_float::PI);
    }

    s_float atan2(const s_uint& uiX, const s_uint& uiY)
    {
        return ::atan2(static_cast<float>(uiX.Get()), static_cast<float>(uiY.Get()))/(2*s_float::PI);
    }

    s_float pow(const s_uint& uiValue, const s_float& fValue)
    {
        return ::pow(static_cast<float>(uiValue.Get()), static_cast<float>(fValue.Get()));
    }

    s_double pow(const s_uint& uiValue, const s_double& dValue)
    {
        return ::pow(static_cast<double>(uiValue.Get()), static_cast<double>(dValue.Get()));
    }

    s_float sqrt(const s_uint& uiValue)
    {
        return ::sqrt(static_cast<float>(uiValue.Get()));
    }

    s_float exp(const s_uint& uiValue)
    {
        return ::exp(static_cast<float>(uiValue.Get()));
    }

// s_float

    s_float cos(const s_float& fValue)
    {
        return ::cos((fValue*2*s_float::PI).Get());
    }

    s_float acos(const s_float& fValue)
    {
        return ::acos(fValue.Get())/(2*s_float::PI);
    }

    s_float sin(const s_float& fValue)
    {
        return ::sin((fValue*2*s_float::PI).Get());
    }

    s_float asin(const s_float& fValue)
    {
        return ::asin(fValue.Get())/(2*s_float::PI);
    }

    s_float tan(const s_float& fValue)
    {
        return ::tan((fValue*2*s_float::PI).Get());
    }

    s_float atan(const s_float& fValue)
    {
        return ::atan(fValue.Get())/(2*s_float::PI);
    }

    s_float atan2(const s_float& fX, const s_float& fY)
    {
        return ::atan2(fX.Get(), fY.Get())/(2*s_float::PI);
    }

    s_float fabs(const s_float& fValue)
    {
        return ::fabs(fValue.Get());
    }

    s_float pow(const s_float& fValue1, const s_float& fValue2)
    {
        return ::pow(static_cast<float>(fValue1.Get()), static_cast<float>(fValue2.Get()));
    }

    s_double pow(const s_float& fValue, const s_double& dValue)
    {
        return ::pow(static_cast<double>(fValue.Get()), static_cast<double>(dValue.Get()));
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
        return ::cos((dValue*2*s_double::PI).Get());
    }

    s_double acos(const s_double& dValue)
    {
        return ::acos(dValue.Get())/(2*s_double::PI);
    }

    s_double sin(const s_double& dValue)
    {
        return ::sin((dValue*2*s_double::PI).Get());
    }

    s_double asin(const s_double& dValue)
    {
        return ::asin(dValue.Get())/(2*s_double::PI);
    }

    s_double tan(const s_double& dValue)
    {
        return ::tan((dValue*2*s_double::PI).Get());
    }

    s_double atan(const s_double& dValue)
    {
        return ::atan(dValue.Get())/(2*s_double::PI);
    }

    s_double atan2(const s_double& dX, const s_double& dY)
    {
        return ::atan2(dX.Get(), dY.Get())/(2*s_double::PI);
    }

    s_double fabs(const s_double& dValue)
    {
        return ::fabs(dValue.Get());
    }

    s_double pow(const s_double& dValue1, const s_double& dValue2)
    {
        return ::pow(static_cast<double>(dValue1.Get()), static_cast<double>(dValue2.Get()));
    }

    s_double pow(const s_double& dValue, const s_float& fValue)
    {
        return ::pow(static_cast<double>(dValue.Get()), static_cast<double>(fValue.Get()));
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
