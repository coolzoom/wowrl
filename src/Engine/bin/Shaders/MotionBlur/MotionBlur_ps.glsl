// Provided by Ogre
uniform sampler2D mTexture;
uniform sampler2D mMask;

uniform mat4  mViewProjInverse;
uniform mat4  mPrevViewProj;
uniform float mFPS;
uniform vec4  mMaxUV;

void main()
{
    vec2 tTexCoord = gl_TexCoord[0].st;
    gl_FragColor = texture2D(mTexture, tTexCoord);
    vec4 vMask = texture2D(mMask, tTexCoord);
    
    if (vMask.r == 0) vMask.r = 1;
    vec4 tViewportPosition = vec4(tTexCoord.s/mMaxUV.s*2.0 - 1.0, (1.0 - tTexCoord.t/mMaxUV.t)*2.0 - 1.0, vMask.r, 1.0);
    vec4 tPosition = mViewProjInverse * tViewportPosition;
    tPosition /= tPosition.w;
    
    vec4 tPrevPosition = mPrevViewProj * tPosition;
    tPrevPosition /= tPrevPosition.w;
    
    vec2 tVelocity = mFPS*float(BLUR_INTENSITY)/float(BLUR_QUALITY)*vec2(1.0, -1.0)*(tPrevPosition - tViewportPosition).xy;
    
    tTexCoord += tVelocity;
    
    float tWeight = float(BLUR_QUALITY);
    float tTempMask;
    for (int i = 1; i < BLUR_QUALITY; ++i)
    {
        tTempMask = texture2D(mMask, tTexCoord).a*vMask.a;
        tWeight -= (1.0-tTempMask);
        tTexCoord = vec2(clamp(tTexCoord.s, 0.0, mMaxUV.s), clamp(tTexCoord.t, 0.0, mMaxUV.t));
        gl_FragColor += texture2D(mTexture, tTexCoord)*tTempMask;
        tTexCoord += tVelocity;
    }
    
    gl_FragColor /= tWeight;
}