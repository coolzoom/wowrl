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

#include <frost_utils_axisalignedbox.h>

using namespace std;

namespace Frost
{
    const s_str Camera::CLASS_NAME = "Camera";

    Camera::Camera( s_ptr<Ogre::SceneManager> pSceneManager ) : MovableObject(s_uint::NaN, pSceneManager)
    {
        pOgreCamera_ = pSceneManager_->createCamera(("Camera_"+uiID_).Get());
        pOgreCamera_->setNearClipDistance(1);
        pOgreCamera_->setFarClipDistance(500);
        pOgreCamera_->setFixedYawAxis(true);
        pNode_->attachObject(pOgreCamera_.Get());
    }

    Camera::~Camera()
    {
        pSceneManager_->destroyCamera(pOgreCamera_.Get());
    }

    Vector Camera::ProjectOnScreen( const Vector& mPoint ) const
    {
        Ogre::Matrix4 mViewProj = pOgreCamera_->getProjectionMatrix() * pOgreCamera_->getViewMatrix();
        Ogre::Vector4 mProj = mViewProj * Ogre::Vector4(Vector::FrostToOgre(mPoint));

        return Vector(mProj.x * 0.5f / mProj.w + 0.5f, 1.0f - (mProj.y * 0.5f / mProj.w + 0.5f), mProj.z/mProj.w);
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
