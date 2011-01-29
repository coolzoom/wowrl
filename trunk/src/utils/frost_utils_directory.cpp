/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_directory.h"
#include "frost_utils_stdhelper.h"
#include "frost_utils_log.h"

#include <sys/types.h>
#include <sys/stat.h>

#ifdef FROST_LINUX
    // Emulate directory and file listing windows functions
    // Note : Taken directly from Ogre3D
    // http://www.ogre3d.org/

    #include <dirent.h>
    #include <unistd.h>
    #include <fnmatch.h>
    #include <string.h>

    struct _finddata_t
    {
        char *name;
        int attrib;
        unsigned long size;
    };
    struct _find_search_t
    {
        char *pattern;
        char *curfn;
        char *directory;
        int dirlen;
        DIR *dirfd;
    };

    #define _A_NORMAL 0x00  /* Normalfile-Noread/writerestrictions */
    #define _A_RDONLY 0x01  /* Read only file */
    #define _A_HIDDEN 0x02  /* Hidden file */
    #define _A_SYSTEM 0x04  /* System file */
    #define _A_SUBDIR 0x10  /* Subdirectory */
    #define _A_ARCH   0x20  /* Archive file */

    int _findclose(long id);
    int _findnext(long id, struct _finddata_t *data);

    long _findfirst(const char *pattern, struct _finddata_t *data)
    {
        _find_search_t *fs = new _find_search_t;
        fs->curfn = NULL;
        fs->pattern = NULL;

        const char *mask = strrchr(pattern, '/');
        if (mask)
        {
            fs->dirlen = mask - pattern;
            mask++;
            fs->directory = (char *)malloc(fs->dirlen + 1);
            memcpy(fs->directory, pattern, fs->dirlen);
            fs->directory [fs->dirlen] = 0;
        }
        else
        {
            mask = pattern;
            fs->directory = strdup(".");
            fs->dirlen = 1;
        }

        fs->dirfd = opendir(fs->directory);
        if (!fs->dirfd)
        {
            _findclose((long)fs);
            return -1;
        }

        if (strcmp(mask, "*.*") == 0)
            mask += 2;
        fs->pattern = strdup(mask);

        if (_findnext((long)fs, data) < 0)
        {
            _findclose((long)fs);
            return -1;
        }

        return (long)fs;
    }

    int _findnext(long id, struct _finddata_t *data)
    {
        _find_search_t *fs = (_find_search_t*)id;

        dirent *entry;
        for (;;)
        {
            if (!(entry = readdir(fs->dirfd)))
                return -1;

            if (fnmatch(fs->pattern, entry->d_name, 0) == 0)
                break;
        }

        if (fs->curfn)
            free(fs->curfn);
        data->name = fs->curfn = strdup(entry->d_name);

        size_t namelen = strlen(entry->d_name);
        char *xfn = new char [fs->dirlen + 1 + namelen + 1];
        sprintf(xfn, "%s/%s", fs->directory, entry->d_name);

        struct stat stat_buf;
        if (stat(xfn, &stat_buf))
        {
            data->attrib = _A_NORMAL;
            data->size = 0;
        }
        else
        {
            if (S_ISDIR(stat_buf.st_mode))
                data->attrib = _A_SUBDIR;
            else
                data->attrib = _A_NORMAL;

            data->size = stat_buf.st_size;
        }

        delete [] xfn;

        if (data->name [0] == '.')
            data->attrib |= _A_HIDDEN;

        return 0;
    }

    int _findclose(long id)
    {
        int ret;
        _find_search_t *fs = (_find_search_t *)id;

        ret = fs->dirfd ? closedir(fs->dirfd) : 0;
        free(fs->pattern);
        free(fs->directory);
        if (fs->curfn)
            free(fs->curfn);
        delete fs;

        return ret;
    }
#else
    #include <windows.h>
    #include <direct.h>
    #include <io.h>
#endif

using namespace std;

