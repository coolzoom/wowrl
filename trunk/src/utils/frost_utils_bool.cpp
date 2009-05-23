/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_bool.h"
#include "frost_utils_str.h"
#include "frost_utils_array.h"

using namespace std;

#define BOOL_TRUE "true"
#define BOOL_ONE  "1"
#define BOOL_YES  "yes"

namespace Frost
{
    bool s_bool::bDummy = false;
    const s_str s_bool::CLASS_NAME = "s_bool";

    s_bool::s_bool()
    {
        bValue_ = false;
    }

    s_bool::s_bool( const bool& bValue )
    {
        bValue_ = bValue;
    }

    s_bool::s_bool( const string_object& sValue )
    {
        if ((s_str(sValue) == BOOL_TRUE) ||
            (s_str(sValue) == BOOL_ONE)  ||
            (s_str(sValue) == BOOL_YES))
            bValue_ = true;
        else
            bValue_ = false;
    }

    s_bool::s_bool( const string_element* sValue )
    {
        if ((s_str(sValue) == BOOL_TRUE) ||
            (s_str(sValue) == BOOL_ONE)  ||
            (s_str(sValue) == BOOL_YES))
            bValue_ = true;
        else
            bValue_ = false;
    }

    s_bool::s_bool( const s_str& sValue )
    {
        if ((sValue == BOOL_TRUE) ||
            (sValue == BOOL_ONE)  ||
            (sValue == BOOL_YES))
            bValue_ = true;
        else
            bValue_ = false;
    }

    const s_str& s_bool::GetAsString(const s_str& sTrue, const s_str& sFalse) const
    {
        return (bValue_ ? sTrue : sFalse);
    }

    s_bool::operator MemberFn() const
    {
        if (bValue_)
            return &s_bool::Get;
        else
            return NULL;
    }

    s_bool s_bool::operator== ( const s_bool& mValue ) const
    {
        return (bValue_ == mValue.bValue_);
    }

    s_bool s_bool::operator!= ( const s_bool& mValue ) const
    {
        return (bValue_ != mValue.bValue_);
    }

    s_bool& s_bool::operator= ( const s_bool& mValue )
    {
        bValue_ = mValue.bValue_;
        return *this;
    }

    bool s_bool::operator! () const
    {
        return (!bValue_);
    }

    s_str operator+ ( const string_element* sLeft, const s_bool& bRight )
    {
        return s_str(sLeft) << bRight;
    }

    s_str s_bool::operator+ ( const string_element* sValue ) const
    {
        return s_str(*this) + sValue;
    }

    #ifdef USE_UNICODE
        s_str operator+ ( const char* sLeft, const s_bool& bRight )
        {
            return s_str(sLeft) << bRight;
        }

        s_str s_bool::operator+ ( const char* sValue ) const
        {
            return s_str(*this) + sValue;
        }
    #endif


    s_str s_bool::operator+ ( const s_str& sValue ) const
    {
        return s_str(*this) + sValue;
    }

    s_ctnr<s_bool> s_bool::operator, ( const s_bool& bValue ) const
    {
        s_ctnr<s_bool> mContainer;
        mContainer.Push(*this);
        mContainer.Push(bValue);
        return mContainer;
    }
}
