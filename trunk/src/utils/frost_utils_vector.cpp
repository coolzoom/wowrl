/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_vector.h"
#include "frost_utils_math.h"
#include "frost_utils_str.h"

#include <OgreQuaternion.h>
#include <OgreVector3.h>

#define SQRT_3  1.7320508075688772935274463415059
#define ISQRT_3 0.5773502691896257645091487805019
#define ISQRT_2 0.7071067811865475244008443621048

namespace Frost
{
    const s_str Vector::CLASS_NAME = "Vector";

    const Vector Vector::NaN      = Vector(s_float::NaN, s_float::NaN, s_float::NaN, s_float::NaN);
    const Vector Vector::ZERO     = Vector();
    const Vector Vector::UNIT     = Vector(1.0f,    1.0f,    1.0f,    SQRT_3);
    const Vector Vector::UNIT_X   = Vector(1.0f,    0.0f,    0.0f,    1.0f);
    const Vector Vector::UNIT_Y   = Vector(0.0f,    1.0f,    0.0f,    1.0f);
    const Vector Vector::UNIT_Z   = Vector(0.0f,    0.0f,    1.0f,    1.0f);
    const Vector Vector::UNIT_XY  = Vector(ISQRT_2, ISQRT_2, 0.0f,    1.0f);
    const Vector Vector::UNIT_XZ  = Vector(ISQRT_2, 0.0f,    ISQRT_2, 1.0f);
    const Vector Vector::UNIT_YZ  = Vector(0.0f,    ISQRT_2, ISQRT_2, 1.0f);
    const Vector Vector::UNIT_XYZ = Vector(ISQRT_3, ISQRT_3, ISQRT_3, 1.0f);

    Vector::Vector()
    {
        fNorm_ = 0.0f;
    }

    Vector::Vector( const s_float& fX, const s_float& fY, const s_float& fZ ) :
        fX_(fX), fY_(fY), fZ_(fZ)
    {
        bRebuildNorm_ = true;
    }

    Vector::Vector( const s_float& fX, const s_float& fY, const s_float& fZ, const s_float& fNorm ) :
        fX_(fX), fY_(fY), fZ_(fZ), fNorm_(fNorm)
    {
    }

    Vector::Vector( const s_float& fValue ) :
        fX_(fValue), fY_(fValue), fZ_(fValue)
    {
        fNorm_ = fabs(fValue)*SQRT_3;
    }

    s_bool Vector::IsNull() const
    {
        if (!bRebuildNorm_)
            return fNorm_.IsNull();
        else
            return ( (fX_.IsNull()) && (fY_.IsNull()) && (fZ_.IsNull()) );
    }

    const s_float& Vector::GetNorm()
    {
        UpdateNorm_();
        return fNorm_;
    }

    const s_float& Vector::GetLenght()
    {
        UpdateNorm_();
        return fNorm_;
    }

    Vector Vector::GetUnit()
    {
        UpdateNorm_();
        return (*this)/fNorm_;
    }

    void Vector::Normalize()
    {
        UpdateNorm_();
        fX_ /= fNorm_;
        fY_ /= fNorm_;
        fZ_ /= fNorm_;
        fNorm_ = 1.0f;
    }

    void Vector::Rotate( const s_float& fAngle, const Vector& mAxis )
    {
        Ogre::Quaternion mQuad(
            Ogre::Radian(fAngle.GetRad().Get()),
            FrostToOgre(mAxis)
        );

        Ogre::Vector3 mTemp = FrostToOgre(*this);
        mTemp = mQuad*mTemp;

        (*this) = OgreToFrost(mTemp);
    }

    const s_float& Vector::X() const
    {
        return fX_;
    }

    s_float& Vector::XR()
    {
        bRebuildNorm_ = true;
        return fX_;
    }

    void Vector::X( const s_float& fX )
    {
        fX_ = fX;
        bRebuildNorm_ = true;
    }

    const s_float& Vector::Y() const
    {
        return fY_;
    }

    s_float& Vector::YR()
    {
        bRebuildNorm_ = true;
        return fY_;

    }

    void Vector::Y( const s_float& fY )
    {
        fY_ = fY;
        bRebuildNorm_ = true;
    }

    const s_float& Vector::Z() const
    {
        return fZ_;
    }

    s_float& Vector::ZR()
    {
        bRebuildNorm_ = true;
        return fZ_;
    }

    void Vector::Z( const s_float& fZ )
    {
        fZ_ = fZ;
        bRebuildNorm_ = true;
    }

    bool Vector::operator! () const
    {
        return (!fX_ || !fY_ || !fZ_);
    }

    Vector::operator MemberFn() const
    {
        if (fX_ && fY_ && fZ_)
            return &Vector::Normalize;
        else
            return NULL;
    }

