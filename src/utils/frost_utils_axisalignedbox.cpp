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

    s_bool AxisAlignedBox::Contains(const AxisAlignedBox& mBox) const
    {
        for (uint ui = 0; ui < 8; ++ui)
        {
            if (this->Contains(mBox[ui]))
                return true;
        }

        return false;
    }

    s_bool AxisAlignedBox::Contains(const Vector& mPoint) const
    {
        return (mPoint.X().IsInRange(mMin_.X(), mMax_.X()) &&
                mPoint.Y().IsInRange(mMin_.Y(), mMax_.Y()) &&
                mPoint.Z().IsInRange(mMin_.Z(), mMax_.Z()));
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
                fLowestT = s_float::Min(fLowestT, fT);
                mIntersection = mPoint;
            }
        }
        if (mRayOrigin.X() >= mMax_.X() && mRayDirection.X() < 0.0f)
        {
            s_float fT = (mMax_.X() - mRayOrigin.X())/mRayDirection.X();
            Vector mPoint = mRayOrigin + mRayDirection*fT;
            if (mPoint.Y().IsInRange(mMin_.Y(), mMax_.Y()) && mPoint.Z().IsInRange(mMin_.Z(), mMax_.Z()))
            {
                fLowestT = s_float::Min(fLowestT, fT);
                mIntersection = mPoint;
            }
        }

        // Y
        if (mRayOrigin.Y() <= mMin_.Y() && mRayDirection.Y() > 0.0f)
        {
            s_float fT = (mMin_.Y() - mRayOrigin.Y())/mRayDirection.Y();
            Vector mPoint = mRayOrigin + mRayDirection*fT;
            if (mPoint.X().IsInRange(mMin_.X(), mMax_.X()) && mPoint.Z().IsInRange(mMin_.Z(), mMax_.Z()))
            {
                fLowestT = s_float::Min(fLowestT, fT);
                mIntersection = mPoint;
            }
        }
        if (mRayOrigin.Y() >= mMax_.Y() && mRayDirection.Y() < 0.0f)
        {
            s_float fT = (mMax_.Y() - mRayOrigin.Y())/mRayDirection.Y();
            Vector mPoint = mRayOrigin + mRayDirection*fT;
            if (mPoint.X().IsInRange(mMin_.X(), mMax_.X()) && mPoint.Z().IsInRange(mMin_.Z(), mMax_.Z()))
            {
                fLowestT = s_float::Min(fLowestT, fT);
                mIntersection = mPoint;
            }
        }

        // Z
        if (mRayOrigin.Z() <= mMin_.Z() && mRayDirection.Z() > 0.0f)
        {
            s_float fT = (mMin_.Z() - mRayOrigin.Z())/mRayDirection.Z();
            Vector mPoint = mRayOrigin + mRayDirection*fT;
            if (mPoint.X().IsInRange(mMin_.X(), mMax_.X()) && mPoint.Y().IsInRange(mMin_.Y(), mMax_.Y()))
            {
                fLowestT = s_float::Min(fLowestT, fT);
                mIntersection = mPoint;
            }
        }
        if (mRayOrigin.Z() >= mMax_.Z() && mRayDirection.Z() < 0.0f)
        {
            s_float fT = (mMax_.Z() - mRayOrigin.Z())/mRayDirection.Z();
            Vector mPoint = mRayOrigin + mRayDirection*fT;
            if (mPoint.X().IsInRange(mMin_.X(), mMax_.X()) && mPoint.Y().IsInRange(mMin_.Y(), mMax_.Y()))
            {
                fLowestT = s_float::Min(fLowestT, fT);
                mIntersection = mPoint;
            }
        }

        return fLowestT.IsValid();

        // Fall back to Ogre here (not the time to rewrite the code)
        /*#ifdef USING_OGRE
        Ogre::AxisAlignedBox mAAB = FrostToOgre(*this);
        Ogre::Ray            mRay(Vector::FrostToOgre(mRayOrigin), Vector::FrostToOgre(mRayDirection));

        std::pair<bool, Ogre::Real> mResult = mRay.intersects(mAAB);
        if (mResult.first)
        {
            mIntersection = Vector::OgreToFrost(mRay.getPoint(mResult.second));
            return true;
        }
        else
            return false;
        #else



            return false;
        #endif*/
    }

    #ifdef USING_OGRE
    Ogre::AxisAlignedBox AxisAlignedBox::FrostToOgre(const AxisAlignedBox& mBox)
    {
        if (mBox.GetMin().IsInfinite() || mBox.GetMax().IsInfinite())
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

    s_str operator + (const string_element* sLeft, const AxisAlignedBox& mRight)
    {
        return s_str(sLeft)+mRight;
    }

    s_str operator + (const s_str& sLeft, const AxisAlignedBox& mRight)
    {
        return sLeft+"(min : "+mRight.GetMin()+", max : "+mRight.GetMax()+")";
    }

    s_str& operator << (s_str& sLeft, const AxisAlignedBox& mRight)
    {
        return sLeft << "(min : "+mRight.GetMin()+", max : "+mRight.GetMax()+")";
    }
}
