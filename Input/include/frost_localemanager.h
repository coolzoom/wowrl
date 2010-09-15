/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          Locale manager header         */
/*                                        */
/*                                        */


#ifndef LOCALEMANAGER_H
#define LOCALEMANAGER_H

#include <frost_utils.h>
#include <frost_utils_manager.h>

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

        /// Gets the string corresponding to the provided keyboard button.
        /** \param uiKey The keyboard button
        *   \note This is the normal key, with no modifier key pressed.
        */
        s_str        GetKeyString(const s_uint& uiKey) const;

        /// Gets the string corresponding to the provided keyboard button.
        /** \param uiKey The keyboard button
        *   \note This is the "shifted" key (upper case, mostly).
        */
        s_str        GetKeyShiftString(const s_uint& uiKey) const;

        /// Gets the string corresponding to the provided keyboard button.
        /** \param uiKey The keyboard button
        *   \note This is the "alted" key (non QWERTY keyboards).
        */
        s_str        GetKeyAltString(const s_uint& uiKey) const;

        /// Gets the string corresponding to the combination of two keys.
        /** \param sComboKey   The combo key : "^", "`", ...
        *   \param uiSecondKey The keyboard button that requests combination
        *   \note This is the normal key, with no modifier key pressed.
        *   \note If no combination is available, this function returns both
        *         the combo key and the second key, concatenated.
        */
        s_str        GetKeyComboString(const s_str& sComboKey, const s_uint& uiSecondKey) const;

        /// Gets the string corresponding to the combination of two keys.
        /** \param sComboKey   The combo key : "^", "`", ...
        *   \param uiSecondKey The keyboard button that requests combination
        *   \note This is the "shifted" key (upper case, mostly).
        *   \note If no combination is available, this function returns both
        *         the combo key and the second key, concatenated.
        */
        s_str        GetKeyComboShiftString(const s_str& sComboKey, const s_uint& uiSecondKey) const;

        /// Gets the string corresponding to the combination of two keys.
        /** \param sComboKey   The combo key : "^", "`", ...
        *   \param uiSecondKey The keyboard button that requests combination
        *   \note This is the "alted" key (non QWERTY keyboards).
        *   \note If no combination is available, this function returns both
        *         the combo key and the second key, concatenated.
        */
        s_str        GetKeyComboAltString(const s_str& sComboKey, const s_uint& uiSecondKey) const;

        /// Checks if the provided string is a combo key ("^", "`", ...).
        /** \param sComboKey The string to test
        *   \return 'true' if the provided string is a combo key
        */
        s_bool       IsComboKey(const s_str& sComboKey) const;

        /// Returns the current game locale.
        /** \return The current game locale
        */
        const s_str& GetLocale() const;

        /// Sets the current game locale.
        /** \param sLocale The new locale
        *   \note Calls ReadLocale().
        */
        void         SetLocale(const s_str& sLocale);

        /// Reads localization files.
        /** \note ReadConfig() or SetLocale() must have been called
        *         before this function.
        */
        void         ReadLocale();

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

        s_map< s_uint, s_array<s_str,3> >                 lKeyMap_;
        s_map< s_str, s_map< s_uint, s_array<s_str,3> > > lComboKeyMap_;
    };
}

#endif
