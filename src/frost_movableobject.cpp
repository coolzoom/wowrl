/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          MovableObject header          */
/*                                        */
/*                                        */

#include "frost_movableobject.h"
#include "frost_engine.h"
#include "frost_gfx.h"
#include "frost_pathmanager.h"
#include "frost_path.h"

#include <OgreSceneManager.h>
#include <OgreRoot.h>

using namespace std;

namespace Frost
{
    const s_str MovableObject::CLASS_NAME = "MovableObject";

    MovableObject::MovableObject()
    {
        pNode_ = Ogre::Root::getSingletonPtr()->getSceneManager("FrostSceneMgr")->getRootSceneNode()->createChildSceneNode(
            Ogre::Vector3::ZERO
        );
        //pNode_->setDirection(Ogre::Vector3::UNIT_X);
        pNode_->setFixedYawAxis(true);
    }

    MovableObject::MovableObject( const MovableObject& mObject )
    {
        if (!bOrbits_)
            pNode_ = mObject.pNode_->getParentSceneNode()->createChildSceneNode(mObject.pNode_->getPosition());
        else
        {
            pNode_ = Ogre::Root::getSingletonPtr()->getSceneManager("FrostSceneMgr")->getRootSceneNode()->createChildSceneNode(
                mObject.pNode_->getPosition()
            );
        }
        pNode_->setScale(mObject.pNode_->getScale());
        pNode_->setOrientation(mObject.pNode_->getOrientation());
        pNode_->setFixedYawAxis(true);

        mOrbitCenter_ = mObject.mOrbitCenter_;
        mTrackedPoint_ = mObject.mTrackedPoint_;
        bOrbits_ = mObject.bOrbits_;
        bOrbitCenterRelative_ = mObject.bOrbitCenterRelative_;
        bTracks_ = mObject.bTracks_;
        bTrackedPointRelative_ = mObject.bTrackedPointRelative_;

        if (bOrbits_)
            CreateOrbitNode_();

        if (mObject.pPath_)
        {
            pPath_ = PathManager::GetSingleton()->CopyPath(mObject.pPath_);
        }
    }

    MovableObject::MovableObject( const Vector& mPosition )
    {
        pNode_ = Ogre::Root::getSingletonPtr()->getSceneManager("FrostSceneMgr")->getRootSceneNode()->createChildSceneNode(
            Vector::FrostToOgre(mPosition)
        );
        //pNode_->setDirection(Ogre::Vector3::UNIT_X);
        pNode_->setFixedYawAxis(true);
    }

    MovableObject::~MovableObject()
    {
        Ogre::Root::getSingletonPtr()->getSceneManager("FrostSceneMgr")->destroySceneNode(
            pNode_.Get()
        );
    }

