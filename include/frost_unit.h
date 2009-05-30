/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Unit header              */
/*                                        */
/*                                        */


#ifndef FROST_UNIT_H
#define FROST_UNIT_H

#include "frost.h"
#include "frost_stats.h"

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

    /// Holds generic informations about a specific unit class.
    struct Class
    {
        s_str sName;
        s_str sHealthType;
        s_str sPowerType;
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

        /// Changes this Unit's Class.
        /** \param sClassName The name of the new Class
        */
        void             SetClass(const s_str& sClassName);

        /// Changes this Unit's Class.
        /** \param mClass The new Class
        */
        void             SetClass(const Class& mClass);

        /// Returns this Unit's Class.
        /** \return This Unit's Class
        */
        const Class&     GetClass() const;

        /// Adds some health points to this Unit's total health.
        /** \param fHealthAdd The number of health points to add
        */
        void             AddHealth(const s_float& fHealthAdd);

        /// Sets this Unit's current health points to the maximum possible.
        void             FillHealthGauge();

        /// Sets this Unit's current health points to zero.
        void             EmptyHealthGauge();

        /// Returns this Unit's total health value.
        /** \return This Unit's total health value
        */
        const s_float&   GetHealth() const;

        /// Returns this Unit's maximum health value.
        /** \return This Unit's maximum health value
        */
        s_float          GetMaxHealth() const;

        /// Returns this Unit's health regen ratio.
        /** \return This Unit's health regen ratio
        */
        s_float          GetHealthRegenRatio() const;

        /// Adds some power points to this Unit's total power.
        /** \param fPowerAdd The number of power points to add
        */
        void             AddPower(const s_float& fPowerAdd);

        /// Sets this Unit's current power points to the maximum possible.
        void             FillPowerGauge();

        /// Sets this Unit's current power points to zero.
        void             EmptyPowerGauge();

        /// Returns this Unit's total power value.
        /** \return This Unit's total power value
        */
        const s_float&   GetPower() const;

        /// Returns this Unit's maximum power value.
        /** \return This Unit's maximum power value
        */
        s_float          GetMaxPower() const;

        /// Returns this Unit's power regen ratio.
        /** \return This Unit's power regen ratio
        */
        s_float          GetPowerRegenRatio() const;

        /// Makes this Unit die.
        /** \note The Unit has to be alive, else this function will have
        *         no effect.
        */
        void             Die();

        /// Checks if this Unit is dead.
        /** \return 'true' if this Unit is dead
        */
        const s_bool&    IsDead();

        /// Resurrects this Unit.
        /** \note The Unit has to be dead, else this function will have
        *         no effect.
        */
        void             Resurrect();

        /// Sets one of this Unit's stat.
        /** \param sStatName The code name of this stat
        *   \param iValue    The new value to give to this stat
        */
        void             SetStat(const s_str& sStatName, const s_int& iValue);

        /// Sets one of this Unit's stat.
        /** \param sStatName The code name of this stat
        *   \param fValue    The new value to give to this stat
        */
        void             SetStat(const s_str& sStatName, const s_float& fValue);

        /// Returns one of this Unit's stat.
        /** \param sStatName The code name of this stat
        *   \return The stat value
        */
        s_var            GetStat(const s_str& sStatName) const;

        /// Sets this Unit's level.
        /** \param uiLevel The new level
        *   \note Pass NaN if you want to give no level to this Unit.
        */
        void             SetLevel(const s_uint& uiLevel);

        /// Returns this Unit's level.
        /** \return This Unit's level
        *   \note Returns NaN if this Unit has no level.
        */
        const s_uint&    GetLevel() const;

        /// Tells this Unit that it has been highlithed (or not).
        /** \param bHighlighted 'true' if the Unit has been highlighted
        */
        void             NotifyHighlighted(const s_bool& bHighlighted);

        /// Checks if this Unit is highlighted.
        /** \param 'true' if this Unit is highlithed
        */
        const s_bool&    IsHighlighted() const;

        /// Tells this Unit that it has been selected/deselected.
        /** \param bSelected 'true' if the Unit has been selected
        */
        void             NotifySelected(const s_bool& bSelected);

        /// Checks if this Unit is selected.
        /** \return 'true' if this Unit is selected
        */
        const s_bool&    IsSelected() const;

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

        /// Returns the position of this Unit in the world.
        /** \return The position of this Unit in the world
        */
        Vector           GetPosition() const;

        /// Returns the Camera associated to this Unit.
        /** \return The Camera associated to this Unit
        */
        s_ptr<Camera>    GetCamera();

        /// Returns this Unit's body model.
        /** \return This Unit's body model
        */
        s_refptr<Model>  GetBodyModel();

        /// Returns this Unit's node.
        /** \return This Unit's node
        */
        s_ptr<Node>      GetNode();

        /// Returns this Unit's ID.
        /** \return This Unit's ID
        */
        const s_uint&    GetID() const;

        /// Returns the ID that represents this Unit in the Lua code.
        /** \return The ID that represents this Unit in the Lua code
        */
        s_str            GetLuaID() const;

        /// Creates the associated Lua glue.
        virtual void     CreateGlue();

        /// Pushes this Unit on the provided Lua::State.
        /** \param pLua The State on which to push the Unit
        */
        virtual void     PushOnLua(s_ptr<Lua::State> pLua) const;

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

        void UpdateMovement_(const s_float fDelta);

        s_uint uiID_;
        s_str  sName_;

        s_ptr<LuaUnit> pGlue_;

        Class                        mClass_;
        s_refptr<HealthTypeInstance> pHealthType_;
        s_float                      fHealth_;
        s_refptr<PowerTypeInstance>  pPowerType_;
        s_float                      fPower_;
        Stats                        mStats_;
        s_uint                       uiLevel_;

        s_refptr<Model> pBodyModel_;
        s_ptr<Camera>   pCamera_;
        s_ptr<Node>     pNode_;
        s_refptr<Decal> pSelectionDecal_;
        s_uint          uiSelectionDecalID_;
        s_uint          uiShadowDecalID_;
        s_bool          bCameraMovedAlone_;

        s_bool          bSelected_;
        s_bool          bHighlighted_;
        s_bool          bDead_;

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
        s_float         fJumpHeight_;
        s_float         fJumpDuration_;
        s_float         fForwardRunSpeed_;
        s_float         fForwardWalkSpeed_;
        s_float         fBackwardRunSpeed_;
        s_float         fBackwardWalkSpeed_;
        s_float         fTurnRate_;
    };

    /** \cond NOT_REMOVE_FROM_DOC
    */

    class LuaUnit
    {
    public :

        LuaUnit(lua_State* pLua);

        int _AddBuff(lua_State*);
        int _AddHealth(lua_State*);
        int _AddPower(lua_State*);
        int _Damage(lua_State*);
        int _FillHealthGauge(lua_State*);
        int _FillPowerGauge(lua_State*);
        int _EmptyHealthGauge(lua_State*);
        int _EmptyPowerGauge(lua_State*);
        int _Die(lua_State*);
        int _GetClass(lua_State*);
        int _GetHealthRegenRatio(lua_State*);
        int _GetLevel(lua_State*);
        int _GetPowerRegenRatio(lua_State*);
        int _GetStat(lua_State*);
        int _GetUnitType(lua_State*);
        int _IsInCombat(lua_State*);
        int _IsSitting(lua_State*);
        int _SetAnim(lua_State*);
        int _SetAttacking(lua_State*);

        int    GetDataTable(lua_State*);
        static const char className[];
        static Lunar<LuaUnit>::RegType methods[];
        static const s_str CLASS_NAME;

    protected :

        s_ptr<Unit> pParent_;

        lua_State*  pLua_;
        int         iRef_;
    };

    /** \endcond
    */
}

#endif
