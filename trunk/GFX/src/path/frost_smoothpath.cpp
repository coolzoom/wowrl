/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            SmoothPath source           */
/*                                        */
/*                                        */

#include "path/frost_smoothpath.h"

using namespace std;

namespace Frost
{
    const s_str SmoothPath::CLASS_NAME = "SmoothPath";
    const s_str SmoothPath::Segment::CLASS_NAME = "SmoothPath::Segment";
    const s_str SmoothPath::Iterator::CLASS_NAME = "SmoothPath::Iterator";

    SmoothPath::SmoothPath()
    {
    }

    SmoothPath::SmoothPath( const SmoothPath& mPath )
    {
        lPointPath_ = mPath.lPointPath_;
    }

    SmoothPath::~SmoothPath()
    {
    }

    SmoothPath::Segment::Segment( s_wptr<SmoothPath> pPath, const s_int& iFirstID )
    {
        if (s_refptr<SmoothPath> pLockedPath = pPath.Lock())
        {
            lPointList_[0] = pLockedPath->GetPointIterator(iFirstID);
            lPointList_[1] = pLockedPath->GetPointIterator(iFirstID+1);
            lPointList_[2] = pLockedPath->GetPointIterator(iFirstID+2);
            lPointList_[3] = pLockedPath->GetPointIterator(iFirstID+3);
        }

        // Build arc length map
        s_float fOldX = lPointList_[1]->mPosition.X();
        s_float fOldY = lPointList_[1]->mPosition.Y();
        s_float fOldZ = lPointList_[1]->mPosition.Z();

        lLengthTable_[0.0f] = 0.0f;

        for (s_uint i = 1; i <= 20; ++i)
        {
            s_float t = s_float(i)*0.05f;
            s_float t2 = t*t;
            s_float t3 = t*t*t;

            s_float fX = 0.5f*(
                (2.0f*lPointList_[1]->mPosition.X()) +
                (-lPointList_[0]->mPosition.X() + lPointList_[2]->mPosition.X())*t +
                (2.0f*lPointList_[0]->mPosition.X() - 5.0f*lPointList_[1]->mPosition.X() + 4.0f*lPointList_[2]->mPosition.X() - lPointList_[3]->mPosition.X())*t2 +
                (-lPointList_[0]->mPosition.X() + 3.0f*lPointList_[1]->mPosition.X() - 3.0f*lPointList_[2]->mPosition.X() + lPointList_[3]->mPosition.X())*t3
            );
            s_float fY = 0.5f*(
                (2.0f*lPointList_[1]->mPosition.Y()) +
                (-lPointList_[0]->mPosition.Y() + lPointList_[2]->mPosition.Y())*t +
                (2.0f*lPointList_[0]->mPosition.Y() - 5.0f*lPointList_[1]->mPosition.Y() + 4.0f*lPointList_[2]->mPosition.Y() - lPointList_[3]->mPosition.Y())*t2 +
                (-lPointList_[0]->mPosition.Y() + 3.0f*lPointList_[1]->mPosition.Y() - 3.0f*lPointList_[2]->mPosition.Y() + lPointList_[3]->mPosition.Y())*t3
            );
            s_float fZ = 0.5f*(
                (2.0f*lPointList_[1]->mPosition.Z()) +
                (-lPointList_[0]->mPosition.Z() + lPointList_[2]->mPosition.Z())*t +
                (2.0f*lPointList_[0]->mPosition.Z() - 5.0f*lPointList_[1]->mPosition.Z() + 4.0f*lPointList_[2]->mPosition.Z() - lPointList_[3]->mPosition.Z())*t2 +
                (-lPointList_[0]->mPosition.Z() + 3.0f*lPointList_[1]->mPosition.Z() - 3.0f*lPointList_[2]->mPosition.Z() + lPointList_[3]->mPosition.Z())*t3
            );

            fLength_ += Vector(fX-fOldX, fY-fOldY, fZ-fOldZ).GetNorm();
            lLengthTable_[fLength_] = t;

            fOldX = fX;
            fOldY = fY;
            fOldZ = fZ;
        }
    }

