// Note : inspired from Ogre's example shaders

void main_vs(
            // Inputs
              float4 iPosition      : POSITION,
              float3 iNormal        : NORMAL,

            // Outputs
              out float4 oPosition  : POSITION,
              out float4 oColor     : TEXCOORD0,
            #ifdef MOTION_BLUR
              out float4 oPosition2 : TEXCOORD1,
            #endif

            // Provided by Ogre
              uniform float4x4 mWorldViewProj,
              uniform float4x4 mWorld,
            
              uniform float4   mLightPos[5],
              uniform float4   mLightDir,
              uniform float4   mLightDiffuseColor[5],
              uniform float4   mLightAtten[5],
              uniform float4   mAmbient,
              uniform float4   mDiffuse
            )
{
    float3 tLight = mAmbient.rgb;
    float3 tLightDir;
    float  tDistance;
    float  tAtten;
    
    float3 tPosition = mul(mWorld, iPosition);
    float3 tNormal = normalize(mul(mWorld, iNormal));
    
    // Handle directional light
    tLight += mLightDiffuseColor[0].rgb * saturate(-dot(mLightDir.xyz, tNormal)) / mLightAtten[0].y;

    // Handle point lights
    for (int i = 1; i < 5; ++i)
    {
        tLightDir = normalize(mLightPos[i].xyz - tPosition);
        tDistance = distance(mLightPos[i].xyz, tPosition);

        tAtten = 1.0/(mLightAtten[i].y + tDistance*mLightAtten[i].z + tDistance*tDistance*mLightAtten[i].w);
        tLight += mLightDiffuseColor[i].rgb * saturate(dot(tLightDir, tNormal)) * tAtten;
    }
    
    tLight = saturate(tLight);
    
    oColor = mDiffuse;
    oColor.rgb *= tLight;

    // Apply position and camera projection
    oPosition = mul(mWorldViewProj, iPosition);
    #ifdef MOTION_BLUR
        oPosition2 = oPosition;
    #endif
}
