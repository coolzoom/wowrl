/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_DIRECTORY_H
#define FROST_UTILS_DIRECTORY_H

// Iterates through a Directory's sub directories
#define foreach_dir(subdir, dir) for ((subdir) = (dir).GetNextSubDirectory(); (subdir); (subdir) = (dir).GetNextSubDirectory())

#include "frost_utils_config.h"
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
        /** \return The next sub-directory, 'nullptr' if none
        */
        s_wptr<Directory> GetNextSubDirectory();

        /// Returns the list of the contained files.
        /** \param bWithPath       'true' to include this Directory's relative path
        *   \param sExtensionFilter Use it to filter files depending on their extention
        *   \return The list of the contained files
        *   \note Example : if you use "xml" as the extention filter, then only *.xml
        *         files will appear on the list.<br>
        *         Example : if you use "xml, lua", then you'll only get *.xml and *.lua
        *         files.
        */
        s_ctnr<s_str> GetFileList(const s_bool& bWithPath = false, const s_str& sExtensionFilter = "") const;

        /// Returns the list of the contained files (recursive).
        /** \param bWithPath       'true' to include this Directory's relative path
        *   \param sExtensionFilter Use it to filter files depending on their extention
        *   \return The list of the contained files (recursive)
        *   \note Example : if you use "xml" as the extention filter, then only *.xml
        *         files will appear on the list.<br>
        *         Example : if you use "xml, lua", then you'll only get *.xml and *.lua
        *         files.<br>
        *         This function searches through subdirectories too.
        */
        s_ctnr<s_str> GetRecursiveFileList(const s_bool& bWithPath = false, const s_str& sExtensionFilter = "") const;

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

        /// Checks if a directory exists.
        /** \param sPath The path to the directory to check
        *   \return 'true' if the directory exists
        */
        static s_bool Exists(const s_str& sPath);

        /// Creates a new directory.
        /** \param sPath The path to the directory to create
        *   \return 'true' if the operation was successful
        *   \note This fuction doesn't check if the directory
        *         already exists.
        */
        static s_bool Make(const s_str& sPath);

        static const s_str CLASS_NAME;

    private :

        /// Creates a new directory.
        /** \param sPath The path to the directory to create
        *   \return 'true' if the operation was successful
        *   \note This version is very low level, it does absolutely
        *         no checks, nor does it support nested directories.
        */
        static s_bool Make_(const s_str& sPath);

        /// The sub-directory list
        s_ctnr< s_refptr<Directory> > lSubDirectoryList_;
        /// The contained file list
        s_ctnr<s_str>                 lFileList_;
        /// The sub-directory iterator
        s_uint uiIter_;
        /// This directory name
        s_str  sName_;
        /// This directory relative path (with name)
        s_str  sRelPath_;
    };
}

#endif
