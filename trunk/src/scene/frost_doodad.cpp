/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Doodad source              */
/*                                        */
/*                                        */

#include "scene/frost_doodad.h"

#include "model/frost_modelmanager.h"
#include "model/frost_model.h"
#include "scene/frost_scenemanager.h"
#include "scene/frost_gizmo.h"

#include "scene/frost_zone.h"
#include "scene/frost_physicsmanager.h"

#include <OgreSceneNode.h>

using namespace std;

namespace Frost
{
    const s_str Doodad::CLASS_NAME = "Doodad";

    Doodad::Doodad( const s_str& sName, const s_str& sModelName, s_ptr<Zone> pParent) :
        SceneObject(uiID_, sName), sModelName_(sModelName), pParent_(pParent)
    {
        mOgreInterface_.SetDoodad(this);
        if (Engine::GetSingleton()->GetState() == Engine::STATE_EDITOR)
            mOgreInterface_.EnableMouse(true);

        pModel_ = ModelManager::GetSingleton()->CreateModel("Zone", sModelName, sName_);
        if (pModel_)
        {
            pModel_->AttachTo(this);
            mOgreInterface_.BindEntity(pModel_->GetEntity());
            mOgreInterface_.SetSceneObject(this);
        }

        pNode_->setFixedYawAxis(false);

        bIsSelectable_ = (Engine::GetSingleton()->GetState() == Engine::STATE_EDITOR);
    }

    Doodad::Doodad( const s_uint& uiID, const s_str& sName, const s_str& sModelName, s_ptr<Zone> pParent) :
        MovableObject(uiID), SceneObject(uiID, sName), sModelName_(sModelName), pParent_(pParent)
    {
        mOgreInterface_.SetDoodad(this);
        if (Engine::GetSingleton()->GetState() == Engine::STATE_EDITOR)
            mOgreInterface_.EnableMouse(true);

        pModel_ = ModelManager::GetSingleton()->CreateModel("Zone", sModelName, sName_);
        if (pModel_)
        {
            pModel_->AttachTo(this);
            mOgreInterface_.BindEntity(pModel_->GetEntity());
            mOgreInterface_.SetSceneObject(this);
        }

        pNode_->setFixedYawAxis(false);

        bIsSelectable_ = (Engine::GetSingleton()->GetState() == Engine::STATE_EDITOR);
    }

    Doodad::~Doodad()
    {
    }

    void Doodad::EnableCollisions()
    {
        pModel_->CreateObstacles();

        s_map< s_uint, s_ptr<Obstacle> >::const_iterator iterObs;
        foreach (iterObs, pModel_->GetObstacleList())
        {
            s_ptr<MovableObstacle> pObs = s_ptr<MovableObstacle>::DynamicCast(iterObs->second);
            pObs->SetStatic(true);
        }

        bCollisionsEnabled_ = true;
    }

    const s_bool& Doodad::AreCollisionsEnabled() const
    {
        return bCollisionsEnabled_;
    }

    void Doodad::Show()
    {
        if (bCollisionsEnabled_)
        {
            s_map< s_uint, s_ptr<Obstacle> >::const_iterator iterObs;
            foreach (iterObs, pModel_->GetObstacleList())
            {
                PhysicsManager::GetSingleton()->AddObstacle(iterObs->second);
            }
        }

        bIsShown_ = true;
        if (pModel_)
            pModel_->Show();
    }

    void Doodad::Hide()
    {
        if (bCollisionsEnabled_)
        {
            s_map< s_uint, s_ptr<Obstacle> >::const_iterator iterObs;
            foreach (iterObs, pModel_->GetObstacleList())
            {
                PhysicsManager::GetSingleton()->RemoveObstacle(iterObs->second);
            }
        }

        bIsShown_ = false;
        if (pModel_)
            pModel_->Hide();

        if (pGizmo_)
            pGizmo_->Hide();
    }

    void Doodad::Highlight( const s_bool& bHighlighted )
    {
        if (bHighlighted != bHighlighted_)
        {
            bHighlighted_ = bHighlighted;
            if (bHighlighted_)
                pModel_->Highlight();
            else
                pModel_->Unlight();
        }
    }

