/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          CameraManager header          */
/*                                        */
/*                                        */


#ifndef FROST_CAMERAMANAGER_H
#define FROST_CAMERAMANAGER_H

#include "frost.h"
#include "utils/frost_utils_manager.h"

namespace Frost
{
    /// Manages cameras
    class CameraManager : public Manager<CameraManager>
    {
    friend class Manager<CameraManager>;
    public:

        /// Creates a new camera.
        /** \param mPosition The position of this camera
        *   \return The new camera
        */
        s_ptr<Camera>  CreateCamera(const Vector& mPosition);

        /// Creates a new camera.
        /** \return The new camera
        */
        s_ptr<Camera>   CreateCamera();

        /// Checks if all settings are good for rendering.
        /** \return 'true' if everything is fine
        */
        s_bool          CheckSettings();

        /// Sets the new camera to use to render the scene.
        /** \param pCamera The new camera
        */
        void            SetMainCamera(s_ptr<Camera> pCamera);

        /// Returns the main camera.
        /** \return The main camera
        *   \note This is the camera that is used to render
        *         the scene.
        */
        s_ptr<Camera>  GetMainCamera();

        /// Returns the main viewport.
        /** \return The main viewport
        */
        s_ptr<Ogre::Viewport> GetMainViewport();

        /// Updates all cameras.
        /** \param fDelta The time elapsed since the last call
        *   \note Automatically called by Engine.
        */
        void            UpdateCameras(const s_float& fDelta);

        /// Deletes a camera.
        /** \param pCamera The camera to delete
        *   \note All cameras are automatically deleted
        *         when this manager is deleted.
        */
        void            DeleteCamera(s_ptr<Camera> pCamera);

        static const s_str CLASS_NAME;

    protected:

        /// Default constructor
        /** \note This is a singleton class. That means that there can
        *         only be a single instance at the same time. That's why
        *         the constructor is not in public scope.<br>
        *         If you need to get a pointer to that unique instance,
        *         call CameraManager::GetSingleton().<br>
        *         Note that, if GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close
        *         your program by calling CameraManager::Delete() (this is
        *         automatically done by Engine).
        */
        CameraManager();

        /// Destructor
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        ~CameraManager();

        /// Copy constructor
        CameraManager(const CameraManager& mMgr);

        /// Assignment operator
        CameraManager& operator = (const CameraManager& mMgr);

    private:

        std::map< s_uint, s_ptr<Camera> > lCameraList_;

        s_ptr<Camera>         pMainCamera_;
        s_ptr<Ogre::Viewport> pViewport_;
        s_bool                bNewViewport_;

    };
}


#endif
