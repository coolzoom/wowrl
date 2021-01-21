/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Node source               */
/*                                        */
/*                                        */

#include "scene/frost_node.h"

using namespace std;

namespace Frost
{
    Node::Node( const Vector& mPos ) : MovableObject(mPos)
    {
    }

    Node::~Node()
    {
    }
}
