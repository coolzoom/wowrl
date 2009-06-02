/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_VAR_H
#define FROST_UTILS_VAR_H

#include "frost_utils.h"
#include "frost_utils_int.h"
#include "frost_utils_uint.h"
#include "frost_utils_float.h"
#include "frost_utils_double.h"
#include "frost_utils_str.h"
#include "frost_utils_bool.h"

namespace Frost
{
    class s_int;
    class s_uint;
    class s_float;
    class s_double;
    class s_bool;
    class s_str;
    template<class T> class s_ctnr;

    enum ValueType
    {
        VALUE_NONE = -1,
        VALUE_INT = 0,
        VALUE_UINT,
        VALUE_FLOAT,
        VALUE_DOUBLE,
        VALUE_BOOL,
        VALUE_STRING,
        VALUE_POINTER
    };

    /// Base type : untyped variable
    /** The purpose of this class is to have an untyped
    *   return value / argument. It allows simple manipulation
    *   of events, and a lot of other things.<br>
    *   It only uses more memory than a simple object :<br>
    *   size = sizeof(int) + sizeof(void*) + sizeof(T)<br>
    *   ... where "T" is the type of the value it contains.
    *   But because it needs to convert the contained value
    *   each time you need to access it, it is slower than
    *   using this value out of this class. Use it wisely.
    */
    class s_var
    {
    public :

        s_var();
        s_var(const s_var& vValue);
        s_var(const s_int& iValue);
        s_var(const s_uint& uiValue);
        s_var(const s_float& fValue);
        s_var(const s_double& dValue);
        s_var(const s_bool& bValue);
        s_var(const s_str& sValue);
        s_var(void* pValue);

        ~s_var();

        void SetI(const s_int& iValue);
        void SetUI(const s_uint& uiValue);
        void SetF(const s_float& fValue);
        void SetD(const s_double& dValue);
        void SetB(const s_bool& bValue);
        void SetS(const s_str& sValue);
        void SetP(void* pValue);

        s_int     GetI() const;
        s_uint    GetUI() const;
        s_float   GetF() const;
        s_double  GetD() const;
        s_bool    GetB() const;
        s_str     GetS() const;
        void*     GetP() const;

        ValueType GetType() const;

        s_var&        operator = (const s_var& vValue);
        s_ctnr<s_var> operator , (const s_var& vValue) const;

        static const s_str CLASS_NAME;

    private :

        void Delete_();

        ValueType mType_;
        void*     pPtr_;
    };

    s_str  operator+ (const char* sLeft, const s_var& vRight);
    s_str  operator+ (const s_str& sLeft, const s_var& vRight);
    s_str& operator<< (s_str& sLeft, const s_var& vRight);
}

#endif
