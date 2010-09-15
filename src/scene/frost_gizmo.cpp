/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Gizmo source              */
/*                                        */
/*                                        */

#include "scene/frost_gizmo.h"

#include "model/frost_model.h"
#include "model/frost_modelmanager.h"
#include "material/frost_materialmanager.h"
#include "material/frost_material.h"
#include "camera/frost_cameramanager.h"
#include "camera/frost_camera.h"
#include "frost_inputmanager.h"

#include <OgreEntity.h>
#include <OgreCamera.h>

using namespace std;

namespace Frost
{
    const s_str Gizmo::CLASS_NAME = "Gizmo";

    const Color Gizmo::X_BASE_COLOR = Color(1.0f, 0.0f, 0.0f);
    const Color Gizmo::Y_BASE_COLOR = Color(0.0f, 1.0f, 0.0f);
    const Color Gizmo::Z_BASE_COLOR = Color(0.0f, 0.0f, 1.0f);

    const Color Gizmo::X_HIGHLIGHT_COLOR = Color(1.0f, 0.4f, 0.4f);
    const Color Gizmo::Y_HIGHLIGHT_COLOR = Color(0.4f, 1.0f, 0.4f);
    const Color Gizmo::Z_HIGHLIGHT_COLOR = Color(0.4f, 0.4f, 1.0f);

    const Color Gizmo::ALPHA_MASK = Color(0.5f, 1.0f, 1.0f, 1.0f);

    Gizmo::Gizmo()
    {
        CreateAxis_();
    }

    Gizmo::Gizmo(const Vector& mPos) : MovableObject(mPos)
    {
        CreateAxis_();
    }

    Gizmo::~Gizmo()
    {
    }

    void Gizmo::Show()
    {
        for (int i = 0; i < 3; ++i)
            lAxisList_[i].pModel->Show();

        bIsShown_ = true;
    }

    void Gizmo::Hide()
    {
        for (int i = 0; i < 3; ++i)
            lAxisList_[i].pModel->Hide();

        bIsShown_ = false;
    }

    const s_bool& Gizmo::IsShown() const
    {
        return bIsShown_;
    }

    void Gizmo::Highlight( const s_char& cAxis )
    {
        switch (cAxis.Get())
        {
            case 'X' :
                lAxisList_[0].pModel->GetMaterial()->SetDefaultPass(0);
                lAxisList_[0].pModel->GetMaterial()->SetDiffuse(X_HIGHLIGHT_COLOR * ALPHA_MASK);
                lAxisList_[0].pModel->GetMaterial()->SetDefaultPass(1);
                lAxisList_[0].pModel->GetMaterial()->SetDiffuse(X_HIGHLIGHT_COLOR);
                break;
            case 'Y' :
                lAxisList_[1].pModel->GetMaterial()->SetDefaultPass(0);
                lAxisList_[1].pModel->GetMaterial()->SetDiffuse(Y_HIGHLIGHT_COLOR * ALPHA_MASK);
                lAxisList_[1].pModel->GetMaterial()->SetDefaultPass(1);
                lAxisList_[1].pModel->GetMaterial()->SetDiffuse(Y_HIGHLIGHT_COLOR);
                break;
            case 'Z' :
                lAxisList_[2].pModel->GetMaterial()->SetDefaultPass(0);
                lAxisList_[2].pModel->GetMaterial()->SetDiffuse(Z_HIGHLIGHT_COLOR * ALPHA_MASK);
                lAxisList_[2].pModel->GetMaterial()->SetDefaultPass(1);
                lAxisList_[2].pModel->GetMaterial()->SetDiffuse(Z_HIGHLIGHT_COLOR);
                break;
            default :
                break;
        }
    }

