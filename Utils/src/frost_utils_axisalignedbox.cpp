/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          AxisAlignedBox source         */
/*                                        */
/*                                        */

#include "frost_utils_axisalignedbox.h"

#ifdef USING_OGRE
#include <OgreAxisAlignedBox.h>
#include <OgreRay.h>
#endif

using namespace std;

namespace Frost
{
    AxisAlignedBox::AxisAlignedBox() :
        mMin_(s_float::INFMINUS), mMax_(s_float::INFPLUS)
    {

    }

    AxisAlignedBox::AxisAlignedBox(const Vector& mMin, const Vector& mMax) :
        mMin_(mMin), mMax_(mMax)
    {
    }

    AxisAlignedBox::~AxisAlignedBox()
    {
    }

    void AxisAlignedBox::SetMin( const Vector& mMin )
    {
        mMin_ = mMin;
    }

    void AxisAlignedBox::SetMax( const Vector& mMax )
    {
        mMax_ = mMax;
    }

    const Vector& AxisAlignedBox::GetMin() const
    {
        return mMin_;
    }

    const Vector& AxisAlignedBox::GetMax() const
    {
        return mMax_;
    }

    Vector AxisAlignedBox::operator[] (const s_uint& uiIndex) const
    {
        if (uiIndex == 0)
            return mMin_;
        else if (uiIndex == 1)
            return Vector(mMax_.X(), mMin_.Y(), mMin_.Z());
        else if (uiIndex == 2)
            return Vector(mMax_.X(), mMin_.Y(), mMax_.Z());
        else if (uiIndex == 3)
            return Vector(mMin_.X(), mMin_.Y(), mMax_.Z());
        else if (uiIndex == 4)
            return Vector(mMin_.X(), mMax_.Y(), mMax_.Z());
        else if (uiIndex == 5)
            return Vector(mMin_.X(), mMax_.Y(), mMin_.Z());
        else if (uiIndex == 6)
            return Vector(mMax_.X(), mMax_.Y(), mMin_.Z());
        else if (uiIndex == 7)
            return mMax_;
        else
            return Vector::NaN;
    }

    Vector AxisAlignedBox::operator[] (const uint& uiIndex) const
    {
        if (uiIndex == 0)
            return mMin_;
        else if (uiIndex == 1)
            return Vector(mMax_.X(), mMin_.Y(), mMin_.Z());
        else if (uiIndex == 2)
            return Vector(mMax_.X(), mMin_.Y(), mMax_.Z());
        else if (uiIndex == 3)
            return Vector(mMin_.X(), mMin_.Y(), mMax_.Z());
        else if (uiIndex == 4)
            return Vector(mMin_.X(), mMax_.Y(), mMax_.Z());
        else if (uiIndex == 5)
            return Vector(mMin_.X(), mMax_.Y(), mMin_.Z());
        else if (uiIndex == 6)
            return Vector(mMax_.X(), mMax_.Y(), mMin_.Z());
        else if (uiIndex == 7)
            return mMax_;
        else
            return Vector::NaN;
    }

    AxisAlignedBox AxisAlignedBox::operator + ( const Vector& mAdd ) const
    {
        return AxisAlignedBox(mMin_ + mAdd, mMax_ + mAdd);
    }

    AxisAlignedBox AxisAlignedBox::operator - ( const Vector& mAdd ) const
    {
        return AxisAlignedBox(mMin_ - mAdd, mMax_ - mAdd);
    }

    void AxisAlignedBox::operator += ( const Vector& mAdd )
    {
        mMin_ += mAdd;
        mMax_ += mAdd;
    }

    void AxisAlignedBox::operator -= ( const Vector& mAdd )
    {
        mMin_ -= mAdd;
        mMax_ -= mAdd;
    }

    s_bool AxisAlignedBox::Contains( const AxisAlignedBox& mBox ) const
    {
        if (IsInfinite() || mBox.IsInfinite())
            return true;

        if (mMax_.X() < mBox.mMin_.X())
            return false;
        if (mMax_.Y() < mBox.mMin_.Y())
            return false;
        if (mMax_.Z() < mBox.mMin_.Z())
            return false;

        if (mMin_.X() > mBox.mMax_.X())
            return false;
        if (mMin_.Y() > mBox.mMax_.Y())
            return false;
        if (mMin_.Z() > mBox.mMax_.Z())
            return false;

        return true;
    }

    s_bool AxisAlignedBox::Contains( const Vector& mPoint ) const
    {
        return (mPoint.X().IsInRange(mMin_.X(), mMax_.X()) &&
                mPoint.Y().IsInRange(mMin_.Y(), mMax_.Y()) &&
                mPoint.Z().IsInRange(mMin_.Z(), mMax_.Z()));
    }

