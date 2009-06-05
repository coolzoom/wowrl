/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            TopCamera header            */
/*                                        */
/*                                        */


#ifndef FROST_TOPCAMERA_H
#define FROST_TOPCAMERA_H

#include "frost.h"
#include "camera/frost_camera.h"

namespace Frost
{
    /// A STR like Camera
    /** This camera always looks at a Node positioned
    *   on the ground. This node and the camera can
    *   be moved (view pan), and the camera can "zoom"
    *   (come closer to the node) and orbit around
    *   its node.
    */
    class TopCamera : public Camera
    {
    public :

        /// Constructor.
        /** \note You shouldn't have to call this. Use the
        *         CameraManager instead.
        */
        TopCamera(const s_uint& uiID);

        /// Destructor.
        ~TopCamera();

        /// Tells this Camera that it is the main camera.
        /** \param bMainCamera 'true' if its the new main camera
        *   \note You shouldn't call this. Use CameraManager::SetMainCamera().
        */
        virtual void NotifyMainCamera(const s_bool& bMainCamera);

        /// Sets the movement speed of this camera.
        /** \param fSpeed The new speed of this camera
        *   \note Default is 20.0f.
        */
        void SetSpeed(const s_float& fSpeed);

        /// Returns the movement speed of this camera.
        /** \return The movement speed of this camera
        *   \note Default is 20.0f.
        */
        const s_float& GetSpeed() const;

        /// Makes this camera move to particular point in the world.
        /** \param mDestination The point to go to
        *   \note This point should be on the ground.
        */
        void MoveTo(const Vector& mDestination);

        /// Called whenever an Event occurs.
        /** \param mEvent The Event which has occured
        */
        virtual void OnEvent(const Event& mEvent);

        /// Updates this Camera's parameters.
        /** \param fDelta The time elapsed since the last call
        *   \note Automatically called by CameraManager.
        */
        virtual void Update(const s_float& fDelta);

    private :

        s_float fSpeed_;
        Vector  mDestination_;

    };
}

#endif
