/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*         OgreMeshModelData header       */
/*                                        */
/*                                        */


#ifndef FROST_OGREMESHMODELDATA_H
#define FROST_OGREMESHMODELDATA_H

#include "frost.h"
#include "model/frost_modeldata.h"

namespace Frost
{
    /// Loads Ogre's .mesh model files.
    class OgreMeshModelData : public ModelData
    {
    public :

        /// Default constructor.
        OgreMeshModelData(const s_str& sFile);

        /// Destructor.
        ~OgreMeshModelData();

        static const s_str CLASS_NAME;

    };
}

#endif
