/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            DirectPath source           */
/*                                        */
/*                                        */

#include "path/frost_path_directpath.h"

using namespace std;

namespace Frost
{
    const s_str DirectPath::CLASS_NAME = "DirectPath";

    DirectPath::DirectPath( const s_uint& uiID ) :
        Path(uiID, PATH_DIRECT)
    {

    }

    DirectPath::DirectPath( const s_uint& uiID, const DirectPath& mPath ) :
        Path(uiID, PATH_DIRECT)
    {
        fSpeed_ = mPath.fSpeed_;
        bLoop_ = mPath.bLoop_;
        bLinkEndToStart_ = mPath.bLinkEndToStart_;
        bEnded_ = mPath.bEnded_;
        bReversed_ = mPath.bReversed_;
        bPaused_ = mPath.bPaused_;
        mCurrentPoint_ = mPath.mCurrentPoint_;

        bStarted_ = mPath.bStarted_;
        lPointPath_ = mPath.lPointPath_;
        mPreviousPoint_ = GetPointIterator(s_int(mPath.mPreviousPoint_->uiID));
        mNextPoint_ = GetPointIterator(s_int(mPath.mPreviousPoint_->uiID));
    }

    DirectPath::~DirectPath()
    {

    }

    void DirectPath::AddPoint( const DirectPathPoint& mPoint )
    {
        if (!(bLoop_ && bStarted_))
        {
            lPointPath_.PushBack(mPoint);
            lPointPath_.Back().uiID = lPointPath_.GetSize()-1;

            if (lPointPath_.GetSize() == 1)
            {
                mPreviousPoint_ = lPointPath_.begin();
                mCurrentPoint_ = mPreviousPoint_->mPosition;
            }
            else if (lPointPath_.GetSize() == 2)
            {
                mNextPoint_ = mPreviousPoint_;
                ++mNextPoint_;
            }

            if (bEnded_ && !bReversed_)
            {
                mNextPoint_ = mPreviousPoint_;
                ++mNextPoint_;
                bEnded_ = false;
            }
        }
        else
        {
            Warning(CLASS_NAME,
                "Can't add a new point to a looping DirectPath if it\n"
                "has already started to update."
            );
        }
    }

    void DirectPath::CheckPath_()
    {
        if (lPointPath_.IsEmpty())
        {
            Warning(CLASS_NAME, "Can't use an empty path. Please add points using AddPoint().");
        }
    }

    void DirectPath::Play()
    {
        if (bPaused_)
            bPaused_ = false;
        else
        {
            bEnded_ = false;
            mPreviousPoint_ = lPointPath_.begin();
            mNextPoint_ = mPreviousPoint_;
            ++mNextPoint_;

            mCurrentPoint_ = mPreviousPoint_->mPosition;
        }
    }

