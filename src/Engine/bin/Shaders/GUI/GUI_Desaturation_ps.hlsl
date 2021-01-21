// Note : inspired from Ogre's example shaders

void main_ps( 
            // Inputs
              float2 iTexture   : TEXCOORD0,
              
            // Outputs
              out float4 oColor : COLOR,

            // Provided by Ogre
              uniform sampler2D mTexture : TEXUNIT0
            )
{
    float3 mTemp;
    oColor = tex2D(mTexture, iTexture);
    mTemp.r = mTemp.g = mTemp.b = (oColor.r + oColor.g + oColor.b)/3.0;
    oColor.rgb = mTemp.rgb;
}
