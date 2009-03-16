/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Node source               */
/*                                        */
/*                                        */

#include "frost_node.h"

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

    s_ptr<Ogre::SceneNode> Node::GetOgreNode()
    {
        return pNode_;
    }

    const s_uint& Node::GetID()
    {
        return uiID_;
    }
}
