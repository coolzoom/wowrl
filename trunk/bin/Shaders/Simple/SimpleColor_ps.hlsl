// Note : inspired from Ogre's example shaders

void main_ps( 
            // Inputs
              float4 iColor     : TEXCOORD0,
            #ifdef MOTION_BLUR
              float4 oPosition2 : TEXCOORD1,
            #endif
              
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
        oColor0 = iColor;
        oColor1 = float4(1,1,1,1);
        oColor1.r = oPosition2.z/oPosition2.w;
    #else
        oColor = iColor;
    #endif
}
