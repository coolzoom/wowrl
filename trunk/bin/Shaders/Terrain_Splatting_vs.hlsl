// Note : inspired from Ogre's example shaders

void main_vs(
            // Inputs
              float4 iPosition       : POSITION,
              float3 iNormal         : NORMAL,
              float2 iTexture        : TEXCOORD0,

            // Outputs
              out float4 oPosition   : POSITION,
              out float2 oTexture    : TEXCOORD0,
              out float3 oColor      : TEXCOORD1,

            // Provided by Ogre
              uniform float4x4 mWorldViewProj,
              uniform float4x4 mWorld,
              
              uniform float4   mLightPos[5],
              uniform float4   mLightDiffuseColor[5],
              uniform float4   mLightAtten[5],
              uniform float4   mAmbient
            )
{
    oColor = mAmbient.rgb;
    float3 tLightDir;
    float  tDistance;
    
    float3 tPosition = mul(mWorld, iPosition);
    float3 tNormal = mul(mWorld, iNormal);

    for (int i = 0; i < 5; ++i)
    {
        tLightDir = normalize(mLightPos[i].xyz - tPosition);
        tDistance = distance(mLightPos[i].xyz, tPosition);

        oColor += mLightDiffuseColor[i].rgb *
            saturate(dot(tLightDir, tNormal)) *
            saturate(1.0f/(mLightAtten[i].y + tDistance*mLightAtten[i].z + tDistance*tDistance*mLightAtten[i].w));
    }

    // Apply position and camera projection
    oPosition = mul(mWorldViewProj, iPosition);

    oTexture = iTexture;
}
