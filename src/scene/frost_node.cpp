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
    Node::Node( const s_uint& uiID, const Vector& mPos )
    {
        uiID_ = uiID;
        pNode_->setPosition(Vector::FrostToOgre(mPos));
    }

    Node::~Node()
    {
    }

    const s_uint& Node::GetID()
    {
        return uiID_;
    }
}
