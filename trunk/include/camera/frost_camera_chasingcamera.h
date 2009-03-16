/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          ChasingCamera header          */
/*                                        */
/*                                        */


#ifndef FROST_CHASINGCAMERA_H
#define FROST_CHASINGCAMERA_H

#include "frost.h"
#include "camera/frost_camera.h"

namespace Frost
{
    /// A Camera that follows a Unit
    /** This camera type is the most restrictive one.
    *   It also controls the chased Unit's movements.
    */
    class ChasingCamera : public Camera
    {
    public :

        /// Constructor.
        /** \note You shouldn't have to call this. Use the
        *         CameraManager instead.
        */
        ChasingCamera(const s_uint& uiID, s_ptr<Unit> pUnit);

        /// Destructor.
        ~ChasingCamera();

        /// Tells this Camera that it is the main camera.
        virtual void NotifyMainCamera(const s_bool& bMainCamera);

        /// Called whenever an Event occurs.
        /** \param mEvent The Event which has occured
        */
        virtual void OnEvent(const Event& mEvent);

    private :

        s_ptr<Unit> pUnit_;

    };
}

#endif
