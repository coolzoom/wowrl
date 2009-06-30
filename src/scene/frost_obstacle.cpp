/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Obstacle source            */
/*                                        */
/*                                        */

#include "scene/frost_obstacle.h"

using namespace std;

namespace Frost
{
    const s_str Obstacle::CLASS_NAME = "Obstacle";

    Obstacle::Obstacle( const s_uint& uiID ) :
        uiID_(uiID)
    {
    }

    Obstacle::~Obstacle()
    {
    }

    const s_uint& Obstacle::GetID() const
    {
        return uiID_;
    }
}



