/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_vector.h"
#include "frost_utils_math.h"

#include <OgreQuaternion.h>
#include <OgreVector3.h>
#include <OgreMatrix4.h>

#define SQRT_3  1.7320508075688772935274463415059
#define ISQRT_3 0.5773502691896257645091487805019
#define ISQRT_2 0.7071067811865475244008443621048

namespace Frost
{
    const s_str Vector::CLASS_NAME = "Vector";

    const Vector Vector::NaN      = Vector(s_float::NaN, s_float::NaN, s_float::NaN);
    const Vector Vector::ZERO     = Vector();
    const Vector Vector::UNIT     = Vector(1.0f,    1.0f,    1.0f);
    const Vector Vector::UNIT_X   = Vector(1.0f,    0.0f,    0.0f);
    const Vector Vector::UNIT_Y   = Vector(0.0f,    1.0f,    0.0f);
    const Vector Vector::UNIT_Z   = Vector(0.0f,    0.0f,    1.0f);
    const Vector Vector::UNIT_XY  = Vector(ISQRT_2, ISQRT_2, 0.0f);
    const Vector Vector::UNIT_XZ  = Vector(ISQRT_2, 0.0f,    ISQRT_2);
    const Vector Vector::UNIT_YZ  = Vector(0.0f,    ISQRT_2, ISQRT_2);
    const Vector Vector::UNIT_XYZ = Vector(ISQRT_3, ISQRT_3, ISQRT_3);

    Vector::Vector()
    {
    }

    Vector::Vector( const s_float& fX, const s_float& fY, const s_float& fZ ) :
        fX_(fX), fY_(fY), fZ_(fZ)
    {
    }


    Vector::Vector( const s_float& fValue ) :
        fX_(fValue), fY_(fValue), fZ_(fValue)
    {
    }

    s_bool Vector::IsNull() const
    {
        return (fX_.IsNull() && fY_.IsNull() && fZ_.IsNull());
    }

    s_bool Vector::IsNaN() const
    {
        return (fX_.IsNaN() && fY_.IsNaN() && fZ_.IsNaN());
    }

    s_float Vector::GetNorm()
    {
        return sqrt(fX_*fX_ + fY_*fY_ + fZ_*fZ_);
    }

    s_float Vector::GetLength()
    {
        return sqrt(fX_*fX_ + fY_*fY_ + fZ_*fZ_);
    }

    s_float Vector::GetNormSquared()
    {
        return fX_*fX_ + fY_*fY_ + fZ_*fZ_;
    }

    s_float Vector::GetLengthSquared()
    {
        return fX_*fX_ + fY_*fY_ + fZ_*fZ_;
    }

    Vector Vector::GetUnit()
    {
        Vector mTemp = *this;
        mTemp.Normalize();
        return mTemp;
    }

    void Vector::Normalize()
    {
        s_float fNorm = sqrt(fX_*fX_ + fY_*fY_ + fZ_*fZ_);
        fX_ /= fNorm;
        fY_ /= fNorm;
        fZ_ /= fNorm;
    }

    void Vector::Rotate( const s_float& fAngle, const Vector& mAxis )
    {
        // Code extracted from Ogre for optimum performance
        s_float fHalfAngle = 0.5f*fAngle;
        s_float fSin = sin(fHalfAngle);
        s_float w = cos(fHalfAngle);
        Vector mNAxis = fSin*mAxis;

        Vector uv, uuv;
		uv = mNAxis ^ (*this);
		uuv = mNAxis ^ uv;
		uv *= 2.0f*w;
		uuv *= 2.0f;

		(*this) += uv + uuv;
    }

    void Vector::ScaleUp( const Vector& mScale )
    {
        fX_ *= mScale.fX_;
        fY_ *= mScale.fY_;
        fZ_ *= mScale.fZ_;
    }

    void Vector::ScaleDown( const Vector& mScale )
    {
        fX_ /= mScale.fX_;
        fY_ /= mScale.fY_;
        fZ_ /= mScale.fZ_;
    }

    void Vector::SetNaN()
    {
        fX_ = fY_ = fZ_ = s_float::NaN;
    }

    s_float& Vector::X()
    {
        return fX_;
    }

    s_float& Vector::Y()
    {
        return fY_;
    }

    s_float& Vector::Z()
    {
        return fZ_;
    }

    const s_float& Vector::X() const
    {
        return fX_;
    }

