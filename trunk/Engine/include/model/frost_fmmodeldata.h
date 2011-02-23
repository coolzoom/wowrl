/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            FMModelData header          */
/*                                        */
/*                                        */


#ifndef FROST_FMMODELDATA_H
#define FROST_FMMODELDATA_H

#include "frost.h"
#include "model/frost_modeldata.h"

namespace Frost
{
    /// Loads Frost's FM model files.
    class FMModelData : public ModelData
    {
    public :

        /// Default constructor.
        FMModelData(const s_str& sFile);

        /// Destructor.
        ~FMModelData();

        static const s_str CLASS_NAME;

    };
}

#endif
