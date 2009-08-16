// Note : inspired from Ogre's example shaders

void main_ps( 
            // Inputs
              float2 iTexture   : TEXCOORD0,
              float3 iColor     : TEXCOORD1,
              
            // Outputs
              out float4 oColor : COLOR,

            // Provided by Ogre
              uniform sampler2D mMask     : TEXUNIT0,
              uniform sampler2D mTexture1 : TEXUNIT1,
              uniform sampler2D mTexture2 : TEXUNIT2,
              
              uniform float4x4 mTexCoordMat1,
              uniform float4x4 mTexCoordMat2
            )
{
    float4 tMask = tex2D(mMask, iTexture);
    float  tSum = tMask.a + tMask.r;
    oColor = tex2D(mTexture1, mul(mTexCoordMat1, float4(iTexture.xy, 0, 1)).xy)*tMask.a;
    oColor += tex2D(mTexture2, mul(mTexCoordMat2, float4(iTexture.xy, 0, 1)).xy)*tMask.r;
    oColor /= tSum;
    oColor.rgb *= iColor;
}
