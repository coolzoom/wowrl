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

#include "scene/frost_physicsmanager.h"

#include <OgreSceneNode.h>

using namespace std;

namespace Frost
{
    const s_str Doodad::CLASS_NAME = "Doodad";

    Doodad::Doodad(const s_str& sName, const s_str& sModelName, s_ptr<Zone> pParent) :
        sName_(sName), sModelName_(sModelName), pParent_(pParent), bIsShown_(true)
    {
        mOgreInterface_.SetDoodad(this);
        if (Engine::GetSingleton()->GetState() == Engine::STATE_EDITOR)
            mOgreInterface_.EnableMouse(true);

        pModel_ = ModelManager::GetSingleton()->CreateModel("Zone", sModelName, sName_);
        if (pModel_)
        {
            pModel_->AttachTo(this);
            pModel_->SetOgreInterface(&mOgreInterface_);
            pEntity_ = pModel_->GetEntity();
        }

        pNode_->setFixedYawAxis(false);
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

    const s_bool& Doodad::IsShown()
    {
        return bIsShown_;
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

    const s_str& Doodad::GetName() const
    {
        return sName_;
    }

    const s_str& Doodad::GetModelName() const
    {
        return sModelName_;
    }

    void DoodadOgreInterface::SetDoodad( s_ptr<Doodad> pDoodad )
    {
        pDoodad_ = pDoodad;
    }

    s_bool DoodadOgreInterface::IsSelectable() const
    {
        return Engine::GetSingleton()->GetState() == Engine::STATE_EDITOR;
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
        else if (sEvent == "Selected")
            pDoodad_->Select(true);
        else if (sEvent == "Deselected")
            pDoodad_->Select(false);
    }
}
