/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Model structures            */
/*                                        */
/*                                        */


#ifndef FROST_FMMODELSTRUCTS_H
#define FROST_FMMODELSTRUCTS_H

/** \cond NOT_REMOVE_FROM_DOC
*/

struct ModelHeader
{
    char   id[2];
    uchar  version[4];
    ushort nAnimation;
    ushort nBone;
    ushort nSubMesh;
    float  bbox[6];
};

struct ModelSubMesh
{
    uint   id;
    ushort nVertex;
    ushort nIndice;
    uint   oNextSubMesh;
};

struct ModelVertex
{
    float pos[3];
    float normal[3];
    float texcoords[2];
    uchar weights[4];
    uchar bones[4];
};

struct ModelAnimation
{
    uint animID;
    uint timeStart;
    uint timeEnd;
    bool loop;
};

struct ModelBone
{
    float pivot[3];
    short parent;
    ushort nKey;
    ushort oNextBone;
};

struct ModelAnimationKey
{
    uint   key;
    float  translation[3];
    float  scaling[3];
    float  rotation[4];
};

/** \endcond
*/

#endif
