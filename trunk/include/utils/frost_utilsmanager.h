/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               XXX header               */
/*                                        */
/*                                        */


#ifndef FROST_UTILSMANAGER_H
#define FROST_UTILSMANAGER_H

#include "frost_utils.h"
#include "frost_utils_manager.h"

namespace Frost
{
    class UtilsManager : public Manager<UtilsManager>
    {
    friend class Manager<UtilsManager>;
    public :

        typedef void (*LogFunction)(const s_str&, const s_bool&);
        void SetLogFunction(LogFunction pFunc);
        void Log(const s_str& sMessage, const s_bool& bTimeStamps);

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

    };
}

#endif
