// Input (names are forced by Ogre)
attribute vec4 vertex;
attribute vec3 normal;

// Output
varying vec4 vColor;
#ifdef MOTION_BLUR
    varying vec4 vPosition;
#endif

// Provided by Ogre
uniform mat4 mWorldViewProj;
uniform mat4 mWorld;

uniform vec4 mLightPos[5];
uniform vec4 mLightDir;
uniform vec4 mLightDiffuseColor[5];
uniform vec4 mLightAtten[5];
uniform vec4 mAmbient;
uniform vec4 mDiffuse;

void main()
{
    vec3  tLight = mAmbient.rgb;
    vec3  tLightDir;
    float tDistance;
    float tAtten;
    
    vec3 tPosition = (mWorld * vertex).xyz;
    vec3 tNormal = normalize((mWorld * vec4(normal, 0.0)).xyz);
    
    // Handle directional light
    tLight += mLightDiffuseColor[0].rgb * max(-dot(mLightDir.xyz, tNormal), 0.0) / mLightAtten[0].y;
    
    // Handle point lights
    for (int i = 1; i < 5; ++i)
    {
        tLightDir = normalize(mLightPos[i].xyz - tPosition);
        tDistance = distance(mLightPos[i].xyz, tPosition);

        tAtten = 1.0/(mLightAtten[i].y + tDistance*mLightAtten[i].z + tDistance*tDistance*mLightAtten[i].w);
        tLight += mLightDiffuseColor[i].rgb * max(dot(tLightDir, tNormal), 0.0) * tAtten;
    }
    
    tLight = clamp(tLight, 0.0, 1.0);
    
    vColor = mDiffuse;
    vColor.rgb *= tLight;

    // Apply position and camera projection
    gl_Position = mWorldViewProj * vertex;
    #ifdef MOTION_BLUR
        vPosition = gl_Position;
    #endif
}
