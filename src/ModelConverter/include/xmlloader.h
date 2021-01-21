/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */


#ifndef XMLLOADER_H
#define XMLLOADER_H

#include "converter.h"
#include "modeldata.h"

namespace Frost
{
    class XMLLoader
    {
    public :

        static ModelData LoadModelData(const s_str& sFile);
        static void      WriteModelData(const ModelData& mData, const s_str& sFile);

        static const s_str CLASS_NAME;

    };
}

#endif
