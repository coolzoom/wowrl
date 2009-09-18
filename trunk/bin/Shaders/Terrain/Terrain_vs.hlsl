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
            #ifdef SPECULAR
              out float3 oSpecColor : TEXCOORD2,
            #endif
            #ifdef MOTION_BLUR
              out float4 oPosition2 : TEXCOORD3,
            #endif

            // Provided by Ogre
              uniform float4x4 mWorldViewProj,
              uniform float4x4 mWorld,
              
              uniform float4x4 mTexCoordMat,
              
            #ifdef SPECULAR
              uniform float4   mCamPos,
            #endif
            
              uniform float4   mLightPos[5],
              uniform float4   mLightDiffuseColor[5],
              uniform float4   mLightAtten[5],
              uniform float4   mSunDir,
              uniform float4   mSunColor,
              uniform float4   mAmbient
            )
{
    oColor = mAmbient.rgb;
    float3 tLightDir;
    float  tDistance;
    float  tAtten;
    
    float3 tPosition = mul(mWorld, iPosition);
    float3 tNormal = normalize(mul(mWorld, iNormal));
    
    #ifdef SPECULAR
        float3 tReflected;
        oSpecColor = float3(0.0, 0.0, 0.0);
        float3 tEyeDir = normalize(mCamPos.xyz - tPosition);
    #endif

    for (int i = 0; i < 5; ++i)
    {
        tLightDir = normalize(mLightPos[i].xyz - tPosition);
        tDistance = distance(mLightPos[i].xyz, tPosition);

        tAtten = 1.0/(mLightAtten[i].y + tDistance*mLightAtten[i].z + tDistance*tDistance*mLightAtten[i].w);
        oColor += mLightDiffuseColor[i].rgb * saturate(dot(tLightDir, tNormal)) * tAtten;
        
        #ifdef SPECULAR
            tReflected = 2.0*tNormal*dot(tLightDir, tNormal) - tLightDir;
            oSpecColor += mLightDiffuseColor[i].rgb * saturate(dot(tReflected, tEyeDir)) * tAtten;
        #endif
    }
    
    oColor += mSunColor.rgb * saturate(dot(mSunDir.xyz, tNormal));
    #ifdef SPECULAR
        tReflected = 2.0*tNormal*dot(mSunDir.xyz, tNormal) - mSunDir.xyz;
        oSpecColor += mSunColor.rgb * saturate(dot(tReflected, tEyeDir));
    #endif

    // Apply position and camera projection
    oPosition = mul(mWorldViewProj, iPosition);
    #ifdef MOTION_BLUR
        oPosition2 = oPosition;
    #endif

    oTexture = mul(mTexCoordMat, iTexture);
}
