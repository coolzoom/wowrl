// Note : inspired from Ogre's example shaders

// Provided by Ogre
uniform sampler2D mMask;
uniform sampler2D mTexture1;
uniform sampler2D mTexture2;

uniform mat4 mTexCoordMat1;
uniform mat4 mTexCoordMat2;

varying vec3 vColor;

void main()
{
    vec4 tMask = texture2D(mMask, gl_TexCoord[0].st);
    float tSum = tMask.a + tMask.r;
    gl_FragColor  = texture2D(mTexture1, (mTexCoordMat1 * vec4(gl_TexCoord[0].st, 0, 1)).st)*tMask.a;
    gl_FragColor += texture2D(mTexture2, (mTexCoordMat2 * vec4(gl_TexCoord[0].st, 0, 1)).st)*tMask.r;
    gl_FragColor /= tSum;
    gl_FragColor.rgb *= vColor;
}
