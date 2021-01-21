/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               XXX header               */
/*                                        */
/*                                        */


#ifndef FROST_NODE_H
#define FROST_NODE_H

#include "frost.h"
#include "scene/frost_movableobject.h"

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
        Node(const Vector& mPos);

        /// Destructor.
        ~Node();

        static const s_str CLASS_NAME;

    private :

    };
}

#endif
