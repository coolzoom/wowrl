// Provided by Ogre
uniform sampler2D mTexture;

uniform mat4  mViewProjInverse;
uniform mat4  mPrevViewProj;
uniform int   mIterNumber;

void main()
{
    vec2 tTexCoord = gl_TexCoord[0].st;
    gl_FragColor = texture2D(mTexture, tTexCoord);
    
    vec4 tViewportPosition = vec4(tTexCoord.x*2 - 1, (1 - tTexCoord.y)*2 - 1, gl_FragColor.a, 1);
    vec4 tPosition = mViewProjInverse * tViewportPosition;
    tPosition /= tPosition.w;
    
    vec4 tPrevPosition = mPrevViewProj * tPosition;
    tPrevPosition /= tPrevPosition.w;
    
    vec2 tVelocity = (tViewportPosition - tPrevPosition).xy/2;
    
    tTexCoord += tVelocity/mIterNumber;
    
    for (int i = 1; i < mIterNumber; ++i)
    {
        gl_FragColor += texture2D(mTexture, tTexCoord);
        tTexCoord += tVelocity/mIterNumber;
    }
    
    gl_FragColor /= mIterNumber;
}