    void Gizmo::Unlight( const s_char& cAxis )
    {
        switch (cAxis.Get())
        {
            case 'X' :
                lAxisList_[0].pModel->GetMaterial()->SetDefaultPass(0);
                lAxisList_[0].pModel->GetMaterial()->SetDiffuse(X_BASE_COLOR * ALPHA_MASK);
                lAxisList_[0].pModel->GetMaterial()->SetDefaultPass(1);
                lAxisList_[0].pModel->GetMaterial()->SetDiffuse(X_BASE_COLOR);
                break;
            case 'Y' :
                lAxisList_[1].pModel->GetMaterial()->SetDefaultPass(0);
                lAxisList_[1].pModel->GetMaterial()->SetDiffuse(Y_BASE_COLOR * ALPHA_MASK);
                lAxisList_[1].pModel->GetMaterial()->SetDefaultPass(1);
                lAxisList_[1].pModel->GetMaterial()->SetDiffuse(Y_BASE_COLOR);
                break;
            case 'Z' :
                lAxisList_[2].pModel->GetMaterial()->SetDefaultPass(0);
                lAxisList_[2].pModel->GetMaterial()->SetDiffuse(Z_BASE_COLOR * ALPHA_MASK);
                lAxisList_[2].pModel->GetMaterial()->SetDefaultPass(1);
                lAxisList_[2].pModel->GetMaterial()->SetDiffuse(Z_BASE_COLOR);
                break;
            default :
                break;
        }
    }

    void Gizmo::SetTool( const Tool& mTool )
    {
        mTool_ = mTool;
    }

    const Gizmo::Tool& Gizmo::GetTool() const
    {
        return mTool_;
    }

    void Gizmo::SetControlledObject( s_ptr<MovableObject> pObj )
    {
        pControlledObject_ = pObj;

        if (pControlledObject_)
        {
            AttachTo(pControlledObject_);
            s_ptr<Ogre::Entity> pEntity = pObj->GetOgreEntity();
            if (pEntity)
            {
                s_ptr<Ogre::Mesh> pMesh = pEntity->getMesh().get();
                if (pMesh)
                {
                    AxisAlignedBox mBox = AxisAlignedBox::OgreToFrost(pMesh->getBounds());
                    if (!mBox.IsInfinite())
                    {
                        Vector mSize = mBox.GetMax() - mBox.GetMin();
                        s_float fScale = s_float::Min(mSize.X(), s_float::Min(mSize.Y(), mSize.Z()));
                        SetScale(fScale*Vector::UNIT);
                    }
                }
            }
        }
    }

    s_ptr<MovableObject> Gizmo::GetControlledObject()
    {
        return pControlledObject_;
    }

    void Gizmo::StartDrag( const s_char& cAxis )
    {
        switch (cAxis.Get())
        {
            case 'X' :
                lAxisList_[0].bDragged = true;
                lAxisList_[0].pDraggedPlane = nullptr;
                break;
            case 'Y' :
                lAxisList_[1].bDragged = true;
                lAxisList_[1].pDraggedPlane = nullptr;
                break;
            case 'Z' :
                lAxisList_[2].bDragged = true;
                lAxisList_[2].pDraggedPlane = nullptr;
                break;
            default :
                break;
        }
    }

    void Gizmo::StopDrag( const s_char& cAxis )
    {
        switch (cAxis.Get())
        {
            case 'X' :
                lAxisList_[0].bDragged = false;
                break;
            case 'Y' :
                lAxisList_[1].bDragged = false;
                break;
            case 'Z' :
                lAxisList_[2].bDragged = false;
                break;
            default :
                break;
        }
    }

