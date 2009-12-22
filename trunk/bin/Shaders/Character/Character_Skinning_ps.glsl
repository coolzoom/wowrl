// Note : inspired from Ogre's example shaders

// Provided by Ogre
uniform sampler2D mTexture;
uniform vec4      mLightPos[5];
uniform vec4      mLightDiffuseColor[5];
uniform vec4      mLightAtten[5];
uniform vec4      mSunDir;
uniform vec4      mSunColor;
uniform vec4      mAmbient;

uniform vec4      mMotionBlurMask;

// Input
varying vec3  vBlendedNormal;
varying vec3  vBlendedPosition;
#ifdef MOTION_BLUR
    varying vec4 vPosition;
#endif

void main()
{
    vec3  tLightColor = mAmbient.rgb;
    vec3  tLightDir;
    float tDistance;
    float tAtten;

    for (int i = 0; i < 5; ++i)
    {
        tLightDir = normalize(mLightPos[i].xyz - vBlendedPosition);
        tDistance = distance(mLightPos[i].xyz, vBlendedPosition);
        
        tAtten = 1.0/(mLightAtten[i].y + tDistance*mLightAtten[i].z + tDistance*tDistance*mLightAtten[i].w);
        tLightColor += mLightDiffuseColor[i].rgb * max(dot(tLightDir, vBlendedNormal), 0.0) * tAtten;
    }
    
    tLightColor += mSunColor.rgb * max(dot(mSunDir.xyz, vBlendedNormal), 0.0);
    tLightColor = clamp(tLightColor, 0.0, 1.0);

    #ifdef MOTION_BLUR
        gl_FragData[0] = texture2D(mTexture, gl_TexCoord[0].st);
        gl_FragData[0].rgb *= tLightColor;
        gl_FragData[1] = mMotionBlurMask;
        gl_FragData[1].r = vPosition.z/vPosition.w;
    #else
        gl_FragColor = texture2D(mTexture, gl_TexCoord[0].st);
        gl_FragColor.rgb *= tLightColor;
    #endif
}
