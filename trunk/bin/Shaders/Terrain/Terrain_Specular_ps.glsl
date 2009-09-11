// Note : inspired from Ogre's example shaders

// Provided by Ogre
uniform sampler2D mMask;
uniform sampler2D mTexture;
uniform sampler2D mTextureS;

varying vec3 vColor;
varying vec3 vSpecColor;
#ifdef MOTION_BLUR
    varying vec4 vPosition;
#endif

void main()
{
    vec4 tSpec = texture2D(mTextureS, gl_TexCoord[0].st);
    
    #ifdef MOTION_BLUR
        gl_FragData[0] = texture2D(mTexture,  gl_TexCoord[0].st);
        gl_FragData[0].rgb *= vColor;
        gl_FragData[0].rgb += vSpecColor*tSpec.rgb*tSpec.a;
        gl_FragData[0].a = vPosition.z/vPosition.w;
        gl_FragData[1] = vec4(1,1,1,1);
    #else
        gl_FragColor = texture2D(mTexture,  gl_TexCoord[0].st);
        gl_FragColor.rgb *= vColor;
        gl_FragColor.rgb += vSpecColor*tSpec.rgb*tSpec.a;
    #endif
}
