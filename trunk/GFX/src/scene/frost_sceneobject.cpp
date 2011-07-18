/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           SceneObject header           */
/*                                        */
/*                                        */

#include "scene/frost_sceneobject.h"
#include "scene/frost_scenemanager.h"
#include "frost_engine.h"

#include <OgreEntity.h>

using namespace std;

namespace Frost
{
    const s_str SceneObject::CLASS_NAME = "SceneObject";

    SceneObject::SceneObject( const s_uint& uiID, const s_str& sName ) :
        sName_(sName), uiSceneID_(uiID), bIsShown_(true), bSelected_(false), bIsSelectable_(false)
    {
        SceneManager::GetSingleton()->RegisterSceneObject(this);
    }

    SceneObject::~SceneObject()
    {
        SceneManager::GetSingleton()->UnregisterSceneObject(this);
    }

    const s_str& SceneObject::GetName() const
    {
        return sName_;
    }

    void SceneObject::Hide()
    {
        bIsShown_ = false;
    }

    void SceneObject::Show()
    {
        bIsShown_ = true;
    }

    const s_bool& SceneObject::IsShown() const
    {
        return bIsShown_;
    }

    void SceneObject::Select( const s_bool& bSelected )
    {
        bSelected_ = bSelected;
    }

    const s_bool& SceneObject::IsSelected() const
    {
        return bSelected_;
    }

    const s_bool& SceneObject::IsSelectable() const
    {
        return bIsSelectable_;
    }

    const s_uint& SceneObject::GetID() const
    {
        return uiSceneID_;
    }

    OgreInterface::OgreInterface()
    {
    }

    OgreInterface::~OgreInterface()
    {
    }

    void OgreInterface::BindEntity( s_ptr<Ogre::Entity> pEntity )
    {
        if (pEntity)
            pEntity->setUserObject(this);
    }

    void OgreInterface::SetSceneObject( s_ptr<SceneObject> pSceneObject )
    {
        pSceneObject_ = pSceneObject;
    }

    s_ptr<SceneObject> OgreInterface::GetSceneObject() const
    {
        return pSceneObject_;
    }

    void OgreInterface::SetPriority( const s_int& iPriority )
    {
        iPriority_ = iPriority;
    }

    const s_int& OgreInterface::GetPriority() const
    {
        return iPriority_;
    }

    void OgreInterface::EnableMouse( const s_bool& bEnable )
    {
        bMouseEnabled_ = bEnable;
    }

    s_bool OgreInterface::IsMouseEnabled() const
    {
        return bMouseEnabled_;
    }

    VisibilityAction::VisibilityAction( s_ptr<SceneObject> pObject, const s_bool& bVisible ) :
        uiObjectID_(pObject->GetID()), bVisible_(bVisible)
    {
    }

    void VisibilityAction::Do()
    {
        s_ptr<SceneObject> pObject = SceneManager::GetSingleton()->GetSceneObjectByID(uiObjectID_);
        if (bVisible_)
            pObject->Show();
        else
            pObject->Hide();
    }

    void VisibilityAction::Undo()
    {
        s_ptr<SceneObject> pObject = SceneManager::GetSingleton()->GetSceneObjectByID(uiObjectID_);
        if (bVisible_)
            pObject->Hide();
        else
            pObject->Show();
    }
}
