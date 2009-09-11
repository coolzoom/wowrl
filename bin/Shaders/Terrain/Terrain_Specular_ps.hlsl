// Note : inspired from Ogre's example shaders

void main_ps( 
            // Inputs
              float2 iTexture   : TEXCOORD0,
              float3 iColor     : TEXCOORD1,
              float3 iSpecColor : TEXCOORD2,
            #ifdef MOTION_BLUR
              float4 oPosition2 : TEXCOORD3,
            #endif

            // Provided by Ogre
              uniform sampler2D mTexture  : TEXUNIT0,
              uniform sampler2D mTextureS : TEXUNIT1,
              
            // Outputs
            #ifdef MOTION_BLUR
              out float4 oColor0 : COLOR0,
              out float4 oColor1 : COLOR1
            #else
              out float4 oColor : COLOR
            #endif
            )
{
    float4 tSpec = tex2D(mTextureS, iTexture);

    #ifdef MOTION_BLUR
        oColor0 = tex2D(mTexture, iTexture);
        oColor0.rgb *= iColor;
        oColor0.rgb += iSpecColor*tSpec.rgb*tSpec.a;
        oColor0.a = oPosition2.z/oPosition2.w;
        oColor1 = float4(1,1,1,1);
    #else
        oColor = tex2D(mTexture, iTexture);
        oColor.rgb *= iColor;
        oColor.rgb += iSpecColor*tSpec.rgb*tSpec.a;
    #endif
}
