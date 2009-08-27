// Note : inspired from Ogre's example shaders

// Provided by Ogre
uniform sampler2D mMask;
uniform sampler2D mTexture;
uniform sampler2D mTextureS;

varying vec3 vColor;
varying vec3 vSpecColor;
#ifdef MOTION_BLUR
    varying float vDepth;
#endif

void main()
{
    gl_FragColor = texture2D(mTexture,  gl_TexCoord[0].st);
    
    vec4 tSpec = texture2D(mTextureS, gl_TexCoord[0].st);
    
    gl_FragColor.rgb *= vColor;
    gl_FragColor.rgb += vSpecColor*tSpec.rgb*tSpec.a;
    #ifdef MOTION_BLUR
        gl_FragColor.a = vDepth;
    #endif
}
