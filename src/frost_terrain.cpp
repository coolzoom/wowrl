/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Terrain source             */
/*                                        */
/*                                        */

#include "frost_terrain.h"

using namespace std;

namespace Frost
{
    const s_str Terrain::CLASS_NAME = "Terrain";

    Terrain::Terrain( const s_uint& uiID ) :
        uiID_(uiID)
    {
    }

    Terrain::~Terrain()
    {
    }

    const s_uint& Terrain::GetID() const
    {
        return uiID_;
    }
}
