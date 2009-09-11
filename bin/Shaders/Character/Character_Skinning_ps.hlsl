// Note : inspired from Ogre's example shaders

void main_ps( 
            // Inputs
              float2 iTexture   : TEXCOORD0,
              float3 iNormal    : TEXCOORD1,
              float3 iPosition  : TEXCOORD2,
            #ifdef MOTION_BLUR
              float4 oPosition2 : TEXCOORD3,
            #endif

            // Outputs
            #ifdef MOTION_BLUR
              out float4 oColor0 : COLOR0,
              out float4 oColor1 : COLOR1,
            #else
              out float4 oColor : COLOR,
            #endif

            // Provided by Ogre
              uniform sampler2D mTexture : TEXUNIT0,
              
            #ifdef MOTION_BLUR
              uniform float4    mMotionBlurMask,
            #endif
              
              uniform float4    mLightPos[5],
              uniform float4    mLightDiffuseColor[5],
              uniform float4    mLightAtten[5],
              uniform float4    mSunDir,
              uniform float4    mSunColor,
              uniform float4    mAmbient
              
            )
{
    float3 tLightColor = mAmbient.rgb;
    float3 tLightDir;
    float  tDistance;
    float  tAtten;

    for (int i = 0; i < 5; ++i)
    {
        tLightDir = normalize(mLightPos[i].xyz - iPosition);
        tDistance = distance(mLightPos[i].xyz, iPosition);
        
        tAtten = 1.0/(mLightAtten[i].y + tDistance*mLightAtten[i].z + tDistance*tDistance*mLightAtten[i].w);
        tLightColor += mLightDiffuseColor[i].rgb * saturate(dot(tLightDir, iNormal)) * tAtten;
    }
    
    tLightColor += mSunColor.rgb * saturate(dot(mSunDir.xyz, iNormal));

    #ifdef MOTION_BLUR
        oColor0 = tex2D(mTexture, iTexture);
        oColor0.rgb *= tLightColor;
        oColor0.a = oPosition2.z/oPosition2.w;
        
        oColor1 = mMotionBlurMask;
    #else
        oColor = tex2D(mTexture, iTexture);
        oColor.rgb *= tLightColor;
    #endif
}
