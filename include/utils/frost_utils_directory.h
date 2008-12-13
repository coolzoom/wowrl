/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_DIRECTORY_H
#define FROST_UTILS_DIRECTORY_H

#include "frost_utils.h"
#include "frost_utils_uint.h"
#include "frost_utils_str.h"
#include "frost_utils_ptr.h"

namespace Frost
{
    class s_str;
    class s_uint;

    /// A tool for directory listing
    /** This class iterates through every sub-
    *   directories, store them in a vector and
    *   list their sub-directories, ...
    */
    class Directory
    {
    public :

        /// Call that constructor to open a directory.
        /** \param sName The directory path (relative to the program).
        */
        Directory(const s_str& sName);

        /// Iterates through sub-directories.
        /** \return The next sub-directory, 'NULL' if none
        */
        s_ptr<Directory> GetNextSubDirectory();

        /// Returns this directory's name.
        /** \return This directory's name
        */
        const s_str& GetName() const;

        static const s_str CLASS_NAME;

    private :

        /// The sub-directory list
        std::vector<Directory> lSubDirectoryList_;
        /// The sub-directory iterator
        s_uint uiIter_;
        /// This directory name
        s_str  sName_;
    };
}

#endif
