// Input (names are forced by Ogre)
attribute vec4 vertex;
attribute vec3 normal;
attribute vec4 uv1;

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
uniform vec4 mLightDir;
uniform vec4 mLightDiffuseColor[5];
uniform vec4 mLightAtten[5];
uniform vec4 mAmbient;

uniform mat4 mTexCoordMat;

void main()
{
    vColor = mAmbient.rgb;
    vec3  tLightDir;
    float tDistance;
    float tAtten;
    float tDot;
    
    vec3 tPosition = (mWorld * vertex).xyz;
    vec3 tNormal = normalize((mWorld * vec4(normal, 0.0)).xyz);

    #ifdef SPECULAR
        vec3 tReflected;
        vSpecColor = vec3(0.0, 0.0, 0.0);
        vec3 tEyeDir = normalize(mCamPos.xyz - tPosition);
    #endif
    
    // Handle directional light
    tDot = -dot(mLightDir.xyz, tNormal);
    vColor += mLightDiffuseColor[0].rgb * max(tDot, 0.0) / mLightAtten[0].y;
    #ifdef SPECULAR
        tReflected = 2.0*tNormal*tDot + mLightDir.xyz;
        vSpecColor += mLightDiffuseColor[0].rgb * max(dot(tReflected, tEyeDir), 0.0) / mLightAtten[0].y;
    #endif
    
    // Handle point lights
    for (int i = 1; i < 5; ++i)
    {
        tLightDir = normalize(mLightPos[i].xyz - tPosition);
        tDistance = distance(mLightPos[i].xyz, tPosition);
        tDot = dot(tLightDir, tNormal);

        tAtten = 1.0/(mLightAtten[i].y + tDistance*mLightAtten[i].z + tDistance*tDistance*mLightAtten[i].w);
        vColor += mLightDiffuseColor[i].rgb * max(tDot, 0.0) * tAtten;
        
        #ifdef SPECULAR
            tReflected = 2.0*tNormal*dot(tLightDir, tNormal) - tLightDir;
            vSpecColor += mLightDiffuseColor[i].rgb * max(tDot, 0.0) * tAtten;
        #endif
    }
    
    vColor = clamp(vColor, 0.0, 1.0);

    // Apply position and camera projection
    gl_Position = mWorldViewProj * vertex;
    #ifdef MOTION_BLUR
        vPosition = gl_Position;
    #endif

    gl_TexCoord[0] = mTexCoordMat * uv1;
}
