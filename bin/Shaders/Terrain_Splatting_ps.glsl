// Note : inspired from Ogre's example shaders

// Provided by Ogre
uniform sampler2D mMask;
uniform sampler2D mTexture1;
uniform sampler2D mTexture2;
#if LAYER > 2
    uniform sampler2D mTexture3;
#endif
#if LAYER > 3
    uniform sampler2D mTexture4;
#endif

#ifdef SPECULAR
    uniform sampler2D mTexture1S;
    uniform sampler2D mTexture2S;
    #if LAYER > 2
        uniform sampler2D mTexture3S;
    #endif
    #if LAYER > 3
        uniform sampler2D mTexture4S;
    #endif
#endif

varying vec3 vColor;
#ifdef SPECULAR
    varying vec3 vSpecColor;
#endif

void main()
{
    vec4 tMask = texture2D(mMask, gl_TexCoord[0].st);
    
    gl_FragColor  = texture2D(mTexture1,  gl_TexCoord[1].st)*tMask.a;
    gl_FragColor += texture2D(mTexture2,  gl_TexCoord[2].st)*tMask.r;
    #if LAYER > 2
        gl_FragColor += texture2D(mTexture3, gl_TexCoord[3].st)*tMask.g;
    #endif
    #if LAYER > 3
        gl_FragColor += texture2D(mTexture4, gl_TexCoord[4].st)*tMask.b;
    #endif
    
    #ifdef SPECULAR
        vec4 tSpec = texture2D(mTexture1S, gl_TexCoord[1].st)*tMask.a;
        tSpec     += texture2D(mTexture2S, gl_TexCoord[2].st)*tMask.r;
        #if LAYER > 2
            tSpec += texture2D(mTexture3S, gl_TexCoord[3].st)*tMask.g;
        #endif
        #if LAYER > 3
            tSpec += texture2D(mTexture4S, gl_TexCoord[4].st)*tMask.b;
        #endif
    #endif
    
    gl_FragColor.rgb *= vColor;
    
    #ifdef SPECULAR
        gl_FragColor.rgb += vSpecColor*tSpec.rgb*tSpec.a;
    #endif
}
