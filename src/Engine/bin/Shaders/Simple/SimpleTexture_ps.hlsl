// Note : inspired from Ogre's example shaders

void main_ps( 
            // Inputs
              float2 iTexture   : TEXCOORD0,
              float3 iColor     : TEXCOORD1,
            #ifdef MOTION_BLUR
              float4 oPosition2 : TEXCOORD2,
            #endif

            // Provided by Ogre
              uniform sampler2D mTexture  : TEXUNIT0,
              
            // Outputs
            #ifdef MOTION_BLUR
              out float4 oColor0 : COLOR0,
              out float4 oColor1 : COLOR1
            #else
              out float4 oColor : COLOR
            #endif
            )
{
    #ifdef MOTION_BLUR
        oColor0 = tex2D(mTexture, iTexture);
        oColor0.rgb *= iColor;
        oColor1 = float4(1,1,1,1);
        oColor1.r = oPosition2.z/oPosition2.w;
    #else
        oColor = tex2D(mTexture, iTexture);
        oColor.rgb *= iColor;
    #endif
}
