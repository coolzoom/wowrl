/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          MovableObject header          */
/*                                        */
/*                                        */

#include "scene/frost_movableobject.h"
#include "scene/frost_scenemanager.h"
#include "frost_engine.h"
#include "path/frost_path.h"

#include <OgreSceneManager.h>
#include <OgreRoot.h>
#include <OgreEntity.h>

using namespace std;

namespace Frost
{
    const s_str MovableObject::CLASS_NAME = "MovableObject";

    MovableObject::MovableObject() : mInitialDirection_(-Vector::UNIT_Z)
    {
        uiID_ = SceneManager::GetSingleton()->RegisterObject(this);
        pNode_ = Engine::GetSingleton()->GetOgreSceneManager()->getRootSceneNode()->createChildSceneNode(
            Ogre::Vector3::ZERO
        );
        pNode_->setFixedYawAxis(true);
    }

    MovableObject::MovableObject( const MovableObject& mObject ) : mInitialDirection_(-Vector::UNIT_Z)
    {
        uiID_ = SceneManager::GetSingleton()->RegisterObject(this);

        mOrbitCenter_ = mObject.mOrbitCenter_;
        mTrackedPoint_ = mObject.mTrackedPoint_;
        bOrbits_ = mObject.bOrbits_;
        bOrbitCenterRelative_ = mObject.bOrbitCenterRelative_;
        bTracks_ = mObject.bTracks_;
        bTrackedPointRelative_ = mObject.bTrackedPointRelative_;

        if (!bOrbits_)
            pNode_ = mObject.pNode_->getParentSceneNode()->createChildSceneNode(mObject.pNode_->getPosition());
        else
        {
            pNode_ = Engine::GetSingleton()->GetOgreSceneManager()->getRootSceneNode()->createChildSceneNode(
                mObject.pNode_->getPosition()
            );
        }
        pNode_->setScale(mObject.pNode_->getScale());
        pNode_->setOrientation(mObject.pNode_->getOrientation());
        pNode_->setFixedYawAxis(true);

        if (bOrbits_)
            CreateOrbitNode_();

        if (mObject.pPathIterator_)
        {
            pPathIterator_ = Path::CreateIterator(mObject.pPathIterator_->GetPath());
        }
    }

    MovableObject::MovableObject( const Vector& mPosition ) : mInitialDirection_(-Vector::UNIT_Z)
    {
        uiID_ = SceneManager::GetSingleton()->RegisterObject(this);
        pNode_ = Engine::GetSingleton()->GetOgreSceneManager()->getRootSceneNode()->createChildSceneNode(
            Vector::FrostToOgre(mPosition)
        );
        pNode_->setFixedYawAxis(true);
    }

    MovableObject::~MovableObject()
    {
        SceneManager::GetSingleton()->UnregisterObject(this);

        UnlockTracking();
        s_ctnr< s_ptr<MovableObject> >::iterator iter = lLookingAtList_.Begin();
        while (iter != lLookingAtList_.End())
        {
            s_ptr<MovableObject> pObj = (*iter);
            iter = lLookingAtList_.Erase(iter);
            pObj->UnlockTracking();
        }

        Engine::GetSingleton()->GetOgreSceneManager()->destroySceneNode(pNode_.Get());

        s_ctnr< s_ptr<LuaMovableObject> >::iterator iterGlue;
        foreach (iterGlue, lGlueList_)
        {
            iterGlue->Delete();
        }
    }

    void MovableObject::SetOgreInterface( s_ptr<OgreInterface> pOgreInterface )
    {
        if (pEntity_)
        {
            pEntity_->setUserObject(pOgreInterface.Get());
        }
    }

    void MovableObject::AttachTo( s_ptr<MovableObject> pObject, const s_bool& bInheritRot, const s_bool& bInheritScale )
    {
        if (pObject)
        {
            pParent_ = pObject;
            if (bOrbits_)
            {
                pTargetNode_->getParent()->removeChild(pTargetNode_.Get());
                pParent_->GetOgreNode()->addChild(pTargetNode_.Get());
                pTargetNode_->setInheritOrientation(bInheritRot.Get());
                pTargetNode_->setInheritScale(bInheritScale.Get());
            }
            else
            {
                pNode_->getParent()->removeChild(pNode_.Get());
                pParent_->GetOgreNode()->addChild(pNode_.Get());
                pNode_->setInheritOrientation(bInheritRot.Get());
                pNode_->setInheritScale(bInheritScale.Get());
            }
            bInherits_ = true;
        }
    }

