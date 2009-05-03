/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               GFX header               */
/*                                        */
/*                                        */


#ifndef FROST_GFX_H
#define FROST_GFX_H

#include "frost.h"

namespace Frost
{
    namespace GFX
    {
        /// Converts Frost coordinate system to Ogre's
        /** \param[out] fX X coordinate
        *   \param[out] fY Y coordinate
        *   \param[out] fZ Z coordinate
        *   \note (x, y, z) -> (y, z, x)
        */
        void FrostToOgrePosition(s_float& fX, s_float& fY, s_float& fZ);

        /// Converts Ogre coordinate system to Frost's
        /** \param[out] fX X coordinate
        *   \param[out] fY Y coordinate
        *   \param[out] fZ Z coordinate
        *   \note (x, y, z) -> (z, x, y)
        */
        void OgreToFrostPosition(s_float& fX, s_float& fY, s_float& fZ);

        /// Converts a Frost color to an Ogre one.
        /** \param mColor The color to convert
        *   \return The Ogre color
        */
        Ogre::ColourValue FrostToOgreColor(const Color& mColor);
    }
}

#endif
