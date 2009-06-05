/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            SmoothPath source           */
/*                                        */
/*                                        */

#include "path/frost_path_smoothpath.h"

using namespace std;

namespace Frost
{
    const s_str SmoothPath::CLASS_NAME = "SmoothPath";

    SmoothPath::SmoothPath( const s_uint& uiID ) :
        Path(uiID, PATH_SMOOTH)
    {

    }

    SmoothPath::SmoothPath( const s_uint& uiID, const SmoothPath& mPath ) :
        Path(uiID, PATH_SMOOTH)
    {
        fSpeed_ = mPath.fSpeed_;
        bLoop_ = mPath.bLoop_;
        bLinkEndToStart_ = mPath.bLinkEndToStart_;
        bEnded_ = mPath.bEnded_;
        bReversed_ = mPath.bReversed_;
        bPaused_ = mPath.bPaused_;
        mActualPoint_ = mPath.mActualPoint_;

        bStarted_ = mPath.bStarted_;
        fDistance_ = mPath.fDistance_;
        lPointPath_ = mPath.lPointPath_;
        if (bStarted_)
            CheckPath_();
    }

    SmoothPath::~SmoothPath()
    {

    }

    SmoothPathSegment::SmoothPathSegment( s_ptr<SmoothPath> pPath, const s_int& iFirstID )
    {
        lP_[0] = pPath->GetPointIterator(iFirstID);;
        lP_[1] = pPath->GetPointIterator(iFirstID+1);
        lP_[2] = pPath->GetPointIterator(iFirstID+2);
        lP_[3] = pPath->GetPointIterator(iFirstID+3);

        // Build arc length map
        s_float fOldX = lP_[1]->mPosition.X();
        s_float fOldY = lP_[1]->mPosition.Y();
        s_float fOldZ = lP_[1]->mPosition.Z();

        lLengthTable_[0.0f] = 0.0f;

        for (s_float t = 0.05f; t < 1.04f; t += 0.05f)
        {
            s_float t2 = t*t;
            s_float t3 = t*t*t;

            s_float fX = 0.5f*(
                (2.0f*lP_[1]->mPosition.X()) +
                (-lP_[0]->mPosition.X() + lP_[2]->mPosition.X())*t +
                (2.0f*lP_[0]->mPosition.X() - 5.0f*lP_[1]->mPosition.X() + 4.0f*lP_[2]->mPosition.X() - lP_[3]->mPosition.X())*t2 +
                (-lP_[0]->mPosition.X() + 3.0f*lP_[1]->mPosition.X() - 3.0f*lP_[2]->mPosition.X() + lP_[3]->mPosition.X())*t3
            );
            s_float fY = 0.5f*(
                (2.0f*lP_[1]->mPosition.Y()) +
                (-lP_[0]->mPosition.Y() + lP_[2]->mPosition.Y())*t +
                (2.0f*lP_[0]->mPosition.Y() - 5.0f*lP_[1]->mPosition.Y() + 4.0f*lP_[2]->mPosition.Y() - lP_[3]->mPosition.Y())*t2 +
                (-lP_[0]->mPosition.Y() + 3.0f*lP_[1]->mPosition.Y() - 3.0f*lP_[2]->mPosition.Y() + lP_[3]->mPosition.Y())*t3
            );
            s_float fZ = 0.5f*(
                (2.0f*lP_[1]->mPosition.Z()) +
                (-lP_[0]->mPosition.Z() + lP_[2]->mPosition.Z())*t +
                (2.0f*lP_[0]->mPosition.Z() - 5.0f*lP_[1]->mPosition.Z() + 4.0f*lP_[2]->mPosition.Z() - lP_[3]->mPosition.Z())*t2 +
                (-lP_[0]->mPosition.Z() + 3.0f*lP_[1]->mPosition.Z() - 3.0f*lP_[2]->mPosition.Z() + lP_[3]->mPosition.Z())*t3
            );

            //fDistance_ += sqrt((fX-fOldX)*(fX-fOldX) + (fY-fOldY)*(fY-fOldY) + (fZ-fOldZ)*(fZ-fOldZ));
            fLength_ += Vector(fX-fOldX, fY-fOldY, fZ-fOldZ).GetNorm();
            lLengthTable_[fLength_] = t;

            fOldX = fX;
            fOldY = fY;
            fOldZ = fZ;
        }
    }

