/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          SceneManager header           */
/*                                        */
/*                                        */

#include "scene/frost_scenemanager.h"
#include "scene/frost_plane.h"
#include "scene/frost_node.h"

#include <OgreMeshManager.h>

using namespace std;

namespace Frost
{
    const s_str SceneManager::CLASS_NAME = "SceneManager";

    SceneManager::SceneManager()
    {
    }

    SceneManager::~SceneManager()
    {
        Log("Closing "+CLASS_NAME+"...");
        s_map<s_uint, s_ptr<Plane> >::iterator iterPlane;
        foreach (iterPlane, lPlaneList_)
        {
            iterPlane->second.Delete();
        }

        s_map<s_uint, s_ptr<Node> >::iterator iterNode;
        foreach (iterNode, lNodeList_)
        {
            iterNode->second.Delete();
        }
    }

    void SceneManager::Initialize()
    {
        // Create the default plane mesh
        Ogre::Plane mPlane(Ogre::Vector3::UNIT_Y, 0);
        Ogre::MeshManager::getSingletonPtr()->createPlane(
            "Plane", "Frost", mPlane,
            150, 150, 25, 25, true, 1, 1, 1, Ogre::Vector3::UNIT_Z
        );
    }

    s_ptr<Plane> SceneManager::CreatePlane()
    {
        s_ptr<Plane> pPlane = new Plane();
        lPlaneList_[pPlane->GetID()] = pPlane;

        return pPlane;
    }

    s_ptr<Plane> SceneManager::CreatePlane( const s_float& fWidth, const s_float& fHeight, const s_float& fDensity )
    {
        s_ptr<Plane> pPlane = new Plane(fWidth, fHeight, fDensity);
        lPlaneList_[pPlane->GetID()] = pPlane;

        return pPlane;
    }

    s_ptr<MovableObject> SceneManager::GetMovableObjectByID( const s_uint& uiID ) const
    {
        s_map< s_uint, s_ptr<MovableObject> >::const_iterator iterMovable;
        iterMovable = lObjectList_.Get(uiID);

        if (iterMovable != lObjectList_.End())
            return iterMovable->second;
        else
            return nullptr;
    }

    void SceneManager::DeletePlane( s_ptr<Plane> pPlane )
    {
        if (pPlane)
        {
            s_map< s_uint, s_ptr<Plane> >::iterator iterPlane;
            iterPlane = lPlaneList_.Get(pPlane->GetID());

            if (iterPlane != lPlaneList_.End())
            {
                if (iterPlane->second->GetID() == pPlane->GetID())
                {
                    // Everything went fine, delete, erase from map and return
                    iterPlane->second.Delete();
                    lObjectList_.Erase(iterPlane->first);
                    lPlaneList_.Erase(iterPlane);

                    return;
                }
            }

            Warning(CLASS_NAME,
                "Trying to call DeletePlane on a Plane that has not been created "
                "by SceneManager (ID:"+pPlane->GetID()+")."
            );
        }
    }

    s_ptr<Node> SceneManager::CreateNode( const Vector& mPos )
    {
        s_ptr<Node> pNode = new Node(mPos);
        lNodeList_[pNode->GetID()] = pNode;

        return pNode;
    }

    void SceneManager::DeleteNode( s_ptr<Node> pNode )
    {
        if (pNode)
        {
            s_map< s_uint, s_ptr<Node> >::iterator iterNode;
            iterNode = lNodeList_.Get(pNode->GetID());

            if (iterNode != lNodeList_.End())
            {
                if (iterNode->second->GetID() == pNode->GetID())
                {
                    // Everything went fine, delete, erase from map and return
                    iterNode->second.Delete();
                    lObjectList_.Erase(iterNode->first);
                    lNodeList_.Erase(iterNode);

                    return;
                }
            }

            Warning(CLASS_NAME,
                "Trying to call DeleteNode on a Node that has not been created "
                "by SceneManager (ID:"+pNode->GetID()+")."
            );
        }
    }

    const s_uint& SceneManager::GetNewID(s_ptr<MovableObject> pObject)
    {
        uiObjectCounter_++;
        lObjectList_[uiObjectCounter_] = pObject;
        return uiObjectCounter_;
    }
}
