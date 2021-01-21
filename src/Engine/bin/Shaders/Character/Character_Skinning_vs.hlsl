// Note : inspired from Ogre's example shaders

void main_vs(
            // Inputs
              float4 iPosition       : POSITION,
              float3 iNormal         : NORMAL,
              float4 iWeight         : BLENDWEIGHT,
              float4 iBone           : BLENDINDICES,
              float2 iTexture        : TEXCOORD0,

            // Outputs
              out float4 oPosition   : POSITION,
              out float2 oTexture    : TEXCOORD0,
              out float3 oNormal     : TEXCOORD1,
              out float3 oPosition3D : TEXCOORD2,
            #ifdef MOTION_BLUR
              out float4 oPosition2  : TEXCOORD3,
            #endif

            // Provided by Ogre
              uniform float4x4 mViewProj,   // viewproj_matrix
              uniform float3x4 mBoneMat[60] // world_matrix_array_3x4
            )
{
    oNormal.xyz = 0;
    oPosition.xyzw = 0;

    // Temporary variables
    float tBoneIndex;
    float tWeight;
    
    for (int i = 0; i < 4; ++i)
    {
        tBoneIndex = iBone[i];
        tWeight = iWeight[i];

        // Apply bones to the vertex
        oPosition += tWeight*float4(mul(mBoneMat[tBoneIndex], iPosition).xyz, 1.0);

        // Apply bones to the normal
        oNormal += tWeight*mul((float3x3)mBoneMat[tBoneIndex], iNormal);
    }

    // Apply position and camera projection
    oPosition3D = oPosition.xyz;
    oPosition = mul(mViewProj, oPosition);
    #ifdef MOTION_BLUR
        oPosition2 = oPosition;
    #endif

    // Do not change texture coordinates
    oTexture = iTexture;
}
