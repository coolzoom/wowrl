/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */


#ifndef M2LOADER_H
#define M2LOADER_H

#include "converter.h"
#include "modeldata.h"

namespace Frost
{
    class M2Loader
    {
    public :

        static ModelData LoadModelData(const s_str& sFile);

        static const s_str CLASS_NAME;

    };
}

#endif