    void Doodad::Select( const s_bool& bSelected )
    {
        if (bSelected != bSelected_)
        {
            bSelected_ = bSelected;
            if (bSelected_)
            {
                pModel_->GetOgreNode()->showBoundingBox(true);
                ShowGizmo();
            }
            else
            {
                pModel_->GetOgreNode()->showBoundingBox(false);
                HideGizmo();
            }
        }
    }

    void Doodad::ShowGizmo()
    {
        if (!pGizmo_)
        {
            pGizmo_ = SceneManager::GetSingleton()->CreateGizmo();
            pGizmo_->SetControlledObject(this);
            pGizmo_->SetExtents(pModel_->GetBoundingBox());
        }

        pGizmo_->Show();
    }

    void Doodad::HideGizmo()
    {
        if (pGizmo_)
            pGizmo_->Hide();
    }

    void Doodad::Update( const s_float& fDelta )
    {
        MovableObject::Update(fDelta);

        if (pModel_)
        {
            pModel_->Update(fDelta);

            if (!bObstacleUpdated_)
            {
                s_map< s_uint, s_ptr<Obstacle> >::const_iterator iterObs;
                foreach (iterObs, pModel_->GetObstacleList())
                {
                    s_ptr<MovableObstacle> pObs = s_ptr<MovableObstacle>::DynamicCast(iterObs->second);
                    pObs->ForceUpdate();
                }
                bObstacleUpdated_ = true;
            }
        }

        if (pGizmo_)
            pGizmo_->Update(fDelta);
    }

    s_wptr<Model> Doodad::GetModel()
    {
        return pModel_;
    }

    const s_str& Doodad::GetModelName() const
    {
        return sModelName_;
    }

    s_ptr<Zone> Doodad::GetZone() const
    {
        return pParent_;
    }

    const s_str& Doodad::GetType() const
    {
        return CLASS_NAME;
    }

    s_refptr<EditorAction> Doodad::CreateDeleteAction()
    {
        return s_refptr<EditorAction>(new DoodadDeleteAction(this));
    }

    void DoodadOgreInterface::SetDoodad( s_ptr<Doodad> pDoodad )
    {
        pDoodad_ = pDoodad;
    }

    s_bool DoodadOgreInterface::IsMouseEnabled() const
    {
        return OgreInterface::IsMouseEnabled() && pDoodad_->IsShown();
    }

    void DoodadOgreInterface::On( const s_str& sEvent )
    {
        if (sEvent == "Enter")
            pDoodad_->Highlight(true);
        else if (sEvent == "Leave")
            pDoodad_->Highlight(false);
    }

    DoodadDeleteAction::DoodadDeleteAction( s_ptr<Doodad> pDoodad ) :
        uiObjectID_(pDoodad->GetID()), pZone_(pDoodad->GetZone()),
        sName_(pDoodad->GetName()), sModelName_(pDoodad->GetModelName()),
        bCollisionsEnabled_(pDoodad->AreCollisionsEnabled()), bIsShown_(pDoodad->IsShown()),
        mPosition_(pDoodad->GetPosition(false)), mScale_(pDoodad->GetScale(false)),
        mOrientation_(pDoodad->GetOrientation(false))
    {
    }

    void DoodadDeleteAction::Do()
    {
        pZone_->DeleteDoodad(sName_);
        EventManager::GetSingleton()->FireEvent(Event("OBJECT_REMOVED"));
    }

    void DoodadDeleteAction::Undo()
    {
        s_ptr<Doodad> pDoodad = pZone_->AddDoodad(uiObjectID_, sName_, sModelName_);
        if (bCollisionsEnabled_) pDoodad->EnableCollisions();
        if (bIsShown_) pDoodad->Show(); else pDoodad->Hide();
        pDoodad->SetPosition(mPosition_);
        pDoodad->SetScale(mScale_);
        pDoodad->SetOrientation(mOrientation_);
        EventManager::GetSingleton()->FireEvent(Event("OBJECT_ADDED"));
    }
}