    Vector SmoothPathSegment::CalcPoint( const s_float& fDistance )
    {
        s_float t;
        if (fDistance <= 0.0f)
        {
            t = 0.0f;
        }
        else
        {
            map<s_float, s_float>::iterator iterFirst, iterSecond;
            iterSecond = lLengthTable_.lower_bound(fDistance);
            if (iterSecond != lLengthTable_.end())
            {
                iterFirst = iterSecond;
                iterFirst--;

                t = iterFirst->second +
                    ((fDistance-iterFirst->first) /
                    (iterSecond->first-iterFirst->first)) *
                    (iterSecond->second - iterFirst->second);
            }
            else
            {
                t = 1.0f;
            }
        }

        s_float t2 = t*t;
        s_float t3 = t*t*t;
        Vector mVec;

        mVec.X(
            0.5f*(
            (2.0f*lP_[1]->mPosition.X()) +
            (-lP_[0]->mPosition.X() + lP_[2]->mPosition.X())*t +
            (2.0f*lP_[0]->mPosition.X() - 5.0f*lP_[1]->mPosition.X() + 4.0f*lP_[2]->mPosition.X() - lP_[3]->mPosition.X())*t2 +
            (-lP_[0]->mPosition.X() + 3.0f*lP_[1]->mPosition.X() - 3.0f*lP_[2]->mPosition.X() + lP_[3]->mPosition.X())*t3)
        );
        mVec.Y(
            0.5f*(
            (2.0f*lP_[1]->mPosition.Y()) +
            (-lP_[0]->mPosition.Y() + lP_[2]->mPosition.Y())*t +
            (2.0f*lP_[0]->mPosition.Y() - 5.0f*lP_[1]->mPosition.Y() + 4.0f*lP_[2]->mPosition.Y() - lP_[3]->mPosition.Y())*t2 +
            (-lP_[0]->mPosition.Y() + 3.0f*lP_[1]->mPosition.Y() - 3.0f*lP_[2]->mPosition.Y() + lP_[3]->mPosition.Y())*t3)
        );
        mVec.Z(
            0.5f*(
            (2.0f*lP_[1]->mPosition.Z()) +
            (-lP_[0]->mPosition.Z() + lP_[2]->mPosition.Z())*t +
            (2.0f*lP_[0]->mPosition.Z() - 5.0f*lP_[1]->mPosition.Z() + 4.0f*lP_[2]->mPosition.Z() - lP_[3]->mPosition.Z())*t2 +
            (-lP_[0]->mPosition.Z() + 3.0f*lP_[1]->mPosition.Z() - 3.0f*lP_[2]->mPosition.Z() + lP_[3]->mPosition.Z())*t3)
        );

        return mVec;
    }

    const SmoothPathPoint& SmoothPathSegment::GetPoint( const s_uint& uiIndex ) const
    {
        return (*lP_[uiIndex.Get()]);
    }

    const s_float& SmoothPathSegment::GetLength() const
    {
        return fLength_;
    }

    void SmoothPath::AddPoint( const SmoothPathPoint& mPoint )
    {
        if (!bStarted_)
        {
            lPointPath_.push_back(mPoint);
        }
        else
        {
            Warning(CLASS_NAME,
                "Can't add a new point to a SmoothPath if it has already started to update."
            );
        }
    }

    void SmoothPath::CheckPath_()
    {
        if (lPointPath_.empty())
        {
            Warning(CLASS_NAME, "Can't use empty path. Please add points using AddPoint().");
        }
        else if (lPointPath_.size() < 4)
        {
            Warning(CLASS_NAME, "Can't use a SmoothPath with less than 4 points.");
        }
        else
        {
            // Build segments
            if (!(bLoop_ && bLinkEndToStart_))
            {
                for (s_int i = 0; i < lPointPath_.size()-3; i++)
                {
                    lSegmentList_.push_back(SmoothPathSegment(this, i));
                }
            }
            else
            {
                for (s_int i = -1; i < lPointPath_.size()-1; i++)
                {
                    lSegmentList_.push_back(SmoothPathSegment(this, i));
                }
            }

            mSegment_ = lSegmentList_.begin();
            mActualPoint_ = mSegment_->GetPoint(1).mPosition;
        }
    }

    void SmoothPath::SetLoop( const s_bool& bLoop, const s_bool& bLinkEndToStart )
    {
        if (!bStarted_)
        {
            bLoop_ = bLoop;
            if (bLoop_)
                bLinkEndToStart_ = bLinkEndToStart;
        }
        else
        {
            Warning(CLASS_NAME,
                "Can't call SetLoop() if the path has already started to update."
            );
        }
    }

