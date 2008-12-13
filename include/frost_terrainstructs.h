/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           Terrain structures           */
/*                                        */
/*                                        */

#ifndef FROST_TERRAINSTRUCTS_H
#define FROST_TERRAINSTRUCTS_H

namespace Frost
{
    struct TerrainHeader
    {
        /// Makes sure we are opening a terrain file
        char sID[4];
        /// Makes sure the loading code supports this file
        char sVersion[4];
        /// The dimension of the terrain in number of points (width, horizontal)
        uint uiNumWPoint;
        /// The dimension of the terrain in number of points (height, horizontal)
        uint uiNumHPoint;
        /// The offset at which the loader can get the first point
        uint uiOffPoints;
    };

    struct TerrainPoint
    {
        /// The height of this point
        float fHeight;
        /// Special flags : 11111111
        /** # : hole : if '1', this point is a hole
        */
        uchar usFlags;
    };
}

#endif
