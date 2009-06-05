/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_DIRECTORY_H
#define FROST_UTILS_DIRECTORY_H

// Iterates through a Directory's sub directories
#define foreach_dir(subdir, dir) for ((subdir) = (dir).GetNextSubDirectory(); (subdir); (subdir) = (dir).GetNextSubDirectory())

#include "frost_utils.h"
#include "frost_utils_types.h"

namespace Frost
{
    /// A tool for directory listing
    /** This class iterates through every sub-
    *   directories, store them in a vector and
    *   list their sub-directories, ...
    */
    class Directory
    {
    public :

        /// Call that constructor to open a directory.
        /** \param sRelPath The directory path (relative to the program).
        */
        Directory(const s_str& sRelPath);

        /// Iterates through sub-directories.
        /** \return The next sub-directory, 'NULL' if none
        */
        s_ptr<Directory> GetNextSubDirectory();

        /// Returns the list of the contained files.
        /** \return The list of the contained files
        */
        const std::vector<s_str>& GetFileList() const;

        /// Returns this directory's name.
        /** \return This directory's name
        *   \note For example : Interface/Base_UI.<br>
        *         This function would return "Base_UI".
        */
        const s_str& GetName() const;

        /// Returns this directory's relative path.
        /** \return This directory's relative path
        *   \note For example : Interface/Base_UI.<br>
        *         This function would return "Interface/Base_UI".
        */
        const s_str& GetRelPath() const;

        static const s_str CLASS_NAME;

    private :

        /// The sub-directory list
        std::vector<Directory> lSubDirectoryList_;
        /// The contained file list
        std::vector<s_str>     lFileList_;
        /// The sub-directory iterator
        s_uint uiIter_;
        /// This directory name
        s_str  sName_;
        /// This directory relative path (with name)
        s_str  sRelPath_;
    };
}

#endif
