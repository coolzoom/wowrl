/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          SceneManager header           */
/*                                        */
/*                                        */

#include "scene/frost_scenemanager.h"
#include "scene/frost_sceneobject.h"
#include "scene/frost_plane.h"
#include "scene/frost_node.h"
#include "scene/frost_gizmo.h"
#include "scene/frost_zonemanager.h"
#include "camera/frost_cameramanager.h"
#include "camera/frost_camera.h"
#include "model/frost_modelmanager.h"
#include "frost_inputmanager.h"

#include <OgreMeshManager.h>
#include <OgreRay.h>
#include <OgreSceneManager.h>

using namespace std;

namespace Frost
{
    const s_str SceneManager::CLASS_NAME = "SceneManager";

    SceneManager::SceneManager()
    {
        Log("Creating "+CLASS_NAME+".");
    }

    SceneManager::~SceneManager()
    {
        Log("Closing "+CLASS_NAME+"...");
        s_map<s_uint, s_ptr<MovableObject> >::iterator iterObject;
        foreach (iterObject, lCreatedObjectList_)
        {
            iterObject->second.Delete();
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

        // Register the GizmoAxis mesh
        ModelManager::GetSingleton()->LinkModelNameToFile("GizmoXAxis", "Models/GizmoXAxis.fm");
        ModelManager::GetSingleton()->LinkModelNameToFile("GizmoYAxis", "Models/GizmoYAxis.fm");
        ModelManager::GetSingleton()->LinkModelNameToFile("GizmoZAxis", "Models/GizmoZAxis.fm");
    }

    s_ptr<Plane> SceneManager::CreatePlane()
    {
        s_ptr<Plane> pPlane = new Plane();
        lCreatedObjectList_[pPlane->GetID()] = pPlane;

        return pPlane;
    }

    s_ptr<Plane> SceneManager::CreatePlane( const s_float& fWidth, const s_float& fHeight, const s_float& fDensity )
    {
        s_ptr<Plane> pPlane = new Plane(fWidth, fHeight, fDensity);
        lCreatedObjectList_[pPlane->GetID()] = pPlane;

        return pPlane;
    }

    void SceneManager::DeletePlane( s_ptr<Plane> pPlane )
    {
        DeleteObject(pPlane);
    }

    s_ptr<Node> SceneManager::CreateNode( const Vector& mPos )
    {
        s_ptr<Node> pNode = new Node(mPos);
        lCreatedObjectList_[pNode->GetID()] = pNode;

        return pNode;
    }

    void SceneManager::DeleteNode( s_ptr<Node> pNode )
    {
        DeleteObject(pNode);
    }

    s_ptr<Gizmo> SceneManager::CreateGizmo( const Vector& mPos )
    {
        s_ptr<Gizmo> pGizmo = new Gizmo(mPos);
        lCreatedObjectList_[pGizmo->GetID()] = pGizmo;

        return pGizmo;
    }

    void SceneManager::DeleteGizmo( s_ptr<Gizmo> pGizmo)
    {
        DeleteObject(pGizmo);
    }

    void SceneManager::DeleteObject( s_ptr<MovableObject> pObject )
    {
        if (pObject)
        {
            s_map< s_uint, s_ptr<MovableObject> >::iterator iterObject;
            iterObject = lCreatedObjectList_.Get(pObject->GetID());

            if (iterObject != lCreatedObjectList_.End())
            {
                if (iterObject->second->GetID() == pObject->GetID())
                {
                    // Everything went fine, delete, erase from map and return
                    iterObject->second.Delete();
                    lCreatedObjectList_.Erase(iterObject);
                    return;
                }
            }

            Warning(CLASS_NAME,
                "Trying to call DeleteObject on a MovableObject that has not been created "
                "by SceneManager (ID:"+pObject->GetID()+")."
            );
        }
    }

    s_ptr<MovableObject> SceneManager::GetMovableObjectByID( const s_uint& uiID ) const
    {
        s_map< s_uint, s_ptr<MovableObject> >::const_iterator iterMovable;
        iterMovable = lRegisteredObjectList_.Get(uiID);

        if (iterMovable != lRegisteredObjectList_.End())
            return iterMovable->second;
        else
            return nullptr;
    }

    s_ptr<SceneObject> SceneManager::GetSceneObjectByID( const s_uint& uiID ) const
    {
        s_map< s_uint, s_ptr<SceneObject> >::const_iterator iterObject;
        iterObject = lSceneObjectList_.Get(uiID);

        if (iterObject != lSceneObjectList_.End())
            return iterObject->second;
        else
            return nullptr;
    }

    const s_uint& SceneManager::RegisterObject( s_ptr<MovableObject> pObject )
    {
        if (pObject->GetID().IsValid())
        {
            lRegisteredObjectList_[pObject->GetID()] = pObject;
            return pObject->GetID();
        }
        else
        {
            ++uiObjectCounter_;
            lRegisteredObjectList_[uiObjectCounter_] = pObject;
            return uiObjectCounter_;
        }
    }

    void SceneManager::RegisterSceneObject( s_ptr<SceneObject> pObject )
    {
        lSceneObjectList_[pObject->GetID()] = pObject;
    }

    void SceneManager::UnregisterObject( s_ptr<MovableObject> pObject )
    {
        if (pObject)
        {
            s_map< s_uint, s_ptr<MovableObject> >::iterator iterObject;
            iterObject = lRegisteredObjectList_.Get(pObject->GetID());

            if (iterObject != lRegisteredObjectList_.End())
            {
                if (iterObject->second->GetID() == pObject->GetID())
                {
                    // Everything went fine, delete, erase from map and return
                    lRegisteredObjectList_.Erase(iterObject);
                    return;
                }
            }

            Warning(CLASS_NAME,
                "Trying to call UnregisterObject on a MovableObject that has not been registered "
                "to SceneManager (ID:"+pObject->GetID()+")."
            );
        }
    }

    void SceneManager::UnregisterSceneObject( s_ptr<SceneObject> pObject )
    {
        if (pObject)
        {
            s_map< s_uint, s_ptr<SceneObject> >::iterator iterObject;
            iterObject = lSceneObjectList_.Get(pObject->GetID());

            if (iterObject != lSceneObjectList_.End())
            {
                if (iterObject->second->GetID() == pObject->GetID())
                {
                    // Everything went fine, delete, erase from map and return
                    lSceneObjectList_.Erase(iterObject);
                    return;
                }
            }

            Warning(CLASS_NAME,
                "Trying to call UnregisterSceneObject on a SceneObject that has not been registered "
                "to SceneManager (ID:"+pObject->GetID()+")."
            );
        }
    }

    void SceneManager::StartMoving( s_ptr<MovableObject> pObject, Vector::Constraint mConstraint, const s_bool& bOnGround )
    {
        if (pObject)
        {
            mCurrentTransformation_ = TRANSFORM_TRANSLATION;
            mTransformationConstraint_ = mConstraint;
            bConstrainOnGround_ = bOnGround;
            pTransformedObject_ = pObject;
        }
        else
        {
            StopAllTransformations();
        }
    }

    void SceneManager::StartSizing( s_ptr<MovableObject> pObject, Vector::Constraint mConstraint )
    {
        if (pObject)
        {
            mCurrentTransformation_ = TRANSFORM_SCALING;
            mTransformationConstraint_ = mConstraint;
            pTransformedObject_ = pObject;
        }
        else
        {
            StopAllTransformations();
        }
    }

    void SceneManager::StartRotating( s_ptr<MovableObject> pObject, Vector::Constraint mConstraint )
    {
        if (pObject)
        {
            mCurrentTransformation_ = TRANSFORM_ROTATION;
            mTransformationConstraint_ = mConstraint;
            pTransformedObject_ = pObject;
        }
        else
        {
            StopAllTransformations();
        }
    }

    void SceneManager::StopAllTransformations()
    {
        mCurrentTransformation_ = TRANSFORM_NONE;
        pTransformedObject_ = nullptr;
    }

    const s_map< s_uint, s_ptr<SceneObject> >& SceneManager::GetSceneObjectList() const
    {
        return lSceneObjectList_;
    }

    const s_map< s_uint, s_ptr<SceneObject> >& SceneManager::GetSelectedSceneObjectList() const
    {
        return lSelectedObjectList_;
    }

    void SceneManager::SelectSceneObject( s_ptr<SceneObject> pObject )
    {
        ClearSelection();
        AddSceneObjectToSelection(pObject);
    }

    void SceneManager::DeselectSceneObject( s_ptr<SceneObject> pObject )
    {
        if (!pObject)
            return;

        if (pMouseOveredObject_ && pMouseOveredObject_->GetSceneObject() == pObject)
            pMouseOveredObject_ = nullptr;

        s_map< s_uint, s_ptr<SceneObject> >::iterator iter = lSelectedObjectList_.Get(pObject->GetID());
        if (iter != lSelectedObjectList_.End())
        {
            iter->second->Select(false);
            lSelectedObjectList_.Erase(iter);
        }

        if (lSelectedObjectList_.GetSize() == 0)
            EventManager::GetSingleton()->FireEvent(Event("NO_OBJECT_SELECTED"));
    }

    void SceneManager::AddSceneObjectToSelection( s_ptr<SceneObject> pObject )
    {
        if (!pObject)
            return;

        if (lSelectedObjectList_.GetSize() == 0)
            EventManager::GetSingleton()->FireEvent(Event("OBJECT_SELECTED"));

        s_map< s_uint, s_ptr<SceneObject> >::iterator iter = lSelectedObjectList_.Get(pObject->GetID());
        if (iter == lSelectedObjectList_.End())
        {
            lSelectedObjectList_[pObject->GetID()] = pObject;
            pObject->Select(true);
        }
    }

    void SceneManager::ClearSelection()
    {
        s_map< s_uint, s_ptr<SceneObject> >::iterator iter;
        foreach (iter, lSelectedObjectList_)
        {
            iter->second->Select(false);
        }

        lSelectedObjectList_.Clear();

        EventManager::GetSingleton()->FireEvent(Event("NO_OBJECT_SELECTED"));
    }

    void SceneManager::Update( const s_float& fDelta )
    {
        s_ptr<InputManager> pInputMgr = InputManager::GetSingleton();
        s_ptr<Engine>       pFrost = Engine::GetSingleton();

        s_ptr<OgreInterface> pNewObject;

        // Handle mouse overing
        if (InputManager::GetSingleton()->CanGroupReceiveClicks("WORLD"))
        {
            Ogre::Ray mRay = CameraManager::GetSingleton()->GetMainCamera()->GetOgreCamera()->getCameraToViewportRay(
                (pInputMgr->GetMousePosX()/s_float(pFrost->GetScreenWidth())).Get(),
                (pInputMgr->GetMousePosY()/s_float(pFrost->GetScreenHeight())).Get()
            );
            s_ptr<Ogre::RaySceneQuery> pRayQuery = pFrost->GetOgreSceneManager()->createRayQuery(mRay);

            pRayQuery->setSortByDistance(true);
            Ogre::RaySceneQueryResult& mRes = pRayQuery->execute();
            Ogre::RaySceneQueryResult::iterator iter = mRes.begin();

            foreach (iter, mRes)
            {
                s_ptr<OgreInterface> pObject = s_ptr<OgreInterface>::DynamicCast(iter->movable->getUserObject());
                if ((pObject && pObject->IsMouseEnabled()) && ((!pNewObject) ||
                    (pObject->GetPriority() > pNewObject->GetPriority())))
                    pNewObject = pObject;
            }

            pFrost->GetOgreSceneManager()->destroyQuery(pRayQuery.Get());
        }

        if (pNewObject)
        {
            if (pMouseOveredObject_ != pNewObject)
            {
                if (pMouseOveredObject_)
                    pMouseOveredObject_->On("Leave");

                pMouseOveredObject_ = pNewObject;

                if (pMouseOveredObject_)
                    pMouseOveredObject_->On("Enter");
            }
        }
        else
        {
            if (pMouseOveredObject_)
                pMouseOveredObject_->On("Leave");

            pMouseOveredObject_ = nullptr;
        }

        // Handle selection
        if (InputManager::GetSingleton()->CanGroupReceiveClicks("WORLD"))
        {
            if (pInputMgr->MouseIsPressed(MOUSE_LEFT))
            {
                if (pMouseOveredObject_)
                {
                    s_ptr<SceneObject> pObj = pMouseOveredObject_->GetSceneObject();
                    if (pObj)
                    {
                        if (pObj->IsSelectable())
                        {
                            if (pInputMgr->ShiftPressed())
                            {
                                if (pObj->IsSelected())
                                    DeselectSceneObject(pObj);
                                else
                                    AddSceneObjectToSelection(pObj);
                            }
                            else
                                SelectSceneObject(pObj);
                        }
                    }

                    pMouseOveredObject_->On("MousePressed");
                    pMouseOveredObject_->On("DragStart");

                    pDraggedObject_ = pMouseOveredObject_;
                }
                else
                {
                    ClearSelection();
                }
            }
            if (pInputMgr->MouseIsReleased(MOUSE_LEFT))
            {
                if (pMouseOveredObject_)
                    pMouseOveredObject_->On("MouseReleased");

                if (pDraggedObject_)
                {
                    pDraggedObject_->On("DragStop");
                    pDraggedObject_ = nullptr;
                }
            }
        }

        if (!pTransformedObject_)
            return;

        switch (mCurrentTransformation_)
        {
            case TRANSFORM_TRANSLATION :
            {
                if (bConstrainOnGround_)
                {
                    Vector mPos = ZoneManager::GetSingleton()->GetTerrainUnderMouse();
                    if (!mPos.IsNaN())
                    {
                        pTransformedObject_->SetPosition(mPos);
                    }
                }
                else
                {

                }

                break;
            }
            case TRANSFORM_SCALING :
            {
                break;
            }
            case TRANSFORM_ROTATION :
            {
                break;
            }
            default : break;
        }
    }
}
