/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Event source              */
/*                                        */
/*                                        */

#include "frost_utils_event.h"
#include "frost_utils_var.h"

using namespace std;

namespace Frost
{
    const s_str Event::CLASS_NAME = "Event";

    Event::Event()
    {
    }

    Event::Event( const s_str& sName, const s_bool& bOncePerFrame )
    {
        sName_ = sName;
        bOncePerFrame_ = bOncePerFrame;
    }

    void Event::SetName( const s_str& sName )
    {
        sName_ = sName;
    }

    void Event::SetOncePerFrame( const s_bool& bOncePerFrame )
    {
        bOncePerFrame_ = bOncePerFrame;
    }

    void Event::Add( const s_var& mValue )
    {
        lArgList_.PushBack(mValue);
    }

    s_ptr<const s_var> Event::Get( const s_uint& uiIndex ) const
    {
        return &lArgList_[uiIndex];
    }

    s_uint Event::GetNumParam() const
    {
        return lArgList_.GetSize();
    }

    const s_str& Event::GetName() const
    {
        return sName_;
    }

    const s_bool& Event::IsOncePerFrame() const
    {
        return bOncePerFrame_;
    }

    s_var& Event::operator [] ( const s_uint& uiIndex )
    {
        return lArgList_[uiIndex];
    }

    const s_var& Event::operator [] ( const s_uint& uiIndex ) const
    {
        return lArgList_[uiIndex];
    }
}



