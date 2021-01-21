// Note : inspired from Ogre's example shaders

// Provided by Ogre
uniform sampler2D mTexture;
#ifdef SPECULAR
    uniform sampler2D mTextureS;
#endif

varying vec3 vColor;
#ifdef SPECULAR
    varying vec3 vSpecColor;
#endif
#ifdef MOTION_BLUR
    varying vec4 vPosition;
#endif

void main()
{
    #ifdef SPECULAR
        vec4 tSpec = texture2D(mTextureS, gl_TexCoord[0].st);
    #endif
    
    #ifdef MOTION_BLUR
        gl_FragData[0] = texture2D(mTexture, gl_TexCoord[0].st);
        gl_FragData[0].rgb *= vColor;
        #ifdef SPECULAR
            gl_FragData[0].rgb += vSpecColor*tSpec.rgb*tSpec.a;
        #endif
        gl_FragData[1] = vec4(1,1,1,1);
        gl_FragData[1].r = vPosition.z/vPosition.w;
    #else
        gl_FragColor = texture2D(mTexture, gl_TexCoord[0].st);
        gl_FragColor.rgb *= vColor;
        #ifdef SPECULAR
            gl_FragColor.rgb += vSpecColor*tSpec.rgb*tSpec.a;
        #endif
    #endif
}