    Vector Vector::operator - () const
    {
        if (!bRebuildNorm_)
            return Vector(-fX_, -fY_, -fZ_, fNorm_);
        else
            return Vector(-fX_, -fY_, -fZ_);
    }

    Vector Vector::operator * ( const s_float& fValue ) const
    {
        if (!bRebuildNorm_)
            return Vector(fX_*fValue, fY_*fValue, fZ_*fValue, fNorm_*fValue);
        else
            return Vector(fX_*fValue, fY_*fValue, fZ_*fValue);
    }

    Vector Vector::operator / ( const s_float& fValue ) const
    {
        if (!bRebuildNorm_)
            return Vector(fX_/fValue, fY_/fValue, fZ_/fValue, fNorm_/fValue);
        else
            return Vector(fX_/fValue, fY_/fValue, fZ_/fValue);
    }

    void Vector::operator *= ( const s_float& fValue )
    {
        fX_ *= fValue; fY_ *= fValue; fZ_ *= fValue;
        if (!bRebuildNorm_)
            fNorm_ *= fValue;
    }

    void Vector::operator /= ( const s_float& fValue )
    {
        fX_ /= fValue; fY_ /= fValue; fZ_ /= fValue;
        if (!bRebuildNorm_)
            fNorm_ *= fValue;
    }

    Vector Vector::operator + ( const Vector& mVec ) const
    {
        return Vector(fX_ + mVec.fX_, fY_ + mVec.fY_, fZ_ + mVec.fZ_);
    }

    Vector Vector::operator - ( const Vector& mVec ) const
    {
        return Vector(fX_ - mVec.fX_, fY_ - mVec.fY_, fZ_ - mVec.fZ_);
    }

    s_float Vector::operator * ( const Vector& mVec ) const
    {
        return fX_*mVec.fX_ + fY_*mVec.fY_ + fZ_*mVec.fZ_;
    }

    Vector Vector::operator ^ (const Vector& mVec) const
    {
        return Vector(fY_*mVec.Z() - fZ_*mVec.Y(),
                      fZ_*mVec.X() - fX_*mVec.Z(),
                      fX_*mVec.Y() - fY_*mVec.X());
    }

    void Vector::operator += ( const Vector& mVec )
    {
        fX_ += mVec.fX_; fY_ += mVec.fY_; fZ_ += mVec.fZ_;
        bRebuildNorm_ = true;
    }

    void Vector::operator -= ( const Vector& mVec )
    {
        fX_ -= mVec.fX_; fY_ -= mVec.fY_; fZ_ -= mVec.fZ_;
        bRebuildNorm_ = true;
    }

    bool Vector::operator == ( const Vector& mVec ) const
    {
        return ( (fX_ == mVec.fX_) && (fY_ == mVec.fY_) && (fZ_ == mVec.fZ_) );
    }

    bool Vector::operator != ( const Vector& mVec ) const
    {
        return ( (fX_ != mVec.fX_) || (fY_ != mVec.fY_) || (fZ_ != mVec.fZ_) );
    }

    Vector& Vector::operator = ( const Vector& mVec )
    {
        fX_ = mVec.fX_; fY_ = mVec.fY_; fZ_ = mVec.fZ_;
        bRebuildNorm_ = mVec.bRebuildNorm_;
        if (!bRebuildNorm_)
            fNorm_ = mVec.fNorm_;

        return *this;
    }

    void Vector::UpdateNorm_()
    {
        if (bRebuildNorm_)
        {
            fNorm_ = sqrt(fX_*fX_ + fY_*fY_ + fZ_*fZ_);
            bRebuildNorm_ = false;
        }
    }

    Ogre::Vector3 Vector::FrostToOgre( const Vector &mVector )
    {
        /*return Ogre::Vector3(
            mVector.Y().Get(),
            mVector.Z().Get(),
            mVector.X().Get()
        );*/
        return Ogre::Vector3(
            mVector.X().Get(),
            mVector.Y().Get(),
            mVector.Z().Get()
        );
    }

    Vector Vector::OgreToFrost( const Ogre::Vector3 &mVector )
    {
        //return Vector(mVector.z, mVector.x, mVector.y);
        return Vector(mVector.x, mVector.y, mVector.z);
    }

    s_str operator + (const char* sLeft, const Vector& mRight)
    {
        return s_str(sLeft)+"("+s_str(mRight.X())+", "+s_str(mRight.Y())+", "+s_str(mRight.Z())+")";
    }

    s_str operator + (const s_str& sLeft, const Vector& mRight)
    {
        return sLeft+"("+s_str(mRight.X())+", "+s_str(mRight.Y())+", "+s_str(mRight.Z())+")";
    }
}
