/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           MovableUnit header           */
/*                                        */
/*                                        */

#ifndef FROST_MOVABLEUNIT_H
#define FROST_MOVABLEUNIT_H

#include "frost.h"
#include "unit/frost_unit.h"

namespace Frost
{
    /// Abstract Unit type.
    /**
    */
    class MovableUnit : public Unit
    {
    public :

        /// Constructor.
        /** \param uiID  This MovableUnit's unique ID
        *   \param sName This MovableUnit's name
        */
        MovableUnit(const s_uint& uiID, const s_str& sName);

        /// Destructor.
        virtual ~MovableUnit();

        /// Makes this Unit jump.
        void         Jump();

        /// Makes this Unit move forward.
        void         SetMoveForward(const s_bool& bMoveForward);

        /// Makes this Unit move backward.
        void         SetMoveBackward(const s_bool& bMoveBackward);

        /// Enables/disables walking.
        /** \note By default, a Unit runs.
        */
        void         ToggleWalking();

        /// Makes this Unit walk/run.
        /** \param bWalk 'true' to make it walk
        *   \note By default, a Unit runs.
        */
        void         SetWalking(const s_bool& bWalk);

        /// Makes this Unit move to the left.
        void         SetMoveLeft(const s_bool& bMoveLeft);

        /// Makes this Unit move to the right.
        void         SetMoveRight(const s_bool& bMoveRight);

        /// Enables/disables turning.
        /** \note If the Unit doesn't turn, it strafes.
        */
        void         ToggleTurning();

        /// Makes this Unit turn/strafe.
        /** \param bTurn 'true' to make it turn, 'false' to
        *                make it strafe
        *   \note By default, a Unit turns.
        */
        void         SetTurning(const s_bool& bTurn);

        /// Stops this Unit's movement.
        void         StopMovement();

        /// Enables physical interractions for this Unit.
        /** \note Disabled by default.
        */
        void         EnablePhysics();

        /// Disables physical interractions for this Unit.
        /** \note Disabled by default.
        */
        void         DisablePhysics();

        /// Checks if physics is enabled for this Unit.
        /** \return 'true' if physics is enabled for this Unit
        *   \note Disabled by default.
        */
        s_bool       IsPhysicsEnabled() const;

        /// Forces a Unit to touch the ground.
        /** \note For this function to have any effect, you must
        *         enable physics by calling EnablePhysics().
        */
        void         ForceOnGround();

        /// Returns this Unit's PhysicsHandler.
        /** \return This Unit's PhysicsHandler
        *   \note Returns NULL if none (i.e if IsPhysicsEnabled()
        *         returns 'false').
        */
        s_ptr<PhysicsHandler> GetPhysicsHandler();

        /// Creates the associated Lua glue.
        /** \param pLua The Lua::State on which to create the glue
        */
        virtual void CreateGlue(s_ptr<Lua::State> pLua);

        /// Updates this Unit's parameters.
        /** \param fDelta The time elapsed since the last call
        *   \note Automatically called by UnitManager.
        */
        virtual void Update(const s_float& fDelta);

    protected :

        void UpdateMovement_(const s_float& fDelta);
        void SetYaw_(const s_float& fNewYaw);

        Vector  mMovementDirection_;
        s_bool  bWalk_;
        s_bool  bTurn_;
        s_float fCumuledYaw_;

        s_bool  bJumping_;
        s_float fJumpVSpeed_;
        s_float fCumuledJumpHeight_;
        s_float fJumpDuration_;

        s_float fForwardRunSpeed_;
        s_float fForwardWalkSpeed_;
        s_float fBackwardRunSpeed_;
        s_float fBackwardWalkSpeed_;
        s_float fTurnRate_;

        s_ptr<PhysicsHandler> pHandler_;

    };

    /** \cond NOT_REMOVE_FROM_DOC
    */

    class LuaMovableUnit : public LuaUnit
    {
    public :

        LuaMovableUnit(lua_State* pLua);

        int _Jump(lua_State*);
        int _SetMoveForward(lua_State*);
        int _SetMoveBackward(lua_State*);
        int _SetMoveLeft(lua_State*);
        int _SetMoveRight(lua_State*);
        int _ToggleTurning(lua_State*);
        int _ToggleWalking(lua_State*);

        static const char className[];
        static Lunar<LuaMovableUnit>::RegType methods[];
        static const s_str CLASS_NAME;

    protected :

        s_ptr<MovableUnit> pParentMovable_;
    };

    /** \endcond
    */
}

#endif
