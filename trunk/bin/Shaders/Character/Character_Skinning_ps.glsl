// Note : inspired from Ogre's example shaders

// Provided by Ogre
uniform sampler2D mTexture;
uniform vec4      mLightPos[5];
uniform vec4      mLightDiffuseColor[5];
uniform vec4      mLightAtten[5];
uniform vec4      mAmbient;

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

    for (int i = 0; i < 5; ++i)
    {
        tLightDir = normalize(mLightPos[i].xyz - vBlendedPosition);
        tDistance = distance(mLightPos[i].xyz, vBlendedPosition);
    
        tLightColor += mLightDiffuseColor[i].rgb *
            clamp(dot(tLightDir, vBlendedNormal), 0.0f, 1.0f) *
            clamp(1.0f/(mLightAtten[i].y + tDistance*mLightAtten[i].z + tDistance*tDistance*mLightAtten[i].w), 0.0f, 1.0f);
    }
        
    gl_FragColor = texture2D(mTexture, gl_TexCoord[0].st);
    gl_FragColor.rgb *= tLightColor;
    #ifdef MOTION_BLUR
        gl_FragColor.a = vPosition.z/vPosition.w;
    #endif
}
