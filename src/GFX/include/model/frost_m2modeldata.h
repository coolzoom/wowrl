/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            M2ModelData header          */
/*                                        */
/*                                        */


#ifndef FROST_M2MODELDATA_H
#define FROST_M2MODELDATA_H

#include "frost.h"
#include "model/frost_modeldata.h"

namespace Frost
{
    /// Loads Blizzard's .m2 model files.
    class M2ModelData : public ModelData
    {
    public :

        /// Default constructor.
        M2ModelData(const s_str& sFile);

        /// Destructor.
        ~M2ModelData();

        static const s_str CLASS_NAME;

    };
}

#endif
