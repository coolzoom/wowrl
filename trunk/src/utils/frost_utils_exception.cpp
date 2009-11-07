/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Exception source            */
/*                                        */
/*                                        */

#include "frost_utils_exception.h"

using namespace std;

namespace Frost
{
    Exception::Exception() : sMessage_("Undefined exception.")
    {
    }

    Exception::Exception( const s_str& sMessage ) : sMessage_(sMessage)
    {
    }

    Exception::Exception( const s_str& sClassName, const s_str& sMessage ) :
        sMessage_(sClassName+" : "+sMessage)
    {
    }

    Exception::~Exception()
    {
    }

    const s_str& Exception::GetDescription() const
    {
        return sMessage_;
    }
}
