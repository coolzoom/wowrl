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

        void         Initialize();

        s_str        GetLocalizedString(const s_str& sStr);
        const s_str& GetLocale() const;
        void         SetLocale(const s_str& sLocale);

        s_bool       ReadConfig();
        s_bool       ReadLocale();

        static const s_str CLASS_NAME;

    protected :

        /// Default constructor
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

        /// Destructor
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        ~LocaleManager();

    private :

        s_str             sLocale_;
        s_ptr<Lua::State> pLua_;
    };
}

#endif
