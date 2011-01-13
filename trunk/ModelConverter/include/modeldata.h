/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */


#ifndef MODELDATA_H
#define MODELDATA_H

#include "converter.h"

namespace Frost
{
    struct Quaternion
    {
        Quaternion()
        {
        }

        Quaternion(const s_float& w, const s_float& x, const s_float& y, const s_float& z)
        {
            fW = w;
            fX = x;
            fY = y;
            fZ = z;
        }

        Quaternion(const Vector& mAxis, const s_float& fAngle)
        {
            s_float fHalfAngle = 0.5f*fAngle;
            s_float fSin = sin(fHalfAngle);

            fW = cos(fHalfAngle);
            fX = fSin*mAxis.X();
            fY = fSin*mAxis.Y();
            fZ = fSin*mAxis.Z();
        }

        Quaternion operator + (const Quaternion& mQuat) const
        {
            return Quaternion(fW + mQuat.fW, fX + mQuat.fX, fY + mQuat.fY, fZ + mQuat.fZ);
        }

        Quaternion operator - (const Quaternion& mQuat) const
        {
            return Quaternion(fW - mQuat.fW, fX - mQuat.fX, fY - mQuat.fY, fZ - mQuat.fZ);
        }

        s_float fW, fX, fY, fZ;
    };

    Quaternion operator * (const s_float& fValue, const Quaternion& mQuat)
    {
        return Quaternion(fValue*mQuat.fW, fValue*mQuat.fX, fValue*mQuat.fY, fValue*mQuat.fZ);
    }

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

        s_map<s_uint, Vector>     lTranslationAnimation;
        s_map<s_uint, Vector>     lScalingAnimation;
        s_map<s_uint, Quaternion> lRotationAnimation;

        s_array<s_uint> lTimeline;
    };

    struct Animation
    {
        s_uint uiID;
        s_uint uiBegin;
        s_uint uiEnd;
        s_bool bLoop;
    };

    struct ModelData
    {
        s_array<SubMesh> lSubMeshList;

        AxisAlignedBox mBoundingBox;

        s_array<Bone>      lBoneList;
        s_array<Animation> lAnimList;
    };
}

#endif