    void Gizmo::Update( const s_float& fDelta )
    {
        MovableObject::Update(fDelta);

        for (s_uint i = 0; i < 3; ++i)
        {
            lAxisList_[i].lDragPlaneList[0]->Update(fDelta);
            lAxisList_[i].lDragPlaneList[1]->Update(fDelta);
        }

        switch (mTool_)
        {
            case TOOL_MOVE :
            {
                if (!pControlledObject_)
                    break;

                s_ctnr< s_ptr<Axis> > lDraggedList;
                for (s_uint i = 0; i < 3; ++i)
                {
                    if (lAxisList_[i].bDragged)
                        lDraggedList.PushBack(&lAxisList_[i]);
                }

                if (lDraggedList.GetSize() == 1)
                {
                    s_ptr<Axis> pAxis = lDraggedList.Back();

                    // Cast a ray from the mouse
                    s_ptr<Camera> pMainCam = CameraManager::GetSingleton()->GetMainCamera();
                    Ogre::Ray mRay = pMainCam->GetOgreCamera()->getCameraToViewportRay(
                        (InputManager::GetSingleton()->GetMousePosX()/s_float(Engine::GetSingleton()->GetScreenWidth())).Get(),
                        (InputManager::GetSingleton()->GetMousePosY()/s_float(Engine::GetSingleton()->GetScreenHeight())).Get()
                    );

                    Vector mRayOrigin    = Vector::OgreToFrost(mRay.getOrigin());
                    Vector mRayDirection = Vector::OgreToFrost(mRay.getDirection());

                    if (!pAxis->pDraggedPlane)
                    {
                        // Choose which drag plane to use
                        s_float fDot1 = fabs(mRayDirection*pAxis->lDragPlaneList[0]->GetDirection());
                        s_float fDot2 = fabs(mRayDirection*pAxis->lDragPlaneList[1]->GetDirection());

                        // Pick the one which is most facing the camera
                        pAxis->pDraggedPlane = (fDot1 > fDot2) ? pAxis->lDragPlaneList[0] : pAxis->lDragPlaneList[1];

                        // Store the beginning dragging position
                        pAxis->pDraggedPlane->GetRayIntersection(mRayOrigin, mRayDirection, pAxis->mOldDragPos);
                    }
                    else
                    {
                        // Get the intersection of the ray and the drag plane
                        Vector mPos;
                        if (pAxis->pDraggedPlane->GetRayIntersection(mRayOrigin, mRayDirection, mPos))
                        {
                            Vector mMovement = mPos - pAxis->mOldDragPos;

                            if (pAxis->uiID == 0)
                            {
                                mMovement.Y() = 0.0f;
                                mMovement.Z() = 0.0f;
                            }
                            else if (pAxis->uiID == 1)
                            {
                                mMovement.Z() = 0.0f;
                                mMovement.X() = 0.0f;
                            }
                            else if (pAxis->uiID == 2)
                            {
                                mMovement.X() = 0.0f;
                                mMovement.Y() = 0.0f;
                            }

                            // Move the object
                            pControlledObject_->Translate(mMovement);
                            pAxis->mOldDragPos = mPos;
                        }
                    }
                }

                break;
            }
            default : break;
        }
    }

