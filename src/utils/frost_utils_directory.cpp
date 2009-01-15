/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_directory.h"

#include "frost_utils_ptr.h"

#include "OgreArchiveManager.h"
#include "OgreStringVector.h"

using namespace std;

namespace Frost
{
    const s_str Directory::CLASS_NAME = "Directory";

    Directory::Directory( const s_str& sRelPath )
    {
        uiIter_ = 0u;
        sRelPath_ = sRelPath;
        sName_ = sRelPath_.Cut("/").back();

        s_ptr<Ogre::Archive> pFrostMain = Ogre::ArchiveManager::getSingleton().load("./", "FileSystem");
        if (pFrostMain->exists(sRelPath_.Get()))
        {
            s_ptr<Ogre::Archive> pArchive = Ogre::ArchiveManager::getSingleton().load(sRelPath_.Get(), "FileSystem");
            if (pArchive)
            {
                Ogre::StringVectorPtr pSV = pArchive->list(false, true);
                Ogre::StringVector::iterator iter;
                for (iter = pSV->begin(); iter != pSV->end(); iter++)
                {
                    lSubDirectoryList_.push_back(Directory(sRelPath_ + "/" + (*iter)));
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
        if (uiIter_ < lSubDirectoryList_.size())
        {
            pSub = &lSubDirectoryList_[uiIter_.Get()];
            uiIter_++;
        }
        else
            uiIter_ = 0u;

        return pSub;
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
