void main_ps( 
            // Inputs
              float2 iTexture   : TEXCOORD0,

            // Outputs
              out float4 oColor : COLOR,

            // Provided by Ogre
              uniform sampler2D mTexture,
              uniform float4x4  mViewProjInverse,
              uniform float4x4  mPrevViewProj,
              uniform float     mFPS,
              uniform float4    mMaxUV
            )
{
    oColor = tex2D(mTexture, iTexture);
    
    float4 tViewportPosition = float4(iTexture.x/mMaxUV.x*2 - 1, (1 - iTexture.y/mMaxUV.y)*2 - 1, oColor.a, 1);
    float4 tPosition = mul(mViewProjInverse, tViewportPosition);
    tPosition /= tPosition.w;
    
    float4 tPrevPosition = mul(mPrevViewProj, tPosition);
    tPrevPosition /= tPrevPosition.w;
    
    float2 tVelocity = mFPS*BLUR_INTENSITY/BLUR_QUALITY*float2(1, -1)*(tViewportPosition - tPrevPosition).xy;
    
    iTexture += tVelocity;

    for (int i = 1; i < BLUR_QUALITY; ++i)
    {
        iTexture = float2(clamp(iTexture.x, 0, mMaxUV.x), clamp(iTexture.y, 0, mMaxUV.y));
        oColor += tex2D(mTexture, iTexture);
        iTexture += tVelocity;
    }
    
    oColor /= BLUR_QUALITY;
}
