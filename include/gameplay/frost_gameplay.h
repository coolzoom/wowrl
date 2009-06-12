/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Gameplay header            */
/*                                        */
/*                                        */

#ifndef FROST_GAMEPLAY_H
#define FROST_GAMEPLAY_H

#include "frost.h"

namespace Frost
{
    /// Controls how the user can interract with the world
    class Gameplay : public EventReceiver
    {
    public :

        /// Constructor.
        Gameplay(const s_str& sName);

        /// Destructor.
        ~Gameplay();

        /// Creates the associated Lua glue.
        void CreateGlue();

        /// Sets the Camera used by this Gameplay.
        /** \param pCamera The Camera used by this Gameplay
        */
        void SetCamera(s_ptr<Camera> pCamera);

        /// Creates a new Camera and links it to this Gameplay.
        /** \param mPosition The initial position of this Camera
        *   \return The new Camera
        */
        s_ptr<Camera> CreateCamera(const Vector& mPosition = Vector::ZERO);

        /// Tells this Gameplay it is beeing currently used or not.
        /** \param bIsCurrentGameplay 'true' if its the new current gameplay
        */
        void NotifyCurrentGameplay(const s_bool& bIsCurrentGameplay);

        /// Tells this Frame it can call a script.
        /** \param sScriptName The name of the script
        */
        void NotifyScriptDefined(const s_str& sScriptName);

        /// Calls a script.
        /** \param sScriptName The name of the script
        */
        void On(const s_str& sScriptName);

        /// Calls the OnEvent script.
        /** \param mEvent The Event that occured
        */
        void OnEvent(const Event& mEvent);

        /// Sets whether single selection is allowed.
        /** \param bAllow 'true' to allow it
        */
        void SetAllowSingleSelection(const s_bool& bAllow);

        /// Sets whether mutliple selection is allowed.
        /** \param bAllow 'true' to allow it
        */
        void SetAllowMultipleSelection(const s_bool& bAllow);

        /// Checks if single selection is allowed.
        /** \return 'true' if single selection is allowed
        */
        const s_bool& AllowSingleSelection() const;

        /// Checks if mutliple selection is allowed.
        /** \return 'true' if mutliple selection is allowed
        */
        const s_bool& AllowMultipleSelection() const;

        /// Tells this Gameplay to react to a certain event.
        /** \param sEventName The name of the event
        */
        void RegisterEvent(const s_str& sEventName);

        /// Returns the name of this Gameplay.
        /** \return The name of this Gameplay
        */
        const s_str& GetName() const;

        static const s_str CLASS_NAME;

    protected :

        s_str  sName_;
        s_bool bIsCurrentGameplay_;

        s_ptr<Camera> pCamera_;
        s_ctnr< s_ptr<Camera> > lCameraList_;

        s_ctnr<s_str> lRegisteredEventList_;
        s_ctnr<s_str> lDefinedScriptList_;

        s_ptr<LuaGameplay> pGlue_;

        s_bool bAllowSingleSelection_;
        s_bool bAllowMultipleSelection_;

    };

    /** \cond NOT_REMOVE_FROM_DOC
    */

    /// Gameplay Lua glue
    class LuaGameplay
    {
    public :

        /// Constructor.
        LuaGameplay(lua_State* pLua);

        int _RegisterEvent(lua_State*);
        int _SetCamera(lua_State*);
        int _CreateCamera(lua_State*);

        int GetDataTable(lua_State *L);
        static const char className[];
        static Lunar<LuaGameplay>::RegType methods[];
        static const s_str CLASS_NAME;

    protected :

        s_ptr<Gameplay> pParent_;

        lua_State* pLua_;
        int        iRef_;
    };

    /** \endcond
    */
}

#endif
