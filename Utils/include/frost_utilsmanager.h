/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               XXX header               */
/*                                        */
/*                                        */


#ifndef FROST_UTILSMANAGER_H
#define FROST_UTILSMANAGER_H

#include "frost_utils_config.h"
#include "frost_utils_manager.h"

namespace Frost
{
    /// Manages base functions (log...).
    class UtilsManager : public Manager<UtilsManager>
    {
    friend class Manager<UtilsManager>;
    public :

        typedef void (*LogFunction)(const s_str&, const s_bool&, const s_uint&);

        /// Initializes this manager.
        /** \note Calls srand().
        */
        void Initialize();

        /// Sets the log function to use.
        /** \param pFunc The log function
        *   \note This function will be used by Frost::Log(), Frost::Warning()
        *         and Frost::Error().<br>
        *         Its signature must be :<br>
        *         &nsbp&nsbp&nsbpvoid Func(const s_str&, const s_bool&);<br>
        *         See UtilsManager::Log() for more infos about these params.
        */
        void SetLogFunction(LogFunction pFunc);

        /// Shortcut to the user specified log function.
        /** \param sMessage    The message to log
        *   \param bTimeStamps 'true' to display time stamps
        *   \param uiOffset    The line jump offset
        */
        void Log(const s_str& sMessage, const s_bool& bTimeStamps, const s_uint& uiOffset);

        /// Sets the highest log level allowed for all following messages.
        /** \param uiLevel The new highest level
        *   \note When using Log<5>("blabla"), you specify that this message
        *         has a log level of 5. By default, the highest level allowed
        *         is 0 : if you want your message to be printed on the log file,
        *         you must call SetLogLevel(5) or higher.
        *         This allows quick selection of which log messages to print.
        *         The admitted convention is that higher level messages are
        *         less important or extremely precise, so you may not want them
        *         to be printed all the time.
        */
        void SetLogLevel(const s_uint& uiLevel);

        /// Returns the current highest log level allowed.
        /** \return The current highest log level allowed
        *   \note See SetLogLevel.
        */
        const s_uint& GetLogLevel() const;

        static const s_str CLASS_NAME;

    protected :

        /// Default constructor
        /** \note This is a singleton class. That means that there can
        *         only be a single instance at the same time. That's why
        *         the constructor is not in public scope.<br>
        *         If you need to get a pointer to that unique instance,
        *         call UtilsManager::GetSingleton().<br>
        *         Note that, if GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close
        *         your program by calling UtilsManager::Delete() (this is
        *         automatically done by Engine).
        */
        UtilsManager();

        /// Destructor
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        virtual ~UtilsManager();

        /// Copy constructor
        UtilsManager(const UtilsManager& mMgr);

        /// Assignment operator
        UtilsManager& operator = (const UtilsManager& mMgr);

    private :

        LogFunction pLogFunction_;
        s_uint      uiLogLevel_;

    };
}

#endif
