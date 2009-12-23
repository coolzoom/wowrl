/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          AxisAlignedBox source         */
/*                                        */
/*                                        */

#include "frost_utils_axisalignedbox.h"

#include <OgreAxisAlignedBox.h>
#include <OgreRay.h>

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
        // Fall back to Ogre here (not the time to rewrite the code)
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
    }

    Ogre::AxisAlignedBox AxisAlignedBox::FrostToOgre(const AxisAlignedBox& mBox)
    {
        return Ogre::AxisAlignedBox(Vector::FrostToOgre(mBox.GetMin()), Vector::FrostToOgre(mBox.GetMax()));
    }

    AxisAlignedBox AxisAlignedBox::OgreToFrost(const Ogre::AxisAlignedBox& mBox)
    {
        return AxisAlignedBox(Vector::OgreToFrost(mBox.getMinimum()), Vector::OgreToFrost(mBox.getMaximum()));
    }
}



