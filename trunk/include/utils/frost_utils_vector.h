/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_VECTOR3_H
#define FROST_UTILS_VECTOR3_H

#include "frost_utils.h"
#include "frost_utils_float.h"
#include "frost_utils_bool.h"

namespace Ogre
{
    class Vector3;
}

namespace Frost
{
    class s_float;
    class s_bool;

    /// A simple 3d coordinate storage.
    /** This class is used to store a three dimensions
    *   set of coordinates (x, y, z). It provides a few
    *   basic operations (+, -, (in)equality).<br>
    *   Value type is float.
    */
    class Vector
    {
    public :

        /// Default constructor.
        Vector();

        /// Constructor.
        /** \param fX The X coordinate
        *   \param fY The Y coordinate
        *   \param fZ The Z coordinate
        */
        Vector(const s_float& fX, const s_float& fY, const s_float& fZ);

        /// Constructor.
        /** \param fX    The X coordinate
        *   \param fY    The Y coordinate
        *   \param fZ    The Z coordinate
        *   \param fNorm The pre-calculated norm
        */
        Vector(const s_float& fX, const s_float& fY, const s_float& fZ, const s_float& fNorm);

        /// Constructor.
        /** \param fValue The value to put in all coordinates
        */
        Vector(const s_float& fValue);

        /// Checks if this vector is null (0, 0, 0).
        /** \return 'true' if this vector is null (0, 0, 0)
        */
        s_bool         IsNull() const;

        /// Returns the norm of this vector.
        /** \return The norm of this vector
        *   \note GetLenght() does exactly the same thing.
        */
        const s_float& GetNorm();

        /// Returns the lenght of this vector.
        /** \return The lenght of this vector
        *   \note GetNorm() does exactly the same thing.
        */
        const s_float& GetLenght();

        /// Returns this vector divided by its norm.
        /** \return This vector divided by its norm
        */
        Vector         GetUnit();

        /// Divides this vector by its length.
        void           Normalize();

        /// Rotates this vector around one of its axis.
        /** \param fAngle The angle to apply
        *   \param mAxis  The axis around which you want to rotate
        *   \note Uses Ogre::Quaternion.
        */
        void           Rotate(const s_float& fAngle, const Vector& mAxis);

        /// Returns a const reference to the X coordinate.
        /** \return A const reference to the X coordinate
        */
        const s_float& X() const;

        /// Returns a reference to the X coordinate.
        /** \return A reference to the X coordinate
        */
        s_float&       XR();

        /// Sets the X coordinate.
        /** \param fX The new X coordinate
        */
        void           X(const s_float& fX);

        /// Returns a const reference to the Y coordinate.
        /** \return A const reference to the Y coordinate
        */
        const s_float& Y() const;

        /// Returns a reference to the Y coordinate.
        /** \return A reference to the Y coordinate
        */
        s_float&       YR();

        /// Sets the Y coordinate.
        /** \param fY The new Y coordinate
        */
        void           Y(const s_float& fY);

        /// Returns a const reference to the Z coordinate.
        /** \return A const reference to the Z coordinate
        */
        const s_float& Z() const;

        /// Returns a reference to the Z coordinate.
        /** \return A reference to the Z coordinate
        */
        s_float&       ZR();

        /// Sets the Z coordinate.
        /** \param fZ The new Z coordinate
        */
        void           Z(const s_float& fZ);

        bool           operator !  () const;
        typedef        void (Vector::*MemberFn)();
        operator       MemberFn() const;

        Vector         operator -  () const;

        Vector         operator *  (const s_float& fValue) const;
        Vector         operator /  (const s_float& fValue) const;
        void           operator *= (const s_float& fValue);
        void           operator /= (const s_float& fValue);

        Vector         operator +  (const Vector& mVec) const;
        Vector         operator -  (const Vector& mVec) const;
        s_float        operator *  (const Vector& mVec) const;
        Vector         operator ^  (const Vector& mVec) const;
        void           operator += (const Vector& mVec);
        void           operator -= (const Vector& mVec);

        bool           operator == (const Vector& mVec) const;
        bool           operator != (const Vector& mVec) const;

        Vector&        operator =  (const Vector& mVec);

        /// (nan, nan, nan)
        static const Vector NaN;
        /// (0,0,0)
        static const Vector ZERO;
        /// (1,1,1)
        static const Vector UNIT;
        /// (1,0,0)
        static const Vector UNIT_X;
        /// (0,1,0)
        static const Vector UNIT_Y;
        /// (0,0,1)
        static const Vector UNIT_Z;
        /// (sqrt(2),sqrt(2),0)
        static const Vector UNIT_XY;
        /// (sqrt(2),0,sqrt(2))
        static const Vector UNIT_XZ;
        /// (0,sqrt(2),sqrt(2))
        static const Vector UNIT_YZ;
        /// (sqrt(3),sqrt(3),sqrt(3))
        static const Vector UNIT_XYZ;

        static const s_str CLASS_NAME;

        /// Converts a Frost vector to an Ogre one.
        /** \param mVector The vector to convert
        *   \return The Ogre vector
        *   \note Also adjusts coordinates.
        */
        static Ogre::Vector3 FrostToOgre(const Vector& mVector);

        /// Converts an Ogre vector to a Frost one.
        /** \param mVector The vector to convert
        *   \return The Frost vector
        *   \note Also adjusts coordinates.
        */
        static Vector OgreToFrost(const Ogre::Vector3& mVector);

    private :

        void UpdateNorm_();

        s_float fX_;
        s_float fY_;
        s_float fZ_;

        s_float fNorm_;
        s_bool  bRebuildNorm_;
    };

    s_str  operator + (const char* sLeft, const Vector& mRight);
    s_str  operator + (const s_str& sLeft, const Vector& mRight);
    s_str& operator << (s_str& sLeft, const Vector& mRight);
    Vector operator * (const s_float& fLeft, const Vector& mRight);
;
}

#endif
