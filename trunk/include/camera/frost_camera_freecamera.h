/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            FreeCamera header           */
/*                                        */
/*                                        */


#ifndef FROST_FREECAMERA_H
#define FROST_FREECAMERA_H

#include "frost.h"
#include "camera/frost_camera.h"

namespace Frost
{
    /// A very flexible Camera
    /** This camera type is mainly usefull for
    *   debugging, but it can also be used by the
    *   player to inspect the world.
    */
    class FreeCamera : public Camera
    {
    public :

        /// Constructor.
        /** \note You shouldn't have to call this. Use the
        *         CameraManager instead.
        */
        FreeCamera(const s_uint& uiID);

        /// Destructor.
        ~FreeCamera();

        /// Tells this Camera that it is the main camera.
        /** \param bMainCamera 'true' if its the new main camera
        *   \note You shouldn't call this. Use CameraManager::SetMainCamera().
        */
        virtual void NotifyMainCamera(const s_bool& bMainCamera);

        /// Sets the movement speed of this camera.
        /** \param fSpeed The new speed of this camera
        *   \note Default is 20.0f
        */
        void SetSpeed(const s_float& fSpeed);

        /// Returns the movement speed of this camera.
        /** \return The movement speed of this camera
        *   \note Default is 20.0f
        */
        const s_float& GetSpeed() const;

        /// Called whenever an Event occurs.
        /** \param mEvent The Event which has occured
        */
        virtual void OnEvent(const Event& mEvent);

    private :

        s_float fSpeed_;

    };
}

#endif
