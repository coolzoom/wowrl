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

#include "frost.h"

// NOTE : OS specific code (potentially).
// Relies on :
//    sizeof(char)  = sizeof(uchar)  = 1
//    sizeof(short) = sizeof(ushort) = 2
//    sizeof(int)   = sizeof(uint)   = 4
//    sizeof(float)                  = 4

#ifdef CPP_0X
    // C++0x provides static assertions
    static_assert(sizeof(char)  == 1, "FM loading requires sizeof(char) = 1");
    static_assert(sizeof(short) == 2, "FM loading requires sizeof(short) = 2");
    static_assert(sizeof(int)   == 4, "FM loading requires sizeof(int) = 4");
    static_assert(sizeof(float) == 4, "FM loading requires sizeof(float) = 4");
#else
    // Else we need to use a trick...
    void MODELSTRUCT_H_Assertions()
    {
        // If these conditions aren't met, the compiler will
        // throw an error concerning a duplicated case value.
        COMPILE_TIME_ASSERT(sizeof(char)  == 1);
        COMPILE_TIME_ASSERT(sizeof(short) == 2);
        COMPILE_TIME_ASSERT(sizeof(int)   == 4);
        COMPILE_TIME_ASSERT(sizeof(float) == 4);
    }
#endif

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
