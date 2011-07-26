/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          AxisAlignedBox header         */
/*                                        */
/*                                        */


#ifndef FROST_AXISALIGNEDBOX_H
#define FROST_AXISALIGNEDBOX_H

#include "frost_utils_types.h"
#include "frost_utils_vector.h"

#ifdef USING_OGRE
namespace Ogre
{
    class AxisAlignedBox;
}
#endif

namespace Frost
{
    /// An axis aligned bounding box (AABB).
    class AxisAlignedBox
    {
    public :

        /// Enumerates all faces of an axis aligned box
        enum Face
        {
            LEFT = 0,
            RIGHT,
            FRONT,
            BACK,
            TOP,
            BOTTOM
        };

        /// Default constructor.
        /** \note The default box is infinite.
        */
        AxisAlignedBox();

        /// Constructor.
        /** \param mMin The first point of the box
        *   \param mMax The second point of the box
        *   \note mMin's coordinates must be inferior to mMax's.
        *         These two points are opposite corners of the box.
        */
        AxisAlignedBox(const Vector& mMin, const Vector& mMax);

        /// Destructor.
        ~AxisAlignedBox();

        /// Sets the first point of the box.
        /** \param mMin The first point of the box
        */
        void          SetMin(const Vector& mMin);

        /// Sets the second point of the box.
        /** \param mMax The second point of the box
        */
        void          SetMax(const Vector& mMax);

        /// Returns the first point of the box.
        /** \return The first point of the box
        */
        const Vector& GetMin() const;

        /// Returns the second point of the box.
        /** \return The second point of the box
        */
        const Vector& GetMax() const;

        /// Checks if this box contains another.
        /** \param mBox The other box
        *   \return 'true' if this box contains the other
        *   \note The box can be partially contained.
        */
        s_bool        Contains(const AxisAlignedBox& mBox) const;

        /// Checks if this box contains a point.
        /** \param mPoint The point
        *   \return 'true' if this box contains the point
        */
        s_bool        Contains(const Vector& mPoint) const;

        /// Enlarges this box to contain the provided point.
        /** \param mPoint The point
        */
        void          Include(const Vector& mPoint);

        /// Checks if this box is infinite in all directions.
        /** \return 'true' if this box is infinite in all directions
        */
        s_bool        IsInfinite() const;

        /// Checks if a ray intersects this box.
        /** \param mRayOrigin         The start point of the ray
        *   \param mRayDirection      The direction on which to cast the ray
        *   \param[out] mIntersection The intersection point
        *   \return 'true' if the ray has intersected the box
        */
        s_bool        GetRayIntersection(const Vector& mRayOrigin, const Vector& mRayDirection, Vector& mIntersection) const;

        /// Checks if a ray intersects this box.
        /** \param mRayOrigin         The start point of the ray
        *   \param mRayDirection      The direction on which to cast the ray
        *   \param[out] mIntersection The intersection point
        *   \param[out] mFace         The face of the box on which relies the intersection
        *   \return 'true' if the ray has intersected the box
        */
        s_bool        GetRayIntersection(const Vector& mRayOrigin, const Vector& mRayDirection, Vector& mIntersection, Face& mFace) const;

        Vector operator[] (const s_uint& uiIndex) const;
        Vector operator[] (const uint& uiIndex) const;

        AxisAlignedBox operator + (const Vector& mAdd) const;
        AxisAlignedBox operator - (const Vector& mAdd) const;

        void operator += (const Vector& mAdd);
        void operator -= (const Vector& mAdd);

        #ifdef USING_OGRE
        /// Converts a Frost::AxisAlignedBox to an Ogre::AxisAlignedBox.
        /** \param mBox The box to convert
        */
        static Ogre::AxisAlignedBox FrostToOgre(const AxisAlignedBox& mBox);

        /// Converts a Ogre::AxisAlignedBox to an Frost::AxisAlignedBox.
        /** \param mBox The box to convert
        */
        static AxisAlignedBox OgreToFrost(const Ogre::AxisAlignedBox& mBox);
        #endif

        static const s_str CLASS_NAME;

    private :

        Vector mMin_, mMax_;

    };

    /** \cond NOT_REMOVE_FROM_DOC
    */
    template<> class StringConverter<string_element, AxisAlignedBox>
    {
    public :

        typedef s_str::string string;

        static string Convert(const AxisAlignedBox& mBox);
    };
    /** \endcond
    */
}

#endif
