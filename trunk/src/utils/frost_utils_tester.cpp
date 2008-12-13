/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_tester.h"
#include "frost_utils.h"
#include "frost_utils_str.h"

using namespace std;

namespace Frost
{
    const s_str Tester::CLASS_NAME = "Tester";

    Tester::Tester()
    {
        bGood_ = true;
    }

    Tester& Tester::operator=( const bool& b )
    {
        if (bGood_)
            bGood_ = b;

        return *this;
    }

    Tester& Tester::operator=( const s_bool& b )
    {
        if (bGood_)
            bGood_ = b;

        return *this;
    }

    Tester::operator bool() const
    {
        return bGood_.Get();
    }

    bool Tester::operator ! () const
    {
        return bGood_.Get();
    }

    Tester::operator MemberFn() const
    {
        if (bGood_)
            return& Tester::Reset;
        else
            return NULL;
    }

    Tester::operator s_bool() const
    {
        return bGood_;
    }

    void Tester::Reset()
    {
        bGood_ = true;
    }
}
