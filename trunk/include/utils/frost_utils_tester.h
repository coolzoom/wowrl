/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_TESTER_H
#define FROST_UTILS_TESTER_H

#include "frost_utils.h"
#include "frost_utils_bool.h"

namespace Frost
{
    /// A modified bool class
    /** It starts at 'true' and once set to false,
    *   it never comes back to true anymore.
    *   Usefull for testing several actions and
    *   stating if at least one of them failed.
    */
    class Tester
    {
    public :

        Tester();
        void     Reset();

        Tester&  operator = (const bool& bBool);
        Tester&  operator = (const s_bool& bBool);
        bool     operator ! () const;
        typedef  void (Tester::*MemberFn)();
        operator MemberFn() const;

        operator bool() const;
        operator s_bool() const;

        static const s_str CLASS_NAME;

    private :

        s_bool bGood_;
    };
}

#endif
