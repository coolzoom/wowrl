/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               XXX header               */
/*                                        */
/*                                        */


#ifndef FROST_NODE_H
#define FROST_NODE_H

#include "frost.h"
#include "frost_movableobject.h"

namespace Frost
{
    /// An abstract point in the space
    /** This class is used to position world objects
    *   accurately and easilly.
    */
    class Node : public MovableObject
    {
    public :

        /// Constructor.
        /** \note You shouldn't have to call this. Use the
        *         SceneManager instead.
        */
        Node(const s_uint& uiID, const Vector& mPos);

        /// Destructor.
        ~Node();

        /// Returns the associated Ogre::SceneNode.
        /** \return The associated Ogre::SceneNode
        */
        s_ptr<Ogre::SceneNode> GetOgreNode();

        /// Returns this camera's ID.
        /** \return This camera's ID
        */
        const s_uint& GetID();

        static const s_str CLASS_NAME;

    private :

        s_uint uiID_;
    };
}

#endif