    void SmoothPath::Play()
    {
        if (bPaused_)
            bPaused_ = false;
        else
        {
            bEnded_ = false;

            mSegment_ = lSegmentList_.begin();
            mActualPoint_ = mSegment_->GetPoint(1).mPosition;
        }
    }

    void SmoothPath::Update( const s_float& fDelta )
    {
        if (!bStarted_)
        {
            CheckPath_();
            bStarted_ = true;
        }

        if ( (lPointPath_.size() >= 4) && !bEnded_ && !bPaused_ )
        {
            if (!bReversed_)
            {
                s_float fTranslation = fDelta*fSpeed_;
                s_float fRemaining = mSegment_->GetLength() - fDistance_;

                // Check if we have reached the next point, and maybe got even further.
                while (fRemaining < fTranslation)
                {
                    // We have reached the next point.
                    mSegment_++;
                    if (mSegment_ == lSegmentList_.end())
                    {
                        // We have reached the end of the path...
                        if (bLoop_)
                        {
                            // Looping is enabled : restart the path at
                            // its beginning.
                            mSegment_ = lSegmentList_.begin();
                        }
                        else
                        {
                            // Looping is disabled : the path has done
                            // its job, there's no need to continue.
                            bEnded_ = true;
                            mSegment_--;
                            mActualPoint_ = mSegment_->GetPoint(2).mPosition;
                            return;
                        }
                    }

                    fTranslation -= fRemaining;
                    fRemaining = mSegment_->GetLength();
                    fDistance_ = 0.0f;
                }

                fDistance_ += fTranslation;

                mActualPoint_ = mSegment_->CalcPoint(fDistance_);
            }
            else
            {
                s_float fTranslation = fDelta*fSpeed_;

                // Check if we have reached the next point, and maybe got even further.
                while (fDistance_ < fTranslation)
                {
                    // We have reached the next point.
                    if (mSegment_ == lSegmentList_.begin())
                    {
                        // We have reached the beginning of the path...
                        if (bLoop_)
                        {
                            // Looping is enabled : restart the path at
                            // its end.
                            mSegment_ = lSegmentList_.end();
                            mSegment_--;
                        }
                        else
                        {
                            // Looping is disabled : the path has done
                            // its job, there's no need to continue.
                            bEnded_ = true;
                            mActualPoint_ = mSegment_->GetPoint(1).mPosition;
                            return;
                        }
                    }
                    else
                    {
                        mSegment_--;
                    }

                    fTranslation -= fDistance_;
                    fDistance_ = mSegment_->GetLength();
                }

                fDistance_ -= fTranslation;

                mActualPoint_ = mSegment_->CalcPoint(fDistance_);
            }
        }
    }

    const SmoothPathPoint& SmoothPath::GetPoint( const s_int& iIndex ) const
    {
        if ( (iIndex < lPointPath_.size()) && (iIndex >= 0) )
        {
            list<SmoothPathPoint>::const_iterator iterPoint = lPointPath_.begin();
            for (s_int i = 0; i < iIndex; i++)
                iterPoint++;

            return (*iterPoint);
        }
        else
        {
            s_int iNewIndex = iIndex;
            while (iNewIndex >= lPointPath_.size())
                iNewIndex -= lPointPath_.size();

            while (iNewIndex < 0)
                iNewIndex += lPointPath_.size();

            list<SmoothPathPoint>::const_iterator iterPoint = lPointPath_.begin();
            for (s_int i = 0; i < iNewIndex; i++)
                iterPoint++;

            return (*iterPoint);
        }
    }

    list<SmoothPathPoint>::iterator SmoothPath::GetPointIterator( const s_int& iIndex )
    {
        if ( (iIndex < lPointPath_.size()) && (iIndex >= 0) )
        {
            list<SmoothPathPoint>::iterator iterPoint = lPointPath_.begin();
            for (s_int i = 0; i < iIndex; i++)
                iterPoint++;

            return iterPoint;
        }
        else
        {
            s_int iNewIndex = iIndex;
            while (iNewIndex >= lPointPath_.size())
                iNewIndex -= lPointPath_.size();

            while (iNewIndex < 0)
                iNewIndex += lPointPath_.size();

            list<SmoothPathPoint>::iterator iterPoint = lPointPath_.begin();
            for (s_int i = 0; i < iNewIndex; i++)
                iterPoint++;

            return iterPoint;
        }
    }

    s_uint SmoothPath::GetPointNumber() const
    {
        return lPointPath_.size();
    }
}
