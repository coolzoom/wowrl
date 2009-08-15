/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               XXX header               */
/*                                        */
/*                                        */


#ifndef FROST_TERRAINDATA_H
#define FROST_TERRAINDATA_H

#include "frost.h"

namespace Frost
{
    /// Holds raw data for terrain loading
    /** Can be used to load custom file formats.
    *   \note Only used for loading purposes.
    */
    class TerrainData
    {
    public :

        /// Default constructor.
        TerrainData(const s_str& sFile);

        /// Destructor.
        ~TerrainData();

        static const s_str CLASS_NAME;

    // private : (not private to simplify Terrain creation)

        s_uint uiNumXPoint;
        s_uint uiNumYPoint;

        void _CreateBuffers(Ogre::SubMesh* pSub, uint iVertexNbr, uint iIndexNbr, float* lVertices, float* lUVs, ushort* lIndices);

        s_ptr<Ogre::Mesh>    pMesh_;

    };
}

#endif
