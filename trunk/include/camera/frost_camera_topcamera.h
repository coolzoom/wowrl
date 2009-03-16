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
