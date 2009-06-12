/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Node source               */
/*                                        */
/*                                        */

#include "scene/frost_node.h"

#include <OgreSceneNode.h>

using namespace std;

namespace Frost
{
    Node::Node( const Vector& mPos )
    {
        pNode_->setPosition(Vector::FrostToOgre(mPos));
    }

    Node::~Node()
    {
    }
}
