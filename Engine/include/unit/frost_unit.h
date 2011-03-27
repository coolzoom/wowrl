/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Unit header              */
/*                                        */
/*                                        */


#ifndef FROST_UNIT_H
#define FROST_UNIT_H

#include "frost.h"
#include "unit/frost_stats.h"
#include "scene/frost_sceneobject.h"

#include <frost_utils_lua.h>
#include <frost_utils_eventreceiver.h>

namespace Frost
{
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
    class Unit : public EventReceiver, public SceneObject
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

        /// Tells this Unit that is is being controlled by the player.
        virtual void     SetPlayerControlled(const s_bool& bPlayerControlled);

        /// Checks if this Unit is being controlled by the player.
        /** \return 'true' if this Unit is being controlled by the player
        */
        const s_bool&    IsPlayerControlled() const;

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

        /// Checks if this Unit is hostile toward another.
        /** \param pReference The other Unit
        *   \note If the other Unit is ommited, then this function will
        *         check if this Unit is hostile toward the player.
        */
        s_bool           IsHostile(s_ptr<Unit> pReference = nullptr) const;

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

        /// Makes this Unit fully lighted (no shadow).
        /** \param bHighlighted 'true' to make the Unit highlighted
        */
        void             Highlight(const s_bool& bHighlighted);

        /// Checks if this Unit is highlighted.
        /** \return 'true' if this Unit is highlithed
        */
        const s_bool&    IsHighlighted() const;

        /// Tells this Unit that it has been selected/deselected.
        /** \param bSelected 'true' if the Unit has been selected
        */
        void             Select(const s_bool& bSelected);

        /// Instantaneously move this Unit to a given position.
        /** \param mDestination The new position of the Unit
        */
        void             Teleport(const Vector& mDestination);

        /// Orient this Unit so it looks at the other.
        /** \param pUnit The Unit to look at
        *   \note This Unit will keep on looking at the other
        *         until you call LookAt(nullptr).
        */
        void             LookAtUnit(s_ptr<Unit> pUnit);

        /// Orient this Unit so it looks at an object.
        /** \param pObject The Object to look at
        *   \note This Unit will keep on looking at the object
        *         until you call LookAt(nullptr).
        */
        void             LookAtObject(s_ptr<MovableObject> pObject);

        /// Orient this Unit so it looks at the given position.
        /** \param mPosition The point to look at
        *   \note If you need continuous traking, use the other
        *         LookAt functions.
        */
        void             LookAt(const Vector& mPosition);

        /// Moves this Unit's Camera on its local axis.
        /** \param fZoom The amount
        */
        void             ZoomCamera(const s_float& fZoom);

        /// Moves this Unit's Camera around its anchor point.
        /** \param fYaw   Angle to rotate horizontaly
        *   \param fPitch Angle to rotate verticaly
        */
        virtual void     RotateCamera(const s_float& fYaw, const s_float& fPitch);

        /// Rotates this Unit's Model.
        /** \param fYaw   Angle to rotate horizontaly
        *   \param fPitch Angle to rotate verticaly
        *   \note Also rotates the Camera like RotateCamera().
        */
        virtual void     RotateModel(const s_float& fYaw, const s_float& fPitch);

        /// Returns the position of this Unit in the world.
        /** \return The position of this Unit in the world
        */
        Vector           GetPosition() const;

        /// Enables motion blur for this unit.
        /** \note See DisableMotionBlur() for more infos.
        */
        void             EnableMotionBlur();

        /// Disables motion blur for this unit.
        /** \note This unit will no longer cast motion blur nor receive it.
        */
        void             DisableMotionBlur();

        /// Checks if motion blur is enabled for this unit.
        /** \return 'true' if motion blur is enabled for this unit
        */
        const s_bool&    IsMotionBlurEnabled() const;

        /// Returns this Unit's body model.
        /** \return This Unit's body model
        */
        s_wptr<Model>    GetBodyModel();

        /// Returns this Unit's node.
        /** \return This Unit's node
        */
        s_ptr<Node>      GetNode();

        /// Returns this Unit's camera.
        /** \return This Unit's camera
        */
        s_ptr<Camera>    GetCamera();

        /// Returns the ID that represents this Unit in the Lua code.
        /** \return The ID that represents this Unit in the Lua code
        */
        s_str            GetLuaID() const;

        /// Creates the associated Lua glue.
        /** \param pLua The Lua::State on which to create the glue
        */
        virtual void     CreateGlue(s_ptr<Lua::State> pLua);

        /// Pushes this Unit on the provided Lua::State.
        /** \param pLua The Lua::State on which to push the glue
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

        /// Returns this object's type.
        /** \return This object's type
        *   \note Simply returns CLASS_NAME.
        */
        virtual const s_str& GetType() const;

        /// Creates an EditorAction capable of both creating and deleting this object.
        /** \return An EditorAction capable of both creating and deleting this object
        */
        virtual s_refptr<EditorAction> CreateDeleteAction();

        static const s_str CLASS_NAME;

    protected :

        s_ctnr< s_ptr<LuaUnit> > lGlueList_;

        Class                        mClass_;
        s_refptr<HealthTypeInstance> pHealthType_;
        s_float                      fHealth_;
        s_refptr<PowerTypeInstance>  pPowerType_;
        s_float                      fPower_;
        Stats                        mStats_;
        s_uint                       uiLevel_;

        s_refptr<Model> pBodyModel_;
        s_ptr<Node>     pNode_;
        s_ptr<Camera>   pCamera_;

        s_refptr<Decal> pSelectionDecal_;
        s_refptr<Decal> pShadowDecal_;
        s_bool          bMotionBlurEnabled_;

        s_bool          bSelected_;
        s_bool          bHighlighted_;
        s_bool          bDead_;

        s_bool          bCameraMovedAlone_;

        s_bool          bPlayerControlled_;
    };

    /** \cond NOT_REMOVE_FROM_DOC
    */

    class LuaUnit
    {
    public :

        LuaUnit(lua_State* pLua);

        void NotifyDeleted();

        int _AddBuff(lua_State*);
        int _AddHealth(lua_State*);
        int _AddPower(lua_State*);
        int _Damage(lua_State*);
        int _DisableMotionBlur(lua_State*);
        int _FillHealthGauge(lua_State*);
        int _FillPowerGauge(lua_State*);
        int _EmptyHealthGauge(lua_State*);
        int _EmptyPowerGauge(lua_State*);
        int _EnableMotionBlur(lua_State*);
        int _Die(lua_State*);
        int _GetCamera(lua_State*);
        int _GetClass(lua_State*);
        int _GetHealthRegenRatio(lua_State*);
        int _GetLevel(lua_State*);
        int _GetPowerRegenRatio(lua_State*);
        int _GetStat(lua_State*);
        int _GetUnitType(lua_State*);
        int _IsInCombat(lua_State*);
        int _IsSitting(lua_State*);
        int _RotateCamera(lua_State*);
        int _RotateModel(lua_State*);
        int _SetAnim(lua_State*);
        int _SetAttacking(lua_State*);
        int _SetPlayerControlled(lua_State*);
        int _ZoomCamera(lua_State*);

        int    GetDataTable(lua_State*);
        static const char className[];
        static const char* classList[];
        static Lunar<LuaUnit>::RegType methods[];
        static const s_str CLASS_NAME;

    protected :

        s_bool CheckParent_();

        s_uint      uiID_;
        s_str       sName_;
        s_ptr<Unit> pParent_;

        lua_State*  pLua_;
        int         iRef_;
    };

    /** \endcond
    */
}

#endif
