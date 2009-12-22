// Note : inspired from Ogre's example shaders

// Provided by Ogre
uniform sampler2D mTexture;

varying vec3 vColor;
#ifdef MOTION_BLUR
    varying vec4 vPosition;
#endif

void main()
{
    #ifdef MOTION_BLUR
        gl_FragData[0] = texture2D(mTexture, gl_TexCoord[0].st);
        gl_FragData[0].rgb *= vColor;
        gl_FragData[1] = vec4(1,1,1,1);
        gl_FragData[1].r = vPosition.z/vPosition.w;
    #else
        gl_FragColor = texture2D(mTexture, gl_TexCoord[0].st);
        gl_FragColor.rgb *= vColor;
    #endif
}
