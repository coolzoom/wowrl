/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_VECTOR3_H
#define FROST_UTILS_VECTOR3_H

#include "frost_utils.h"
#include "frost_utils_types.h"

namespace Ogre
{
    class Vector3;
    class Matrix4;
}

namespace Frost
{
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
        /** \param fValue The value to put in all coordinates
        */
        Vector(const s_float& fValue);

        /// Checks if this vector is null (0, 0, 0).
        /** \return 'true' if this vector is null (0, 0, 0)
        */
        s_bool         IsNull() const;

        /// Checks if this vector only contains NaN attributes.
        /** \return 'true' if this vector only contains NaN attributes
        */
        s_bool         IsNaN() const;

        /// Returns the norm of this vector.
        /** \return The norm of this vector
        *   \note GetLength() does exactly the same thing.
        */
        s_float        GetNorm();

        /// Returns the length of this vector.
        /** \return The length of this vector
        *   \note GetNorm() does exactly the same thing.
        */
        s_float        GetLength();

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

        /// Makes this vector invalid.
        void           SetNaN();

        /// Returns a reference to the X coordinate.
        /** \return A reference to the X coordinate
        */
        s_float&       X();

        /// Returns a reference to the Y coordinate.
        /** \return A reference to the Y coordinate
        */
        s_float&       Y();

        /// Returns a reference to the Z coordinate.
        /** \return A reference to the Z coordinate
        */
        s_float&       Z();

        /// Returns a const reference to the X coordinate.
        /** \return A const reference to the X coordinate
        */
        const s_float& X() const;

        /// Returns a const reference to the Y coordinate.
        /** \return A const reference to the Y coordinate
        */
        const s_float& Y() const;

        /// Returns a const reference to the Z coordinate.
        /** \return A const reference to the Z coordinate
        */
        const s_float& Z() const;

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

        s_bool         operator == (const Vector& mVec) const;
        s_bool         operator != (const Vector& mVec) const;

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

        s_float fX_;
        s_float fY_;
        s_float fZ_;
    };

    s_str  operator + (const char* sLeft, const Vector& mRight);
    s_str  operator + (const s_str& sLeft, const Vector& mRight);
    s_str& operator << (s_str& sLeft, const Vector& mRight);
    Vector operator * (const s_float& fLeft, const Vector& mRight);
    Vector operator * (const Ogre::Matrix4& mLeft, const Vector& mRight);
}

#endif
