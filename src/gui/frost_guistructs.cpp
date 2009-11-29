/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            GUI Structs source          */
/*                                        */
/*                                        */

#include "gui/frost_guistructs.h"

#include "material/frost_materialmanager.h"

using namespace std;

namespace Frost
{
    Quad::Quad()
    {
        pMat = MaterialManager::GetSingleton()->GetDefault2D();
    }
}
