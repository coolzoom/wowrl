// Input (names are forced by Ogre)
attribute vec4 vertex;
attribute vec3 normal;
attribute vec4 uv0;

// Output
varying vec3 vColor;
varying vec3 vSpecColor;
#ifdef MOTION_BLUR
    varying vec4 vPosition;
#endif

// Provided by Ogre
uniform mat4 mWorldViewProj;
uniform mat4 mWorld;

uniform vec4 mCamPos;
uniform vec4 mLightPos[5];
uniform vec4 mLightDiffuseColor[5];
uniform vec4 mLightAtten[5];
uniform vec4 mSunDir;
uniform vec4 mSunColor;
uniform vec4 mAmbient;

uniform mat4 mTexCoordMat;

void main()
{
    vColor = mAmbient.rgb;
    vec3  tLightDir;
    float tDistance;
    float tAtten;
    
    vec3 tPosition = (mWorld * vertex).xyz;
    vec3 tNormal = normalize((mWorld * vec4(normal, 1.0)).xyz);

    vec3 tReflected;
    vSpecColor = vec3(0.0, 0.0, 0.0);
    vec3 tEyeDir = normalize(mCamPos.xyz - tPosition);
    
    for (int i = 0; i < 5; ++i)
    {
        tLightDir = normalize(mLightPos[i].xyz - tPosition);
        tDistance = distance(mLightPos[i].xyz, tPosition);

        tAtten = 1.0/(mLightAtten[i].y + tDistance*mLightAtten[i].z + tDistance*tDistance*mLightAtten[i].w);
        vColor += mLightDiffuseColor[i].rgb * max(dot(tLightDir, tNormal), 0.0) * tAtten;
        
        tReflected = 2.0*tNormal*dot(tLightDir, tNormal) - tLightDir;
        vSpecColor += mLightDiffuseColor[i].rgb * max(dot(tReflected, tEyeDir), 0.0) * tAtten;
    }
    
    vColor += mSunColor.rgb * max(dot(mSunDir.xyz, tNormal), 0.0);
    tReflected = 2.0*tNormal*dot(mSunDir.xyz, tNormal) - mSunDir.xyz;
    vSpecColor += mSunColor.rgb * max(dot(tReflected, tEyeDir), 0.0);

    // Apply position and camera projection
    gl_Position = mWorldViewProj * vertex;
    #ifdef MOTION_BLUR
        vPosition = gl_Position;
    #endif

    gl_TexCoord[0] = mTexCoordMat * uv0;
}
