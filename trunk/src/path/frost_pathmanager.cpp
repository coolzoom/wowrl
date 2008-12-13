/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               XXX source               */
/*                                        */
/*                                        */

#include "frost_pathmanager.h"
#include "frost_path_directpath.h"
#include "frost_path_manualpath.h"
#include "frost_path_smoothpath.h"

using namespace std;

namespace Frost
{
    const s_str PathManager::CLASS_NAME = "PathManager";

    PathManager::PathManager()
    {
    }

    PathManager::~PathManager()
    {
    }

    s_refptr<DirectPath> PathManager::CreateDirectPath()
    {
        s_refptr<DirectPath> pPath(new DirectPath(uiCounter_));
        uiCounter_++;

        return pPath;
    }

    s_refptr<ManualPath> PathManager::CreateManualPath()
    {
        s_refptr<ManualPath> pPath(new ManualPath(uiCounter_));
        uiCounter_++;

        return pPath;
    }

    s_refptr<SmoothPath> PathManager::CreateSmoothPath()
    {
        s_refptr<SmoothPath> pPath(new SmoothPath(uiCounter_));
        uiCounter_++;

        return pPath;
    }

    s_refptr<Path> PathManager::CopyPath( s_refptr<Path> pPath )
    {
        s_refptr<Path> pNewPath;

        if (pPath)
        {
            if (pPath->GetType() == PATH_DIRECT)
                pNewPath = s_refptr<Path>((Path*)new DirectPath(uiCounter_, *((DirectPath*)pPath.Get())));
            else if (pPath->GetType() == PATH_SMOOTH)
                pNewPath = s_refptr<Path>((Path*)new SmoothPath(uiCounter_, *((SmoothPath*)pPath.Get())));
            else if (pPath->GetType() == PATH_MANUAL)
                pNewPath = s_refptr<Path>((Path*)new ManualPath(uiCounter_, *((ManualPath*)pPath.Get())));

            uiCounter_++;

            if (!pNewPath)
                Error(CLASS_NAME, "Couldn't copy Path (ID:"+pPath->GetID()+").");
        }
        else
            Error(CLASS_NAME, "Can't copy a NULL Path.");

        return pNewPath;
    }
}
