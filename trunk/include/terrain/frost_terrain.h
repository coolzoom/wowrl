/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Terrain header             */
/*                                        */
/*                                        */


#ifndef FROST_TERRAIN_H
#define FROST_TERRAIN_H

#include "frost.h"

namespace Frost
{
    /// A deformed plane mesh
    class Terrain
    {
    public :

        /// Default constructor.
        /** \param uiID The unique ID to give to that Terrain
        *   \note You shouldn't have to call this. Use the
        *         SceneManager instead.
        */
        Terrain(const s_uint& uiID);

        /// Destructor
        ~Terrain();

        /// Returns this Terrain's unique ID.
        /** \return This Terrain's unique ID
        */
        const s_uint& GetID() const;

        static const s_str CLASS_NAME;

    private :

        s_uint                       uiID_;
        s_ptr<Ogre::Entity>         pEntity_;
        s_ptr<Ogre::StaticGeometry> pStatic_;

    };
}

#endif