    const s_float& Vector::Y() const
    {
        return fY_;
    }

    const s_float& Vector::Z() const
    {
        return fZ_;
    }

    bool Vector::operator! () const
    {
        return (!fX_.IsValid() || !fY_.IsValid() || !fZ_.IsValid());
    }

    Vector::operator MemberFn() const
    {
        if (fX_.IsValid() && fY_.IsValid() && fZ_.IsValid())
            return &Vector::Normalize;
        else
            return NULL;
    }

    Vector Vector::operator - () const
    {
        return Vector(-fX_, -fY_, -fZ_);
    }

    Vector Vector::operator * ( const s_float& fValue ) const
    {
        return Vector(fX_*fValue, fY_*fValue, fZ_*fValue);
    }

    Vector Vector::operator / ( const s_float& fValue ) const
    {
        return Vector(fX_/fValue, fY_/fValue, fZ_/fValue);
    }

    void Vector::operator *= ( const s_float& fValue )
    {
        fX_ *= fValue; fY_ *= fValue; fZ_ *= fValue;
    }

    void Vector::operator /= ( const s_float& fValue )
    {
        fX_ /= fValue; fY_ /= fValue; fZ_ /= fValue;
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
        return Vector(fY_*mVec.fZ_ - fZ_*mVec.fY_,
                      fZ_*mVec.fX_ - fX_*mVec.fZ_,
                      fX_*mVec.fY_ - fY_*mVec.fX_);
    }

    void Vector::operator += ( const Vector& mVec )
    {
        fX_ += mVec.fX_; fY_ += mVec.fY_; fZ_ += mVec.fZ_;
    }

    void Vector::operator -= ( const Vector& mVec )
    {
        fX_ -= mVec.fX_; fY_ -= mVec.fY_; fZ_ -= mVec.fZ_;
    }

    s_bool Vector::operator == ( const Vector& mVec ) const
    {
        return ( (fX_ == mVec.fX_) && (fY_ == mVec.fY_) && (fZ_ == mVec.fZ_) );
    }

    s_bool Vector::operator != ( const Vector& mVec ) const
    {
        return ( (fX_ != mVec.fX_) || (fY_ != mVec.fY_) || (fZ_ != mVec.fZ_) );
    }

    Vector& Vector::operator = ( const Vector& mVec )
    {
        fX_ = mVec.fX_; fY_ = mVec.fY_; fZ_ = mVec.fZ_;
        return *this;
    }

    Ogre::Vector3 Vector::FrostToOgre( const Vector &mVector )
    {
        return Ogre::Vector3(
            mVector.X().Get(),
            mVector.Y().Get(),
            mVector.Z().Get()
        );
    }

    Vector Vector::OgreToFrost( const Ogre::Vector3 &mVector )
    {
        return Vector(mVector.x, mVector.y, mVector.z);
    }

    s_str operator + (const string_element* sLeft, const Vector& mRight)
    {
        return s_str(sLeft)+mRight;
    }

    s_str operator + (const s_str& sLeft, const Vector& mRight)
    {
        return sLeft+"("+s_str(mRight.X())+", "+s_str(mRight.Y())+", "+s_str(mRight.Z())+")";
    }

    s_str& operator << (s_str& sLeft, const Vector& mRight)
    {
        return sLeft << "("+s_str(mRight.X())+", "+s_str(mRight.Y())+", "+s_str(mRight.Z())+")";
    }

    Vector operator * (const s_float& fLeft, const Vector& mRight)
    {
        return mRight*fLeft;
    }

    Vector operator * (const Ogre::Matrix4& mLeft, const Vector& mRight)
    {
        Vector mR;

        s_float fInvW = 1.0f /
            (mLeft[3][0]*mRight.X() + mLeft[3][1]*mRight.Y() + mLeft[3][2]*mRight.Z() + mLeft[3][3]);

        mR.X() = (mLeft[0][0]*mRight.X() + mLeft[0][1]*mRight.Y() + mLeft[0][2]*mRight.Z() + mLeft[0][3])*fInvW;
        mR.Y() = (mLeft[1][0]*mRight.X() + mLeft[1][1]*mRight.Y() + mLeft[1][2]*mRight.Z() + mLeft[1][3])*fInvW;
        mR.Z() = (mLeft[2][0]*mRight.X() + mLeft[2][1]*mRight.Y() + mLeft[2][2]*mRight.Z() + mLeft[2][3])*fInvW;

        return mR;
    }
}
