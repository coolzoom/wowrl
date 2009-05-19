/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            LuaManager header           */
/*                                        */
/*                                        */


#ifndef FROST_UTILS_LUAMANAGER_H
#define FROST_UTILS_LUAMANAGER_H

#include "frost_utils.h"
#include "frost_utils_manager.h"
#include "frost_utils_lua_state.h"

namespace Frost
{
    /// Manages Lua states
    /** This class is responsible for Lua state
    *   creation and destruction. It holds a default
    *   Lua state that you must set by yourself.
    */
    class LuaManager : public Manager<LuaManager>
    {
    friend class Manager<LuaManager>;
    public :

        /// Creates a new Lua state.
        /** \return The new Lua state
        */
        s_ptr<Lua::State> CreateLua();

        /// Closes a Lua state.
        /** \param pLua The state to close
        *   \note If you forget to close your Lua state, it will
        *         be closed when this manager is destroyed.
        */
        void              CloseLua(s_ptr<Lua::State> pLua);

        /// Returns the Lua state wrapper associated with a raw Lua state.
        /** \param pLua The raw Lua state
        *   \return The associated wrapper (NULL if none)
        */
        s_ptr<Lua::State> GetState(lua_State* pLua);

        /// Returns the default Lua state.
        /** \return The default Lua state
        */
        s_ptr<Lua::State> GetDefaultLua() const;

        /// Sets the default Lua state.
        /** \param The new default Lua state
        */
        void              SetDefaultLua(s_ptr<Lua::State> pLua);

        static const s_str CLASS_NAME;

    protected :

        /// Default constructor
        /** \note This is a singleton class. That means that there can
        *         only be a single instance at the same time. That's why
        *         the constructor is not in public scope.<br>
        *         If you need to get a pointer to that unique instance,
        *         call LuaManager::GetSingleton().<br>
        *         Note that, if GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close
        *         your program by calling LuaManager::Delete() (this is
        *         automatically done by Engine).
        */
        LuaManager();

        /// Destructor
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        virtual ~LuaManager();

        /// Copy constructor
        LuaManager(const LuaManager& mMgr);

        /// Assignment operator
        LuaManager& operator = (const LuaManager& mMgr);

    private :

        s_ptr<Lua::State>                         pDefaultLua_;
        std::map< lua_State*, s_ptr<Lua::State> > lLuaStateList_;

    };
}

#endif