    void MovableObject::Detach()
    {
        if (pParent_)
        {
            if (bOrbits_)
            {
                pParent_->GetOgreNode()->removeChild(pTargetNode_.Get());
                Engine::GetSingleton()->GetOgreSceneManager()->getRootSceneNode()->addChild(pTargetNode_.Get());
            }
            else
            {
                pParent_->GetOgreNode()->removeChild(pNode_.Get());
                Engine::GetSingleton()->GetOgreSceneManager()->getRootSceneNode()->addChild(pNode_.Get());
            }

            pParent_ = nullptr;
        }

        bInherits_ = false;
    }

    void MovableObject::CreateOrbitNode_()
    {
        Vector mPosition = GetPosition();

        // - Create target orbiting node
        pTargetNode_ = Engine::GetSingleton()->GetOgreSceneManager()->getRootSceneNode()->createChildSceneNode();
        pTargetNode_->setPosition(Vector::FrostToOgre(mOrbitCenter_));
        pTargetNode_->setFixedYawAxis(true);
        pTargetNode_->lookAt(-Vector::FrostToOgre(mPosition), Ogre::SceneNode::TS_WORLD);

        // - Change the main node's parent
        pNode_->getParent()->removeChild(pNode_.Get());
        pTargetNode_->addChild(pNode_.Get());

        pNode_->setPosition(Ogre::Vector3(0, 0, (mPosition - mOrbitCenter_).GetLength().Get()));
        pNode_->setFixedYawAxis(true);
        pNode_->lookAt(Ogre::Vector3(0, 0, 0), Ogre::SceneNode::TS_PARENT);
    }

    void MovableObject::RemoveOrbitNode_()
    {
        pTargetNode_->removeChild(pNode_.Get());
        pTargetNode_->getParent()->addChild(pNode_.Get());
        Engine::GetSingleton()->GetOgreSceneManager()->destroySceneNode(
            pTargetNode_.Get()
        );
    }

    void MovableObject::LookAt( const Vector& mTrackedPoint, const s_bool& bTrackedPointRelative )
    {
        UnlockOrbiting();

        bTrackedPointRelative_ = bTrackedPointRelative;
        mTrackedPoint_ = mTrackedPoint;

        bTracks_ = true;
    }

    void MovableObject::LookAt( s_ptr<MovableObject> pObject )
    {
        UnlockOrbiting();

        if (pLookAtObject_)
            pLookAtObject_->NotifyLookedAt(this, false);

        pLookAtObject_ = pObject;

        if (pLookAtObject_)
        {
            pLookAtObject_->NotifyLookedAt(this, true);
            bTracks_ = true;
        }
        else
            bTracks_ = false;
    }

    void MovableObject::NotifyLookedAt( s_ptr<MovableObject> pObject, const s_bool& bLookingAt )
    {
        if (bLookingAt)
        {
            if (!lLookingAtList_.Find(pObject))
            {
                lLookingAtList_.PushBack(pObject);
            }
        }
        else
        {
            s_ctnr< s_ptr<MovableObject> >::iterator iter = lLookingAtList_.Get(pObject);
            if (iter != lLookingAtList_.End())
            {
                lLookingAtList_.Erase(iter);
            }
        }
    }

    void MovableObject::OrbitAround( const Vector& mOrbitCenter )
    {
        UnlockOrbiting();
        UnlockTracking();

        mOrbitCenter_ = mOrbitCenter;
        bOrbits_ = true;

        CreateOrbitNode_();
    }

