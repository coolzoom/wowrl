/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Path source               */
/*                                        */
/*                                        */

#include "path/frost_path.h"

using namespace std;

namespace Frost
{
    const s_str Path::CLASS_NAME           = "Path";
    const s_str Path::Iterator::CLASS_NAME = "Path::Iterator";

    Path::Iterator::Iterator() : bPaused_(true)
    {
    }

    Path::Iterator::~Iterator()
    {

    }

    void Path::Iterator::SetLoop( const s_bool& bLoop, const s_bool& bLinkEndToStart )
    {
        bLoop_ = bLoop;
        bLinkEndToStart_ = bLinkEndToStart;
    }

    void Path::Iterator::SetSpeed( const s_float& fSpeed )
    {
        fSpeed_ = fabs(fSpeed);

        if (((fSpeed < 0.0f) && !bReversed_) ||
            ((fSpeed > 0.0f) && bReversed_ ))
        {
            Reverse();
        }
    }

    void Path::Iterator::Reverse()
    {
        bReversed_ = !bReversed_;
    }

    void Path::Iterator::Pause()
    {
        bPaused_ = true;
    }

    void Path::Iterator::Play()
    {
        bPaused_ = false;
    }

    const Vector& Path::Iterator::GetPosition() const
    {
        return mPosition_;
    }

    const s_bool& Path::Iterator::IsEnded() const
    {
        return bEnded_;
    }

    const s_bool& Path::Iterator::IsPaused() const
    {
        return bPaused_;
    }

    s_refptr<Path::Iterator> Path::CreateIterator( s_wptr<Path> pPath )
    {
        if (s_refptr<Path> pLocked = pPath.Lock())
        {
            s_refptr<Path::Iterator> pIterator = pLocked->CreateIterator();
            pIterator->SetPath(pLocked);
            return pIterator;
        }
        else
            return s_refptr<Path::Iterator>();
    }
}
