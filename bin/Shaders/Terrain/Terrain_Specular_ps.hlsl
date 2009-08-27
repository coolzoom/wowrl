// Note : inspired from Ogre's example shaders

void main_ps( 
            // Inputs
              float2 iTexture   : TEXCOORD0,
              float3 iColor     : TEXCOORD1,
              float3 iSpecColor : TEXCOORD2,
            #ifdef MOTION_BLUR
              float iDepth      : TEXCOORD3,
            #endif

            // Provided by Ogre
              uniform sampler2D mTexture  : TEXUNIT0,
              uniform sampler2D mTextureS : TEXUNIT1,
              
            // Outputs
              out float4 oColor : COLOR
            )
{
    oColor = tex2D(mTexture, iTexture);

    float4 tSpec = tex2D(mTextureS, iTexture);
    
    oColor.rgb *= iColor;
    oColor.rgb += iSpecColor*tSpec.rgb*tSpec.a;
    #ifdef MOTION_BLUR
        oColor.a = iDepth;
    #endif
}
