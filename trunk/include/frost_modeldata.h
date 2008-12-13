/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               XXX header               */
/*                                        */
/*                                        */


#ifndef FROST_MODELDATA_H
#define FROST_MODELDATA_H

#include "frost.h"

namespace Frost
{
    /// Holds raw data for model loading
    /** Can be used to load custom file formats.
    *   \note Only used for loading purposes.
    */
    class ModelData
    {
    public :

        /// Default constructor
        ModelData(const s_str& sFile, const s_str& sModelName);

        /// Destructor
        ~ModelData();

        static const s_str CLASS_NAME;

    // private : (not private to simplify Model creation)

        void _CreateBuffers(Ogre::SubMesh* pSub, uint iVertexNbr, uint iIndexNbr, float* lVertices, float* lUVs, ushort* lIndices);

        s_ptr<Ogre::Mesh>    pMesh_;
        s_refptr<AnimManager> pAnimMgr_;
        s_str                 sModelName_;

        std::map< s_uint, s_ptr<ModelPart> > lModelPartList_;
        std::map<s_uint, MeshAnimation>      lAnimList_;

    };
}

#endif
