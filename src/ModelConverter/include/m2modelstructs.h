/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Model structures            */
/*                                        */
/*                                        */


#ifndef FROST_M2MODELSTRUCTS_H
#define FROST_M2MODELSTRUCTS_H

/** \cond NOT_REMOVE_FROM_DOC
*/

// NOTE : OS specific code (potentially).
// Relies on :
//    sizeof(char)  = sizeof(uchar)  = 1
//    sizeof(short) = sizeof(ushort) = 2
//    sizeof(int)   = sizeof(uint)   = 4
//    sizeof(float)                  = 4

#ifdef CPP_0X
    // C++0x provides static assertions
    static_assert(sizeof(char)  == 1, "M2 loading requires sizeof(char) = 1");
    static_assert(sizeof(short) == 2, "M2 loading requires sizeof(short) = 2");
    static_assert(sizeof(int)   == 4, "M2 loading requires sizeof(int) = 4");
    static_assert(sizeof(float) == 4, "M2 loading requires sizeof(float) = 4");
#else
    #define COMPILE_TIME_ASSERT(condition) switch(0) {case 0: case (condition): ;}

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
    char id[4];
    uchar version[4];
    uint nameLength;
    uint nameOfs;
    uint type;

    uint nGlobalSequences;
    uint ofsGlobalSequences;
    uint nAnimations;
    uint ofsAnimations;
    uint nC;
    uint ofsC;
    uint nD;
    uint ofsD;
    uint nBones;
    uint ofsBones;
    uint nF;
    uint ofsF;

    uint nVertices;
    uint ofsVertices;
    uint nViews;
    uint ofsViews;

    uint nColors;
    uint ofsColors;

    uint nTextures;
    uint ofsTextures;

    uint nTransparency; // H
    uint ofsTransparency;
    uint nI;   // always unused ?
    uint ofsI;
    uint nTexAnims;    // J
    uint ofsTexAnims;
    uint nTexReplace;
    uint ofsTexReplace;

    uint nTexFlags;
    uint ofsTexFlags;
    uint nY;
    uint ofsY;

    uint nTexLookup;
    uint ofsTexLookup;

    uint nTexUnitLookup;        // L
    uint ofsTexUnitLookup;
    uint nTransparencyLookup; // M
    uint ofsTransparencyLookup;
    uint nTexAnimLookup;
    uint ofsTexAnimLookup;

    float floats[14];

    uint nBoundingTriangles;
    uint ofsBoundingTriangles;
    uint nBoundingVertices;
    uint ofsBoundingVertices;
    uint nBoundingNormals;
    uint ofsBoundingNormals;

    uint nAttachments; // O
    uint ofsAttachments;
    uint nAttachLookup; // P
    uint ofsAttachLookup;
    uint nQ; // Q
    uint ofsQ;
    uint nLights; // R
    uint ofsLights;
    uint nCameras; // S
    uint ofsCameras;
    uint nT;
    uint ofsT;
    uint nRibbonEmitters; // U
    uint ofsRibbonEmitters;
    uint nParticleEmitters; // V
    uint ofsParticleEmitters;

};

struct ModelVertex
{
    float pos[3];
    uchar weights[4];
    uchar bones[4];
    float normal[3];
    float texcoords[2];
    float unk1, unk2; // always 0,0 so this is probably unused
};

struct ModelView
{
    uint nIndex, ofsIndex; // Vertices in this model (index into vertices[])
    uint nTris, ofsTris;     // indices
    uint nProps, ofsProps; // additional vertex properties
    uint nSub, ofsSub;     // materials/renderops/submeshes
    uint nTex, ofsTex;     // material properties/textures
    int lod;                 // LOD bias?
};

struct ModelSubMesh
{
    uint id;
    ushort ofsVertex;
    ushort nVertex;
    ushort ofsTris;
    ushort nTris;
    ushort unk1, unk2, unk3, unk4;
    float unk5[3];
    float unk6[4];
};

struct ModelTexUnit
{
    ushort flags;        // Flags
    short order;        // ?
    ushort op;            // Material this texture is part of (index into mat)
    ushort op2;            // Always same as above?
    short colorIndex;    // color or -1
    ushort flagsIndex;    // more flags...
    ushort texunit;        // Texture unit (0 or 1)
    ushort d4;            // ? (seems to be always 1)
    ushort textureid;    // Texture id (index into global texture list)
    ushort texunit2;    // copy of texture unit val?
    ushort transid;        // transparency id (index into transparency list)
    ushort texanimid;    // texture animation id
};

struct ModelTextureDef
{
    uint type;
    uint flags;
    uint nameLen;
    uint nameOfs;
};

struct ModelAnimation
{
    uint animID;
    uint timeStart;
    uint timeEnd;

    float moveSpeed;

    uint loopType;
    uint flags;
    uint d1;
    uint d2;
    uint playSpeed;  // note: this can't be play speed because it's 0 for some models

    float boxA[3], boxB[3];
    float rad;

    short next;

    short s;
};

struct AnimationBlock
{
    short type;        // interpolation type (0=none, 1=linear, 2=hermite)
    short seq;        // global sequence id or -1
    uint nRanges;
    uint ofsRanges;
    uint nTimes;
    uint ofsTimes;
    uint nKeys;
    uint ofsKeys;
};

struct ModelBoneDef
{
    int animid;
    int flags;
    short parent; // parent bone index
    short geoid;
    // new int added to the bone definitions.  Added in WoW 2.0
    int unknown;
    AnimationBlock translation;
    AnimationBlock rotation;
    AnimationBlock scaling;
    float pivot[3];
};

struct ModelColorDef
{
    AnimationBlock color;
    AnimationBlock opacity;
};

struct ModelTransDef
{
    AnimationBlock trans;
};

/** \endcond
*/

#endif
