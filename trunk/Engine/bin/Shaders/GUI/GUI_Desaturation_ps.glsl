// Note : inspired from Ogre's example shaders

// Provided by Ogre
uniform sampler2D mTexture;

void main()
{
    vec3 mTemp;
    gl_FragColor = texture2D(mTexture, gl_TexCoord[0].st);
    mTemp.r = mTemp.g = mTemp.b = (gl_FragColor.r + gl_FragColor.g + gl_FragColor.b)/3.0;
    gl_FragColor.rgb = mTemp.rgb;
}
