/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Unit header              */
/*                                        */
/*                                        */


#ifndef FROST_UNIT_H
#define FROST_UNIT_H

#include "frost.h"
#include "frost_eventreceiver.h"
#include "frost_timers.h"

namespace Frost
{
    enum Movement
    {
        MOVEMENT_NONE,
        MOVEMENT_FORWARD,
        MOVEMENT_BACKWARD
    };

    enum LateralMovement
    {
        LMOVEMENT_NONE,
        LMOVEMENT_STRAFE_LEFT,
        LMOVEMENT_STRAFE_RIGHT,
        LMOVEMENT_TURN_LEFT,
        LMOVEMENT_TURN_RIGHT,
    };

    /// Abstract interractive object
    /** Can be either a Character or a Creature.<br>
    *   A Unit can move on its own and interract with
    *   the world.
    */
    class Unit : public EventReceiver
    {
    public :

        /// Constructor.
        /** \param uiID  This Unit's unique ID
        *   \param sName This Unit's name
        *   \note You shouldn't have to call this. Use the
        *         UnitManager instead.
        */
        Unit(const s_uint& uiID, const s_str& sName);

        /// Destructor.
        virtual ~Unit();

        /// Makes this Unit jump.
        void             Jump();

        /// Makes this Unit move forward.
        void             SetMoveForward(const s_bool& bMoveForward);

        /// Makes this Unit move backward.
        void             SetMoveBackward(const s_bool& bMoveBackward);

        /// Enables/disables walking.
        /** \note By default, a Unit runs.
        */
        void             ToggleWalking();

        /// Makes this Unit walk/run.
        /** \param bWalk 'true' to make it walk
        *   \note By default, a Unit runs.
        */
        void             SetWalking(const s_bool& bWalk);

        /// Makes this Unit move to the left.
        void             SetMoveLeft(const s_bool& bMoveLeft);

        /// Makes this Unit move to the right.
        void             SetMoveRight(const s_bool& bMoveRight);

        /// Enables/disables turning.
        /** \note If the Unit doesn't turn, it strafes.
        */
        void             ToggleTurning();

        /// Makes this Unit turn/strafe.
        /** \param bTurn 'true' to make it turn, 'false' to
        *                make it strafe
        *   \note By default, a Unit turns.
        */
        void             SetTurning(const s_bool& bTurn);

        /// Stops this Unit's movement.
        void             StopMovement();

        /// Moves this Unit's Camera on its local axis.
        /** \param fZoom The amount
        */
        void             ZoomCamera(const s_float& fZoom);

        /// Moves this Unit's Camera around its anchor point.
        /** \param fYaw   Angle to rotate horizontaly
        *   \param fPitch Angle to rotate verticaly
        */
        void             RotateCamera(const s_float& fYaw, const s_float& fPitch);

        /// Rotates this Unit's Model.
        /** \param fYaw   Angle to rotate horizontaly
        *   \param fPitch Angle to rotate verticaly
        *   \note Also rotates the Camera like RotateCamera().
        */
        void             RotateModel(const s_float& fYaw, const s_float& fPitch);

        /// Returns the Camera associated to this Unit.
        /** \return The Camera associated to this Unit
        */
        s_ptr<Camera>    GetCamera();

        /// Returns this Unit's body model.
        /** \return This Unit's body model
        */
        s_refptr<Model>  GetBodyModel();

        /// Returns this Unit's ID.
        /** \return This Unit's ID
        */
        const s_uint&    GetID() const;

        /// Called whenever an Event occurs.
        /** \param mEvent The Event which has occured
        */
        virtual void     OnEvent(const Event& mEvent);

        /// Updates this Unit's parameters.
        /** \param fDelta The time elapsed since the last call
        *   \note Automatically called by UnitManager.
        */
        virtual void     Update(const s_float& fDelta);

        static const s_str CLASS_NAME;

    protected :

        void SetMovement_(Movement mMovementType);
        void SetLateralMovement_(LateralMovement mLMovementType);

        s_uint          uiID_;
        s_str           sName_;
        s_refptr<Model> pBodyModel_;
        s_ptr<Camera>   pCamera_;
        s_bool          bCameraMovedAlone_;

        s_bool          bJumping_;
        Vector          mJumpPosition_;
        Vector          mJumpDirection_;
        Vector          mJumpHMovement_;
        Timer           mJumpTimer_;
        Movement        mMovementType_;
        Movement        mJumpMovementType_;
        LateralMovement mLMovementType_;
        s_bool          bWalk_;
        s_bool          bMoveForward_;
        s_bool          bMoveBackward_;
        s_bool          bStrafeLeft_;
        s_bool          bStrafeRight_;
        s_bool          bTurnLeft_;
        s_bool          bTurnRight_;
        s_bool          bTurn_;
        s_float         fCumuledYaw_;

        s_float fJumpHeight_;
        s_float fJumpDuration_;
        s_float fForwardRunSpeed_;
        s_float fForwardWalkSpeed_;
        s_float fBackwardRunSpeed_;
        s_float fBackwardWalkSpeed_;
        s_float fTurnRate_;
    };
}

#endif
