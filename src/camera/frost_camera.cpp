/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               XXX source               */
/*                                        */
/*                                        */

#include "camera/frost_camera.h"
#include "frost_engine.h"

#include <OgreAxisAlignedBox.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>

using namespace std;

namespace Frost
{
    const s_str Camera::CLASS_NAME = "Camera";

    Camera::Camera() : MovableObject()
    {
        pOgreCamera_ = Frost::Engine::GetSingleton()->GetOgreSceneManager()->createCamera(
            ("Camera_"+uiID_).Get()
        );
        pOgreCamera_->setNearClipDistance(1);
        pOgreCamera_->setFarClipDistance(200);
        pOgreCamera_->setFixedYawAxis(true);
        pNode_->attachObject(pOgreCamera_.Get());
    }

    Camera::~Camera()
    {
        Frost::Engine::GetSingleton()->GetOgreSceneManager()->destroyCamera(
            pOgreCamera_.Get()
        );
    }

    s_bool Camera::IsVisible( const Vector& mPoint ) const
    {
        return pOgreCamera_->isVisible(Vector::FrostToOgre(mPoint));
    }

    s_bool Camera::IsVisible( const AxisAlignedBox& mBox ) const
    {
        return pOgreCamera_->isVisible(AxisAlignedBox::FrostToOgre(mBox));
    }

    s_float Camera::GetFarClipDistance() const
    {
        return pOgreCamera_->getFarClipDistance();
    }

    void Camera::NotifyMainCamera( const s_bool& bMainCamera )
    {
    }

    s_ptr<Ogre::Camera> Camera::GetOgreCamera()
    {
        return pOgreCamera_;
    }
}
