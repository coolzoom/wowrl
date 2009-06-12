/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               XXX source               */
/*                                        */
/*                                        */

#include "camera/frost_camera.h"
#include "frost_engine.h"
#include "path/frost_pathmanager.h"
#include "path/frost_path.h"

#include <OgreSceneManager.h>
#include <OgreCamera.h>

using namespace std;

namespace Frost
{
    const s_str Camera::CLASS_NAME = "Camera";

    Camera::Camera()
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

    void Camera::NotifyMainCamera( const s_bool& bMainCamera )
    {
    }

    s_ptr<Ogre::Camera> Camera::GetOgreCamera()
    {
        return pOgreCamera_;
    }
}
