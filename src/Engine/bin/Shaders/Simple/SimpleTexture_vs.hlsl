// Note : inspired from Ogre's example shaders

void main_vs(
            // Inputs
              float4 iPosition      : POSITION,
              float3 iNormal        : NORMAL,
              float2 iTexture       : TEXCOORD0,

            // Outputs
              out float4 oPosition  : POSITION,
              out float2 oTexture   : TEXCOORD0,
              out float3 oColor     : TEXCOORD1,
            #ifdef MOTION_BLUR
              out float4 oPosition2 : TEXCOORD2,
            #endif

            // Provided by Ogre
              uniform float4x4 mWorldViewProj,
              uniform float4x4 mWorld,
              
              uniform float4x4 mTexCoordMat,
            
              uniform float4   mLightPos[5],
              uniform float4   mLightDir,
              uniform float4   mLightDiffuseColor[5],
              uniform float4   mLightAtten[5],
              uniform float4   mAmbient
            )
{
    oColor = mAmbient.rgb;
    float3 tLightDir;
    float  tDistance;
    float  tAtten;
    
    float3 tPosition = mul(mWorld, iPosition);
    float3 tNormal = normalize(mul(mWorld, iNormal));

    // Handle directional light
    oColor += mLightDiffuseColor[0].rgb * saturate(-dot(mLightDir.xyz, tNormal)) / mLightAtten[0].y;

    // Handle point lights
    for (int i = 1; i < 5; ++i)
    {
        tLightDir = normalize(mLightPos[i].xyz - tPosition);
        tDistance = distance(mLightPos[i].xyz, tPosition);

        tAtten = 1.0/(mLightAtten[i].y + tDistance*mLightAtten[i].z + tDistance*tDistance*mLightAtten[i].w);
        oColor += mLightDiffuseColor[i].rgb * saturate(dot(tLightDir, tNormal)) * tAtten;
    }
    
    oColor = saturate(oColor);

    // Apply position and camera projection
    oPosition = mul(mWorldViewProj, iPosition);
    #ifdef MOTION_BLUR
        oPosition2 = oPosition;
    #endif

    oTexture = mul(mTexCoordMat, iTexture);
}