    void MovableObject::SetDirection( const Vector& mDirection )
    {
        if (!mDirection.IsNull())
        {
            UnlockOrbiting();
            UnlockTracking();

            pNode_->setDirection(
                Vector::FrostToOgre(mDirection), Ogre::Node::TS_PARENT,
                Vector::FrostToOgre(mInitialDirection_)
            );
        }
    }

    void MovableObject::SetOrientation( const s_float& fX, const s_float& fY, const s_float& fZ, const s_float& fW )
    {
        UnlockOrbiting();
        UnlockTracking();

        pNode_->setOrientation(fW.Get(), fX.Get(), fY.Get(), fZ.Get());
    }

    void MovableObject::SetInitialDirection( const Vector& mInitialDirection )
    {
        mInitialDirection_ = mInitialDirection;
    }

    void MovableObject::SetScale( const Vector& mScale )
    {
        pNode_->setScale(Vector::FrostToOgre(mScale));
    }

    void MovableObject::SetPosition( const Vector& mPosition )
    {
        RemovePath();
        UnlockOrbiting();

        pNode_->setPosition(Vector::FrostToOgre(mPosition));
    }

    s_wptr<Path::Iterator> MovableObject::SetPath( s_wptr<Path> pPath )
    {
        pPathIterator_ = Path::CreateIterator(pPath);
        if (pPathIterator_)
            UnlockOrbiting();

        return pPathIterator_;
    }

    void MovableObject::RemovePath()
    {
        pPathIterator_.SetNull();
    }

    void MovableObject::Translate( const Vector& mTranslation, const s_bool& bLocalSpace )
    {
        RemovePath();

        if (!mTranslation.IsNull())
        {
            if (bLocalSpace)
            {
                pNode_->translate(Vector::FrostToOgre(mTranslation), Ogre::SceneNode::TS_LOCAL);
            }
            else
            {
                pNode_->translate(Vector::FrostToOgre(mTranslation), Ogre::SceneNode::TS_PARENT);
            }
        }
    }

    void MovableObject::Yaw( const s_float& fValue )
    {
        if (bOrbits_)
        {
            RemovePath();

            pTargetNode_->yaw(Ogre::Radian(fValue.GetRad().Get()), Ogre::SceneNode::TS_WORLD);
        }
        else
        {
            UnlockTracking();

            pNode_->yaw(Ogre::Radian(fValue.GetRad().Get()), Ogre::SceneNode::TS_WORLD);
        }
    }

    void MovableObject::Pitch( const s_float& fValue )
    {
        if (bOrbits_)
        {
            RemovePath();

            pTargetNode_->pitch(Ogre::Radian(fValue.GetRad().Get()), Ogre::SceneNode::TS_LOCAL);
        }
        else
        {
            UnlockTracking();

            pNode_->pitch(Ogre::Radian(fValue.GetRad().Get()), Ogre::SceneNode::TS_LOCAL);
        }
    }

    void MovableObject::Roll( const s_float& fValue )
    {
        if (!fValue.IsNull())
        {
            pNode_->roll(Ogre::Radian(fValue.GetRad().Get()), Ogre::SceneNode::TS_LOCAL);
        }
    }

    void MovableObject::UnlockOrbiting()
    {
        if (bOrbits_)
        {
            RemoveOrbitNode_();
            bOrbits_ = false;
        }
    }

    void MovableObject::UnlockTracking()
    {
        bTracks_ = false;
        if (pLookAtObject_)
        {
            pLookAtObject_->NotifyLookedAt(this, false);
            pLookAtObject_ = nullptr;
        }
    }

    const s_bool& MovableObject::IsOrbiting() const
    {
        return bOrbits_;
    }

    const s_bool& MovableObject::IsTracking() const
    {
        return bTracks_;
    }

    Vector MovableObject::Transform( const Vector& mPoint ) const
    {
        Ogre::Quaternion mQuat = pNode_->_getDerivedOrientation();
        return Vector::OgreToFrost(mQuat*Vector::FrostToOgre(mPoint));
    }

    s_ptr<Ogre::SceneNode> MovableObject::GetOgreNode()
    {
        return pNode_;
    }

