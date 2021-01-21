/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          ManualModelData header        */
/*                                        */
/*                                        */


#ifndef FROST_MANUALMODELDATA_H
#define FROST_MANUALMODELDATA_H

#include "frost.h"
#include "model/frost_modeldata.h"

namespace Frost
{
    namespace ManualModel
    {
        struct Vertex
        {
            Vector  mPos;
            Vector  mNormal;
            s_float fU, fV;

            s_array<s_uint,4>  lBoneIndexList;
            s_array<s_float,4> lBoneWeightList;
        };

        struct Triangle
        {
            Triangle() {}
            Triangle(const s_uint& ui1, const s_uint& ui2, const s_uint& ui3)
            {
                lVertexList[0] = ui1;
                lVertexList[1] = ui2;
                lVertexList[2] = ui3;
            }

            s_array<s_uint,3> lVertexList;
        };

        struct SubMesh
        {
            s_uint uiID;
            s_array<Vertex>   lVertexList;
            s_array<Triangle> lTriangleList;
        };

        struct Bone
        {
            Vector mPos;
            s_uint uiParent;

            s_map<s_uint, Vector>           lTranslationAnimation;
            s_map<s_uint, Vector>           lScalingAnimation;
            s_map<s_uint, Ogre::Quaternion> lRotationAnimation;

            s_array<s_uint> lTimeline;
        };

        struct Animation
        {
            s_uint uiID;
            s_uint uiBegin;
            s_uint uiEnd;
            s_bool bLoop;
        };

        struct Model
        {
            s_array<SubMesh> lSubMeshList;

            AxisAlignedBox mBoundingBox;

            s_array<Bone>      lBoneList;
            s_array<Animation> lAnimList;
        };
    }

    /// Contains the necessary data to create a Model.
    class ManualModelData : public ModelData
    {
    public:

        /// Default constructor.
        ManualModelData(const ManualModel::Model& mModel, const s_str& sName);

        static const s_str CLASS_NAME;

    protected:

    };
}

#endif
