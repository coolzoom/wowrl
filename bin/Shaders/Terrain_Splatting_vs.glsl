// Input (names are forced by Ogre)
attribute vec4 vertex;
attribute vec3 normal;
attribute vec4 uv0;

// Output
varying vec3 vNormal;
varying vec3 vPosition;
varying vec3 vColor;

// Provided by Ogre
uniform mat4 mWorldViewProj;
uniform mat4 mWorld;

uniform vec4 mLightPos[5];
uniform vec4 mLightDiffuseColor[5];
uniform vec4 mLightAtten[5];
uniform vec4 mAmbient;

void main()
{
    vColor = mAmbient.rgb;
    vec3  tLightDir;
    float tDistance;
    
    vPosition = (mWorld * vertex).xyz;
    vNormal = (mWorld * vec4(normal, 1)).xyz;

    for (int i = 0; i < 5; ++i)
    {
        tLightDir = normalize(mLightPos[i].xyz - vPosition);
        tDistance = distance(mLightPos[i].xyz, vPosition);
    
        vColor += mLightDiffuseColor[i].rgb *
            clamp(dot(tLightDir, vNormal), 0.0f, 1.0f) *
            clamp(1.0f/(mLightAtten[i].y + tDistance*mLightAtten[i].z + tDistance*tDistance*mLightAtten[i].w), 0.0f, 1.0f);
    }

    // Apply position and camera projection
    gl_Position = mWorldViewProj * vertex;

    gl_TexCoord[0] = uv0;
}
