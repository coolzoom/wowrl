// Note : inspired from Ogre's example shaders

void main_ps( 
            // Inputs
              float2 iTexture0  : TEXCOORD0,
              float2 iTexture1  : TEXCOORD1,
              float2 iTexture2  : TEXCOORD2,
            #if LAYER > 2
              float2 iTexture3  : TEXCOORD3,
            #endif
            #if LAYER > 3
              float2 iTexture4  : TEXCOORD4,
            #endif
              float3 iColor     : TEXCOORD5,
            #ifdef SPECULAR
              float3 iSpecColor : TEXCOORD6,
            #endif
            #ifdef MOTION_BLUR
              float4 oPosition2 : TEXCOORD7,
            #endif

            // Provided by Ogre
              uniform sampler2D mMask     : TEXUNIT0,
              uniform sampler2D mTexture1 : TEXUNIT1,
            
            #ifdef SPECULAR
                
              uniform sampler2D mTexture1S   : TEXUNIT2,
              uniform sampler2D mTexture2    : TEXUNIT3,
              uniform sampler2D mTexture2S   : TEXUNIT4,
              #if LAYER > 2
                uniform sampler2D mTexture3  : TEXUNIT5,
                uniform sampler2D mTexture3S : TEXUNIT6,
              #endif
              #if LAYER > 3
                uniform sampler2D mTexture4  : TEXUNIT7,
                uniform sampler2D mTexture4S : TEXUNIT8,
              #endif
              
            #else
            
                uniform sampler2D mTexture2 : TEXUNIT2,
              #if LAYER > 2
                uniform sampler2D mTexture3 : TEXUNIT3,
              #endif
              #if LAYER > 3
                uniform sampler2D mTexture4 : TEXUNIT4,
              #endif
              
            #endif
              
            // Outputs
              out float4 oColor : COLOR
            )
{
    float4 tMask = tex2D(mMask, iTexture0);
    
    oColor  = tex2D(mTexture1, iTexture1)*tMask.a;
    oColor += tex2D(mTexture2, iTexture2)*tMask.r;
    #if LAYER > 2
        oColor += tex2D(mTexture3, iTexture3)*tMask.g;
    #endif
    #if LAYER > 3
        oColor += tex2D(mTexture4, iTexture4)*tMask.b;
    #endif
    
    #ifdef SPECULAR
        float4 tSpec = tex2D(mTexture1S, iTexture1)*tMask.a;
        tSpec       += tex2D(mTexture2S, iTexture2)*tMask.r;
        #if LAYER > 2
            tSpec   += tex2D(mTexture3S, iTexture3)*tMask.g;
        #endif
        #if LAYER > 3
            tSpec   += tex2D(mTexture4S, iTexture4)*tMask.b;
        #endif
    #endif
    
    oColor.rgb *= iColor;
    
    #ifdef SPECULAR
        oColor.rgb += iSpecColor*tSpec.rgb*tSpec.a;
    #endif
    #ifdef MOTION_BLUR
        oColor.a = oPosition2.z/oPosition2.w;
    #endif
}
