/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          CamereManager source          */
/*                                        */
/*                                        */

#include "frost_cameramanager.h"
#include "frost_camera.h"

#include <OgreCamera.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>

using namespace std;

namespace Frost
{
    const s_str CameraManager::CLASS_NAME = "CameraManager";

    CameraManager::CameraManager()
    {
    }

    CameraManager::~CameraManager()
    {
        map<s_uint, s_ptr<Camera> >::iterator iterCamera;
        foreach (iterCamera, lCameraList_)
        {
            iterCamera->second.Delete();
        }

        if (pMainCamera_)
        {
            Engine::GetSingleton()->GetRenderWindow()->removeViewport(
                pViewport_->getZOrder()
            );
        }
    }

    s_ptr<Camera> CameraManager::CreateCamera( const Vector &mPosition )
    {
        s_ptr<Camera> pCam(new Camera(uiCounter_));
        lCameraList_[uiCounter_] = pCam;
        pCam->SetPosition(mPosition);
        uiCounter_++;

        return pCam;
    }

    s_ptr<Camera> CameraManager::CreateCamera()
    {
        s_ptr<Camera> pCam(new Camera(uiCounter_));
        lCameraList_[uiCounter_] = pCam;
        uiCounter_++;

        return pCam;
    }

    s_bool CameraManager::CheckSettings()
    {
        Tester mGood;

        if (pMainCamera_ == NULL)
        {
            Error(CLASS_NAME, "No main camera defined.");
            mGood = false;
        }
        if (pViewport_ == NULL)
        {
            Error(CLASS_NAME, "Viewport not created.");
            mGood = false;
        }

        if (mGood && bNewViewport_)
        {
            map< s_uint, s_ptr<Camera> >::iterator iterCamera;
            foreach (iterCamera, lCameraList_)
            {
                iterCamera->second->GetOgreCamera()->setAspectRatio(
                    Ogre::Real(pViewport_->getActualWidth()) /
                    Ogre::Real(pViewport_->getActualHeight())
                );
            }
            bNewViewport_ = false;
        }

        return mGood;
    }

    void CameraManager::SetMainCamera( s_ptr<Camera> pCamera )
    {
        if (pCamera != NULL)
        {
            pMainCamera_ = pCamera;

            if (pViewport_ == NULL)
            {
                pViewport_ = Engine::GetSingleton()->GetRenderWindow()->addViewport(
                    pMainCamera_->GetOgreCamera().Get()
                );
                pViewport_->setBackgroundColour(Ogre::ColourValue(0.0f,0.0f,0.0f));
                bNewViewport_ = true;
            }
            else
            {
                pViewport_->setCamera(pMainCamera_->GetOgreCamera().Get());
            }
        }
    }

    s_ptr<Camera> CameraManager::GetMainCamera()
    {
        return pMainCamera_;
    }

    s_ptr<Ogre::Viewport> CameraManager::GetMainViewport()
    {
        return pViewport_;
    }

    void CameraManager::UpdateCameras(const s_float& fDelta)
    {
        map< s_uint, s_ptr<Camera> >::iterator iterCamera;
        foreach (iterCamera, lCameraList_)
        {
            iterCamera->second->Update(fDelta);
        }
    }

    void CameraManager::DeleteCamera( s_ptr<Camera> pCamera )
    {
        if (pCamera)
        {
            map< s_uint, s_ptr<Camera> >::iterator iterCamera;
            iterCamera = lCameraList_.find(pCamera->GetID());

            if (iterCamera != lCameraList_.end())
            {
                if (iterCamera->second->GetID() == pCamera->GetID())
                {
                    // Everything went fine, delete and erase from map
                    iterCamera->second.Delete();
                    lCameraList_.erase(iterCamera);
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
