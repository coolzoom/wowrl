/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */


#ifndef FTLOADER_H
#define FTLOADER_H

#include "converter.h"
#include "modeldata.h"

namespace Frost
{
    class FTLoader
    {
    public :

        static ModelData LoadModelData(const s_str& sFile);

        static const s_str CLASS_NAME;

    };
}

#endif
