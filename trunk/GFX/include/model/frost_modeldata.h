/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             ModelData header           */
/*                                        */
/*                                        */


#ifndef FROST_MODELDATA_H
#define FROST_MODELDATA_H

#include "frost.h"

#include "scene/frost_meshobstacle.h"

namespace Frost
{
    /// Holds raw data for model loading.
    /** Can be used to load custom file formats.
    *   \note Only used for loading purposes.
    */
    class ModelData
    {
    friend class Model;
    public :

        /// Default constructor.
        /** \param sFile The file from which to load the model
        *   \note This constructor only creates an empty model, and
        *         nothing more. Actual model loading is done by derived
        *         classes.
        */
        ModelData(const s_str& sFile);

        /// Destructor.
        /** \note Takes care of deleting the ModelParts.
        */
        virtual ~ModelData();

        static const s_str CLASS_NAME;

    protected :

        void CreateBuffers_( const s_uint& uiSubID, Ogre::SubMesh* pSub, uint iVertexNbr, uint iIndexNbr, float* lVertices, float* lUVs, ushort* lIndices);

        s_ptr<Ogre::Mesh> pMesh_;
        s_str             sModelName_;

        s_map< s_uint, s_ptr<ModelPart> > lModelPartList_;
        s_map<s_uint, MeshAnimation>      lAnimList_;

        s_map< s_uint, s_array<MeshObstacle::Triangle> > lTriangleList_;
        s_map< s_uint, AxisAlignedBox >                  lBoxList_;

    };
}

#endif
