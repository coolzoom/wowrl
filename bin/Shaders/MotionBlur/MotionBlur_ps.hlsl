// Note : inspired from Ogre's example shaders

void main_ps( 
            // Inputs
              float2 iTexture   : TEXCOORD0,

            // Outputs
              out float4 oColor : COLOR,

            // Provided by Ogre
              uniform sampler2D mTexture,
              uniform float4x4  mViewProjInverse,
              uniform float4x4  mPrevViewProj
            )
{
    oColor = tex2D(mTexture, iTexture);
    
    float4 tViewportPosition = float4(iTexture.x*2 - 1, (1 - iTexture.y)*2 - 1, oColor.a, 1);
    float4 tPosition = mul(mViewProjInverse, tViewportPosition);
    tPosition /= tPosition.w;
    
    float4 tPrevPosition = mul(mPrevViewProj, tPosition);
    tPrevPosition /= tPrevPosition.w;
    
    float2 tVelocity = (tViewportPosition - tPrevPosition).xy/2.0;
    
    iTexture += tVelocity/5.0;

    for (int i = 1; i < 5; ++i)
    {
        oColor += tex2D(mTexture, iTexture);
        iTexture += tVelocity/5.0;
    }
    
    oColor /= 5.0;
}
