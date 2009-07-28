/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           Font manager header          */
/*                                        */
/*                                        */


#ifndef FROST_FONTMANAGER_H
#define FROST_FONTMANAGER_H

#include "frost.h"
#include "utils/frost_utils_manager.h"

namespace Frost
{
    /// Manages font creation
    class FontManager : public Manager<FontManager>
    {
    friend class Manager<FontManager>;
    public :

        /// Creates or retreives an Ogre::Font.
        /** \param sFontFile The path to the .tff file
        *   \param uiSize    The size at which to render the font
        *   \note This function will create the font if it doens't exists,
        *         or return a pointer to it if has already been created.
        */
        s_ptr<Ogre::Font> GetFont(const s_str& sFontFile, const s_uint& uiSize);

        /// Returns the name of the default font.
        /** \return The name of the default font
        *   \note This value is read from config files.
        */
        const s_str&      GetDefaultFont() const;

        /// Reads the configuration files.
        /** \return 'true' if everything went fine
        */
        s_bool            ReadConfig();

        static const s_str CLASS_NAME;

    protected :

        /// Default constructor
        /** \note This is a singleton class. That means that there can
        *         only be a single instance at the same time. That's why
        *         the constructor is not in public scope.<br>
        *         If you need to get a pointer to that unique instance,
        *         call FontManager::GetSingleton().<br>
        *         Note that, if GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close
        *         your program by calling FontManager::Delete() (this is
        *         automatically done by Engine).
        */
        FontManager();

        /// Destructor
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        ~FontManager();

        /// Copy constructor
        FontManager(const FontManager& mMgr);

        /// Assignment operator
        FontManager& operator = (const FontManager& mMgr);

    private :

        s_str sDefaultFont_;

        s_map< s_str, s_ptr<Ogre::Font> > lFontList_;
    };
}

#endif
