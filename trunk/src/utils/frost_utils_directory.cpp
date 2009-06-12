/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_directory.h"
#include "frost_utils_stdhelper.h"
#include "frost_utils_log.h"

#include <OgreArchiveManager.h>
#include <OgreStringVector.h>

using namespace std;

namespace Frost
{
    const s_str Directory::CLASS_NAME = "Directory";

    Directory::Directory( const s_str& sRelPath )
    {
        uiIter_ = 0u;
        sRelPath_ = sRelPath;
        sName_ = sRelPath_.Cut("/").Back();

        s_ptr<Ogre::Archive> pFrostMain = Ogre::ArchiveManager::getSingleton().load("./", "FileSystem");
        if (pFrostMain->exists(sRelPath_.GetASCII()))
        {
            s_ptr<Ogre::Archive> pArchive = Ogre::ArchiveManager::getSingleton().load(sRelPath_.GetASCII(), "FileSystem");
            if (pArchive)
            {
                Ogre::StringVectorPtr pSV = pArchive->list(false, true);
                Ogre::StringVector::iterator iter;
                foreach (iter, *pSV)
                {
                    lSubDirectoryList_.PushBack(Directory(sRelPath_ + "/" + s_str(*iter)));
                }

                pSV = pArchive->list(false, false);
                foreach (iter, *pSV)
                {
                    lFileList_.PushBack(*iter);
                }
            }
            else
                Error(CLASS_NAME, "Couldn't create Archive for \""+sRelPath_+"\".");
        }
        else
        {
            Error(CLASS_NAME, "Couldn't find folder \""+sRelPath_+"\".");
        }
    }

    s_ptr<Directory> Directory::GetNextSubDirectory()
    {
        s_ptr<Directory> pSub;
        if (uiIter_ < lSubDirectoryList_.GetSize())
        {
            pSub = &lSubDirectoryList_[uiIter_];
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

    const s_str& Directory::GetName() const
    {
        return sName_;
    }

    const s_str& Directory::GetRelPath() const
    {
        return sRelPath_;
    }
}