    void MovableObject::Attach( s_ptr<MovableObject> pObject, const s_bool& bInheritRot, const s_bool& bInheritScale )
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
                Ogre::Root::getSingletonPtr()->getSceneManager("FrostSceneMgr")->getRootSceneNode()->addChild(pTargetNode_.Get());
            }
            else
            {
                pParent_->GetOgreNode()->removeChild(pNode_.Get());
                Ogre::Root::getSingletonPtr()->getSceneManager("FrostSceneMgr")->getRootSceneNode()->addChild(pNode_.Get());
            }

            pParent_ = NULL;
        }

        bInherits_ = false;
    }

    void MovableObject::CreateOrbitNode_()
    {
        Vector mPosition = GetPosition();

        // - Create target orbiting node
        if (bOrbitCenterRelative_)
        {
            pTargetNode_ = pNode_->getParentSceneNode()->createChildSceneNode();
            pTargetNode_->setPosition(Vector::FrostToOgre(mOrbitCenter_));
        }
        else
        {
            pTargetNode_ = Ogre::Root::getSingletonPtr()->getSceneManager("FrostSceneMgr")->getRootSceneNode()->createChildSceneNode();
            pTargetNode_->setPosition(Vector::FrostToOgre(mOrbitCenter_));
        }
        pTargetNode_->setFixedYawAxis(true);
        pTargetNode_->lookAt(-Vector::FrostToOgre(mPosition), Ogre::SceneNode::TS_WORLD);

        // - Change the main node's parent
        pNode_->getParent()->removeChild(pNode_.Get());
        pTargetNode_->addChild(pNode_.Get());

        pNode_->setPosition(Ogre::Vector3(0, 0, (mPosition - mOrbitCenter_).GetLenght().Get()));
        pNode_->setFixedYawAxis(true);
        pNode_->lookAt(Ogre::Vector3(0, 0, 0), Ogre::SceneNode::TS_PARENT);
    }

    void MovableObject::RemoveOrbitNode_()
    {
        pTargetNode_->removeChild(pNode_.Get());
        pTargetNode_->getParent()->addChild(pNode_.Get());
        Ogre::Root::getSingletonPtr()->getSceneManager("FrostSceneMgr")->destroySceneNode(
            pTargetNode_.Get()
        );
    }

    void MovableObject::LookAt( const Vector &mTrackedPoint, const s_bool& bTrackedPointRelative )
    {
        UnlockOrbiting();

        bTrackedPointRelative_ = bTrackedPointRelative;
        mTrackedPoint_ = mTrackedPoint;
        bTracks_ = true;
    }

    void MovableObject::OrbitAround( const Vector &mOrbitCenter, const s_bool& bOrbitCenterRelative )
    {
        UnlockOrbiting();
        UnlockTracking();

        bOrbitCenterRelative_ = bOrbitCenterRelative;
        mOrbitCenter_ = mOrbitCenter;
        bOrbits_ = true;

        CreateOrbitNode_();
    }

    void MovableObject::SetDirection( const Vector &mDirection )
    {
        if (!mDirection.IsNull())
        {
            UnlockOrbiting();
            UnlockTracking();

            pNode_->setDirection(Vector::FrostToOgre(mDirection), Ogre::Node::TS_PARENT);
        }
    }

    void MovableObject::SetPosition( const Vector &mPosition )
    {
        RemovePath();
        UnlockOrbiting();

        pNode_->setPosition(Vector::FrostToOgre(mPosition));
    }

    void MovableObject::SetPath( s_refptr<Path> pPath )
    {
        if (pPath.IsValid())
        {
            UnlockOrbiting();
            pPath_ = PathManager::GetSingleton()->CopyPath(pPath);
        }
    }

    void MovableObject::RemovePath()
    {
        pPath_.SetNull();
    }

    void MovableObject::Translate( const Vector &mTranslation, const s_bool &bLocalSpace )
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

    void MovableObject::Yaw( const s_float &fValue )
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

    void MovableObject::Pitch( const s_float &fValue )
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

    void MovableObject::Roll( const s_float &fValue )
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
    }


    const s_bool& MovableObject::IsOrbiting() const
    {
        return bOrbits_;
    }

    const s_bool& MovableObject::IsTracking() const
    {
        return bTracks_;
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

    Vector MovableObject::GetDirection( const s_bool& bRelative ) const
    {
        if (bRelative)
            return Vector::OgreToFrost(pNode_->getOrientation() * -Ogre::Vector3::UNIT_Z);
        else
            return Vector::OgreToFrost(pNode_->_getDerivedOrientation() * -Ogre::Vector3::UNIT_Z);
    }

    s_refptr<Path> MovableObject::GetPath()
    {
        return pPath_;
    }

    const Vector& MovableObject::GetTrackedPoint() const
    {
        return mTrackedPoint_;
    }

    const Vector& MovableObject::GetOrbitCenter() const
    {
        return mOrbitCenter_;
    }

    void MovableObject::Update( const s_float& fDelta )
    {
        if (pPath_ != NULL)
        {
            pPath_->Update(fDelta);

            pNode_->setPosition(Vector::FrostToOgre(pPath_->GetActualPoint()));
        }

        if (bTracks_)
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

    void MovableObject::OnEvent( const Event& mEvent )
    {

    }
}
