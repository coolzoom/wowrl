/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          SceneManager header           */
/*                                        */
/*                                        */

#include "scene/frost_scenemanager.h"
#include "scene/frost_plane.h"
#include "scene/frost_node.h"
#include "terrain/frost_terrain.h"

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
            150, 150, 25, 25, true, 1, 1, 1, Ogre::Vector3::UNIT_Z
        );
    }

    s_uint SceneManager::AddDecalOnGround( s_ptr<Decal> pDecal )
    {
        return 0;
    }

    void SceneManager::RemoveDecalFromGround( const s_uint& uiID )
    {
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
        map< s_uint, s_ptr<MovableObject> >::const_iterator iterMovable;
        iterMovable = lObjectList_.find(uiID);

        if (iterMovable != lObjectList_.end())
            return iterMovable->second;
        else
            return NULL;
    }

    void SceneManager::DeletePlane( s_ptr<Plane> pPlane )
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
                    lObjectList_.erase(iterPlane->first);
                    return;
                }
            }

            Warning(CLASS_NAME,
                "Trying to call DeletePlane on a Plane that has not been created by SceneManager (ID:"+pPlane->GetID()+")."
            );
        }
    }

    void SceneManager::DeleteTerrain( s_ptr<Terrain> pTerrain )
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
                    lObjectList_.erase(iterTerrain->first);
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
        s_ptr<Node> pNode = new Node(mPos);
        lNodeList_[pNode->GetID()] = pNode;

        return pNode;
    }

    void SceneManager::DeleteNode( s_ptr<Node> pNode )
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
                    lObjectList_.erase(iterNode->first);
                    return;
                }
            }

            Warning(CLASS_NAME,
                "Trying to call DeleteNode on a Node that has not been created by SceneManager (ID:"+pNode->GetID()+")."
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
