// Note : inspired from Ogre's example shaders

void main_vs(
            // Inputs
              float4 iPosition       : POSITION,
              float3 iNormal         : NORMAL,
              float2 iTexture        : TEXCOORD0,

            // Outputs
              out float4 oPosition   : POSITION,
              out float2 oTexture0   : TEXCOORD0,
              out float2 oTexture1   : TEXCOORD1,
              out float2 oTexture2   : TEXCOORD2,
            #if LAYER > 2
              out float2 oTexture3   : TEXCOORD3,
            #endif
            #if LAYER > 3
              out float2 oTexture4   : TEXCOORD4,
            #endif
              out float3 oColor      : TEXCOORD5,
            #ifdef SPECULAR
              out float3 oSpecColor  : TEXCOORD6,
            #endif
            #ifdef MOTION_BLUR
              out float oDepth       : TEXCOORD7,
            #endif

            // Provided by Ogre
              uniform float4x4 mWorldViewProj,
              uniform float4x4 mWorld,
             
              uniform float4x4 mTexCoordMat1,
              uniform float4x4 mTexCoordMat2,
            #if LAYER > 2
              uniform float4x4 mTexCoordMat3,
            #endif
            #if LAYER > 3
              uniform float4x4 mTexCoordMat4,
            #endif
            
            #ifdef SPECULAR
              uniform float4   mCamPos,
            #endif
            
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
    
    #ifdef SPECULAR
        float3 tReflected;
        oSpecColor = float3(0,0,0);
        float3 tEyeDir = normalize(mCamPos.xyz - tPosition);
    #endif

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
            
            #ifdef SPECULAR
                tReflected = 2*tNormal*dot(tLightDir, tNormal) - tLightDir;
                oSpecColor += mLightDiffuseColor[i].rgb * saturate(dot(tReflected, tEyeDir)) * tAtten;
            #endif
        }
    }

    // Apply position and camera projection
    oPosition = mul(mWorldViewProj, iPosition);
    #ifdef MOTION_BLUR
        oDepth = oPosition.x/oPosition.w;
    #endif

    oTexture0 = iTexture;
    oTexture1 = mul(mTexCoordMat1, iTexture);
    oTexture2 = mul(mTexCoordMat2, iTexture);
    #if LAYER > 2
        oTexture3 = mul(mTexCoordMat3, iTexture);
    #endif
    #if LAYER > 3
        oTexture4 = mul(mTexCoordMat4, iTexture);
    #endif
}