    Vector MovableObject::GetPosition( const s_bool& bRelative ) const
    {
        if (bRelative)
            return Vector::OgreToFrost(pNode_->getPosition());
        else
            return Vector::OgreToFrost(pNode_->_getDerivedPosition());
    }

    Vector MovableObject::GetScale( const s_bool& bRelative) const
    {
        if (bRelative)
            return Vector::OgreToFrost(pNode_->getScale());
        else
            return Vector::OgreToFrost(pNode_->_getDerivedScale());
    }

    Vector MovableObject::GetDirection( const s_bool& bRelative ) const
    {
        if (bRelative)
            return Vector::OgreToFrost(pNode_->getOrientation() * -Ogre::Vector3::UNIT_Z);
        else
            return Vector::OgreToFrost(pNode_->_getDerivedOrientation() * -Ogre::Vector3::UNIT_Z);
    }

    const Vector& MovableObject::GetInitialDirection() const
    {
        return mInitialDirection_;
    }

    s_wptr<Path> MovableObject::GetPath()
    {
        return pPathIterator_->GetPath();
    }

    s_wptr<Path::Iterator> MovableObject::GetPathIterator()
    {
        return pPathIterator_;
    }

    const Vector& MovableObject::GetTrackedPoint() const
    {
        return mTrackedPoint_;
    }

    const Vector& MovableObject::GetOrbitCenter() const
    {
        return mOrbitCenter_;
    }

    s_ptr<Ogre::SceneNode> MovableObject::GetOrbitNode()
    {
        return pTargetNode_;
    }

    const s_uint& MovableObject::GetID() const
    {
        return uiID_;
    }

    void MovableObject::Update( const s_float& fDelta )
    {
        if (pPathIterator_.IsValid())
        {
            pPathIterator_->Update(fDelta);
            pNode_->setPosition(Vector::FrostToOgre(pPathIterator_->GetPosition()));
        }

        if (bTracks_)
        {
            if (pLookAtObject_)
            {
                pNode_->lookAt(
                    pLookAtObject_->pNode_->_getDerivedPosition(),
                    Ogre::Node::TS_WORLD,
                    Vector::FrostToOgre(mInitialDirection_)
                );
            }
            else
            {
                if (bTrackedPointRelative_)
                {
                    if (pParent_)
                        pNode_->lookAt(Vector::FrostToOgre(mTrackedPoint_), Ogre::Node::TS_PARENT);
                    else
                        pNode_->lookAt(Vector::FrostToOgre(mTrackedPoint_), Ogre::Node::TS_PARENT);
                }
                else
                    pNode_->lookAt(Vector::FrostToOgre(mTrackedPoint_), Ogre::Node::TS_WORLD);
            }
        }
    }

    void MovableObject::OnEvent( const Event& mEvent )
    {

    }

    void MovableObject::CreateGlue( s_ptr<Lua::State> pLua )
    {
        pLua->PushNumber(uiID_);
        lGlueList_.PushBack(
            pLua->Push<LuaMovableObject>(new LuaMovableObject(pLua->GetState()))
        );
        pLua->SetGlobal("Movable_"+uiID_);
    }

    void MovableObject::PushOnLua( s_ptr<Lua::State> pLua ) const
    {
        pLua->PushGlobal("Movable_"+uiID_);
    }

    OgreInterface::OgreInterface()
    {
    }

    OgreInterface::~OgreInterface()
    {
    }

    void OgreInterface::SetMovableObject( s_ptr<MovableObject> pMovableObject )
    {
        pMovableObject_ = pMovableObject;
    }

    s_ptr<MovableObject> OgreInterface::GetMovableObject() const
    {
        return pMovableObject_;
    }

    void OgreInterface::SetPriority( const s_int& iPriority )
    {
        iPriority_ = iPriority;
    }

    const s_int& OgreInterface::GetPriority() const
    {
        return iPriority_;
    }

    s_bool OgreInterface::IsSelectable() const
    {
        return false;
    }

    void OgreInterface::EnableMouse( const s_bool& bEnable )
    {
        bMouseEnabled_ = bEnable;
    }

    const s_bool& OgreInterface::IsMouseEnabled() const
    {
        return bMouseEnabled_;
    }
}
