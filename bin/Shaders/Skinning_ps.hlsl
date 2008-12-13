// Note : inspired from Ogre's example shaders

void main_ps( 
            // Inputs
              float2 iTexture   : TEXCOORD0,
              float3 iNormal    : TEXCOORD1,
              float3 iPosition  : TEXCOORD2,

            // Outputs
              out float4 oColor : COLOR,

            // Provided by Ogre
              uniform sampler2D mTexture : TEXUNIT0,
              uniform float4    mLightPos[5],
              uniform float4    mLightDiffuseColor[5],
              uniform float4    mLightAtten[5],
              uniform float4    mAmbient
            )
{
    float3 tLightColor = mAmbient.rgb;
    float3 tLightDir;
    float  tDistance;

    for (int i = 0; i < 5; ++i)
    {
        tLightDir = normalize(mLightPos[i].xyz - iPosition);
        tDistance = distance(mLightPos[i].xyz, iPosition);

        tLightColor += mLightDiffuseColor[i].rgb *
            saturate(dot(tLightDir, iNormal)) *
            saturate(1.0f/(mLightAtten[i].y + tDistance*mLightAtten[i].z + tDistance*tDistance*mLightAtten[i].w));
    }

    oColor = tex2D(mTexture, iTexture);
    oColor.rgb *= tLightColor;
}