    void AxisAlignedBox::Include( const Vector& mPoint )
    {
        mMax_.X() = s_float::Max(mMax_.X(), mPoint.X());
        mMax_.Y() = s_float::Max(mMax_.Y(), mPoint.Y());
        mMax_.Z() = s_float::Max(mMax_.Z(), mPoint.Z());

        mMin_.X() = s_float::Min(mMin_.X(), mPoint.X());
        mMin_.Y() = s_float::Min(mMin_.Y(), mPoint.Y());
        mMin_.Z() = s_float::Min(mMin_.Z(), mPoint.Z());
    }

    s_bool AxisAlignedBox::IsInfinite() const
    {
        return mMin_.IsInfinite() || mMax_.IsInfinite();
    }

    s_bool AxisAlignedBox::GetRayIntersection( const Vector& mRayOrigin, const Vector& mRayDirection, Vector& mIntersection ) const
    {
        if (Contains(mRayOrigin))
        {
            mIntersection = mRayOrigin;
            return true;
        }

        // Check collision with each face of the bounding box (the 3 closest)
        s_float fLowestT = s_float::INFPLUS;

        // X
        if (mRayOrigin.X() <= mMin_.X() && mRayDirection.X() > 0.0f)
        {
            s_float fT = (mMin_.X() - mRayOrigin.X())/mRayDirection.X();
            Vector mPoint = mRayOrigin + mRayDirection*fT;
            if (mPoint.Y().IsInRange(mMin_.Y(), mMax_.Y()) && mPoint.Z().IsInRange(mMin_.Z(), mMax_.Z()))
            {
                if (fT < fLowestT)
                {
                    fLowestT = fT;
                    mIntersection = mPoint;
                }
            }
        }
        if (mRayOrigin.X() >= mMax_.X() && mRayDirection.X() < 0.0f)
        {
            s_float fT = (mMax_.X() - mRayOrigin.X())/mRayDirection.X();
            Vector mPoint = mRayOrigin + mRayDirection*fT;
            if (mPoint.Y().IsInRange(mMin_.Y(), mMax_.Y()) && mPoint.Z().IsInRange(mMin_.Z(), mMax_.Z()))
            {
                if (fT < fLowestT)
                {
                    fLowestT = fT;
                    mIntersection = mPoint;
                }
            }
        }

        // Y
        if (mRayOrigin.Y() <= mMin_.Y() && mRayDirection.Y() > 0.0f)
        {
            s_float fT = (mMin_.Y() - mRayOrigin.Y())/mRayDirection.Y();
            Vector mPoint = mRayOrigin + mRayDirection*fT;
            if (mPoint.X().IsInRange(mMin_.X(), mMax_.X()) && mPoint.Z().IsInRange(mMin_.Z(), mMax_.Z()))
            {
                if (fT < fLowestT)
                {
                    fLowestT = fT;
                    mIntersection = mPoint;
                }
            }
        }
        if (mRayOrigin.Y() >= mMax_.Y() && mRayDirection.Y() < 0.0f)
        {
            s_float fT = (mMax_.Y() - mRayOrigin.Y())/mRayDirection.Y();
            Vector mPoint = mRayOrigin + mRayDirection*fT;
            if (mPoint.X().IsInRange(mMin_.X(), mMax_.X()) && mPoint.Z().IsInRange(mMin_.Z(), mMax_.Z()))
            {
                if (fT < fLowestT)
                {
                    fLowestT = fT;
                    mIntersection = mPoint;
                }
            }
        }

        // Z
        if (mRayOrigin.Z() <= mMin_.Z() && mRayDirection.Z() > 0.0f)
        {
            s_float fT = (mMin_.Z() - mRayOrigin.Z())/mRayDirection.Z();
            Vector mPoint = mRayOrigin + mRayDirection*fT;
            if (mPoint.X().IsInRange(mMin_.X(), mMax_.X()) && mPoint.Y().IsInRange(mMin_.Y(), mMax_.Y()))
            {
                if (fT < fLowestT)
                {
                    fLowestT = fT;
                    mIntersection = mPoint;
                }
            }
        }
        if (mRayOrigin.Z() >= mMax_.Z() && mRayDirection.Z() < 0.0f)
        {
            s_float fT = (mMax_.Z() - mRayOrigin.Z())/mRayDirection.Z();
            Vector mPoint = mRayOrigin + mRayDirection*fT;
            if (mPoint.X().IsInRange(mMin_.X(), mMax_.X()) && mPoint.Y().IsInRange(mMin_.Y(), mMax_.Y()))
            {
                if (fT < fLowestT)
                {
                    fLowestT = fT;
                    mIntersection = mPoint;
                }
            }
        }

        return fLowestT.IsValid();
    }

