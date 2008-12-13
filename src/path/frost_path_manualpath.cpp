/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            ManualPath source           */
/*                                        */
/*                                        */

#include "frost_path_manualpath.h"

using namespace std;

namespace Frost
{
    const s_str ManualPath::CLASS_NAME = "ManualPath";

    ManualPath::ManualPath( const s_uint& uiID ) :
        Path(uiID, PATH_MANUAL)
    {

    }

    ManualPath::ManualPath( const s_uint& uiID, const ManualPath& mPath ) :
        Path(uiID, PATH_MANUAL)
    {
        fSpeed_ = mPath.fSpeed_;
        bLoop_ = mPath.bLoop_;
        bLinkEndToStart_ = mPath.bLinkEndToStart_;
        bEnded_ = mPath.bEnded_;
        bReversed_ = mPath.bReversed_;
        bPaused_ = mPath.bPaused_;
        mActualPoint_ = mPath.mActualPoint_;


    }

    ManualPath::~ManualPath()
    {

    }

    void ManualPath::Update( const s_float& fDelta )
    {

    }
}