    Vector SmoothPath::Segment::CalcPoint( const s_float& fDistance )
    {
        s_float t;
        if (fDistance <= 0.0f)
        {
            t = 0.0f;
        }
        else
        {
            s_map<s_float, s_float>::iterator iterFirst, iterSecond;
            iterSecond = lLengthTable_.LowerBound(fDistance);
            if (iterSecond != lLengthTable_.End())
            {
                iterFirst = iterSecond;
                --iterFirst;

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

        mVec.X() =
            0.5f*(
            (2.0f*lPointList_[1]->mPosition.X()) +
            (-lPointList_[0]->mPosition.X() + lPointList_[2]->mPosition.X())*t +
            (2.0f*lPointList_[0]->mPosition.X() - 5.0f*lPointList_[1]->mPosition.X() + 4.0f*lPointList_[2]->mPosition.X() - lPointList_[3]->mPosition.X())*t2 +
            (-lPointList_[0]->mPosition.X() + 3.0f*lPointList_[1]->mPosition.X() - 3.0f*lPointList_[2]->mPosition.X() + lPointList_[3]->mPosition.X())*t3)
        ;
        mVec.Y() =
            0.5f*(
            (2.0f*lPointList_[1]->mPosition.Y()) +
            (-lPointList_[0]->mPosition.Y() + lPointList_[2]->mPosition.Y())*t +
            (2.0f*lPointList_[0]->mPosition.Y() - 5.0f*lPointList_[1]->mPosition.Y() + 4.0f*lPointList_[2]->mPosition.Y() - lPointList_[3]->mPosition.Y())*t2 +
            (-lPointList_[0]->mPosition.Y() + 3.0f*lPointList_[1]->mPosition.Y() - 3.0f*lPointList_[2]->mPosition.Y() + lPointList_[3]->mPosition.Y())*t3)
        ;
        mVec.Z() =
            0.5f*(
            (2.0f*lPointList_[1]->mPosition.Z()) +
            (-lPointList_[0]->mPosition.Z() + lPointList_[2]->mPosition.Z())*t +
            (2.0f*lPointList_[0]->mPosition.Z() - 5.0f*lPointList_[1]->mPosition.Z() + 4.0f*lPointList_[2]->mPosition.Z() - lPointList_[3]->mPosition.Z())*t2 +
            (-lPointList_[0]->mPosition.Z() + 3.0f*lPointList_[1]->mPosition.Z() - 3.0f*lPointList_[2]->mPosition.Z() + lPointList_[3]->mPosition.Z())*t3)
        ;

        return mVec;
    }

    const SmoothPath::Point& SmoothPath::Segment::GetPoint( const s_uint& uiIndex ) const
    {
        return (*lPointList_[uiIndex]);
    }

    const s_float& SmoothPath::Segment::GetLength() const
    {
        return fLength_;
    }

    void SmoothPath::AddPoint( const Point& mPoint )
    {
        lPointPath_.PushBack(mPoint);
    }

    const SmoothPath::Point& SmoothPath::GetPoint( const s_int& iIndex ) const
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

    s_ctnr<SmoothPath::Point>::const_iterator SmoothPath::GetPointIterator( const s_int& iIndex ) const
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

    s_uint SmoothPath::GetPointNumber() const
    {
        return lPointPath_.GetSize();
    }

    s_refptr<Path::Iterator> SmoothPath::CreateIterator() const
    {
        return s_refptr<Path::Iterator>(new SmoothPath::Iterator());
    }

    SmoothPath::Iterator::Iterator() : bFirstPlay_(true)
    {
    }

    SmoothPath::Iterator::~Iterator()
    {
    }

    void SmoothPath::Iterator::Play()
    {
        Path::Iterator::Play();

        if (bFirstPlay_)
        {
            if (pPath_->GetPointNumber() >= 4)
            {
                // Build segments
                if (!(bLoop_ && bLinkEndToStart_))
                {
                    for (s_int i = 0; i < s_int(pPath_->GetPointNumber())-3; ++i)
                    {
                        lSegmentList_.PushBack(Segment(pPath_, i));
                    }
                }
                else
                {
                    for (s_int i = -1; i < s_int(pPath_->GetPointNumber())-1; ++i)
                    {
                        lSegmentList_.PushBack(Segment(pPath_, i));
                    }
                }

                if (!bReversed_)
                {
                    mSegment_ = lSegmentList_.Begin();
                    mPosition_ = mSegment_->GetPoint(1).mPosition;
                }
                else
                {
                    mSegment_ = lSegmentList_.End()-1;
                    mPosition_ = mSegment_->GetPoint(2).mPosition;
                }

                mPosition_ = mSegment_->GetPoint(1).mPosition;
            }
            else
            {
                bPaused_ = true;
                throw Exception(CLASS_NAME, "Can't play a SmoothPath with less than 4 points.");
            }

            bFirstPlay_ = false;
        }
    }

    void SmoothPath::Iterator::Update( const s_float& fDelta )
    {
        if ( (pPath_->GetPointNumber() >= 4) && !bEnded_ && !bPaused_ )
        {
            if (!bReversed_)
            {
                s_float fTranslation = fDelta*fSpeed_;
                s_float fRemaining = mSegment_->GetLength() - fDistance_;

                // Check if we have reached the next point, and maybe got even further.
                while (fTranslation > fRemaining)
                {
                    // We have reached the next point.
                    ++mSegment_;
                    if (mSegment_ == lSegmentList_.End())
                    {
                        // We have reached the end of the path...
                        if (bLoop_)
                        {
                            // Looping is enabled : restart the path at
                            // its beginning.
                            mSegment_ = lSegmentList_.Begin();
                        }
                        else
                        {
                            // Looping is disabled : the path has done
                            // its job, there's no need to continue.
                            bEnded_ = true;
                            --mSegment_;
                            mPosition_ = mSegment_->GetPoint(2).mPosition;
                            return;
                        }
                    }

                    fTranslation -= fRemaining;
                    fRemaining = mSegment_->GetLength();
                    fDistance_ = 0.0f;
                }

                fDistance_ += fTranslation;

                mPosition_ = mSegment_->CalcPoint(fDistance_);
            }
            else
            {
                s_float fTranslation = fDelta*fSpeed_;

                // Check if we have reached the next point, and maybe got even further.
                while (fTranslation > fDistance_)
                {
                    // We have reached the next point.
                    if (mSegment_ == lSegmentList_.Begin())
                    {
                        // We have reached the beginning of the path...
                        if (bLoop_)
                        {
                            // Looping is enabled : restart the path at
                            // its end.
                            mSegment_ = lSegmentList_.End();
                            --mSegment_;
                        }
                        else
                        {
                            // Looping is disabled : the path has done
                            // its job, there's no need to continue.
                            bEnded_ = true;
                            mPosition_ = mSegment_->GetPoint(1).mPosition;
                            return;
                        }
                    }
                    else
                    {
                        --mSegment_;
                    }

                    fTranslation -= fDistance_;
                    fDistance_ = mSegment_->GetLength();
                }

                fDistance_ -= fTranslation;

                mPosition_ = mSegment_->CalcPoint(fDistance_);
            }
        }
    }

    void SmoothPath::Iterator::SetPath( s_refptr<Path> pPath )
    {
        pPath_ = s_refptr<SmoothPath>::DynamicCast(pPath);
    }

    s_wptr<Path> SmoothPath::Iterator::GetPath() const
    {
        return pPath_;
    }
}