    s_bool AxisAlignedBox::GetRayIntersection( const Vector& mRayOrigin, const Vector& mRayDirection, Vector& mIntersection, Face& mFace ) const
    {
        if (Contains(mRayOrigin))
        {
            mIntersection = mRayOrigin;
            return true;
        }

        // Check collision with each face of the bounding box (the 3 closest)
        s_float fLowestT = s_float::INFPLUS;

        // X
        if (mRayOrigin.X() <= mMin_.X() && mRayDirection.X() > 0.0f)
        {
            s_float fT = (mMin_.X() - mRayOrigin.X())/mRayDirection.X();
            Vector mPoint = mRayOrigin + mRayDirection*fT;
            if (mPoint.Y().IsInRange(mMin_.Y(), mMax_.Y()) && mPoint.Z().IsInRange(mMin_.Z(), mMax_.Z()))
            {
                if (fT < fLowestT)
                {
                    fLowestT = fT;
                    mIntersection = mPoint;
                    mFace = LEFT;
                }
            }
        }
        if (mRayOrigin.X() >= mMax_.X() && mRayDirection.X() < 0.0f)
        {
            s_float fT = (mMax_.X() - mRayOrigin.X())/mRayDirection.X();
            Vector mPoint = mRayOrigin + mRayDirection*fT;
            if (mPoint.Y().IsInRange(mMin_.Y(), mMax_.Y()) && mPoint.Z().IsInRange(mMin_.Z(), mMax_.Z()))
            {
                if (fT < fLowestT)
                {
                    fLowestT = fT;
                    mIntersection = mPoint;
                    mFace = RIGHT;
                }
            }
        }

        // Y
        if (mRayOrigin.Y() <= mMin_.Y() && mRayDirection.Y() > 0.0f)
        {
            s_float fT = (mMin_.Y() - mRayOrigin.Y())/mRayDirection.Y();
            Vector mPoint = mRayOrigin + mRayDirection*fT;
            if (mPoint.X().IsInRange(mMin_.X(), mMax_.X()) && mPoint.Z().IsInRange(mMin_.Z(), mMax_.Z()))
            {
                if (fT < fLowestT)
                {
                    fLowestT = fT;
                    mIntersection = mPoint;
                    mFace = BOTTOM;
                }
            }
        }
        if (mRayOrigin.Y() >= mMax_.Y() && mRayDirection.Y() < 0.0f)
        {
            s_float fT = (mMax_.Y() - mRayOrigin.Y())/mRayDirection.Y();
            Vector mPoint = mRayOrigin + mRayDirection*fT;
            if (mPoint.X().IsInRange(mMin_.X(), mMax_.X()) && mPoint.Z().IsInRange(mMin_.Z(), mMax_.Z()))
            {
                if (fT < fLowestT)
                {
                    fLowestT = fT;
                    mIntersection = mPoint;
                    mFace = TOP;
                }
            }
        }

        // Z
        if (mRayOrigin.Z() <= mMin_.Z() && mRayDirection.Z() > 0.0f)
        {
            s_float fT = (mMin_.Z() - mRayOrigin.Z())/mRayDirection.Z();
            Vector mPoint = mRayOrigin + mRayDirection*fT;
            if (mPoint.X().IsInRange(mMin_.X(), mMax_.X()) && mPoint.Y().IsInRange(mMin_.Y(), mMax_.Y()))
            {
                if (fT < fLowestT)
                {
                    fLowestT = fT;
                    mIntersection = mPoint;
                    mFace = FRONT;
                }
            }
        }
        if (mRayOrigin.Z() >= mMax_.Z() && mRayDirection.Z() < 0.0f)
        {
            s_float fT = (mMax_.Z() - mRayOrigin.Z())/mRayDirection.Z();
            Vector mPoint = mRayOrigin + mRayDirection*fT;
            if (mPoint.X().IsInRange(mMin_.X(), mMax_.X()) && mPoint.Y().IsInRange(mMin_.Y(), mMax_.Y()))
            {
                if (fT < fLowestT)
                {
                    fLowestT = fT;
                    mIntersection = mPoint;
                    mFace = BACK;
                }
            }
        }

        return fLowestT.IsValid();
    }

    #ifdef USING_OGRE
    Ogre::AxisAlignedBox AxisAlignedBox::FrostToOgre(const AxisAlignedBox& mBox)
    {
        if (mBox.IsInfinite())
            return Ogre::AxisAlignedBox(Ogre::AxisAlignedBox::EXTENT_INFINITE);
        else
            return Ogre::AxisAlignedBox(Vector::FrostToOgre(mBox.GetMin()), Vector::FrostToOgre(mBox.GetMax()));
    }

    AxisAlignedBox AxisAlignedBox::OgreToFrost(const Ogre::AxisAlignedBox& mBox)
    {
        if (mBox.isInfinite())
            return AxisAlignedBox();
        else
            return AxisAlignedBox(Vector::OgreToFrost(mBox.getMinimum()), Vector::OgreToFrost(mBox.getMaximum()));
    }
    #endif

    s_str::string StringConverter<string_element, AxisAlignedBox>::Convert( const AxisAlignedBox& mBox )
    {
        return "(min : "+StringConverter<string_element, Vector>::Convert(mBox.GetMin())+
              ", max : "+StringConverter<string_element, Vector>::Convert(mBox.GetMax())+")";
    }
}
