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
        Camera();

        /// Destructor.
        virtual ~Camera();

        /// Checks if a point is visible in this Camera.
        /** \param mPoint The point to check
        *   \return 'true' if the point is visible in this Camera
        */
        s_bool IsVisible(const Vector& mPoint) const;

        /// Checks if a box is visible in this Camera.
        /** \param mMin The first point of the box
        *   \param mMax The second point of the box
        *   \return 'true' if the box is visible in this Camera
        */
        s_bool IsVisible(const Vector& mMin, const Vector& mMax) const;

        /// Returns this Camera's far clip distance.
        /** \return This Camera's far clip distance
        */
        s_float GetFarClipDistance() const;

        /// Tells this Camera that it is the main camera.
        /** \param bMainCamera 'true' if its the new main camera
        *   \note You shouldn't call this. Use CameraManager::SetMainCamera().
        */
        virtual void NotifyMainCamera(const s_bool& bMainCamera);

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
