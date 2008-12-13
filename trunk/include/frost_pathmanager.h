/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               XXX header               */
/*                                        */
/*                                        */


#ifndef FROST_PATHMANAGER_H
#define FROST_PATHMANAGER_H

#include "frost.h"
#include "utils/frost_utils_manager.h"

namespace Frost
{
    /// Manages Path creation and pathfinding
    class PathManager : public Manager<PathManager>
    {
    friend class Manager<PathManager>;
    public:

        /// Creates a new DirectPath.
        /** \return The new DirectPath
        */
        s_refptr<DirectPath> CreateDirectPath();

        /// Creates a new ManualPath.
        /** \return The new DirectPath
        */
        s_refptr<ManualPath> CreateManualPath();

        /// Creates a new SmoothPath.
        /** \return The new DirectPath
        */
        s_refptr<SmoothPath> CreateSmoothPath();

        /// Creates a full copy of an existing path.
        /** \param pPath The path to copy
        *   \return The new path
        */
        s_refptr<Path>       CopyPath(s_refptr<Path> pPath);

        static const s_str CLASS_NAME;

    protected:

        /// Default constructor
        /** \note This is a singleton class. That means that there can
        *         only be a single instance at the same time. That's why
        *         the constructor is not in public scope.<br>
        *         If you need to get a pointer to that unique instance,
        *         call PathManager::GetSingleton().<br>
        *         Note that, if GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close
        *         your program by calling SpriteManager::Delete() (this is
        *         automatically done by Engine).
        */
        PathManager();

        /// Destructor
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        ~PathManager();

    private:

        s_uint uiCounter_;

    };
}

#endif
