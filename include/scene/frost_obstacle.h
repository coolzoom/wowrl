/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Obstacle header            */
/*                                        */
/*                                        */


#ifndef FROST_OBSTACLE_H
#define FROST_OBSTACLE_H

#include "frost.h"

namespace Frost
{
    class Obstacle
    {
    public :

        Obstacle(const s_uint& uiID);

        ~Obstacle();

        const s_uint& GetID() const;

        static const s_str CLASS_NAME;

    private :

        s_uint uiID_;
    };
}

#endif
