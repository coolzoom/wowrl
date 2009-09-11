/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          CamereManager source          */
/*                                        */
/*                                        */

#include "camera/frost_cameramanager.h"
#include "camera/frost_camera.h"
#include "gui/frost_guistructs.h"

#include <OgreCamera.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreRenderTexture.h>

using namespace std;

namespace Frost
{
    const s_str CameraManager::CLASS_NAME = "CameraManager";

    CameraManager::CameraManager()
    {
    }

    CameraManager::~CameraManager()
    {
        Log("Closing "+CLASS_NAME+"...");
        s_map<s_uint, s_ptr<Camera> >::iterator iterCamera;
        foreach (iterCamera, lCameraList_)
        {
            iterCamera->second.Delete();
        }
    }

    s_ptr<Camera> CameraManager::CreateCamera( const Vector& mPosition )
    {
        s_ptr<Camera> pCam = new Camera();
        lCameraList_[pCam->GetID()] = pCam;
        pCam->SetPosition(mPosition);

        return pCam;
    }

    s_ptr<Camera> CameraManager::CreateCamera()
    {
        s_ptr<Camera> pCam = new Camera();
        lCameraList_[pCam->GetID()] = pCam;

        return pCam;
    }

    s_bool CameraManager::CheckSettings()
    {
        if (pMainCamera_ == NULL)
        {
            Error(CLASS_NAME, "No main camera defined.");
            return false;
        }
        if (pMainViewport_ == NULL)
        {
            Error(CLASS_NAME, "Viewport not created.");
            return false;
        }

        if (bNewViewport_)
        {
            s_map< s_uint, s_ptr<Camera> >::iterator iterCamera;
            foreach (iterCamera, lCameraList_)
            {
                iterCamera->second->GetOgreCamera()->setAspectRatio(
                    Ogre::Real(pMainViewport_->getActualWidth()) /
                    Ogre::Real(pMainViewport_->getActualHeight())
                );
            }
            bNewViewport_ = false;
        }

        return true;
    }

    void CameraManager::SetMainCamera( s_ptr<Camera> pCamera )
    {
        if (pCamera != NULL)
        {
            if (pMainCamera_ != pCamera)
            {
                if (pMainCamera_)
                    pMainCamera_->NotifyMainCamera(false);
                pMainCamera_ = pCamera;
                pMainCamera_->NotifyMainCamera(true);

                if (Engine::GetSingleton()->GetBoolConstant("EnablePostProcessing"))
                {
                    if (pMainViewport_ == NULL)
                    {
                        pDefaultCamera_ = CreateCamera();
                        pMainViewport_ = Engine::GetSingleton()->GetRenderWindow()->addViewport(
                            pDefaultCamera_->GetOgreCamera().Get()
                        );
                        pMainViewport_->setBackgroundColour(Ogre::ColourValue::Black);
                        pDefaultCamera_->GetOgreCamera()->setAspectRatio(
                            Ogre::Real(pMainViewport_->getActualWidth()) /
                            Ogre::Real(pMainViewport_->getActualHeight())
                        );

                        pSceneViewport_ = Engine::GetSingleton()->GetSceneMultiRenderTarget()->addViewport(
                            pMainCamera_->GetOgreCamera().Get()
                        );
                        pSceneViewport_->setClearEveryFrame(true);
                        pSceneViewport_->setBackgroundColour(Ogre::ColourValue::Black);

                        pSceneViewport_->setDimensions(
                            0, 0,
                            pMainViewport_->getActualWidth()/(float)pSceneViewport_->getActualWidth(),
                            pMainViewport_->getActualHeight()/(float)pSceneViewport_->getActualHeight()
                        );

                        bNewViewport_ = true;
                    }
                    else
                    {
                        pSceneViewport_->setCamera(pMainCamera_->GetOgreCamera().Get());
                    }
                }
                else
                {
                    if (pMainViewport_ == NULL)
                    {
                        pMainViewport_ = Engine::GetSingleton()->GetRenderWindow()->addViewport(
                            pMainCamera_->GetOgreCamera().Get()
                        );
                        pMainViewport_->setBackgroundColour(Ogre::ColourValue::Black);

                        bNewViewport_ = true;
                    }
                    else
                    {
                        pMainViewport_->setCamera(pMainCamera_->GetOgreCamera().Get());
                    }
                }
            }
        }
    }

    s_ptr<Camera> CameraManager::GetMainCamera()
    {
        return pMainCamera_;
    }

    s_ptr<Ogre::Viewport> CameraManager::GetMainViewport()
    {
        return pMainViewport_;
    }

    void CameraManager::UpdateCameras(const s_float& fDelta)
    {
        s_map< s_uint, s_ptr<Camera> >::iterator iterCamera;
        foreach (iterCamera, lCameraList_)
        {
            iterCamera->second->Update(fDelta);
        }
    }

    void CameraManager::DeleteCamera( s_ptr<Camera> pCamera )
    {
        if (pCamera)
        {
            s_map< s_uint, s_ptr<Camera> >::iterator iterCamera;
            iterCamera = lCameraList_.Get(pCamera->GetID());

            if (iterCamera != lCameraList_.end())
            {
                if (iterCamera->second->GetID() == pCamera->GetID())
                {
                    // Everything went fine, delete and erase from map
                    iterCamera->second.Delete();
                    lCameraList_.Erase(iterCamera);
                }
            }
            else
            {
                Warning(CLASS_NAME,
                    "Trying to call DeleteCamera on a Camera that has not been created by CameraManager (ID:"+pCamera->GetID()+")."
                );
            }
        }
    }
}