    void Gizmo::CreateAxis_()
    {
        Color mSelfIllum = Color::WHITE;

        lAxisList_[0].uiID = 0;
        lAxisList_[0].pModel = ModelManager::GetSingleton()->CreateModel("GizmoXAxis", "Gizmo"+uiID_+"XAxis");
        lAxisList_[0].pModel->GetEntity()->setRenderQueueGroup(Ogre::RENDER_QUEUE_MAX);
        lAxisList_[0].pModel->AttachTo(this);
        s_refptr<Material> pXMat = MaterialManager::GetSingleton()->CreateMaterial3D();
            pXMat->SetDiffuse(X_BASE_COLOR * ALPHA_MASK);
            pXMat->SetSelfIllumination(mSelfIllum);
            pXMat->SetShaders("SimpleColor");
            pXMat->SetDepthCheck(false);
            pXMat->SetDepthWrite(false);
        pXMat->CreatePass();
        pXMat->SetDefaultPass(1);
            pXMat->SetDiffuse(X_BASE_COLOR);
            pXMat->SetSelfIllumination(mSelfIllum);
            pXMat->SetShaders("SimpleColor");
        lAxisList_[0].pModel->SetMaterial(pXMat);
        lAxisList_[0].mOgreInterface.SetGizmo(this, 'X');
        lAxisList_[0].mOgreInterface.EnableMouse(true);
        lAxisList_[0].mOgreInterface.SetPriority(1);
        lAxisList_[0].pModel->SetOgreInterface(&lAxisList_[0].mOgreInterface);
        lAxisList_[0].lDragPlaneList[0] = s_refptr<PlaneObstacle>(new PlaneObstacle());
        lAxisList_[0].lDragPlaneList[0]->SetDirection(Vector::UNIT_Y);
        lAxisList_[0].lDragPlaneList[0]->AttachTo(this);
        lAxisList_[0].lDragPlaneList[1] = s_refptr<PlaneObstacle>(new PlaneObstacle());
        lAxisList_[0].lDragPlaneList[1]->SetDirection(Vector::UNIT_Z);
        lAxisList_[0].lDragPlaneList[1]->AttachTo(this);

        lAxisList_[1].uiID = 1;
        lAxisList_[1].pModel = ModelManager::GetSingleton()->CreateModel("GizmoYAxis", "Gizmo"+uiID_+"YAxis");
        lAxisList_[1].pModel->GetEntity()->setRenderQueueGroup(Ogre::RENDER_QUEUE_MAX);
        lAxisList_[1].pModel->AttachTo(this);
        s_refptr<Material> pYMat = MaterialManager::GetSingleton()->CreateMaterial3D();
            pYMat->SetDiffuse(Y_BASE_COLOR * ALPHA_MASK);
            pYMat->SetSelfIllumination(mSelfIllum);
            pYMat->SetShaders("SimpleColor");
            pYMat->SetDepthCheck(false);
            pYMat->SetDepthWrite(false);
        pYMat->CreatePass();
        pYMat->SetDefaultPass(1);
            pYMat->SetDiffuse(Y_BASE_COLOR);
            pYMat->SetSelfIllumination(mSelfIllum);
            pYMat->SetShaders("SimpleColor");
        lAxisList_[1].pModel->SetMaterial(pYMat);
        lAxisList_[1].mOgreInterface.SetGizmo(this, 'Y');
        lAxisList_[1].mOgreInterface.EnableMouse(true);
        lAxisList_[1].mOgreInterface.SetPriority(1);
        lAxisList_[1].pModel->SetOgreInterface(&lAxisList_[1].mOgreInterface);
        lAxisList_[1].lDragPlaneList[0] = s_refptr<PlaneObstacle>(new PlaneObstacle());
        lAxisList_[1].lDragPlaneList[0]->SetDirection(Vector::UNIT_Z);
        lAxisList_[1].lDragPlaneList[0]->AttachTo(this);
        lAxisList_[1].lDragPlaneList[1] = s_refptr<PlaneObstacle>(new PlaneObstacle());
        lAxisList_[1].lDragPlaneList[1]->SetDirection(Vector::UNIT_X);
        lAxisList_[1].lDragPlaneList[1]->AttachTo(this);

        lAxisList_[2].uiID = 2;
        lAxisList_[2].pModel = ModelManager::GetSingleton()->CreateModel("GizmoZAxis", "Gizmo"+uiID_+"ZAxis");
        lAxisList_[2].pModel->GetEntity()->setRenderQueueGroup(Ogre::RENDER_QUEUE_MAX);
        lAxisList_[2].pModel->AttachTo(this);
        s_refptr<Material> pZMat = MaterialManager::GetSingleton()->CreateMaterial3D();
            pZMat->SetDiffuse(Z_BASE_COLOR * ALPHA_MASK);
            pZMat->SetSelfIllumination(mSelfIllum);
            pZMat->SetShaders("SimpleColor");
            pZMat->SetDepthCheck(false);
            pZMat->SetDepthWrite(false);
        pZMat->CreatePass();
        pZMat->SetDefaultPass(1);
            pZMat->SetDiffuse(Z_BASE_COLOR);
            pZMat->SetSelfIllumination(mSelfIllum);
            pZMat->SetShaders("SimpleColor");
        lAxisList_[2].pModel->SetMaterial(pZMat);
        lAxisList_[2].mOgreInterface.SetGizmo(this, 'Z');
        lAxisList_[2].mOgreInterface.EnableMouse(true);
        lAxisList_[2].mOgreInterface.SetPriority(1);
        lAxisList_[2].pModel->SetOgreInterface(&lAxisList_[2].mOgreInterface);
        lAxisList_[2].lDragPlaneList[0] = s_refptr<PlaneObstacle>(new PlaneObstacle());
        lAxisList_[2].lDragPlaneList[0]->SetDirection(Vector::UNIT_X);
        lAxisList_[2].lDragPlaneList[0]->AttachTo(this);
        lAxisList_[2].lDragPlaneList[1] = s_refptr<PlaneObstacle>(new PlaneObstacle());
        lAxisList_[2].lDragPlaneList[1]->SetDirection(Vector::UNIT_Y);
        lAxisList_[2].lDragPlaneList[1]->AttachTo(this);
    }

    void GizmoOgreInterface::SetGizmo( s_ptr<Gizmo> pGizmo, const s_char& cAxis )
    {
        pGizmo_ = pGizmo;
        cAxis_ = cAxis;
    }

    void GizmoOgreInterface::On( const s_str& sEvent )
    {
        if (sEvent == "Enter")
            pGizmo_->Highlight(cAxis_);
        if (sEvent == "Leave")
            pGizmo_->Unlight(cAxis_);
        if (sEvent == "DragStart")
            pGizmo_->StartDrag(cAxis_);
        if (sEvent == "DragStop")
            pGizmo_->StopDrag(cAxis_);
    }

    s_bool GizmoOgreInterface::IsMouseEnabled() const
    {
        return OgreInterface::IsMouseEnabled() && pGizmo_->IsShown();
    }
}
