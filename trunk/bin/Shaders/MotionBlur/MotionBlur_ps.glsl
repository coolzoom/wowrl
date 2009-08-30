// Provided by Ogre
uniform sampler2D mTexture;

uniform mat4  mViewProjInverse;
uniform mat4  mPrevViewProj;
uniform float mFPS;
uniform vec4  mMaxUV;

void main()
{
    vec2 tTexCoord = gl_TexCoord[0].st;
    gl_FragColor = texture2D(mTexture, tTexCoord);
    
    vec4 tViewportPosition = vec4(tTexCoord.s/mMaxUV.s*2 - 1, (1 - tTexCoord.t/mMaxUV.t)*2 - 1, gl_FragColor.a, 1);
    vec4 tPosition = mViewProjInverse * tViewportPosition;
    tPosition /= tPosition.w;
    
    vec4 tPrevPosition = mPrevViewProj * tPosition;
    tPrevPosition /= tPrevPosition.w;
    
    vec2 tVelocity = mFPS*BLUR_INTENSITY/BLUR_QUALITY*vec2(1, -1)*(tViewportPosition - tPrevPosition).xy;
    
    tTexCoord += tVelocity;
    
    for (int i = 1; i < BLUR_QUALITY; ++i)
    {
        tTexCoord = vec2(clamp(tTexCoord.s, 0, mMaxUV.s), clamp(tTexCoord.t, 0, mMaxUV.t));
        gl_FragColor += texture2D(mTexture, tTexCoord);
        tTexCoord += tVelocity;
    }
    
    gl_FragColor /= BLUR_QUALITY;
}