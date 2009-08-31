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
              out float4 oColor : COLOR,

            // Provided by Ogre
              uniform sampler2D mTexture : TEXUNIT0,
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
        
        tAtten = 1.0f/(mLightAtten[i].y + tDistance*mLightAtten[i].z + tDistance*tDistance*mLightAtten[i].w);
        tLightColor += mLightDiffuseColor[i].rgb * saturate(dot(tLightDir, iNormal)) * tAtten;
    }
    
    tLightColor += mSunColor.rgb * saturate(dot(mSunDir.xyz, iNormal));

    oColor = tex2D(mTexture, iTexture);
    oColor.rgb *= tLightColor;
    #ifdef MOTION_BLUR
        oColor.a = oPosition2.z/oPosition2.w;
    #endif
}
