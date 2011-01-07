/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Model structures            */
/*                                        */
/*                                        */


#ifndef FROST_FTMODELSTRUCTS_H
#define FROST_FTMODELSTRUCTS_H

/** \cond NOT_REMOVE_FROM_DOC
*/

struct TerrainHeader
{
    /// Makes sure we are opening a terrain file
    char sID[2];
    /// Makes sure the loading code supports this file
    char sVersion[4];
    /// The dimension of the terrain in world unit on the X axis
    float fXSize;
    /// The dimension of the terrain in world unit on the Y axis
    float fYSize;
    /// The dimension of the terrain in world unit on the Y axis
    float fZSize;
    /// The dimension of the terrain in number of points on the X axis
    uint uiNumXPoint;
    /// The dimension of the terrain in number of points on the Z axis
    uint uiNumZPoint;
    /// The offset at which the loader can get the first point
    uint uiOffPoints;
};

struct TerrainVertex
{
    /// Vertex height
    float fHeight;
    /// Vertex normal
    float fNormal[3];
    /// Texture coordinates
    float fUVs[4];
    /// Flags
    uchar ucFlags;
};

/** \endcond
*/

#endif
