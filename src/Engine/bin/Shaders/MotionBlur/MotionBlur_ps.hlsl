void main_ps( 
            // Inputs
              float2 iTexture   : TEXCOORD0,

            // Outputs
              out float4 oColor : COLOR,

            // Provided by Ogre
              uniform sampler2D mTexture : TEXUNIT0,
              uniform sampler2D mMask    : TEXUNIT1,
              
              uniform float4x4  mViewProjInverse,
              uniform float4x4  mPrevViewProj,
              uniform float     mFPS,
              uniform float4    mMaxUV
            )
{
    oColor = tex2D(mTexture, iTexture);
    float4 vMask = tex2D(mMask, iTexture);
    
    float4 tViewportPosition = float4(iTexture.x/mMaxUV.x*2.0 - 1.0, (1.0 - iTexture.y/mMaxUV.y)*2.0 - 1.0, vMask.r, 1.0);
    float4 tPosition = mul(mViewProjInverse, tViewportPosition);
    tPosition /= tPosition.w;
    
    float4 tPrevPosition = mul(mPrevViewProj, tPosition);
    tPrevPosition /= tPrevPosition.w;
    
    float2 tVelocity = mFPS*BLUR_INTENSITY/float(BLUR_QUALITY)*float2(1.0, -1.0)*(tPrevPosition - tViewportPosition).xy;
    
    iTexture += tVelocity;
    
    float tWeight = float(BLUR_QUALITY);
    float tTempMask;
    for (int i = 1; i < BLUR_QUALITY; ++i)
    {
        tTempMask = tex2D(mMask, iTexture).a*vMask.a;
        tWeight -= (1.0-tTempMask);
        iTexture = float2(clamp(iTexture.x, 0.0, mMaxUV.x), clamp(iTexture.y, 0.0, mMaxUV.y));
        oColor += tex2D(mTexture, iTexture)*tTempMask;
        iTexture += tVelocity;
    }
    
    oColor /= tWeight;
}
