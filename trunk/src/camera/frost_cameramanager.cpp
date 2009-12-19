/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          CamereManager source          */
/*                                        */
/*                                        */

#include "camera/frost_cameramanager.h"
#include "camera/frost_camera.h"
#include "material/frost_shadermanager.h"

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
        mBackgroundColor_ = Color::BLACK;
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

    void CameraManager::CheckSettings()
    {
        if (!pMainCamera_)
        {
            throw Exception(CLASS_NAME, "No main camera defined.");
        }
        if (!pMainViewport_)
        {
            throw Exception(CLASS_NAME, "Viewport not created.");
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
    }

    void CameraManager::SetMainCamera( s_ptr<Camera> pCamera )
    {
        if (pCamera)
        {
            if (pMainCamera_ != pCamera)
            {
                if (pMainCamera_)
                    pMainCamera_->NotifyMainCamera(false);
                pMainCamera_ = pCamera;
                pMainCamera_->NotifyMainCamera(true);

                Ogre::ColourValue mCV;
                mCV.setAsABGR(mBackgroundColor_.GetPacked().Get());

                if (ShaderManager::GetSingleton()->IsPostProcessingEnabled())
                {
                    if (!pMainViewport_)
                    {
                        pDefaultCamera_ = CreateCamera();
                        pMainViewport_ = Engine::GetSingleton()->GetRenderWindow()->addViewport(
                            pDefaultCamera_->GetOgreCamera().Get()
                        );
                        pMainViewport_->setBackgroundColour(mCV);
                        pDefaultCamera_->GetOgreCamera()->setAspectRatio(
                            Ogre::Real(pMainViewport_->getActualWidth()) /
                            Ogre::Real(pMainViewport_->getActualHeight())
                        );

                        pSceneViewport_ = ShaderManager::GetSingleton()->GetSceneMultiRenderTarget()->addViewport(
                            pMainCamera_->GetOgreCamera().Get()
                        );
                        pSceneViewport_->setClearEveryFrame(true);
                        pSceneViewport_->setBackgroundColour(mCV);

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
                    if (!pMainViewport_)
                    {
                        pMainViewport_ = Engine::GetSingleton()->GetRenderWindow()->addViewport(
                            pMainCamera_->GetOgreCamera().Get()
                        );
                        pMainViewport_->setBackgroundColour(mCV);

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

    void CameraManager::SetBackgroundColor( const Color& mColor )
    {
        Ogre::ColourValue mCV;
        mCV.setAsABGR(mColor.GetPacked().Get());

        if (pSceneViewport_)
            pSceneViewport_->setBackgroundColour(mCV);
        else if (pMainViewport_)
            pMainViewport_->setBackgroundColour(mCV);

        mBackgroundColor_ = mColor;
    }

    const Color& CameraManager::GetBackgroundColor() const
    {
        return mBackgroundColor_;
    }

    void CameraManager::UpdateCameras( const s_float& fDelta )
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
