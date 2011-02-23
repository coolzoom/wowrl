/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            DirectPath source           */
/*                                        */
/*                                        */

#include "path/frost_directpath.h"

using namespace std;

namespace Frost
{
    const s_str DirectPath::CLASS_NAME = "DirectPath";
    const s_str DirectPath::Iterator::CLASS_NAME = "DirectPath::Iterator";

    DirectPath::DirectPath()
    {
    }

    DirectPath::DirectPath( const DirectPath& mPath )
    {
        lPointPath_ = mPath.lPointPath_;
    }

    DirectPath::~DirectPath()
    {
    }

    void DirectPath::AddPoint( const Point& mPoint )
    {
        lPointPath_.PushBack(mPoint);
        lPointPath_.Back().uiID = lPointPath_.GetSize()-1;
    }

    const DirectPath::Point& DirectPath::GetPoint( const s_int& iIndex ) const
    {
        if ( (iIndex < s_int(lPointPath_.GetSize())) && (iIndex >= 0) )
        {
            return lPointPath_[s_uint(iIndex)];
        }
        else
        {
            s_int iNewIndex = iIndex;
            while (iNewIndex >= s_int(lPointPath_.GetSize()))
                iNewIndex -= s_int(lPointPath_.GetSize());

            while (iNewIndex < 0)
                iNewIndex += s_int(lPointPath_.GetSize());

            return lPointPath_[s_uint(iNewIndex)];
        }
    }

    s_ctnr<DirectPath::Point>::const_iterator DirectPath::GetPointIterator( const s_int& iIndex ) const
    {
        if ( (iIndex < s_int(lPointPath_.GetSize())) && (iIndex >= 0) )
        {
            return lPointPath_.Begin() + iIndex.Get();
        }
        else
        {
            s_int iNewIndex = iIndex;
            while (iNewIndex >= s_int(lPointPath_.GetSize()))
                iNewIndex -= s_int(lPointPath_.GetSize());

            while (iNewIndex < 0)
                iNewIndex += s_int(lPointPath_.GetSize());

            return lPointPath_.Begin() + iNewIndex.Get();
        }
    }

    s_uint DirectPath::GetPointNumber() const
    {
        return lPointPath_.GetSize();
    }

    s_refptr<Path::Iterator> DirectPath::CreateIterator() const
    {
        return s_refptr<Path::Iterator>(new DirectPath::Iterator());
    }

    DirectPath::Iterator::Iterator() : bFirstPlay_(true)
    {
    }

    DirectPath::Iterator::~Iterator()
    {
    }

    void DirectPath::Iterator::Play()
    {
        Path::Iterator::Play();

        if (bFirstPlay_)
        {
            if (pPath_->GetPointNumber() > 1)
            {
                if (!bReversed_)
                {
                    mPreviousPoint_ = pPath_->lPointPath_.Begin();
                    mNextPoint_ = mPreviousPoint_ + 1;
                }
                else
                {
                    mPreviousPoint_ = pPath_->lPointPath_.End() - 1;
                    mNextPoint_ = mPreviousPoint_ - 1;
                }

                mPosition_ = mPreviousPoint_->mPosition;
            }
            else if (pPath_->GetPointNumber() == 1)
            {
                mPreviousPoint_ = mNextPoint_ = pPath_->lPointPath_.Begin();
                mPosition_ = mPreviousPoint_->mPosition;
            }
            else
            {
                bPaused_ = true;
                throw Exception(CLASS_NAME, "Can't play a DirectPath with no points.");
            }

            bFirstPlay_ = false;
        }
    }

    void DirectPath::Iterator::Reverse()
    {
        Path::Iterator::Reverse();
        std::swap(mPreviousPoint_, mNextPoint_);
    }

    void DirectPath::Iterator::Increment_( s_ctnr<Point>::const_iterator& iterPrev, s_ctnr<Point>::const_iterator& iterNext )
    {
        if (!bReversed_)
        {
            ++iterNext;

            if (iterNext == pPath_->lPointPath_.End())
            {
                if (bLoop_)
                {
                    if (bLinkEndToStart_)
                    {
                        iterPrev = pPath_->lPointPath_.End() - 1;
                        iterNext = pPath_->lPointPath_.Begin();
                    }
                    else
                    {
                        iterPrev = pPath_->lPointPath_.Begin();
                        iterNext = iterPrev + 1;
                    }
                }
                else
                {
                    --iterNext;
                    iterPrev = iterNext;
                }
            }
            else
            {
                iterPrev = iterNext - 1;
            }
        }
        else
        {
            if (iterNext == pPath_->lPointPath_.Begin())
            {
                if (bLoop_)
                {
                    if (bLinkEndToStart_)
                    {
                        iterPrev = pPath_->lPointPath_.Begin();
                        iterNext = pPath_->lPointPath_.End() - 1;
                    }
                    else
                    {
                        iterPrev = pPath_->lPointPath_.End() - 1;
                        iterNext = iterPrev - 1;
                    }
                }
                else
                    --iterPrev;
            }
            else
            {
                iterPrev = iterNext;
                --iterNext;
            }
        }
    }

    void DirectPath::Iterator::Update( const s_float& fDelta )
    {
        if ( (pPath_->GetPointNumber() > 1) && !bEnded_ && !bPaused_ )
        {
            Vector  mDir = mNextPoint_->mPosition - mPosition_;
            s_float fTranslation = fSpeed_*fDelta;
            s_float fDistance = mDir.GetNorm();

            // Check if we have reached a new point
            while (fTranslation > fDistance)
            {
                Increment_(mPreviousPoint_, mNextPoint_);

                if (mNextPoint_ != mPreviousPoint_)
                {
                    mDir = mNextPoint_->mPosition - mPreviousPoint_->mPosition;

                    fTranslation -= fDistance;
                    fDistance = mDir.GetNorm();

                    mPosition_ = mPreviousPoint_->mPosition;
                }
                else
                {
                    // The path has ended, stop movement
                    bEnded_ = true;
                    fTranslation = fDistance;
                }
            }

            // Move
            mPosition_ += mDir.GetUnit()*fTranslation;
        }
    }

    void DirectPath::Iterator::SetPath( s_refptr<Path> pPath )
    {
        pPath_ = s_refptr<DirectPath>::DynamicCast(pPath);
    }

    s_wptr<Path> DirectPath::Iterator::GetPath() const
    {
        return pPath_;
    }
}