    void DirectPath::Update( const s_float& fDelta )
    {
        if (!bStarted_)
        {
            CheckPath_();
            bStarted_ = true;
        }

        if ( (!lPointPath_.IsEmpty()) && (lPointPath_.GetSize() != 1) && !bEnded_ && !bPaused_ )
        {
            Vector mDir;
            s_float fTranslation;
            s_float fDistance;

            if (!bReversed_)
            {
                mDir = mNextPoint_->mPosition - mCurrentPoint_;
                fTranslation = fSpeed_*fDelta;
                fDistance = mDir.GetNorm();

                // Check if we have reached the next point, and maybe got even further.
                while (fDistance < fTranslation)
                {
                    // We have reached the next point.
                    ++mPreviousPoint_;
                    ++mNextPoint_;

                    if (mNextPoint_ == lPointPath_.end())
                    {
                        // We have reached the end of the path...
                        if (bLoop_)
                        {
                            // Looping is enabled...
                            if (bLinkEndToStart_)
                            {
                                // Interpolate between end point and start point
                                mNextPoint_ = lPointPath_.begin();
                            }
                            else
                            {
                                // Restart the path at its beginning
                                mPreviousPoint_ = lPointPath_.begin();
                                mNextPoint_ = mPreviousPoint_;
                                ++mNextPoint_;
                            }
                        }
                        else
                        {
                            // Looping is disabled : the path has done
                            // its job, there's no need to continue.
                            bEnded_ = true;
                            return;
                        }
                    }
                    else if (mPreviousPoint_ == lPointPath_.end())
                    {
                        mPreviousPoint_ = lPointPath_.begin();
                    }

                    mDir = mNextPoint_->mPosition - mPreviousPoint_->mPosition;

                    fTranslation -= fDistance;
                    fDistance = mDir.GetNorm();

                    mCurrentPoint_ = mPreviousPoint_->mPosition;
                }
            }
            else
            {
                // We're travelling the path in reverse order
                mDir = mPreviousPoint_->mPosition - mCurrentPoint_;
                fTranslation = fSpeed_*fDelta;
                fDistance = mDir.GetNorm();

                // Check if we have reached the next point, and maybe got even further.
                while (fDistance < fTranslation)
                {
                    if (mPreviousPoint_ == lPointPath_.begin())
                    {
                        // If we have reached the beginning of the path...
                        if (bLoop_)
                        {
                            // If looping is enabled...
                            if (bLinkEndToStart_)
                            {
                                // Interpolate between end point and start point
                                mPreviousPoint_ = lPointPath_.end();
                                --mPreviousPoint_;
                                mNextPoint_ = lPointPath_.begin();
                            }
                            else
                            {
                                // Restart the path at its beginning
                                mNextPoint_ = lPointPath_.end();
                                --mNextPoint_;
                                mPreviousPoint_ = mNextPoint_;
                                --mPreviousPoint_;
                            }
                        }
                        else
                        {
                            // If looping is disabled, the path has done
                            // its job, there's no need to continue.
                            bEnded_ = true;
                            return;
                        }
                    }
                    else if (mNextPoint_ == lPointPath_.begin())
                    {
                        mNextPoint_ = lPointPath_.end();
                        --mNextPoint_;
                        --mPreviousPoint_;
                    }
                    else
                    {
                        // We have reached the next point.
                        --mPreviousPoint_;
                        --mNextPoint_;
                    }

                    mDir = mPreviousPoint_->mPosition - mNextPoint_->mPosition;

                    fTranslation -= fDistance;
                    fDistance = mDir.GetNorm();

                    mCurrentPoint_ = mNextPoint_->mPosition;
                }
            }

            // Move toward the next point
            mCurrentPoint_ += mDir.GetUnit()*fTranslation;
        }
    }

    const DirectPathPoint& DirectPath::GetPoint( const s_int& iIndex ) const
    {
        if ( (iIndex < s_int(lPointPath_.GetSize())) && (iIndex >= 0) )
        {
            s_ctnr<DirectPathPoint>::const_iterator iterPoint = lPointPath_.Begin();
            for (s_int i = 0; i < iIndex; ++i)
                ++iterPoint;

            return (*iterPoint);
        }
        else
        {
            s_int iNewIndex = iIndex;
            while (iNewIndex >= s_int(lPointPath_.GetSize()))
                iNewIndex -= s_int(lPointPath_.GetSize());

            while (iNewIndex < 0)
                iNewIndex += s_int(lPointPath_.GetSize());

            s_ctnr<DirectPathPoint>::const_iterator iterPoint = lPointPath_.Begin();
            for (s_int i = 0; i < iNewIndex; ++i)
                ++iterPoint;

            return (*iterPoint);
        }
    }

    s_ctnr<DirectPathPoint>::iterator DirectPath::GetPointIterator( const s_int& iIndex )
    {
        if ( (iIndex < s_int(lPointPath_.GetSize())) && (iIndex >= 0) )
        {
            s_ctnr<DirectPathPoint>::iterator iterPoint = lPointPath_.Begin();
            for (s_int i = 0; i < iIndex; ++i)
                ++iterPoint;

            return iterPoint;
        }
        else
        {
            s_int iNewIndex = iIndex;
            while (iNewIndex >= s_int(lPointPath_.GetSize()))
                iNewIndex -= s_int(lPointPath_.GetSize());

            while (iNewIndex < 0)
                iNewIndex += s_int(lPointPath_.GetSize());

            s_ctnr<DirectPathPoint>::iterator iterPoint = lPointPath_.Begin();
            for (s_int i = 0; i < iNewIndex; ++i)
                ++iterPoint;

            return iterPoint;
        }
    }

    s_uint DirectPath::GetPointNumber() const
    {
        return lPointPath_.GetSize();
    }
}