namespace Frost
{
    const s_str Directory::CLASS_NAME = "Directory";

    Directory::Directory( const s_str& sRelPath )
    {
        uiIter_ = 0u;
        sRelPath_ = sRelPath;
        sName_ = sRelPath_.Cut("/").Back();

        s_ctnr<s_str> lDirList;

        long lHandle, res;
        struct _finddata_t tagData;

        s_str sPattern;
        if (sRelPath.IsEmpty())
            sPattern = "*";
        else
            sPattern = sRelPath_ + "/*";

        lHandle = _findfirst(sPattern.c_str(), &tagData);
        res = 0;
        while (lHandle != -1 && res != -1)
        {
            if ((tagData.attrib & _A_HIDDEN) != 0)
            {
                res = _findnext(lHandle, &tagData);
                continue;
            }

            if ((tagData.attrib & _A_SUBDIR) != 0)
            {
                s_str s = tagData.name;
                if (s != "." && s != "..")
                    lDirList.PushBack(s);
            }
            else
                lFileList_.PushBack(tagData.name);

            res = _findnext(lHandle, &tagData);
        }

        if(lHandle != -1)
            _findclose(lHandle);

        s_ctnr<s_str>::iterator iter;
        foreach (iter, lDirList)
        {
            lSubDirectoryList_.PushBack(s_refptr<Directory>(new Directory(
                sRelPath_.IsEmpty() ? (*iter) : (sRelPath_ + "/" + (*iter))
            )));
        }
    }

    s_wptr<Directory> Directory::GetNextSubDirectory()
    {
        s_wptr<Directory> pSub;
        if (uiIter_ < lSubDirectoryList_.GetSize())
        {
            pSub = lSubDirectoryList_[uiIter_];
            ++uiIter_;
        }
        else
            uiIter_ = 0u;

        return pSub;
    }

    s_ctnr<s_str> Directory::GetFileList( const s_bool& bWithPath, const s_str& sExtensionFilter ) const
    {
        if (sExtensionFilter.IsEmpty())
        {
            if (bWithPath)
            {
                s_ctnr<s_str> lNewFileList;

                s_ctnr<s_str>::const_iterator iterFile;
                foreach (iterFile, lFileList_)
                {
                    lNewFileList.PushBack(sRelPath_ + "/" + (*iterFile));
                }

                return lNewFileList;
            }
            else
            {
                return lFileList_;
            }
        }
        else
        {
            s_ctnr<s_str> lNewFileList;

            s_ctnr<s_str> lExtension = sExtensionFilter.Cut(",");
            s_ctnr<s_str>::iterator iterExtension;
            foreach (iterExtension, lExtension)
            {
                iterExtension->Trim(' ');
            }

            s_ctnr<s_str>::const_iterator iterFile;
            foreach (iterFile, lFileList_)
            {
                s_str sExtension = iterFile->Cut(".", 1).Back();
                foreach (iterExtension, lExtension)
                {
                    if (sExtension == (*iterExtension))
                    {
                        if (bWithPath)
                            lNewFileList.PushBack(sRelPath_ + "/" + (*iterFile));
                        else
                            lNewFileList.PushBack(*iterFile);
                        break;
                    }
                }
            }

            return lNewFileList;
        }
    }

    s_ctnr<s_str> Directory::GetRecursiveFileList(const s_bool& bWithPath, const s_str& sExtensionFilter) const
    {
        s_ctnr<s_str> lFileList = GetFileList(bWithPath, sExtensionFilter);

        s_ctnr< s_refptr<Directory> >::const_iterator iterSubDir;
        foreach (iterSubDir, lSubDirectoryList_)
        {
            lFileList.PushBack((*iterSubDir)->GetRecursiveFileList(bWithPath, sExtensionFilter));
        }

        return lFileList;
    }

    const s_str& Directory::GetName() const
    {
        return sName_;
    }

    const s_str& Directory::GetRelPath() const
    {
        return sRelPath_;
    }
}
