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
        /** \param pSceneManager The scene manager in which to create the camera
        *   \note If no scene manager is provided, the default one is used.
        *   \note You shouldn't have to call this. Use the CameraManager instead.
        */
        Camera(s_ptr<Ogre::SceneManager> pSceneManager = nullptr);

        /// Destructor.
        ~Camera();

        /// Projects a point of the 3D world on the screen.
        /** \param mPoint The point to project
        *   \return The projected vector
        *   \note The position on the screen is described by (X,Y),
        *         while the Z component stands for the distance to
        *         the camera (in the [-1, 1] range).
        */
        Vector ProjectOnScreen(const Vector& mPoint) const;

        /// Checks if a point is visible by this Camera.
        /** \param mPoint The point to check
        *   \return 'true' if the point is visible by this Camera
        */
        s_bool IsVisible(const Vector& mPoint) const;

        /// Checks if a box is visible by this Camera.
        /** \param mBox The box to check
        *   \return 'true' if the box is visible by this Camera
        */
        s_bool IsVisible(const AxisAlignedBox& mBox) const;

        /// Returns this Camera's far clip distance.
        /** \return This Camera's far clip distance
        */
        s_float GetFarClipDistance() const;

        /// Tells this Camera that it is the main camera.
        /** \param bMainCamera 'true' if its the new main camera
        *   \note You shouldn't call this. Use CameraManager::SetMainCamera().
        */
        void NotifyMainCamera(const s_bool& bMainCamera);

        /// Returns the associated Ogre::Camera.
        /** \return The associated Ogre::Camera
        */
        s_ptr<Ogre::Camera> GetOgreCamera();

        static const s_str CLASS_NAME;

    protected :

        s_ptr<Ogre::Camera> pOgreCamera_;

    };
}

#endif
