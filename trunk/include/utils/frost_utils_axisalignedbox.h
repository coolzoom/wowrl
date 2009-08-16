/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          AxisAlignedBox header         */
/*                                        */
/*                                        */


#ifndef FROST_AXISALIGNEDBOX_H
#define FROST_AXISALIGNEDBOX_H

#include "frost_utils.h"
#include "frost_utils_vector.h"

namespace Ogre
{
    class AxisAlignedBox;
}

namespace Frost
{
    class AxisAlignedBox
    {
    public :

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

        void SetMin(const Vector& mMin);

        void SetMax(const Vector& mMax);

        const Vector& GetMin() const;

        const Vector& GetMax() const;

        Vector operator[] (const s_uint& uiIndex) const;
        Vector operator[] (const uint& uiIndex) const;

        /// Checks if this box contains another.
        /** \param mBox The other box
        *   \return 'true' if this box contains the other
        *   \note The box can be partially contained.
        */
        s_bool Contains(const AxisAlignedBox& mBox) const;

        /// Checks if this box contains a point.
        /** \param mPoint The point
        *   \return 'true' if this box contains the point
        */
        s_bool Contains(const Vector& mPoint) const;

        /// Converts a Frost::AxisAlignedBox to an Ogre::AxisAlignedBox.
        /** \param mBox The box to convert
        */
        static Ogre::AxisAlignedBox FrostToOgre(const AxisAlignedBox& mBox);

        /// Converts a Ogre::AxisAlignedBox to an Frost::AxisAlignedBox.
        /** \param mBox The box to convert
        */
        static AxisAlignedBox OgreToFrost(const Ogre::AxisAlignedBox& mBox);

        static const s_str CLASS_NAME;

    private :

        Vector mMin_, mMax_;

    };
}

#endif
