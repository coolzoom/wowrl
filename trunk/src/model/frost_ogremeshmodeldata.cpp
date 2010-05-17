/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            ModelData source            */
/*                                        */
/*                                        */

#include "model/frost_ogremeshmodeldata.h"

#include "model/frost_modelmanager.h"
#include "model/frost_model.h"
#include "model/frost_animmanager.h"
#include "material/frost_materialmanager.h"
#include "material/frost_material.h"

#include <OgreMeshManager.h>

#include <utils/frost_utils_file.h>

using namespace std;

namespace Frost
{
    const s_str OgreMeshModelData::CLASS_NAME = "OgreMeshModelData";

    OgreMeshModelData::OgreMeshModelData( const s_str& sFile ) :
        ModelData(sFile)
    {
        if (!File::Exists(sFile))
        {
            throw ModelLoadingException(CLASS_NAME, "Model file \""+sFile+"\" couldn't be loaded.");
        }

        try
        {
            // Load mesh from file
            pMesh_ = Ogre::MeshManager::getSingleton().load(sFile.Get(), "Frost").get();
        }
        catch (Ogre::Exception& e)
        {
            throw ModelLoadingException(CLASS_NAME, e.getDescription());
        }
    }

    OgreMeshModelData::~OgreMeshModelData()
    {
    }
}
