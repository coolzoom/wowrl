/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          GameplayManager header        */
/*                                        */
/*                                        */

#ifndef FROST_GAMEPLAYMANAGER_H
#define FROST_GAMEPLAYMANAGER_H

#include "frost.h"
#include "utils/frost_utils_manager.h"

namespace Frost
{
    /// Manages gameplays
    class GameplayManager : public Manager<GameplayManager>
    {
    friend class Manager<GameplayManager>;
    public:

        /// Returns the Gameplay with the name you provided.
        /** \param sName The name of the Gameplay you're after
        *   \return The Gameplay with the name you provided
        */
        s_ptr<Gameplay> GetGameplay(const s_str& sName);

        /// Sets the current Gameplay.
        /** \param pGameplay The Gameplay to use
        */
        void            SetCurrentGameplay(s_ptr<Gameplay> pGameplay);

        /// Sets the current Gameplay.
        /** \param sGameplay The name of the Gameplay to use
        */
        void            SetCurrentGameplay(const s_str& sGameplay);

        /// Reads all Gameplays.
        /** \return 'true' if everything went fine
        */
        s_bool          ParseData();

        /// Updates the current gameplay.
        /** \param fDelta The elapsed time since last call
        */
        void            Update(const s_float& fDelta);

        /// Returns the Lua state used by Gameplays.
        /** \return The Lua state used by Gameplays
        */
        s_ptr<Lua::State> GetLua();

        static const s_str CLASS_NAME;

    protected:

        /// Default constructor.
        /** \note This is a singleton class. That means that there can
        *         only be a single instance at the same time. That's why
        *         the constructor is not in public scope.<br>
        *         If you need to get a pointer to that unique instance,
        *         call GameplayManager::GetSingleton().<br>
        *         Note that, if GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close
        *         your program by calling GameplayManager::Delete() (this is
        *         automatically done by Engine).
        */
        GameplayManager();

        /// Destructor.
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        ~GameplayManager();

        /// Copy constructor.
        GameplayManager(const GameplayManager& mMgr);

        /// Assignment operator.
        GameplayManager& operator = (const GameplayManager& mMgr);

    private:

        void ParseXMLFile_(const s_str& sFile);
        void ParseGameplayBlock_(s_ptr<XML::Block> pGameplayBlock);

        s_ptr<Gameplay>                 pCurrentGameplay_;
        s_map< s_str, s_ptr<Gameplay> > lGameplayList_;

        s_ptr<Lua::State> pLua_;

    };
}

#endif
