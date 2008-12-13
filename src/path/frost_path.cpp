/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               XXX source               */
/*                                        */
/*                                        */

#include "frost_path.h"

using namespace std;

namespace Frost
{
    const s_str Path::CLASS_NAME = "Path";

    Path::Path( const s_uint& uiID, PathType mType ) :
        uiID_(uiID), mType_(mType)
    {
        fSpeed_ = 20.0f;
        bPaused_ = true;
    }

    Path::~Path()
    {

    }

    void Path::SetLoop( const s_bool& bLoop, const s_bool& bLinkEndToStart )
    {
        bLoop_ = bLoop;
        if (bLoop_)
            bLinkEndToStart_ = bLinkEndToStart;
    }

    void Path::SetSpeed( const s_float& fSpeed )
    {
        fSpeed_ = fabs(fSpeed);

        if (((fSpeed < 0.0f) && !bReversed_) ||
            ((fSpeed > 0.0f) && bReversed_ ))
        {
            Reverse();
        }
    }

    void Path::Reverse()
    {
        bReversed_ = !bReversed_;
    }

    void Path::Pause()
    {
        bPaused_ = true;
    }

    void Path::Play()
    {
        bPaused_ = false;
    }

    const Vector& Path::GetActualPoint() const
    {
        return mActualPoint_;
    }

    const s_uint& Path::GetID() const
    {
        return uiID_;
    }

    PathType Path::GetType() const
    {
        return mType_;
    }

    const s_bool& Path::IsEnded() const
    {
        return bEnded_;
    }

    const s_bool& Path::IsPaused() const
    {
        return bPaused_;
    }

    void Path::Update(const s_float& fDelta)
    {
    }
}
