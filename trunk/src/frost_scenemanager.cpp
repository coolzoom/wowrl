/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          SceneManager header           */
/*                                        */
/*                                        */

#include "frost_scenemanager.h"
#include "frost_plane.h"
#include "frost_terrain.h"
#include "frost_node.h"

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
        map<s_uint, s_ptr<Plane> >::iterator iterPlane;
        foreach (iterPlane, lPlaneList_)
        {
            iterPlane->second.Delete();
        }

        map<s_uint, s_ptr<Terrain> >::iterator iterTerrain;
        foreach (iterTerrain, lTerrainList_)
        {
            iterTerrain->second.Delete();
        }

        map<s_uint, s_ptr<Node> >::iterator iterNode;
        foreach (iterNode, lNodeList_)
        {
            iterNode->second.Delete();
        }
    }

    void SceneManager::Initialize()
    {
        // Create the default plane mesh
        Ogre::Plane mPlane(Ogre::Vector3::UNIT_Y, 0);
        Ogre::MeshManager::getSingleton().createPlane(
            "Plane", "Frost", mPlane,
            150, 150, 150, 150, true, 1, 1, 1, Ogre::Vector3::UNIT_Z
        );
    }

    s_ptr<Plane> SceneManager::CreatePlane()
    {
        s_ptr<Plane> pPlane = new Plane(uiPlaneCounter_);
        lPlaneList_[uiPlaneCounter_] = pPlane;
        uiPlaneCounter_++;

        return pPlane;
    }

    s_ptr<Plane> SceneManager::CreatePlane( const s_float& fWidth, const s_float& fHeight, const s_float& fDensity )
    {
        s_ptr<Plane> pPlane = new Plane(uiPlaneCounter_, fWidth, fHeight, fDensity);
        lPlaneList_[uiPlaneCounter_] = pPlane;
        uiPlaneCounter_++;

        return pPlane;
    }

    void SceneManager::DeletePlane(s_ptr<Plane> pPlane)
    {
        if (pPlane)
        {
            map< s_uint, s_ptr<Plane> >::iterator iterPlane;
            iterPlane = lPlaneList_.find(pPlane->GetID());

            if (iterPlane != lPlaneList_.end())
            {
                if (iterPlane->second->GetID() == pPlane->GetID())
                {
                    // Everything went fine, delete, erase from map and return
                    iterPlane->second.Delete();
                    lPlaneList_.erase(iterPlane);
                    return;
                }
            }

            Warning(CLASS_NAME,
                "Trying to call DeletePlane on a Plane that has not been created by SceneManager (ID:"+pPlane->GetID()+")."
            );
        }
    }

    void SceneManager::DeleteTerrain(s_ptr<Terrain> pTerrain)
    {
        if (pTerrain)
        {
            map< s_uint, s_ptr<Terrain> >::iterator iterTerrain;
            iterTerrain = lTerrainList_.find(pTerrain->GetID());

            if (iterTerrain != lTerrainList_.end())
            {
                if (iterTerrain->second->GetID() == pTerrain->GetID())
                {
                    // Everything went fine, delete, erase from map and return
                    iterTerrain->second.Delete();
                    lTerrainList_.erase(iterTerrain);
                    return;
                }
            }

            Warning(CLASS_NAME,
                "Trying to call DeleteTerrain on a Terrain that has not been created by SceneManager (ID:"+pTerrain->GetID()+")."
            );
        }
    }

    s_ptr<Node> SceneManager::CreateNode( const Vector& mPos )
    {
        s_ptr<Node> pNode = new Node(uiNodeCounter_, mPos);
        lNodeList_[uiNodeCounter_] = pNode;
        uiNodeCounter_++;

        return pNode;
    }

    void SceneManager::DeleteNode(s_ptr<Node> pNode)
    {
        if (pNode)
        {
            map< s_uint, s_ptr<Node> >::iterator iterNode;
            iterNode = lNodeList_.find(pNode->GetID());

            if (iterNode != lNodeList_.end())
            {
                if (iterNode->second->GetID() == pNode->GetID())
                {
                    // Everything went fine, delete, erase from map and return
                    iterNode->second.Delete();
                    lNodeList_.erase(iterNode);
                    return;
                }
            }

            Warning(CLASS_NAME,
                "Trying to call DeleteNode on a Node that has not been created by SceneManager (ID:"+pNode->GetID()+")."
            );
        }
    }
}
