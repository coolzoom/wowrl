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
              out float3 oSpecColor : TEXCOORD2,
            #ifdef MOTION_BLUR
              out float  oDepth     : TEXCOORD3,
            #endif

            // Provided by Ogre
              uniform float4x4 mWorldViewProj,
              uniform float4x4 mWorld,
              
            #ifdef MOTION_BLUR
              uniform float    mCamMaxDepth,
            #endif
             
              uniform float4x4 mTexCoordMat,
            
              uniform float4   mCamPos,
              uniform float4   mLightPos[5],
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
    
    float3 tReflected;
    oSpecColor = float3(0,0,0);
    float3 tEyeDir = normalize(mCamPos.xyz - tPosition);

    for (int i = 0; i < 5; ++i)
    {
        tLightDir = normalize(mLightPos[i].xyz - tPosition);
        tDistance = distance(mLightPos[i].xyz, tPosition);

        if (tDistance < mLightAtten[i].x)
        {
            if (tDistance == 0.0f)
            {
                oColor += mLightDiffuseColor[i].rgb;
            }
            else
            {
                tAtten = 1.0f/(mLightAtten[i].y + tDistance*mLightAtten[i].z + tDistance*tDistance*mLightAtten[i].w);
                oColor += mLightDiffuseColor[i].rgb * saturate(dot(tLightDir, tNormal)) * tAtten;
            }
            
            tReflected = 2*tNormal*dot(tLightDir, tNormal) - tLightDir;
            oSpecColor += mLightDiffuseColor[i].rgb * saturate(dot(tReflected, tEyeDir)) * tAtten;
        }
    }

    // Apply position and camera projection
    oPosition = mul(mWorldViewProj, iPosition);
    #ifdef MOTION_BLUR
        oDepth = oPosition.x/oPosition.w;
    #endif

    oTexture = mul(mTexCoordMat, iTexture);
}
