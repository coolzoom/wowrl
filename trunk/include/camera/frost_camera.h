/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Camera header             */
/*                                        */
/*                                        */


#ifndef FROST_CAMERA_H
#define FROST_CAMERA_H

#include "frost.h"
#include "scene/frost_movableobject.h"

namespace Frost
{
    /// Controls an Ogre::Camera
    /** Works like Ogre::Camera, but inherits from MovableObject.
    */
    class Camera : public MovableObject
    {
    public :

        /// Default constructor.
        /** \note You shouldn't have to call this. Use the
        *         CameraManager instead.
        */
        Camera(const s_uint& uiID);

        /// Destructor.
        virtual ~Camera();

        /// Tells this Camera that it is the main camera.
        /** \param bMainCamera 'true' if its the new main camera
        *   \note You shouldn't call this. Use CameraManager::SetMainCamera().
        */
        virtual void NotifyMainCamera(const s_bool& bMainCamera);

        /// Returns the associated Ogre::Camera.
        /** \return The associated Ogre::Camera
        */
        s_ptr<Ogre::Camera> GetOgreCamera();

        /// Returns this camera's ID.
        /** \return This camera's ID
        */
        const s_uint& GetID();

        static const s_str CLASS_NAME;

    protected :

        s_uint              uiID_;
        s_ptr<Ogre::Camera> pOgreCamera_;

    };
}

#endif
