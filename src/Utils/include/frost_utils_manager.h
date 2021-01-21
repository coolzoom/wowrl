/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Manager header             */
/*                                        */
/*                                        */

/* Warning : This is a precompiled header.
*            Modifying it will trigger a
*            full rebuild !
*/

#ifndef FROST_MANAGER_H
#define FROST_MANAGER_H

#include "frost_utils_types.h"

namespace Frost
{
    /// Abstract singleton pattern
    template<class T>
    class Manager
    {
    friend class s_ptr< Manager<T> >;
    public :

        /// Returns a pointer to the singleton
        /** \note The singleton will be created on the very first call.
        */
        static s_ptr<T> GetSingleton()
        {
            if (!pMgr_)
                pMgr_ = new T();

            return pMgr_;
        }

        /// Deletes the singleton
        /** \note You should call that method when you're not using the
        *         singleton anymore, or if you want to reset it.<br>
        *         Use only if you've called GetSingleton at least once.
        */
        static void Delete()
        {
            delete pMgr_.Get(); pMgr_.SetNull();
        }

    protected :

        /// Default constructor
        /** \note This is a singleton class. That means that there can
        *         only be a single instance at the same time. That's why
        *         the constructor is not in public scope.<br>
        *         If you need to get a pointer to that unique instance,
        *         call Manager::GetSingleton().<br>
        *         Note that, if GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close
        *         your program by calling Manager::Delete().
        */
        Manager() {}

        /// Destructor
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close your
        *         program by calling Manager::Delete() (this is automatically done
        *         by Engine).
        */
        ~Manager() {}

        /// Copy constructor
        Manager(const Manager& mMgr);

        /// Assignment operator
        Manager& operator = (const Manager& mMgr);

    private :

        static s_ptr<T> pMgr_;

    };

    template<class T>
    s_ptr<T> Manager<T>::pMgr_ = nullptr;
}

#endif
