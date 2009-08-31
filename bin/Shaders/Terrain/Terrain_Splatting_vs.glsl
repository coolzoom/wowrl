// Input (names are forced by Ogre)
attribute vec4 vertex;
attribute vec3 normal;
attribute vec4 uv0;

// Output
varying vec3 vColor;
#ifdef SPECULAR
    varying vec3 vSpecColor;
#endif
#ifdef MOTION_BLUR
    varying vec4 vPosition;
#endif

// Provided by Ogre
uniform mat4 mWorldViewProj;
uniform mat4 mWorld;

#ifdef SPECULAR
    uniform vec4 mCamPos;
#endif
uniform vec4 mLightPos[5];
uniform vec4 mLightDiffuseColor[5];
uniform vec4 mLightAtten[5];
uniform vec4 mSunDir;
uniform vec4 mSunColor;
uniform vec4 mAmbient;

uniform mat4 mTexCoordMat1;
uniform mat4 mTexCoordMat2;
#if LAYER > 2
    uniform mat4 mTexCoordMat3;
#endif
#if LAYER > 3
    uniform mat4 mTexCoordMat4;
#endif

void main()
{
    vColor = mAmbient.rgb;
    vec3  tLightDir;
    float tDistance;
    float tAtten;
    
    vec3 tPosition = (mWorld * vertex).xyz;
    vec3 tNormal = normalize((mWorld * vec4(normal, 1.0)).xyz);
    
    #ifdef SPECULAR
        vec3 tReflected;
        vSpecColor = vec3(0.0, 0.0, 0.0);
        vec3 tEyeDir = normalize(mCamPos.xyz - tPosition);
    #endif
    
    for (int i = 0; i < 5; ++i)
    {
        tLightDir = normalize(mLightPos[i].xyz - tPosition);
        tDistance = distance(mLightPos[i].xyz, tPosition);

        tAtten = 1.0/(mLightAtten[i].y + tDistance*mLightAtten[i].z + tDistance*tDistance*mLightAtten[i].w);
        vColor += mLightDiffuseColor[i].rgb * max(dot(tLightDir, tNormal), 0.0) * tAtten;
        
        #ifdef SPECULAR
            tReflected = 2.0*tNormal*dot(tLightDir, tNormal) - tLightDir;
            vSpecColor += mLightDiffuseColor[i].rgb * max(dot(tReflected, tEyeDir), 0.0) * tAtten;
        #endif
    }
    
    vColor += mSunColor.rgb * max(dot(mSunDir.xyz, tNormal), 0.0);
    #ifdef SPECULAR
        tReflected = 2*tNormal*dot(mSunDir.xyz, tNormal) - mSunDir.xyz;
        vSpecColor += mSunColor.rgb * max(dot(tReflected, tEyeDir), 0.0);
    #endif

    // Apply position and camera projection
    gl_Position = mWorldViewProj * vertex;
    #ifdef MOTION_BLUR
        vPosition = gl_Position;
    #endif

    gl_TexCoord[0] = uv0;
    gl_TexCoord[1] = mTexCoordMat1 * uv0;
    gl_TexCoord[2] = mTexCoordMat2 * uv0;
    #if LAYER > 2
        gl_TexCoord[3] = mTexCoordMat3 * uv0;
    #endif
    #if LAYER > 3
        gl_TexCoord[4] = mTexCoordMat4 * uv0;
    #endif
}
