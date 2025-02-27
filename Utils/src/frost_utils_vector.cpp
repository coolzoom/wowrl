/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_vector.h"
#include "frost_utils_math.h"

#ifdef USING_OGRE
#include <OgreQuaternion.h>
#include <OgreVector3.h>
#include <OgreMatrix4.h>
#endif

#define ISQRT_2 0.7071067811865475244008443621048
#define ISQRT_3 0.5773502691896257645091487805019

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

    s_bool Vector::IsInfinite() const
    {
        return (fX_.IsInfinite() || fY_.IsInfinite() || fZ_.IsInfinite());
    }

    s_float Vector::GetNorm() const
    {
        return sqrt(fX_*fX_ + fY_*fY_ + fZ_*fZ_);
    }

    s_float Vector::GetLength() const
    {
        return sqrt(fX_*fX_ + fY_*fY_ + fZ_*fZ_);
    }

    s_float Vector::GetNormSquared() const
    {
        return fX_*fX_ + fY_*fY_ + fZ_*fZ_;
    }

    s_float Vector::GetLengthSquared() const
    {
        return fX_*fX_ + fY_*fY_ + fZ_*fZ_;
    }

    Vector Vector::GetUnit() const
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

    Vector Vector::ScaleUp( const Vector& mV, const Vector& mScale )
    {
        return Vector(mV.fX_*mScale.fX_, mV.fY_*mScale.fY_, mV.fZ_*mScale.fZ_);
    }

    Vector Vector::ScaleDown( const Vector& mV, const Vector& mScale )
    {
        return Vector(mV.fX_/mScale.fX_, mV.fY_/mScale.fY_, mV.fZ_/mScale.fZ_);
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

    #ifdef USING_OGRE
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
    #endif

    Vector operator * (const s_float& fLeft, const Vector& mRight)
    {
        return mRight*fLeft;
    }

    #ifdef USING_OGRE
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

    Vector operator * (const Ogre::Quaternion& mLeft, const Vector& mRight)
    {
        // Code taken from Ogre3D.
		Vector uv, uuv;
		Vector qvec(mLeft.x, mLeft.y, mLeft.z);
		uv = qvec ^ mRight;
		uuv = qvec ^ uv;
		uv *= (2.0f * mLeft.w);
		uuv *= 2.0f;

		return mRight + uv + uuv;
    }
    #endif

    s_str::string StringConverter<string_element, Vector>::Convert( const Vector& mVector )
    {
        return "("+StringConverter<string_element, s_float>::Convert(mVector.X())+
              ", "+StringConverter<string_element, s_float>::Convert(mVector.Y())+
              ", "+StringConverter<string_element, s_float>::Convert(mVector.Z())+")";
    }
}
