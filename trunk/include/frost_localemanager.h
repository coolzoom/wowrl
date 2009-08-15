/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          Locale manager header         */
/*                                        */
/*                                        */


#ifndef FROST_LOCALEMANAGER_H
#define FROST_LOCALEMANAGER_H

#include "frost.h"
#include "utils/frost_utils_manager.h"

namespace Frost
{
    /// Handles localization
    class LocaleManager : public Manager<LocaleManager>
    {
    friend class Manager<LocaleManager>;
    public :

        /// Initializes this manager.
        void         Initialize();

        /// Gets a localized string.
        /** \param sStr The ID of the localized string
        *   \return The associated string
        */
        s_str        GetLocalizedString(const s_str& sStr);

        /// Returns the current game locale.
        /** \return The current game locale
        */
        const s_str& GetLocale() const;

        /// Sets the current game locale.
        /** \param sLocale The new locale
        *   \note Calls ReadLocale().
        */
        void         SetLocale(const s_str& sLocale);

        /// Reads the default locale from Config.lua.
        /** \return 'true' if everything went fine
        */
        s_bool       ReadConfig();

        /// Reads localization files.
        /** \return 'true' if everything went fine
        *   \note ReadConfig() or SetLocale() must have been called
        *         before this function.
        */
        s_bool       ReadLocale();

        static const s_str CLASS_NAME;

    protected :

        /// Default constructor.
        /** \note This is a singleton class. That means that there can
        *         only be a single instance at the same time. That's why
        *         the constructor is not in public scope.<br>
        *         If you need to get a pointer to that unique instance,
        *         call LocaleManager::GetSingleton().<br>
        *         Note that, if GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close
        *         your program by calling LocaleManager::Delete() (this is
        *         automatically done by Engine).
        */
        LocaleManager();

        /// Destructor.
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        ~LocaleManager();

        /// Copy constructor.
        LocaleManager(const LocaleManager& mMgr);

        /// Assignment operator.
        LocaleManager& operator = (const LocaleManager& mMgr);

    private :

        s_str             sLocale_;
        s_ptr<Lua::State> pLua_;
    };
}

#endif
