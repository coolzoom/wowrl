/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */


#ifndef OGREXMLLOADER_H
#define OGREXMLLOADER_H

#include "converter.h"
#include "modeldata.h"

namespace Frost
{
    class OgreXMLLoader
    {
    public :

        static ModelData LoadModelData(const s_str& sFile);

        static const s_str CLASS_NAME;

    };
}

#endif
